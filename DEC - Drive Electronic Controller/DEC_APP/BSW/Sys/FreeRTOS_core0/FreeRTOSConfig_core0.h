#ifndef FREERTOS_CONFIG_CORE0_H
#define FREERTOS_CONFIG_CORE0_H

#define configUSE_PREEMPTION_core0                       1
#define configUSE_IDLE_HOOK_core0                        1
#define configCPU_CLOCK_HZ_core0                         ( ( unsigned long ) 300000000UL )
#define configTICK_RATE_HZ_core0                         ( ( TickType_t_core0 ) 1000UL )
#define configMAX_PRIORITIES_core0                       ( 31 )
#define configMINIMAL_STACK_SIZE_core0                   ( ( unsigned short ) 256 )
#define configTOTAL_HEAP_SIZE_core0                      ( ( size_t ) ( 32U * 1024U ) )
#define configMAX_TASK_NAME_LEN_core0                    ( 254 )
#define configENABLE_BACKWARD_COMPATIBILITY_core0        0
#define configUSE_TRACE_FACILITY_core0                   0
#define configUSE_16_BIT_TICKS_core0                     0
#define configIDLE_SHOULD_YIELD_core0                    0
#define configUSE_MALLOC_FAILED_HOOK_core0               1
#define configCHECK_FOR_STACK_OVERFLOW_core0             1
#define configUSE_TICK_HOOK_core0                        1
#define configUSE_COUNTING_SEMAPHORES_core0              1
#define configUSE_RECURSIVE_MUTEXES_core0                1
#define configUSE_MUTEXES_core0                          1
#define configRECORD_STACK_HIGH_ADDRESS_core0            1
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS_core0    5
/* Software timer configuration. */
#define configUSE_TIMERS_core0                           ( 1 )
#define configTIMER_TASK_PRIORITY_core0                  ( 31 )
#define configTIMER_QUEUE_LENGTH_core0                   ( 15 )
#define configTIMER_TASK_STACK_DEPTH_core0               configMINIMAL_STACK_SIZE_core0
/* Set the following definitions to 1 to include the API function, or zero
 * to exclude the API function. */
#define INCLUDE_vTaskPrioritySet_core0                   1
#define INCLUDE_uxTaskPriorityGet_core0                  1
#define INCLUDE_vTaskDelete_core0                        1
#define INCLUDE_vTaskCleanUpResources_core0              1
#define INCLUDE_vTaskSuspend_core0                       1
#define INCLUDE_vTaskDelayUntil_core0                    1
#define INCLUDE_vTaskDelay_core0                         1
/* Interrupt above priority 31 are not effected by critical sections, but cannot call interrupt safe FreeRTOS_core0 functions. */
#define configMAX_API_CALL_INTERRUPT_PRIORITY_core0      31
/* Default definition of configASSERT_core0(). */
#ifdef DEBUG
#ifdef __TASKING__
#define configASSERT_core0( x_core0 )    if( ( x_core0 ) == 0 ) { __disable(); __debug(); }
#endif
#ifdef __clang__
#define configASSERT_core0( x_core0 )    if( ( x_core0 ) == 0 ) { __builtin_tricore_disable(); __builtin_tricore_debug(); }
#endif
#else
#define configASSERT_core0( x_core0 ) ((void)(x_core0)) /* Empty macro to remove compiler warning(s) about unused variables */
#endif
/* AURIX TCxxx definitions */
#define configCONTEXT_INTERRUPT_PRIORITY_core0    1
#define configTIMER_INTERRUPT_PRIORITY_core0      2 /* This value must not be bigger then context priority */
#define configCPU_NR_core0                        0
#define configPROVIDE_SYSCALL_TRAP_core0          0
#define configSYSCALL_CALL_DEPTH_core0            2
#define configSTM_core0                           ( ( uint32_t * ) (0xF0001000 + configCPU_NR_core0*0x100 ) )
#define configSTM_SRC_core0                       ( ( uint32_t * ) (0xF0038300 + configCPU_NR_core0*0x8) )
#define configSTM_CLOCK_HZ_core0                  ( 100000000 )
#define configSTM_DEBUG_core0                     ( 0 )
#define configCONTEXT_SRC_core0                   ( ( uint32_t * ) 0xF0038990 )
#endif /* FREERTOS_CONFIG_H */
