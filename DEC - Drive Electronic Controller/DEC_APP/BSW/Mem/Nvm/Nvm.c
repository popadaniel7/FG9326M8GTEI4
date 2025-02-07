#include "Crc.h"
#include "string.h"
#include "Fls.h"
#include "Nvm.h"
#include "EncCal.h"

uint32 ActiveSectorAddress = DFLASH_STARTING_ADDRESS; /* Active Sector */
uint32 InactiveSectorAddress = ((DFLASH_STARTING_ADDRESS == 0u) ? (DFLASH_STARTING_ADDRESS + DFLASH_SECTOR_SIZE) : DFLASH_STARTING_ADDRESS);
/* Flag indicating if the inactive sector is erased and ready */
uint8 InactiveSectorReady = 0u;
Nvm_BlockType Nvm_BlockList[NVM_MAX_BLOCKS] =
{
        {{0u, 1u, (uint16)ENCCAL_CALIBRATION_SIZE, 0u, 1u, 0u, 0u}, EncCal_Calibration_Buffer},
        {{0u, 2u, (uint16)ENCCAL_CODING_SIZE, 0u, 1u, 0u, 0u}, (uint32*)EncCal_Coding_Buffer},
        {{0u, 3u, (uint16)sizeof(EncCal_VODataComplete)/sizeof(uint32), 0u, 1u, 0u, 0u}, (uint32*)&EncCal_VODataComplete},
};
uint8 Nvm_WriteAllFinished = 0u;
uint8 Nvm_ReadAllFinished = 0u;

void Nvm_SwitchActiveSector(void);
uint32 Nvm_CalculateCrc(uint32 *data, uint32 length);
uint32 Nvm_FindBlockAddress(uint32 blockId);
uint32 Nvm_FindNextWriteAddress(void);
void Nvm_RestorePersistentBlocks(uint32 newSectorAddress);
void Nvm_SwitchActiveSector(void);
void Nvm_WriteBlock(uint16 blockId, uint32 *data, uint16 length);
void Nvm_ReadBlock(uint32 blockId, uint32 *data, uint32 length);
void Nvm_WriteAll(void);
void Nvm_ReadAll(void);
void Nvm_LoadDefaults(void);
uint8 Nvm_GetBlockStatus(uint32 blockId);
uint32 Nvm_GetBlockCrc(uint32 blockId);

void Nvm_LoadDefaults(void)
{

}

uint32 Nvm_CalculateCrc(uint32 *data, uint32 length)
{
    return Crc_Calculate(data, (uint16)length, 0);
}

uint32 Nvm_FindBlockAddress(uint32 blockId)
{
    uint32 address = ActiveSectorAddress;
    /* Read header from flash memory */
    Nvm_BlockHeaderType header;

    while(address < (ActiveSectorAddress + DFLASH_SECTOR_SIZE))
    {
        Fls_ReadBlock(address, (uint32*)&header, NVM_HEADER_SIZE_U32);
        /* If the header is not programmed, we've reached free space */
        if(header.blockId == 0xFFFFu || header.blockId == 0u)  /* Assuming un-programmed flash shows 0xFFFF... */
        {
            break;
        }
        else
        {
            /* Do nothing. */
        }

        if(header.blockId == blockId)
        {
            return address;
        }
        else
        {
            /* Do nothing. */
        }
        /* Advance address: header + pay-load */
        address += (NVM_HEADER_SIZE_U32 + header.blockSize) * sizeof(uint32);
    }
    return DFLASH_EMPTY_WORD; /* Block not found */
}

uint32 Nvm_FindNextWriteAddress(void)
{
    uint32 address = ActiveSectorAddress;

    while(address < (ActiveSectorAddress + DFLASH_SECTOR_SIZE))
    {
        /* Check if this word is empty */
        if(MEM(address) == DFLASH_EMPTY_WORD)
        {
            break;
        }
        else
        {
            /* Since blocks are stored consecutively, skip to next block using stored header info */
            Nvm_BlockHeaderType header;
            Fls_ReadBlock(address, (uint32*)&header, NVM_HEADER_SIZE_U32);
            /* If header not programmed properly, treat the remaining as free. */
            if((header.blockId == 0xFFFFu) || (header.blockId == 0u))
            {
                break;
            }
            else
            {
                /* Do nothing. */
            }

            address += (NVM_HEADER_SIZE_U32 + header.blockSize) * sizeof(uint32);
        }
    }
    /* If no space in active sector, check if inactive sector is ready; if not, erase it and mark ready */
    if(address >= (ActiveSectorAddress + DFLASH_SECTOR_SIZE))
    {
        if(!InactiveSectorReady)
        {
            Fls_Erase(InactiveSectorAddress);
            InactiveSectorReady = 1u;
        }
        else
        {
            /* Do nothing. */
        }
        /* Switch active sector only if both sectors are full; in our case, we switch if current is full */
        Nvm_SwitchActiveSector();
        address = ActiveSectorAddress;
    }
    else
    {
        /* Do nothing. */
    }

    return address;
}

void Nvm_RestorePersistentBlocks(uint32 newSectorAddress)
{
    for (uint32 i = 0; i < NVM_MAX_BLOCKS; i++)
    {
        if(Nvm_BlockList[i].header.markedForReadAll || Nvm_BlockList[i].header.markedForWriteAll)
        {
            uint32 oldAddress = Nvm_FindBlockAddress(Nvm_BlockList[i].header.blockId);

            if(oldAddress != DFLASH_EMPTY_WORD)
            {
                /* Read the pay-load from old sector */
                uint32 tempData[Nvm_BlockList[i].header.blockSize];
                /* Calculate offset for pay-load in flash = address + header size */
                Fls_ReadBlock(oldAddress + (NVM_HEADER_SIZE_U32 * sizeof(uint32)), tempData, Nvm_BlockList[i].header.blockSize);
                uint32 crc = Nvm_CalculateCrc(tempData, Nvm_BlockList[i].header.blockSize);
                Nvm_BlockList[i].header.crc = crc;
                /* Write the header and pay-load into new sector */
                uint32 writeAddr = newSectorAddress;
                /* Write header */
                Fls_WriteBlock(writeAddr, (uint32 *)&Nvm_BlockList[i].header, NVM_HEADER_SIZE_U32);
                writeAddr += NVM_HEADER_SIZE_U32 * sizeof(uint32);
                /* Write pay-load */
                Fls_WriteBlock(writeAddr, tempData, Nvm_BlockList[i].header.blockSize);
            }
            else
            {
                /* Block not found in old sector, nothing to restore */
            }
        }
        else
        {
            /* Do nothing. */
        }
    }
}

void Nvm_SwitchActiveSector(void)
{
    /* Use inactive sector as new active */
    uint32 newSector = InactiveSectorAddress;
    /* Restore persistent blocks from the current active sector to the new sector */
    Nvm_RestorePersistentBlocks(newSector);
    /* Erase the new active sector after restoration to clear any gaps.
           Note: In some designs you might only erase the unused area. Here we re-erase for clarity. */
    Fls_Erase(newSector);
    /* Update addresses: swap sectors */
    uint32 oldSector = ActiveSectorAddress;
    ActiveSectorAddress = newSector;
    InactiveSectorAddress = oldSector;
    InactiveSectorReady = 0u;
}

void Nvm_WriteBlock(uint16 blockId, uint32 *data, uint16 length)
{
    uint32 address = Nvm_FindNextWriteAddress();
    uint32 crc = Nvm_CalculateCrc(data, length);
    Nvm_BlockHeaderType header = { crc, blockId, length, 0u, 0u, 0u, 0u };
    /* Write header */
    Fls_WriteBlock(address, (uint32 *)&header, NVM_HEADER_SIZE_U32);
    /* Write pay-load data immediately after header */
    Fls_WriteBlock(address + (NVM_HEADER_SIZE_U32 * sizeof(uint32)), data, length);
}

uint8 Nvm_GetBlockStatus(uint32 blockId)
{
    for(uint32 i = 0u; i < NVM_MAX_BLOCKS; i++)
    {
        if(Nvm_BlockList[i].header.blockId == blockId)
        {
            return Nvm_BlockList[i].header.blockStatus;
        }
        else
        {
            /* Do nothing. */
        }
    }
    return 0xFEu; /* Invalid blockId */
}

uint32 Nvm_GetBlockCrc(uint32 blockId)
{
    for(uint32 i = 0u; i < NVM_MAX_BLOCKS; i++)
    {
        if(Nvm_BlockList[i].header.blockId == blockId)
        {
            return Nvm_BlockList[i].header.crc;
        }
        else
        {
            /* Do nothing. */
        }
    }
    return 0xFFFFFFFFu; /* Invalid blockId */
}

void Nvm_ReadBlock(uint32 blockId, uint32 *data, uint32 length)
{
    uint32 address = Nvm_FindBlockAddress(blockId);
    if (address != DFLASH_EMPTY_WORD)
    {
        Nvm_BlockHeaderType header;
        /* Read header from flash */
        Fls_ReadBlock(address, (uint32 *)&header, NVM_HEADER_SIZE_U32);
        /* Read pay-load from flash */
        uint32 tempData[header.blockSize];
        Fls_ReadBlock(address + (NVM_HEADER_SIZE_U32 * sizeof(uint32)), tempData, header.blockSize);
        uint32 calculatedCrc = Nvm_CalculateCrc(tempData, header.blockSize);
        if (calculatedCrc == header.crc)
        {
            memcpy(data, tempData, length * sizeof(uint32));
            header.blockStatus = 0x00u;
        }
        else
        {
            header.blockStatus = 0xFEu;
        }
        /* Optionally update the in-RAM block list status */
        for(uint32 i = 0u; i < NVM_MAX_BLOCKS; i++)
        {
            if(Nvm_BlockList[i].header.blockId == blockId)
            {
                Nvm_BlockList[i].header.blockStatus = header.blockStatus;
                break;
            }
            else
            {
                /* Do nothing. */
            }
        }
    }
    else
    {
        /* Block not found. Optionally, set error status */
    }
}

/* Iterates through the persistent block list and writes blocks marked for write-all */
void Nvm_WriteAll(void)
{
    Nvm_WriteAllFinished = 1u;

    for (uint32 i = 0; i < NVM_MAX_BLOCKS; i++)
    {
        if (Nvm_BlockList[i].header.markedForWriteAll)
        {
            Nvm_WriteBlock(Nvm_BlockList[i].header.blockId, Nvm_BlockList[i].actualBlockValues, Nvm_BlockList[i].header.blockSize);
        }
        else
        {
            /* Do nothing. */
        }
    }

    Nvm_WriteAllFinished = 2u;
}

/* Iterates through the persistent block list and reads blocks marked for read-all */
void Nvm_ReadAll(void)
{
    Nvm_ReadAllFinished = 1u;

    for (uint32 i = 0; i < NVM_MAX_BLOCKS; i++)
    {
        if (Nvm_BlockList[i].header.markedForReadAll)
        {
            Nvm_ReadBlock(Nvm_BlockList[i].header.blockId, Nvm_BlockList[i].actualBlockValues, Nvm_BlockList[i].header.blockSize);
        }
        else
        {
            /* Do nothing. */
        }
    }

    Nvm_ReadAllFinished = 2u;
}
