#include "Ifx_Types.h"

#define NVM_MAX_BLOCKS              3u
#define DFLASH_EMPTY_WORD           0xFFFFFFFFu
#define NVM_HEADER_SIZE_U32         (7u)/* Compute header size in uint32 units (header fields except data payload) */
/* Header: crc, blockId, blockSize, blockStatus, markedForReadAll, markedForWriteAll, padding */
typedef struct
{
        uint32 crc;
        /* The data follows in flash, whose length is in blockSize */
        /* Instead of storing a pointer (which is not valid in flash), we store blockSize and use Nvm_BlockList separately */
        uint16 blockId;
        uint16 blockSize; /* Number of uint32 words */
        uint8 blockStatus;
        uint8 markedForReadAll;
        uint8 markedForWriteAll;
        uint8 padding;
}Nvm_BlockHeaderType;
/* In RAM, we have a list of persistent blocks info. */
typedef struct
{
        Nvm_BlockHeaderType header;
        uint32* actualBlockValues;
}Nvm_BlockType;

extern uint32 ActiveSectorAddress;
extern uint32 InactiveSectorAddress;
extern uint8 InactiveSectorReady;
extern Nvm_BlockType Nvm_BlockList[NVM_MAX_BLOCKS];
extern uint8 Nvm_WriteAllFinished;
extern uint8 Nvm_ReadAllFinished;

extern void Nvm_SwitchActiveSector(void);
extern uint32 Nvm_CalculateCrc(uint32 *data, uint32 length);
extern uint32 Nvm_FindBlockAddress(uint32 blockId);
extern uint32 Nvm_FindNextWriteAddress(void);
extern void Nvm_RestorePersistentBlocks(uint32 newSectorAddress);
extern void Nvm_SwitchActiveSector(void);
extern void Nvm_WriteBlock(uint16 blockId, uint32 *data, uint16 length);
extern void Nvm_ReadBlock(uint32 blockId, uint32 *data, uint32 length);
extern void Nvm_WriteAll(void);
extern void Nvm_ReadAll(void);
extern void Nvm_LoadDefaults(void);
extern uint8 Nvm_GetBlockStatus(uint32 blockId);
extern uint32 Nvm_GetBlockCrc(uint32 blockId);
