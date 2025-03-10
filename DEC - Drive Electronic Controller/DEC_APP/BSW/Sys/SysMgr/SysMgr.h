#include "Ifx_Types.h"

typedef enum
{
    SYSMGR_INIT = 0U,
    SYSMGR_STARTUP = 1U,
    SYSMGR_RUN = 2U,
    SYSMGR_POSTRUN = 3U,
    SYSMGR_SLEEP = 4U
}SysMgr_EcuState_t;

extern SysMgr_EcuState_t SysMgr_EcuState;
extern uint8 SysMgr_NoBusActivity;
extern uint8 SysMgr_Core1OnHalt;
extern uint8 SysMgr_Core2OnHalt;

extern void SysMgr_ProcessResetDtc(void);
extern void SysMgr_EcuStateMachine(void);
extern void SysMgr_GoSleepSequence(void);
extern void SysMgr_WakeupInitScr(void);
extern void SysMgr_PreSleepInitScr(void);
extern void SysMgr_LoadScr(void);
extern void SysMgr_MainFunction(void);
