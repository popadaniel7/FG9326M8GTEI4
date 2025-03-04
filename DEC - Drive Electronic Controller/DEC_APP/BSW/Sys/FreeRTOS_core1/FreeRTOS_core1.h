/*
 * FreeRTOS_core1 Kernel V10.5.1
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS_core1.org
 * https://github.com/FreeRTOS_core1
 *
 */

#ifndef INC_FREERTOS_CORE1_H
#define INC_FREERTOS_CORE1_H

/*
 * Include the generic headers required for the FreeRTOS_core1 port being used.
 */
#include <stddef.h>

/*
 * If stdint.h cannot be located then:
 *   + If using GCC ensure the -nostdint options is *not* being used.
 *   + Ensure the project's include path includes the directory in which your
 *     compiler stores stdint.h.
 *   + Set any compiler options necessary for it to support C99, as technically
 *     stdint.h is only mandatory with C99 (FreeRTOS_core1 does not require C99 in any
 *     other way).
 *   + The FreeRTOS_core1 download includes a simple stdint.h definition that can be
 *     used in cases where none is provided by the compiler.  The files only
 *     contains the typedefs required to build FreeRTOS_core1.  Read the instructions
 *     in FreeRTOS_core1/source/stdint.readme for more information.
 */
#include <stdint.h> /* READ COMMENT ABOVE. */

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

/* Application specific configuration options. */
#include "FreeRTOSConfig_core1.h"

/* Basic FreeRTOS_core1 definitions. */
#include "projdefs_core1.h"

/* Definitions specific to the port being used. */
#include "portable_core1.h"

/* Must be defaulted before configUSE_NEWLIB_REENTRANT_core1 is used below. */
#ifndef configUSE_NEWLIB_REENTRANT_core1
    #define configUSE_NEWLIB_REENTRANT_core1    0
#endif

/* Required if struct _reent is used. */
#if ( configUSE_NEWLIB_REENTRANT_core1 == 1 )

/* Note Newlib support has been included by popular demand, but is not
 * used by the FreeRTOS_core1 maintainers themselves.  FreeRTOS_core1 is not
 * responsible for resulting newlib operation.  User must be familiar with
 * newlib and must provide system-wide implementations of the necessary
 * stubs. Be warned that (at the time of writing) the current newlib design
 * implements a system-wide malloc() that must be provided with locks.
 *
 * See the third party link http://www.nadler.com/embedded/newlibAndFreeRTOS.html
 * for additional information. */
    //#include <reent.h>

    #define configUSE_C_RUNTIME_TLS_SUPPORT_core1    1

    #ifndef configTLS_BLOCK_TYPE_core1
        #define configTLS_BLOCK_TYPE_core1           struct _reent
    #endif

    #ifndef configINIT_TLS_BLOCK_core1
        #define configINIT_TLS_BLOCK_core1( xTLSBlock_core1 )    _REENT_INIT_PTR( &( xTLSBlock_core1 ) )
    #endif

    #ifndef configSET_TLS_BLOCK_core1
        #define configSET_TLS_BLOCK_core1( xTLSBlock_core1 )    _impure_ptr = &( xTLSBlock_core1 )
    #endif

    #ifndef configDEINIT_TLS_BLOCK_core1
        #define configDEINIT_TLS_BLOCK_core1( xTLSBlock_core1 )    _reclaim_reent( &( xTLSBlock_core1 ) )
    #endif
#endif /* if ( configUSE_NEWLIB_REENTRANT_core1 == 1 ) */

#ifndef configUSE_C_RUNTIME_TLS_SUPPORT_core1
    #define configUSE_C_RUNTIME_TLS_SUPPORT_core1    0
#endif

#if ( ( configUSE_NEWLIB_REENTRANT_core1 == 0 ) && ( configUSE_C_RUNTIME_TLS_SUPPORT_core1 == 1 ) )

    #ifndef configTLS_BLOCK_TYPE_core1
        #error Missing definition:  configTLS_BLOCK_TYPE_core1 must be defined in FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT_core1 is set to 1.
    #endif

    #ifndef configINIT_TLS_BLOCK_core1
        #error Missing definition:  configINIT_TLS_BLOCK_core1 must be defined in FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT_core1 is set to 1.
    #endif

    #ifndef configSET_TLS_BLOCK_core1
        #error Missing definition:  configSET_TLS_BLOCK_core1 must be defined in FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT_core1 is set to 1.
    #endif

    #ifndef configDEINIT_TLS_BLOCK_core1
        #error Missing definition:  configDEINIT_TLS_BLOCK_core1 must be defined in FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT_core1 is set to 1.
    #endif
#endif /* if ( ( configUSE_NEWLIB_REENTRANT_core1 == 0 ) && ( configUSE_C_RUNTIME_TLS_SUPPORT_core1 == 1 ) ) */

/*
 * Check all the required application specific macros have been defined.
 * These macros are application specific and (as downloaded) are defined
 * within FreeRTOSConfig.h.
 */

#ifndef configMINIMAL_STACK_SIZE_core1
    #error Missing definition:  configMINIMAL_STACK_SIZE_core1 must be defined in FreeRTOSConfig.h.  configMINIMAL_STACK_SIZE_core1 defines the size (in words) of the stack allocated to the idle task_core1.  Refer to the demo project provided for your port for a suitable value.
#endif

#ifndef configMAX_PRIORITIES_core1
    #error Missing definition:  configMAX_PRIORITIES_core1 must be defined in FreeRTOSConfig.h.  See the Configuration section of the FreeRTOS_core1 API documentation for details.
#endif

#if configMAX_PRIORITIES_core1 < 1
    #error configMAX_PRIORITIES_core1 must be defined to be greater than or equal to 1.
#endif

#ifndef configUSE_PREEMPTION_core1
    #error Missing definition:  configUSE_PREEMPTION_core1 must be defined in FreeRTOSConfig.h as either 1 or 0.  See the Configuration section of the FreeRTOS_core1 API documentation for details.
#endif

#ifndef configUSE_IDLE_HOOK_core1
    #error Missing definition:  configUSE_IDLE_HOOK_core1 must be defined in FreeRTOSConfig.h as either 1 or 0.  See the Configuration section of the FreeRTOS_core1 API documentation for details.
#endif

#ifndef configUSE_TICK_HOOK_core1
    #error Missing definition:  configUSE_TICK_HOOK_core1 must be defined in FreeRTOSConfig.h as either 1 or 0.  See the Configuration section of the FreeRTOS_core1 API documentation for details.
#endif

#ifndef configUSE_16_BIT_TICKS_core1
    #error Missing definition:  configUSE_16_BIT_TICKS_core1 must be defined in FreeRTOSConfig.h as either 1 or 0.  See the Configuration section of the FreeRTOS_core1 API documentation for details.
#endif

#ifndef configUSE_CO_ROUTINES_core1
    #define configUSE_CO_ROUTINES_core1    0
#endif

#ifndef INCLUDE_vTaskPrioritySet_core1
    #define INCLUDE_vTaskPrioritySet_core1    1
#endif

#ifndef INCLUDE_uxTaskPriorityGet_core1
    #define INCLUDE_uxTaskPriorityGet_core1    1
#endif

#ifndef INCLUDE_vTaskDelete_core1
    #define INCLUDE_vTaskDelete_core1    1
#endif

#ifndef INCLUDE_vTaskSuspend_core1
    #define INCLUDE_vTaskSuspend_core1    1
#endif

#ifdef INCLUDE_xTaskDelayUntil_core1
    #ifdef INCLUDE_vTaskDelayUntil_core1

/* INCLUDE_vTaskDelayUntil_core1 was replaced by INCLUDE_xTaskDelayUntil_core1.  Backward
 * compatibility is maintained if only one or the other is defined, but
 * there is a conflict if both are defined. */
        #error INCLUDE_vTaskDelayUntil_core1 and INCLUDE_xTaskDelayUntil_core1 are both defined.  INCLUDE_vTaskDelayUntil_core1 is no longer required and should be removed
    #endif
#endif

#ifndef INCLUDE_xTaskDelayUntil_core1
    #ifdef INCLUDE_vTaskDelayUntil_core1

/* If INCLUDE_vTaskDelayUntil_core1 is set but INCLUDE_xTaskDelayUntil_core1 is not then
 * the project's FreeRTOSConfig.h probably pre-dates the introduction of
 * xTaskDelayUntil and setting INCLUDE_xTaskDelayUntil_core1 to whatever
 * INCLUDE_vTaskDelayUntil_core1 is set to will ensure backward compatibility.
 */
        #define INCLUDE_xTaskDelayUntil_core1    INCLUDE_vTaskDelayUntil_core1
    #endif
#endif

#ifndef INCLUDE_xTaskDelayUntil_core1
    #define INCLUDE_xTaskDelayUntil_core1    1
#endif

#ifndef INCLUDE_vTaskDelay_core1
    #define INCLUDE_vTaskDelay_core1    1
#endif

#ifndef INCLUDE_xTaskGetIdleTaskHandle_core1
    #define INCLUDE_xTaskGetIdleTaskHandle_core1    1
#endif

#ifndef INCLUDE_xTaskAbortDelay_core1
    #define INCLUDE_xTaskAbortDelay_core1    1
#endif

#ifndef INCLUDE_xQueueGetMutexHolder_core1
    #define INCLUDE_xQueueGetMutexHolder_core1    1
#endif

#ifndef INCLUDE_xSemaphoreGetMutexHolder_core1
    #define INCLUDE_xSemaphoreGetMutexHolder_core1    INCLUDE_xQueueGetMutexHolder_core1
#endif

#ifndef INCLUDE_xTaskGetHandle_core1
    #define INCLUDE_xTaskGetHandle_core1    1
#endif

#ifndef INCLUDE_uxTaskGetStackHighWaterMark_core1
    #define INCLUDE_uxTaskGetStackHighWaterMark_core1    1
#endif

#ifndef INCLUDE_uxTaskGetStackHighWaterMark2_core1
    #define INCLUDE_uxTaskGetStackHighWaterMark2_core1    1
#endif

#ifndef INCLUDE_eTaskGetState_core1
    #define INCLUDE_eTaskGetState_core1    1
#endif

#ifndef INCLUDE_xTaskResumeFromISR_core1
    #define INCLUDE_xTaskResumeFromISR_core1    1
#endif

#ifndef INCLUDE_xTimerPendFunctionCall_core1
    #define INCLUDE_xTimerPendFunctionCall_core1    1
#endif

#ifndef INCLUDE_xTaskGetSchedulerState_core1
    #define INCLUDE_xTaskGetSchedulerState_core1    1
#endif

#ifndef INCLUDE_xTaskGetCurrentTaskHandle_core1
    #define INCLUDE_xTaskGetCurrentTaskHandle_core1    1
#endif

#if configUSE_CO_ROUTINES_core1 != 0
    #ifndef configMAX_CO_ROUTINE_PRIORITIES_core1
        #error configMAX_CO_ROUTINE_PRIORITIES_core1 must be greater than or equal to 1.
    #endif
#endif

#ifndef configUSE_DAEMON_TASK_STARTUP_HOOK_core1
    #define configUSE_DAEMON_TASK_STARTUP_HOOK_core1    0
#endif

#ifndef configUSE_APPLICATION_TASK_TAG_core1
    #define configUSE_APPLICATION_TASK_TAG_core1    1
#endif

#ifndef configNUM_THREAD_LOCAL_STORAGE_POINTERS_core1
    #define configNUM_THREAD_LOCAL_STORAGE_POINTERS_core1    0
#endif

#ifndef configUSE_RECURSIVE_MUTEXES_core1
    #define configUSE_RECURSIVE_MUTEXES_core1    0
#endif

#ifndef configUSE_MUTEXES_core1
    #define configUSE_MUTEXES_core1    0
#endif

#ifndef configUSE_TIMERS_core1
    #define configUSE_TIMERS_core1    0
#endif

#ifndef configUSE_COUNTING_SEMAPHORES_core1
    #define configUSE_COUNTING_SEMAPHORES_core1    0
#endif

#ifndef configUSE_ALTERNATIVE_API_core1
    #define configUSE_ALTERNATIVE_API_core1    0
#endif

#ifndef portCRITICAL_NESTING_IN_TCB_core1
    #define portCRITICAL_NESTING_IN_TCB_core1    0
#endif

#ifndef configMAX_TASK_NAME_LEN_core1
    #define configMAX_TASK_NAME_LEN_core1    16
#endif

#ifndef configIDLE_SHOULD_YIELD_core1
    #define configIDLE_SHOULD_YIELD_core1    1
#endif

#if configMAX_TASK_NAME_LEN_core1 < 1
    #error configMAX_TASK_NAME_LEN_core1 must be set to a minimum of 1 in FreeRTOSConfig.h
#endif

#ifndef configASSERT_core1
    #define configASSERT_core1( x_core1 )
    #define configASSERT_DEFINED_core1    0
#else
    #define configASSERT_DEFINED_core1    1
#endif

/* configPRECONDITION_core1 should be defined as configASSERT_core1.
 * The CBMC proofs need a way to track assumptions and assertions.
 * A configPRECONDITION_core1 statement should express an implicit invariant or
 * assumption made.  A configASSERT_core1 statement should express an invariant that must
 * hold explicit before calling the code. */
#ifndef configPRECONDITION_core1
    #define configPRECONDITION_core1( X )    configASSERT_core1( X )
    #define configPRECONDITION_DEFINED_core1    0
#else
    #define configPRECONDITION_DEFINED_core1    1
#endif

#ifndef portMEMORY_BARRIER_core1
    #define portMEMORY_BARRIER_core1()
#endif

#ifndef portSOFTWARE_BARRIER_core1
    #define portSOFTWARE_BARRIER_core1()
#endif

/* The timers_core1 module relies on xTaskGetSchedulerState_core1(). */
#if configUSE_TIMERS_core1 == 1

    #ifndef configTIMER_TASK_PRIORITY_core1
        #error If configUSE_TIMERS_core1 is set to 1 then configTIMER_TASK_PRIORITY_core1 must also be defined.
    #endif /* configTIMER_TASK_PRIORITY_core1 */

    #ifndef configTIMER_QUEUE_LENGTH_core1
        #error If configUSE_TIMERS_core1 is set to 1 then configTIMER_QUEUE_LENGTH_core1 must also be defined.
    #endif /* configTIMER_QUEUE_LENGTH_core1 */

    #ifndef configTIMER_TASK_STACK_DEPTH_core1
        #error If configUSE_TIMERS_core1 is set to 1 then configTIMER_TASK_STACK_DEPTH_core1 must also be defined.
    #endif /* configTIMER_TASK_STACK_DEPTH_core1 */

#endif /* configUSE_TIMERS_core1 */

#ifndef portSET_INTERRUPT_MASK_FROM_ISR_core1
    #define portSET_INTERRUPT_MASK_FROM_ISR_core1()    0
#endif

#ifndef portCLEAR_INTERRUPT_MASK_FROM_ISR_core1
    #define portCLEAR_INTERRUPT_MASK_FROM_ISR_core1( uxSavedStatusValue_core1 )    ( void ) ( uxSavedStatusValue_core1 )
#endif

#ifndef portCLEAN_UP_TCB_core1
    #define portCLEAN_UP_TCB_core1( pxTCB_core1 )    ( void ) ( pxTCB_core1 )
#endif

#ifndef portPRE_TASK_DELETE_HOOK_core1
    #define portPRE_TASK_DELETE_HOOK_core1( pvTaskToDelete_core1, pxYieldPending_core1 )
#endif

#ifndef portSETUP_TCB_core1
    #define portSETUP_TCB_core1( pxTCB_core1 )    ( void ) ( pxTCB_core1 )
#endif

#ifndef configQUEUE_REGISTRY_SIZE_core1
    #define configQUEUE_REGISTRY_SIZE_core1    0U
#endif

#if ( configQUEUE_REGISTRY_SIZE_core1 < 1 )
    #define vQueueAddToRegistry_core1( xQueue_core1, pcName_core1 )
    #define vQueueUnregisterQueue_core1( xQueue_core1 )
    #define pcQueueGetName_core1( xQueue_core1 )
#endif

#ifndef configUSE_MINI_LIST_ITEM_core1
    #define configUSE_MINI_LIST_ITEM_core1    1
#endif

#ifndef portPOINTER_SIZE_TYPE_core1
    #define portPOINTER_SIZE_TYPE_core1    uint32_t
#endif

/* Remove any unused trace macros. */
#ifndef traceSTART_core1

/* Used to perform any necessary initialisation - for example, open a file
 * into which trace is to be written. */
    #define traceSTART_core1()
#endif

#ifndef traceEND_core1

/* Use to close a trace, for example close a file into which trace has been
 * written. */
    #define traceEND_core1()
#endif

#ifndef traceTASK_SWITCHED_IN_core1

/* Called after a task_core1 has been selected to run.  pxCurrentTCB_core1 holds a pointer
 * to the task_core1 control block of the selected task_core1. */
    #define traceTASK_SWITCHED_IN_core1()
#endif

#ifndef traceINCREASE_TICK_COUNT_core1

/* Called before stepping the tick count after waking from tickless idle
 * sleep. */
    #define traceINCREASE_TICK_COUNT_core1( x_core1 )
#endif

#ifndef traceLOW_POWER_IDLE_BEGIN_core1
    /* Called immediately before entering tickless idle. */
    #define traceLOW_POWER_IDLE_BEGIN_core1()
#endif

#ifndef traceLOW_POWER_IDLE_END_core1
    /* Called when returning to the Idle task_core1 after a tickless idle. */
    #define traceLOW_POWER_IDLE_END_core1()
#endif

#ifndef traceTASK_SWITCHED_OUT_core1

/* Called before a task_core1 has been selected to run.  pxCurrentTCB_core1 holds a pointer
 * to the task_core1 control block of the task_core1 being switched out. */
    #define traceTASK_SWITCHED_OUT_core1()
#endif

#ifndef traceTASK_PRIORITY_INHERIT_core1

/* Called when a task_core1 attempts to take a mutex that is already held by a
 * lower priority task_core1.  pxTCBOfMutexHolder_core1 is a pointer to the TCB of the task_core1
 * that holds the mutex.  uxInheritedPriority is the priority the mutex holder
 * will inherit (the priority of the task_core1 that is attempting to obtain the
 * muted. */
    #define traceTASK_PRIORITY_INHERIT_core1( pxTCBOfMutexHolder_core1, uxInheritedPriority )
#endif

#ifndef traceTASK_PRIORITY_DISINHERIT_core1

/* Called when a task_core1 releases a mutex, the holding of which had resulted in
 * the task_core1 inheriting the priority of a higher priority task_core1.
 * pxTCBOfMutexHolder_core1 is a pointer to the TCB of the task_core1 that is releasing the
 * mutex.  uxOriginalPriority_core1 is the task_core1's configured (base) priority. */
    #define traceTASK_PRIORITY_DISINHERIT_core1( pxTCBOfMutexHolder_core1, uxOriginalPriority_core1 )
#endif

#ifndef traceBLOCKING_ON_QUEUE_RECEIVE_core1

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue_core1 is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB_core1 points to the TCB of the
 * task_core1 that attempted the read. */
    #define traceBLOCKING_ON_QUEUE_RECEIVE_core1( pxQueue_core1 )
#endif

#ifndef traceBLOCKING_ON_QUEUE_PEEK_core1

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue_core1 is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB_core1 points to the TCB of the
 * task_core1 that attempted the read. */
    #define traceBLOCKING_ON_QUEUE_PEEK_core1( pxQueue_core1 )
#endif

#ifndef traceBLOCKING_ON_QUEUE_SEND_core1

/* Task is about to block because it cannot write to a
 * queue/mutex/semaphore.  pxQueue_core1 is a pointer to the queue/mutex/semaphore
 * upon which the write was attempted.  pxCurrentTCB_core1 points to the TCB of the
 * task_core1 that attempted the write. */
    #define traceBLOCKING_ON_QUEUE_SEND_core1( pxQueue_core1 )
#endif

#ifndef configCHECK_FOR_STACK_OVERFLOW_core1
    #define configCHECK_FOR_STACK_OVERFLOW_core1    0
#endif

#ifndef configRECORD_STACK_HIGH_ADDRESS_core1
    #define configRECORD_STACK_HIGH_ADDRESS_core1    0
#endif

#ifndef configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H
    #define configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    0
#endif

/* The following event macros are embedded in the kernel API calls. */

#ifndef traceMOVED_TASK_TO_READY_STATE_core1
    #define traceMOVED_TASK_TO_READY_STATE_core1( pxTCB_core1 )
#endif

#ifndef tracePOST_MOVED_TASK_TO_READY_STATE
    #define tracePOST_MOVED_TASK_TO_READY_STATE( pxTCB_core1 )
#endif

#ifndef traceQUEUE_CREATE_core1
    #define traceQUEUE_CREATE_core1( pxNewQueue_core1 )
#endif

#ifndef traceQUEUE_CREATE_FAILED_core1
    #define traceQUEUE_CREATE_FAILED_core1( ucQueueType_core1 )
#endif

#ifndef traceCREATE_MUTEX_core1
    #define traceCREATE_MUTEX_core1( pxNewQueue_core1 )
#endif

#ifndef traceCREATE_MUTEX_FAILED_core1
    #define traceCREATE_MUTEX_FAILED_core1()
#endif

#ifndef traceGIVE_MUTEX_RECURSIVE_core1
    #define traceGIVE_MUTEX_RECURSIVE_core1( pxMutex_core1 )
#endif

#ifndef traceGIVE_MUTEX_RECURSIVE_FAILED_core1
    #define traceGIVE_MUTEX_RECURSIVE_FAILED_core1( pxMutex_core1 )
#endif

#ifndef traceTAKE_MUTEX_RECURSIVE_core1
    #define traceTAKE_MUTEX_RECURSIVE_core1( pxMutex_core1 )
#endif

#ifndef traceTAKE_MUTEX_RECURSIVE_FAILED_core1
    #define traceTAKE_MUTEX_RECURSIVE_FAILED_core1( pxMutex_core1 )
#endif

#ifndef traceCREATE_COUNTING_SEMAPHORE_core1
    #define traceCREATE_COUNTING_SEMAPHORE_core1()
#endif

#ifndef traceCREATE_COUNTING_SEMAPHORE_FAILED_core1
    #define traceCREATE_COUNTING_SEMAPHORE_FAILED_core1()
#endif

#ifndef traceQUEUE_SET_SEND_core1
    #define traceQUEUE_SET_SEND_core1    traceQUEUE_SEND_core1
#endif

#ifndef traceQUEUE_SEND_core1
    #define traceQUEUE_SEND_core1( pxQueue_core1 )
#endif

#ifndef traceQUEUE_SEND_FAILED_core1
    #define traceQUEUE_SEND_FAILED_core1( pxQueue_core1 )
#endif

#ifndef traceQUEUE_RECEIVE_core1
    #define traceQUEUE_RECEIVE_core1( pxQueue_core1 )
#endif

#ifndef traceQUEUE_PEEK_core1
    #define traceQUEUE_PEEK_core1( pxQueue_core1 )
#endif

#ifndef traceQUEUE_PEEK_FAILED_core1
    #define traceQUEUE_PEEK_FAILED_core1( pxQueue_core1 )
#endif

#ifndef traceQUEUE_PEEK_FROM_ISR_core1
    #define traceQUEUE_PEEK_FROM_ISR_core1( pxQueue_core1 )
#endif

#ifndef traceQUEUE_RECEIVE_FAILED_core1
    #define traceQUEUE_RECEIVE_FAILED_core1( pxQueue_core1 )
#endif

#ifndef traceQUEUE_SEND_FROM_ISR_core1
    #define traceQUEUE_SEND_FROM_ISR_core1( pxQueue_core1 )
#endif

#ifndef traceQUEUE_SEND_FROM_ISR_FAILED_core1
    #define traceQUEUE_SEND_FROM_ISR_FAILED_core1( pxQueue_core1 )
#endif

#ifndef traceQUEUE_RECEIVE_FROM_ISR_core1
    #define traceQUEUE_RECEIVE_FROM_ISR_core1( pxQueue_core1 )
#endif

#ifndef traceQUEUE_RECEIVE_FROM_ISR_FAILED_core1
    #define traceQUEUE_RECEIVE_FROM_ISR_FAILED_core1( pxQueue_core1 )
#endif

#ifndef traceQUEUE_PEEK_FROM_ISR_FAILED_core1
    #define traceQUEUE_PEEK_FROM_ISR_FAILED_core1( pxQueue_core1 )
#endif

#ifndef traceQUEUE_DELETE_core1
    #define traceQUEUE_DELETE_core1( pxQueue_core1 )
#endif

#ifndef traceTASK_CREATE_core1
    #define traceTASK_CREATE_core1( pxNewTCB_core1 )
#endif

#ifndef traceTASK_CREATE_FAILED_core1
    #define traceTASK_CREATE_FAILED_core1()
#endif

#ifndef traceTASK_DELETE_core1
    #define traceTASK_DELETE_core1( pxTaskToDelete )
#endif

#ifndef traceTASK_DELAY_UNTIL_core1
    #define traceTASK_DELAY_UNTIL_core1( x_core1 )
#endif

#ifndef traceTASK_DELAY_core1
    #define traceTASK_DELAY_core1()
#endif

#ifndef traceTASK_PRIORITY_SET_core1
    #define traceTASK_PRIORITY_SET_core1( pxTask, uxNewPriority_core1 )
#endif

#ifndef traceTASK_SUSPEND_core1
    #define traceTASK_SUSPEND_core1( pxTaskToSuspend )
#endif

#ifndef traceTASK_RESUME_core1
    #define traceTASK_RESUME_core1( pxTaskToResume )
#endif

#ifndef traceTASK_RESUME_FROM_ISR_core1
    #define traceTASK_RESUME_FROM_ISR_core1( pxTaskToResume )
#endif

#ifndef traceTASK_INCREMENT_TICK_core1
    #define traceTASK_INCREMENT_TICK_core1( xTickCount_core1 )
#endif

#ifndef traceTIMER_CREATE_core1
    #define traceTIMER_CREATE_core1( pxNewTimer_core1 )
#endif

#ifndef traceTIMER_CREATE_FAILED_core1
    #define traceTIMER_CREATE_FAILED_core1()
#endif

#ifndef traceTIMER_COMMAND_SEND_core1
    #define traceTIMER_COMMAND_SEND_core1( xTimer_core1, xMessageID_core1, xMessageValueValue_core1, xReturn_core1 )
#endif

#ifndef traceTIMER_EXPIRED_core1
    #define traceTIMER_EXPIRED_core1( pxTimer_core1 )
#endif

#ifndef traceTIMER_COMMAND_RECEIVED_core1
    #define traceTIMER_COMMAND_RECEIVED_core1( pxTimer_core1, xMessageID_core1, xMessageValue_core1 )
#endif

#ifndef traceMALLOC_core1
    #define traceMALLOC_core1( pvAddress_core1, uiSize_core1 )
#endif

#ifndef traceFREE_core1
    #define traceFREE_core1( pvAddress_core1, uiSize_core1 )
#endif

#ifndef traceEVENT_GROUP_CREATE_core1
    #define traceEVENT_GROUP_CREATE_core1( xEventGroup_core1 )
#endif

#ifndef traceEVENT_GROUP_CREATE_FAILED_core1
    #define traceEVENT_GROUP_CREATE_FAILED_core1()
#endif

#ifndef traceEVENT_GROUP_SYNC_BLOCK_core1
    #define traceEVENT_GROUP_SYNC_BLOCK_core1( xEventGroup_core1, uxBitsToSet_core1, uxBitsToWaitFor_core1 )
#endif

#ifndef traceEVENT_GROUP_SYNC_END_core1
    #define traceEVENT_GROUP_SYNC_END_core1( xEventGroup_core1, uxBitsToSet_core1, uxBitsToWaitFor_core1, xTimeoutOccurred_core1 )    ( void ) ( xTimeoutOccurred_core1 )
#endif

#ifndef traceEVENT_GROUP_WAIT_BITS_BLOCK_core1
    #define traceEVENT_GROUP_WAIT_BITS_BLOCK_core1( xEventGroup_core1, uxBitsToWaitFor_core1 )
#endif

#ifndef traceEVENT_GROUP_WAIT_BITS_END_core1
    #define traceEVENT_GROUP_WAIT_BITS_END_core1( xEventGroup_core1, uxBitsToWaitFor_core1, xTimeoutOccurred_core1 )    ( void ) ( xTimeoutOccurred_core1 )
#endif

#ifndef traceEVENT_GROUP_CLEAR_BITS_core1
    #define traceEVENT_GROUP_CLEAR_BITS_core1( xEventGroup_core1, uxBitsToClear_core1 )
#endif

#ifndef traceEVENT_GROUP_CLEAR_BITS_FROM_ISR_core1
    #define traceEVENT_GROUP_CLEAR_BITS_FROM_ISR_core1( xEventGroup_core1, uxBitsToClear_core1 )
#endif

#ifndef traceEVENT_GROUP_SET_BITS_core1
    #define traceEVENT_GROUP_SET_BITS_core1( xEventGroup_core1, uxBitsToSet_core1 )
#endif

#ifndef traceEVENT_GROUP_SET_BITS_FROM_ISR_core1
    #define traceEVENT_GROUP_SET_BITS_FROM_ISR_core1( xEventGroup_core1, uxBitsToSet_core1 )
#endif

#ifndef traceEVENT_GROUP_DELETE_core1
    #define traceEVENT_GROUP_DELETE_core1( xEventGroup_core1 )
#endif

#ifndef tracePEND_FUNC_CALL_core1
    #define tracePEND_FUNC_CALL_core1( xFunctionToPend_core1, pvParameter1_core1, ulParameter2_core1, ret_core1 )
#endif

#ifndef tracePEND_FUNC_CALL_FROM_ISR_core1
    #define tracePEND_FUNC_CALL_FROM_ISR_core1( xFunctionToPend_core1, pvParameter1_core1, ulParameter2_core1, ret_core1 )
#endif

#ifndef traceQUEUE_REGISTRY_ADD_core1
    #define traceQUEUE_REGISTRY_ADD_core1( xQueue_core1, pcQueueName_core1 )
#endif

#ifndef traceTASK_NOTIFY_TAKE_BLOCK_core1
    #define traceTASK_NOTIFY_TAKE_BLOCK_core1( uxIndexToWait_core1 )
#endif

#ifndef traceTASK_NOTIFY_TAKE_core1
    #define traceTASK_NOTIFY_TAKE_core1( uxIndexToWait_core1 )
#endif

#ifndef traceTASK_NOTIFY_WAIT_BLOCK_core1
    #define traceTASK_NOTIFY_WAIT_BLOCK_core1( uxIndexToWait_core1 )
#endif

#ifndef traceTASK_NOTIFY_WAIT_core1
    #define traceTASK_NOTIFY_WAIT_core1( uxIndexToWait_core1 )
#endif

#ifndef traceTASK_NOTIFY_core1
    #define traceTASK_NOTIFY_core1( uxIndexToNotify_core1 )
#endif

#ifndef traceTASK_NOTIFY_FROM_ISR_core1
    #define traceTASK_NOTIFY_FROM_ISR_core1( uxIndexToNotify_core1 )
#endif

#ifndef traceTASK_NOTIFY_GIVE_FROM_ISR_core1
    #define traceTASK_NOTIFY_GIVE_FROM_ISR_core1( uxIndexToNotify_core1 )
#endif

#ifndef traceSTREAM_BUFFER_CREATE_FAILED_core1
    #define traceSTREAM_BUFFER_CREATE_FAILED_core1( xIsMessageBuffer_core1 )
#endif

#ifndef traceSTREAM_BUFFER_CREATE_STATIC_FAILED_core1
    #define traceSTREAM_BUFFER_CREATE_STATIC_FAILED_core1( xReturn_core1, xIsMessageBuffer_core1 )
#endif

#ifndef traceSTREAM_BUFFER_CREATE_core1
    #define traceSTREAM_BUFFER_CREATE_core1( pxStreamBuffer_core1, xIsMessageBuffer_core1 )
#endif

#ifndef traceSTREAM_BUFFER_DELETE_core1
    #define traceSTREAM_BUFFER_DELETE_core1( xStreamBuffer_core1 )
#endif

#ifndef traceSTREAM_BUFFER_RESET_core1
    #define traceSTREAM_BUFFER_RESET_core1( xStreamBuffer_core1 )
#endif

#ifndef traceBLOCKING_ON_STREAM_BUFFER_SEND_core1
    #define traceBLOCKING_ON_STREAM_BUFFER_SEND_core1( xStreamBuffer_core1 )
#endif

#ifndef traceSTREAM_BUFFER_SEND_core1
    #define traceSTREAM_BUFFER_SEND_core1( xStreamBuffer_core1, xBytesSent_core1 )
#endif

#ifndef traceSTREAM_BUFFER_SEND_FAILED_core1
    #define traceSTREAM_BUFFER_SEND_FAILED_core1( xStreamBuffer_core1 )
#endif

#ifndef traceSTREAM_BUFFER_SEND_FROM_ISR_core1
    #define traceSTREAM_BUFFER_SEND_FROM_ISR_core1( xStreamBuffer_core1, xBytesSent_core1 )
#endif

#ifndef traceBLOCKING_ON_STREAM_BUFFER_RECEIVE_core1
    #define traceBLOCKING_ON_STREAM_BUFFER_RECEIVE_core1( xStreamBuffer_core1 )
#endif

#ifndef traceSTREAM_BUFFER_RECEIVE_core1
    #define traceSTREAM_BUFFER_RECEIVE_core1( xStreamBuffer_core1, xReceivedLength_core1 )
#endif

#ifndef traceSTREAM_BUFFER_RECEIVE_FAILED_core1
    #define traceSTREAM_BUFFER_RECEIVE_FAILED_core1( xStreamBuffer_core1 )
#endif

#ifndef traceSTREAM_BUFFER_RECEIVE_FROM_ISR_core1
    #define traceSTREAM_BUFFER_RECEIVE_FROM_ISR_core1( xStreamBuffer_core1, xReceivedLength_core1 )
#endif

#ifndef configGENERATE_RUN_TIME_STATS_core1
    #define configGENERATE_RUN_TIME_STATS_core1   0
#endif

#if ( configGENERATE_RUN_TIME_STATS_core1 == 1 )

    #ifndef portCONFIGURE_TIMER_FOR_RUN_TIME_STATS_core1
        #error If configGENERATE_RUN_TIME_STATS_core1 is defined then portCONFIGURE_TIMER_FOR_RUN_TIME_STATS_core1 must also be defined.  portCONFIGURE_TIMER_FOR_RUN_TIME_STATS_core1 should call a port layer function to setup a peripheral timer/counter that can then be used as the run time counter time base.
    #endif /* portCONFIGURE_TIMER_FOR_RUN_TIME_STATS_core1 */

    #ifndef portGET_RUN_TIME_COUNTER_VALUE_core1
        #ifndef portALT_GET_RUN_TIME_COUNTER_VALUE_core1
            #error If configGENERATE_RUN_TIME_STATS_core1 is defined then either portGET_RUN_TIME_COUNTER_VALUE_core1 or portALT_GET_RUN_TIME_COUNTER_VALUE_core1 must also be defined.  See the examples provided and the FreeRTOS_core1 web site for more information.
        #endif /* portALT_GET_RUN_TIME_COUNTER_VALUE_core1 */
    #endif /* portGET_RUN_TIME_COUNTER_VALUE_core1 */

#endif /* configGENERATE_RUN_TIME_STATS_core1 */

#ifndef portCONFIGURE_TIMER_FOR_RUN_TIME_STATS_core1

#endif

#ifndef configUSE_MALLOC_FAILED_HOOK_core1
    #define configUSE_MALLOC_FAILED_HOOK_core1    0
#endif

#ifndef portPRIVILEGE_BIT_core1
    #define portPRIVILEGE_BIT_core1    ( ( UBaseType_t_core1 ) 0x00 )
#endif

#ifndef portYIELD_WITHIN_API_core1
    #define portYIELD_WITHIN_API_core1    portYIELD_core1
#endif

#ifndef portSUPPRESS_TICKS_AND_SLEEP_core1
    #define portSUPPRESS_TICKS_AND_SLEEP_core1( xExpectedIdleTime_core1 )
#endif

#ifndef configEXPECTED_IDLE_TIME_BEFORE_SLEEP_core1
    #define configEXPECTED_IDLE_TIME_BEFORE_SLEEP_core1    2
#endif

#if configEXPECTED_IDLE_TIME_BEFORE_SLEEP_core1 < 2
    #error configEXPECTED_IDLE_TIME_BEFORE_SLEEP_core1 must not be less than 2
#endif

#ifndef configUSE_TICKLESS_IDLE_core1
    #define configUSE_TICKLESS_IDLE_core1    0
#endif

#ifndef configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING_core1
    #define configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING_core1( x_core1 )
#endif

#ifndef configPRE_SLEEP_PROCESSING_core1
    #define configPRE_SLEEP_PROCESSING_core1( x_core1 )
#endif

#ifndef configPOST_SLEEP_PROCESSING_core1
    #define configPOST_SLEEP_PROCESSING_core1( x_core1 )
#endif

#ifndef configUSE_QUEUE_SETS_core1
    #define configUSE_QUEUE_SETS_core1    0
#endif

#ifndef portTASK_USES_FLOATING_POINT_core1
    #define portTASK_USES_FLOATING_POINT_core1()
#endif

#ifndef portALLOCATE_SECURE_CONTEXT_core1
    #define portALLOCATE_SECURE_CONTEXT_core1( ulSecureStackSize_core1 )
#endif

#ifndef portDONT_DISCARD_core1
    #define portDONT_DISCARD_core1
#endif

#ifndef configUSE_TIME_SLICING_core1
    #define configUSE_TIME_SLICING_core1    1
#endif

#ifndef configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS_core1
    #define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS_core1    0
#endif

#ifndef configUSE_STATS_FORMATTING_FUNCTIONS_core1
    #define configUSE_STATS_FORMATTING_FUNCTIONS_core1    0
#endif

#ifndef portASSERT_IF_INTERRUPT_PRIORITY_INVALID_core1
    #define portASSERT_IF_INTERRUPT_PRIORITY_INVALID_core1()
#endif

#ifndef configUSE_TRACE_FACILITY_core1
    #define configUSE_TRACE_FACILITY_core1    0
#endif

#ifndef mtCOVERAGE_TEST_MARKER_core1
    #define mtCOVERAGE_TEST_MARKER_core1()
#endif

#ifndef mtCOVERAGE_TEST_DELAY_core1
    #define mtCOVERAGE_TEST_DELAY_core1()
#endif

#ifndef portASSERT_IF_IN_ISR_core1
    #define portASSERT_IF_IN_ISR_core1()
#endif

#ifndef configUSE_PORT_OPTIMISED_TASK_SELECTION_core1
    #define configUSE_PORT_OPTIMISED_TASK_SELECTION_core1    0
#endif

#ifndef configAPPLICATION_ALLOCATED_HEAP_core1
    #define configAPPLICATION_ALLOCATED_HEAP_core1    0
#endif

#ifndef configUSE_TASK_NOTIFICATIONS_core1
    #define configUSE_TASK_NOTIFICATIONS_core1    1
#endif

#ifndef configTASK_NOTIFICATION_ARRAY_ENTRIES_core1
    #define configTASK_NOTIFICATION_ARRAY_ENTRIES_core1    1
#endif

#if configTASK_NOTIFICATION_ARRAY_ENTRIES_core1 < 1
    #error configTASK_NOTIFICATION_ARRAY_ENTRIES_core1 must be at least 1
#endif

#ifndef configUSE_POSIX_ERRNO_core1
    #define configUSE_POSIX_ERRNO_core1    0
#endif

#ifndef configUSE_SB_COMPLETED_CALLBACK_core1

/* By default per-instance callbacks are not enabled for stream buffer or message buffer. */
    #define configUSE_SB_COMPLETED_CALLBACK_core1    0
#endif

#ifndef portTICK_TYPE_IS_ATOMIC_core1
    #define portTICK_TYPE_IS_ATOMIC_core1    0
#endif

#ifndef configSUPPORT_STATIC_ALLOCATION_core1
    /* Defaults to 0 for backward compatibility. */
    #define configSUPPORT_STATIC_ALLOCATION_core1    0
#endif

#ifndef configSUPPORT_DYNAMIC_ALLOCATION_core1
    /* Defaults to 1 for backward compatibility. */
    #define configSUPPORT_DYNAMIC_ALLOCATION_core1    1
#endif

#if ( ( configUSE_STATS_FORMATTING_FUNCTIONS_core1 > 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION_core1 != 1 ) )
    #error configUSE_STATS_FORMATTING_FUNCTIONS_core1 cannot be used without dynamic allocation, but configSUPPORT_DYNAMIC_ALLOCATION_core1 is not set to 1.
#endif

#if ( configUSE_STATS_FORMATTING_FUNCTIONS_core1 > 0 )
    #if ( ( configUSE_TRACE_FACILITY_core1 != 1 ) && ( configGENERATE_RUN_TIME_STATS_core1 != 1 ) )
        #error configUSE_STATS_FORMATTING_FUNCTIONS_core1 is 1 but the functions it enables are not used because neither configUSE_TRACE_FACILITY_core1 or configGENERATE_RUN_TIME_STATS_core1 are 1.  Set configUSE_STATS_FORMATTING_FUNCTIONS_core1 to 0 in FreeRTOSConfig.h.
    #endif
#endif

#ifndef configSTACK_DEPTH_TYPE_core1

/* Defaults to uint16_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if uint16_t is too restrictive. */
    #define configSTACK_DEPTH_TYPE_core1    uint32_t
#endif

#ifndef configRUN_TIME_COUNTER_TYPE_core1

/* Defaults to uint32_t for backward compatibility, but can be overridden in
 * FreeRTOSConfig.h if uint32_t is too restrictive. */

    #define configRUN_TIME_COUNTER_TYPE_core1    uint32_t
#endif

#ifndef configMESSAGE_BUFFER_LENGTH_TYPE_core1

/* Defaults to size_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if lengths will always be less than the number of bytes
 * in a size_t. */
    #define configMESSAGE_BUFFER_LENGTH_TYPE_core1    size_t
#endif

/* Sanity check the configuration. */
#if ( ( configSUPPORT_STATIC_ALLOCATION_core1 == 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION_core1 == 0 ) )
    #error configSUPPORT_STATIC_ALLOCATION_core1 and configSUPPORT_DYNAMIC_ALLOCATION_core1 cannot both be 0, but can both be 1.
#endif

#if ( ( configUSE_RECURSIVE_MUTEXES_core1 == 1 ) && ( configUSE_MUTEXES_core1 != 1 ) )
    #error configUSE_MUTEXES_core1 must be set to 1 to use recursive mutexes
#endif

#ifndef configINITIAL_TICK_COUNT_core1
    #define configINITIAL_TICK_COUNT_core1    0
#endif

#if ( portTICK_TYPE_IS_ATOMIC_core1 == 0 )

/* Either variables of tick type cannot be read atomically, or
 * portTICK_TYPE_IS_ATOMIC_core1 was not set - map the critical sections used when
 * the tick count is returned to the standard critical section macros. */
    #define portTICK_TYPE_ENTER_CRITICAL_core1()                      portENTER_CRITICAL_core1()
    #define portTICK_TYPE_EXIT_CRITICAL_core1()                       portEXIT_CRITICAL_core1()
    #define portTICK_TYPE_SET_INTERRUPT_MASK_FROM_ISR_core1()         portSET_INTERRUPT_MASK_FROM_ISR_core1()
    #define portTICK_TYPE_CLEAR_INTERRUPT_MASK_FROM_ISR_core1( x_core1 )    portCLEAR_INTERRUPT_MASK_FROM_ISR_core1( ( x_core1 ) )
#else

/* The tick type can be read atomically, so critical sections used when the
 * tick count is returned can be defined away. */
    #define portTICK_TYPE_ENTER_CRITICAL_core1()
    #define portTICK_TYPE_EXIT_CRITICAL_core1()
    #define portTICK_TYPE_SET_INTERRUPT_MASK_FROM_ISR_core1()         0
    #define portTICK_TYPE_CLEAR_INTERRUPT_MASK_FROM_ISR_core1( x_core1 )    ( void ) ( x_core1 )
#endif /* if ( portTICK_TYPE_IS_ATOMIC_core1 == 0 ) */

/* Definitions to allow backward compatibility with FreeRTOS_core1 versions prior to
 * V8 if desired. */
#ifndef configENABLE_BACKWARD_COMPATIBILITY_core1
    #define configENABLE_BACKWARD_COMPATIBILITY_core1    1
#endif

#ifndef configPRINTF_core1

/* configPRINTF_core1() was not defined, so define it away to nothing.  To use
 * configPRINTF_core1() then define it as follows (where MyPrintFunction() is
 * provided by the application writer):
 *
 * void MyPrintFunction(const char *pcFormat, ... );
 #define configPRINTF_core1( X )   MyPrintFunction X
 *
 * Then call like a standard printf() function, but placing brackets around
 * all parameters so they are passed as a single parameter.  For example:
 * configPRINTF_core1( ("Value = %d", MyVariable) ); */
    #define configPRINTF_core1( X )
#endif

#ifndef configMAX_core1

/* The application writer has not provided their own MAX macro, so define
 * the following generic implementation. */
    #define configMAX_core1( a, b )    ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#endif

#ifndef configMIN_core1

/* The application writer has not provided their own MIN macro, so define
 * the following generic implementation. */
    #define configMIN_core1( a, b )    ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#endif

#if configENABLE_BACKWARD_COMPATIBILITY_core1 == 1
    #define eTaskStateGet_core1                 eTaskGetState_core1
    #define portTickType_core1                  TickType_t_core1
    #define xTaskHandle_core1                   TaskHandle_t_core1
    #define xQueueHandle_core1                  QueueHandle_t_core1
    #define xSemaphoreHandle_core1              SemaphoreHandle_t_core1
    #define xQueueSetHandle_core1               QueueSetHandle_t_core1
    #define xQueueSetMemberHandle_core1         QueueSetMemberHandle_t_core1
    #define xTimeOutType_core1                  TimeOut_t_core1
    #define xMemoryRegion_core1                 MemoryRegion_t_core1
    #define xTaskParameters_core1               TaskParameters_t_core1
    #define xTaskStatusType_core1               TaskStatus_t_core1
    #define xTimerHandle_core1                  TimerHandle_t_core1
    #define xCoRoutineHandle_core1              CoRoutineHandle_t_core1
    #define pdTASK_HOOK_CODE_core1              TaskHookFunction_t_core1
    #define portTICK_RATE_MS_core1              portTICK_PERIOD_MS_core1
    #define pcTaskGetTaskName_core1             pcTaskGetName_core1
    #define pcTimerGetTimerName_core1           pcTimerGetName_core1
    #define pcQueueGetQueueName_core1           pcQueueGetName_core1
    #define vTaskGetTaskInfo_core1              vTaskGetInfo_core1
    #define xTaskGetIdleRunTimeCounter_core1    ulTaskGetIdleRunTimeCounter_core1

/* Backward compatibility within the scheduler code only - these definitions
 * are not really required but are included for completeness. */
    #define tmrTIMER_CALLBACK_core1             TimerCallbackFunction_t_core1
    #define pdTASK_CODE_core1                   TaskFunction_t_core1
    #define xListItem_core1                     ListItem_t_core1
    #define xList_core1                         List_t_core1

/* For libraries that break the list data hiding, and access list structure
 * members directly (which is not supposed to be done). */
    #define pxContainer_core1                   pvContainer_core1
#endif /* configENABLE_BACKWARD_COMPATIBILITY_core1 */

#if ( configUSE_ALTERNATIVE_API_core1 != 0 )
    #error The alternative API was deprecated some time ago, and was removed in FreeRTOS_core1 V9.0 0
#endif

/* Set configUSE_TASK_FPU_SUPPORT to 0 to omit floating point support even
 * if floating point hardware is otherwise supported by the FreeRTOS_core1 port in use.
 * This constant is not supported by all FreeRTOS_core1 ports that include floating
 * point support. */
#ifndef configUSE_TASK_FPU_SUPPORT_core1
    #define configUSE_TASK_FPU_SUPPORT_core1    0
#endif

/* Set configENABLE_MPU to 1 to enable MPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#ifndef configENABLE_MPU_core1
    #define configENABLE_MPU_core1    0
#endif

/* Set configENABLE_FPU to 1 to enable FPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#ifndef configENABLE_FPU_core1
    #define configENABLE_FPU_core1    0
#endif

/* Set configENABLE_MVE to 1 to enable MVE support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#ifndef configENABLE_MVE_core1
    #define configENABLE_MVE_core1   1
#endif

/* Set configENABLE_TRUSTZONE to 1 enable TrustZone support and 0 to disable it.
 * This is currently used in ARMv8M ports. */
#ifndef configENABLE_TRUSTZONE_core1
    #define configENABLE_TRUSTZONE_core1    1
#endif

/* Set configRUN_FREERTOS_SECURE_ONLY to 1 to run the FreeRTOS_core1 ARMv8M port on
 * the Secure Side only. */
#ifndef configRUN_FREERTOS_SECURE_ONLY_core1
    #define configRUN_FREERTOS_SECURE_ONLY_core1    0
#endif

#ifndef configRUN_ADDITIONAL_TESTS_core1
    #define configRUN_ADDITIONAL_TESTS_core1    0
#endif


/* Sometimes the FreeRTOSConfig.h settings only allow a task_core1 to be created using
 * dynamically allocated RAM, in which case when any task_core1 is deleted it is known
 * that both the task_core1's stack and TCB need to be freed.  Sometimes the
 * FreeRTOSConfig.h settings only allow a task_core1 to be created using statically
 * allocated RAM, in which case when any task_core1 is deleted it is known that neither
 * the task_core1's stack or TCB should be freed.  Sometimes the FreeRTOSConfig.h
 * settings allow a task_core1 to be created using either statically or dynamically
 * allocated RAM, in which case a member of the TCB is used to record whether the
 * stack and/or TCB were allocated statically or dynamically, so when a task_core1 is
 * deleted the RAM that was allocated dynamically is freed again and no attempt is
 * made to free the RAM that was allocated statically.
 * tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE_core1 is only true if it is possible for a
 * task_core1 to be created using either statically or dynamically allocated RAM.  Note
 * that if portUSING_MPU_WRAPPERS_core1 is 1 then a protected task_core1 can be created with
 * a statically allocated stack and a dynamically allocated TCB.
 *
 * The following table lists various combinations of portUSING_MPU_WRAPPERS_core1,
 * configSUPPORT_DYNAMIC_ALLOCATION_core1 and configSUPPORT_STATIC_ALLOCATION_core1 and
 * when it is possible to have both static and dynamic allocation:
 *  +-----+---------+--------+-----------------------------+-----------------------------------+------------------+-----------+
 * | MPU | Dynamic | Static |     Available Functions     |       Possible Allocations        | Both Dynamic and | Need Free |
 * |     |         |        |                             |                                   | Static Possible  |           |
 * +-----+---------+--------+-----------------------------+-----------------------------------+------------------+-----------+
 * | 0   | 0       | 1      | xTaskCreateStatic_core1           | TCB - Static, Stack - Static      | No               | No        |
 * +-----|---------|--------|-----------------------------|-----------------------------------|------------------|-----------|
 * | 0   | 1       | 0      | xTaskCreate_core1                 | TCB - Dynamic, Stack - Dynamic    | No               | Yes       |
 * +-----|---------|--------|-----------------------------|-----------------------------------|------------------|-----------|
 * | 0   | 1       | 1      | xTaskCreate_core1,                | 1. TCB - Dynamic, Stack - Dynamic | Yes              | Yes       |
 * |     |         |        | xTaskCreateStatic_core1           | 2. TCB - Static, Stack - Static   |                  |           |
 * +-----|---------|--------|-----------------------------|-----------------------------------|------------------|-----------|
 * | 1   | 0       | 1      | xTaskCreateStatic_core1,          | TCB - Static, Stack - Static      | No               | No        |
 * |     |         |        | xTaskCreateRestrictedStatic_core1 |                                   |                  |           |
 * +-----|---------|--------|-----------------------------|-----------------------------------|------------------|-----------|
 * | 1   | 1       | 0      | xTaskCreate_core1,                | 1. TCB - Dynamic, Stack - Dynamic | Yes              | Yes       |
 * |     |         |        | xTaskCreateRestricted_core1       | 2. TCB - Dynamic, Stack - Static  |                  |           |
 * +-----|---------|--------|-----------------------------|-----------------------------------|------------------|-----------|
 * | 1   | 1       | 1      | xTaskCreate_core1,                | 1. TCB - Dynamic, Stack - Dynamic | Yes              | Yes       |
 * |     |         |        | xTaskCreateStatic_core1,          | 2. TCB - Dynamic, Stack - Static  |                  |           |
 * |     |         |        | xTaskCreateRestricted_core1,      | 3. TCB - Static, Stack - Static   |                  |           |
 * |     |         |        | xTaskCreateRestrictedStatic_core1 |                                   |                  |           |
 * +-----+---------+--------+-----------------------------+-----------------------------------+------------------+-----------+
 */
#define tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE_core1                                                                                     \
    ( ( ( portUSING_MPU_WRAPPERS_core1 == 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION_core1 == 1 ) && ( configSUPPORT_STATIC_ALLOCATION_core1 == 1 ) ) || \
      ( ( portUSING_MPU_WRAPPERS_core1 == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION_core1 == 1 ) ) )

/*
 * In line with software engineering best practice, FreeRTOS_core1 implements a strict
 * data hiding policy, so the real structures used by FreeRTOS_core1 to maintain the
 * state of tasks, queues, semaphores, etc. are not accessible to the application
 * code.  However, if the application writer wants to statically allocate such
 * an object then the size of the object needs to be known.  Dummy structures
 * that are guaranteed to have the same size and alignment requirements of the
 * real objects are used for this purpose.  The dummy list and list item
 * structures below are used for inclusion in such a dummy structure.
 */
struct xSTATIC_LIST_ITEM_core1
{
    #if ( configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES_core1 == 1 )
        TickType_t_core1 xDummy1_core1;
    #endif
    TickType_t_core1 xDummy2_core1;
    void * pvDummy3_core1[ 4 ];
    #if ( configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES_core1 == 1 )
        TickType_t_core1 xDummy4_core1;
    #endif
};
typedef struct xSTATIC_LIST_ITEM_core1 StaticListItem_t_core1;

#if ( configUSE_MINI_LIST_ITEM_core1 == 1 )
    /* See the comments above the struct xSTATIC_LIST_ITEM_core1 definition. */
    struct xSTATIC_MINI_LIST_ITEM_core1
    {
        #if ( configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES_core1 == 1 )
            TickType_t_core1 xDummy1_core1;
        #endif
        TickType_t_core1 xDummy2_core1;
        void * pvDummy3_core1[ 2 ];
    };
    typedef struct xSTATIC_MINI_LIST_ITEM_core1 StaticMiniListItem_t_core1;
#else /* if ( configUSE_MINI_LIST_ITEM_core1 == 1 ) */
    typedef struct xSTATIC_LIST_ITEM_core1      StaticMiniListItem_t_core1;
#endif /* if ( configUSE_MINI_LIST_ITEM_core1 == 1 ) */

/* See the comments above the struct xSTATIC_LIST_ITEM_core1 definition. */
typedef struct xSTATIC_LIST_core1
{
    #if ( configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES_core1 == 1 )
        TickType_t_core1 xDummy1_core1;
    #endif
    UBaseType_t_core1 uxDummy2_core1;
    void * pvDummy3_core1;
    StaticMiniListItem_t_core1 xDummy4_core1;
    #if ( configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES_core1 == 1 )
        TickType_t_core1 xDummy5_core1;
    #endif
} StaticList_t_core1;

/*
 * In line with software engineering best practice, especially when supplying a
 * library that is likely to change in future versions, FreeRTOS_core1 implements a
 * strict data hiding policy.  This means the Task structure used internally by
 * FreeRTOS_core1 is not accessible to application code.  However, if the application
 * writer wants to statically allocate the memory required to create a task_core1 then
 * the size of the task_core1 object needs to be known.  The StaticTask_t_core1 structure
 * below is provided for this purpose.  Its sizes and alignment requirements are
 * guaranteed to match those of the genuine structure, no matter which
 * architecture is being used, and no matter how the values in FreeRTOSConfig.h
 * are set.  Its contents are somewhat obfuscated in the hope users will
 * recognise that it would be unwise to make direct use of the structure members.
 */
typedef struct xSTATIC_TCB_core1
{
    void * pxDummy1;
    #if ( portUSING_MPU_WRAPPERS_core1 == 1 )
        xMPU_SETTINGS_core1 xDummy2_core1;
    #endif
    StaticListItem_t_core1 xDummy3_core1[ 2 ];
    UBaseType_t_core1 uxDummy5_core1;
    void * pxDummy6_core1;
    uint8_t ucDummy7_core1[ configMAX_TASK_NAME_LEN_core1 ];
    #if ( ( portSTACK_GROWTH_core1 > 0 ) || ( configRECORD_STACK_HIGH_ADDRESS_core1 == 1 ) )
        void * pxDummy8_core1;
    #endif
    #if ( portCRITICAL_NESTING_IN_TCB_core1 == 1 )
        UBaseType_t_core1 uxDummy9_core1;
    #endif
    #if ( configUSE_TRACE_FACILITY_core1 == 1 )
        UBaseType_t_core1 uxDummy10_core1[ 2 ];
    #endif
    #if ( configUSE_MUTEXES_core1 == 1 )
        UBaseType_t_core1 uxDummy12_core1[ 2 ];
    #endif
    #if ( configUSE_APPLICATION_TASK_TAG_core1 == 1 )
        void * pxDummy14_core1;
    #endif
    #if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS_core1 > 0 )
        void * pvDummy15_core1[ configNUM_THREAD_LOCAL_STORAGE_POINTERS_core1 ];
    #endif
    #if ( configGENERATE_RUN_TIME_STATS_core1 == 1 )
        configRUN_TIME_COUNTER_TYPE_core1 ulDummy16_core1;
    #endif
    #if ( ( configUSE_NEWLIB_REENTRANT_core1 == 1 ) || ( configUSE_C_RUNTIME_TLS_SUPPORT_core1 == 1 ) )
        configTLS_BLOCK_TYPE_core1 xDummy17_core1;
    #endif
    #if ( configUSE_TASK_NOTIFICATIONS_core1 == 1 )
        uint32_t ulDummy18_core1[ configTASK_NOTIFICATION_ARRAY_ENTRIES_core1 ];
        uint8_t ucDummy19_core1[ configTASK_NOTIFICATION_ARRAY_ENTRIES_core1 ];
    #endif
    #if ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE_core1 != 0 )
        uint8_t uxDummy20_core1;
    #endif

    #if ( INCLUDE_xTaskAbortDelay_core1 == 1 )
        uint8_t ucDummy21_core1;
    #endif
    #if ( configUSE_POSIX_ERRNO_core1 == 1 )
        int iDummy22_core1;
    #endif
} StaticTask_t_core1;

/*
 * In line with software engineering best practice, especially when supplying a
 * library that is likely to change in future versions, FreeRTOS_core1 implements a
 * strict data hiding policy.  This means the Queue structure used internally by
 * FreeRTOS_core1 is not accessible to application code.  However, if the application
 * writer wants to statically allocate the memory required to create a queue
 * then the size of the queue object needs to be known.  The StaticQueue_t_core1
 * structure below is provided for this purpose.  Its sizes and alignment
 * requirements are guaranteed to match those of the genuine structure, no
 * matter which architecture is being used, and no matter how the values in
 * FreeRTOSConfig.h are set.  Its contents are somewhat obfuscated in the hope
 * users will recognise that it would be unwise to make direct use of the
 * structure members.
 */
typedef struct xSTATIC_QUEUE_core1
{
    void * pvDummy1_core1[ 3 ];

    union
    {
        void * pvDummy2_core1;
        UBaseType_t_core1 uxDummy2_core1;
    } u;

    StaticList_t_core1 xDummy3_core1[ 2 ];
    UBaseType_t_core1 uxDummy4_core1[ 3 ];
    uint8_t ucDummy5_core1[ 2 ];

    #if ( ( configSUPPORT_STATIC_ALLOCATION_core1 == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION_core1 == 1 ) )
        uint8_t ucDummy6_core1;
    #endif

    #if ( configUSE_QUEUE_SETS_core1 == 1 )
        void * pvDummy7_core1;
    #endif

    #if ( configUSE_TRACE_FACILITY_core1 == 1 )
        UBaseType_t_core1 uxDummy8_core1;
        uint8_t ucDummy9_core1;
    #endif
} StaticQueue_t_core1;
typedef StaticQueue_t_core1 StaticSemaphore_t_core1;

/*
 * In line with software engineering best practice, especially when supplying a
 * library that is likely to change in future versions, FreeRTOS_core1 implements a
 * strict data hiding policy.  This means the event group structure used
 * internally by FreeRTOS_core1 is not accessible to application code.  However, if
 * the application writer wants to statically allocate the memory required to
 * create an event group then the size of the event group object needs to be
 * know.  The StaticEventGroup_t_core1 structure below is provided for this purpose.
 * Its sizes and alignment requirements are guaranteed to match those of the
 * genuine structure, no matter which architecture is being used, and no matter
 * how the values in FreeRTOSConfig.h are set.  Its contents are somewhat
 * obfuscated in the hope users will recognise that it would be unwise to make
 * direct use of the structure members.
 */
typedef struct xSTATIC_EVENT_GROUP_core1
{
    TickType_t_core1 xDummy1_core1;
    StaticList_t_core1 xDummy2_core1;

    #if ( configUSE_TRACE_FACILITY_core1 == 1 )
        UBaseType_t_core1 uxDummy3_core1;
    #endif

    #if ( ( configSUPPORT_STATIC_ALLOCATION_core1 == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION_core1 == 1 ) )
        uint8_t ucDummy4_core1;
    #endif
} StaticEventGroup_t_core1;

/*
 * In line with software engineering best practice, especially when supplying a
 * library that is likely to change in future versions, FreeRTOS_core1 implements a
 * strict data hiding policy.  This means the software timer structure used
 * internally by FreeRTOS_core1 is not accessible to application code.  However, if
 * the application writer wants to statically allocate the memory required to
 * create a software timer then the size of the queue object needs to be known.
 * The StaticTimer_t_core1 structure below is provided for this purpose.  Its sizes
 * and alignment requirements are guaranteed to match those of the genuine
 * structure, no matter which architecture is being used, and no matter how the
 * values in FreeRTOSConfig.h are set.  Its contents are somewhat obfuscated in
 * the hope users will recognise that it would be unwise to make direct use of
 * the structure members.
 */
typedef struct xSTATIC_TIMER_core1
{
    void * pvDummy1_core1;
    StaticListItem_t_core1 xDummy2_core1;
    TickType_t_core1 xDummy3_core1;
    void * pvDummy5_core1;
    TaskFunction_t_core1 pvDummy6_core1;
    #if ( configUSE_TRACE_FACILITY_core1 == 1 )
        UBaseType_t_core1 uxDummy7_core1;
    #endif
    uint8_t ucDummy8_core1;
} StaticTimer_t_core1;

/*
 * In line with software engineering best practice, especially when supplying a
 * library that is likely to change in future versions, FreeRTOS_core1 implements a
 * strict data hiding policy.  This means the stream buffer structure used
 * internally by FreeRTOS_core1 is not accessible to application code.  However, if
 * the application writer wants to statically allocate the memory required to
 * create a stream buffer then the size of the stream buffer object needs to be
 * known.  The StaticStreamBuffer_t_core1 structure below is provided for this
 * purpose.  Its size and alignment requirements are guaranteed to match those
 * of the genuine structure, no matter which architecture is being used, and
 * no matter how the values in FreeRTOSConfig.h are set.  Its contents are
 * somewhat obfuscated in the hope users will recognise that it would be unwise
 * to make direct use of the structure members.
 */
typedef struct xSTATIC_STREAM_BUFFER_core1
{
    size_t uxDummy1_core1[ 4 ];
    void * pvDummy2_core1[ 3 ];
    uint8_t ucDummy3_core1;
    #if ( configUSE_TRACE_FACILITY_core1 == 1 )
        UBaseType_t_core1 uxDummy4_core1;
    #endif
    #if ( configUSE_SB_COMPLETED_CALLBACK_core1 == 1 )
        void * pvDummy5_core1[ 2 ];
    #endif
} StaticStreamBuffer_t_core1;

/* Message buffers are built on stream buffers. */
typedef StaticStreamBuffer_t_core1 StaticMessageBuffer_t_core1;

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* INC_FREERTOS_H */
