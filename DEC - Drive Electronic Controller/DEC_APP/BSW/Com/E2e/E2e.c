#include "E2e.h"
#include "Crc.h"

void E2e_UpdateTx(McmcanType *message, uint8 sequenceCounter);
uint8 E2e_CheckRx(McmcanType *message, uint8 sequenceCounter);

void E2e_UpdateTx(McmcanType *message, uint8 sequenceCounter)
{
    uint8 calculatedCrc = 0u;

    message->txData[0u] = sequenceCounter;
    calculatedCrc = (uint8)Crc_CalculateFCECRC((uint32*) message->txData, 2u, 0u);
    message->txData[1u] = calculatedCrc;
}

uint8 E2e_CheckRx(McmcanType *message, uint8 sequenceCounter)
{
    uint8 receivedCrc = 0u;
    uint8 seqCnt = 0u;
    uint8 calculatedCrc = 0u;
    uint8 retVal = 0u;

    receivedCrc = (uint8)message->rxData[1u];
    seqCnt = (uint8)message->rxData[0u];
    calculatedCrc = (uint8)Crc_CalculateFCECRC((uint32*)message->rxData, 2u, 0u);

    if (receivedCrc == calculatedCrc && seqCnt == sequenceCounter) retVal = 0u;
    else retVal = 1u;

    return retVal;
}
