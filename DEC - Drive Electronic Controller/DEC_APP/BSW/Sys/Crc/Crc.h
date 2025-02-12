#include "IfxFce_Crc.h"
#include "Ifx_Types.h"

#define CRC_WRONG_CHECK_VAL     0
/* Setting macro for the expected result of the calculation */
#define CRC_EXPECTED_RESULT     0xD95DEF75
/* Setting macro for error injection */
#define CRC_ERROR_INJECTION     0xAC
#define FCE_CRC_ERROR_MASK  (uint32)(0x0000000FU)   /* FCE error flags                      */

typedef struct
{
    IfxFce_Crc      fce;
    IfxFce_Crc_Crc  fceCrc;
    uint32          crc_result;
} fceCrc;

extern fceCrc g_fceCrc; /* Structure to store information */

extern void Crc_Init(void);
extern uint32 Crc_Calculate(const uint32 *crcData, uint16 crcDataLength, uint32 crcStartValue);
extern uint32 Crc_CalculateFCECRC(const uint32 *crcData, uint16 crcDataLength, uint32 crcStartValue);
