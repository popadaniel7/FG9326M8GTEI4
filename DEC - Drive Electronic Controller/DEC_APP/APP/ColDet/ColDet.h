#include "Ifx_Types.h"

typedef enum
{
    COLLISION_SAFE = 0u,
    COLLISION_WARNING = 1u,
    COLLISION_BRAKE = 2u,
    COLLISION_ERROR = 254u
} CollisionState;

extern CollisionState ColDet_CanTx_CollisionState;
extern uint8 ColDet_CanTx_BrakeLevel;
extern uint8 ColDet_CanTx_IrSenStat;
extern uint8 ColDet_CanRx_CalculatedSpeed;

extern void ColDet_MainFunction(void);
