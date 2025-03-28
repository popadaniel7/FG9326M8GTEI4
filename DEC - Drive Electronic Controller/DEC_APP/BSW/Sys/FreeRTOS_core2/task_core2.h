/*
 * FreeRTOS_core2 Kernel V10.5.1
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
 * https://www.FreeRTOS_core2.org
 * https://github.com/FreeRTOS_core2
 *
 */
#include "FreeRTOS_core2.h"

#ifndef INC_TASK_CORE2_H
#define INC_TASK_CORE2_H

#ifndef INC_FREERTOS_CORE2_H
    #error "include FreeRTOS_core2.h must appear in source files before include task_core2.h"
#endif

#include "list_core2.h"

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

/*-----------------------------------------------------------
* MACROS AND DEFINITIONS
*----------------------------------------------------------*/

/*
 * If tskKERNEL_VERSION_NUMBER_core2 ends with + it represents the version in development
 * after the numbered release.
 *
 * The tskKERNEL_VERSION_MAJOR_core2, tskKERNEL_VERSION_MINOR_core2, tskKERNEL_VERSION_BUILD_core2
 * values will reflect the last released version number.
 */
#define tskKERNEL_VERSION_NUMBER_core2       "V10.5.1"
#define tskKERNEL_VERSION_MAJOR_core2        10
#define tskKERNEL_VERSION_MINOR_core2        5
#define tskKERNEL_VERSION_BUILD_core2        1

/* MPU region parameters passed in ulParameters_core2
 * of MemoryRegion_t_core2 struct. */
#define tskMPU_REGION_READ_ONLY_core2        ( 1UL << 0UL )
#define tskMPU_REGION_READ_WRITE_core2       ( 1UL << 1UL )
#define tskMPU_REGION_EXECUTE_NEVER_core2    ( 1UL << 2UL )
#define tskMPU_REGION_NORMAL_MEMORY_core2    ( 1UL << 3UL )
#define tskMPU_REGION_DEVICE_MEMORY_core2    ( 1UL << 4UL )

/* The direct to task_core2 notification feature used to have only a single notification
 * per task_core2.  Now there is an array of notifications per task_core2 that is dimensioned by
 * configTASK_NOTIFICATION_ARRAY_ENTRIES_core2.  For backward compatibility, any use of the
 * original direct to task_core2 notification defaults to using the first index in the
 * array. */
#define tskDEFAULT_INDEX_TO_NOTIFY_core2     ( 0 )

/**
 * task_core2. h
 *
 * Type by which tasks are referenced.  For example, a call to xTaskCreate_core2
 * returns (via a pointer parameter) an TaskHandle_t_core2 variable that can then
 * be used as a parameter to vTaskDelete_core2 to delete the task_core2.
 *
 * \defgroup TaskHandle_t_core2 TaskHandle_t_core2
 * \ingroup Tasks
 */
struct tskTaskControlBlock_core2; /* The old naming convention is used to prevent breaking kernel aware debuggers. */
typedef struct tskTaskControlBlock_core2 * TaskHandle_t_core2;

/*
 * Defines the prototype to which the application task_core2 hook function must
 * conform.
 */
typedef BaseType_t_core2 (* TaskHookFunction_t_core2)( void * );

/* Task states returned by eTaskGetState_core2. */
typedef enum
{
    eRunning_core2 = 0, /* A task_core2 is querying the state of itself, so must be running. */
    eReady_core2,       /* The task_core2 being queried is in a ready or pending ready list. */
    eBlocked_core2,     /* The task_core2 being queried is in the Blocked state. */
    eSuspended_core2,   /* The task_core2 being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
    eDeleted_core2,     /* The task_core2 being queried has been deleted, but its TCB has not yet been freed. */
    eInvalid_core2      /* Used as an 'invalid state' value. */
} eTaskState_core2;

/* Actions that can be performed when vTaskNotify() is called. */
typedef enum
{
    eNoAction_core2 = 0,            /* Notify the task_core2 without updating its notify value. */
    eSetBits_core2,                 /* Set bits in the task_core2's notification value. */
    eIncrement_core2,               /* Increment the task_core2's notification value. */
    eSetValueWithOverwrite_core2,   /* Set the task_core2's notification value to a specific value even if the previous value has not yet been read by the task_core2. */
    eSetValueWithoutOverwrite_core2 /* Set the task_core2's notification value if the previous value has been read by the task_core2. */
} eNotifyAction_core2;

/*
 * Used internally only.
 */
typedef struct xTIME_OUT_core2
{
    BaseType_t_core2 xOverflowCount_core2;
    TickType_t_core2 xTimeOnEntering_core2;
} TimeOut_t_core2;

/*
 * Defines the memory ranges allocated to the task_core2 when an MPU is used.
 */
typedef struct xMEMORY_REGION_core2
{
    void * pvBaseAddress_core2;
    uint32_t ulLengthInBytes_core2;
    uint32_t ulParameters_core2;
} MemoryRegion_t_core2;

/*
 * Parameters required to create an MPU protected task_core2.
 */
typedef struct xTASK_PARAMETERS_core2
{
    TaskFunction_t_core2 pvTaskCode_core2;
    const char * pcName_core2; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    uint16_t usStackDepth_core2;
    void * pvParameters_core2;
    UBaseType_t_core2 uxPriority_core2;
    StackType_t_core2 * puxStackBuffer_core2;
    MemoryRegion_t_core2 xRegions_core2[ portNUM_CONFIGURABLE_REGIONS_core2 ];
    #if ( ( portUSING_MPU_WRAPPERS_core2 == 1 ) && ( configSUPPORT_STATIC_ALLOCATION_core2 == 1 ) )
        StaticTask_t_core2 * const pxTaskBuffer_core2;
    #endif
} TaskParameters_t_core2;

/* Used with the uxTaskGetSystemState_core2() function to return the state of each task_core2
 * in the system. */
typedef struct xTASK_STATUS_core2
{
    TaskHandle_t_core2 xHandle_core2;                         /* The handle of the task_core2 to which the rest of the information in the structure relates. */
    const char * pcTaskName_core2;                      	  /* A pointer to the task_core2's name.  This value will be invalid if the task_core2 was deleted since the structure was populated! */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    UBaseType_t_core2 xTaskNumber_core2;                      /* A number unique to the task_core2. */
    eTaskState_core2 eCurrentState_core2;                     /* The state in which the task_core2 existed when the structure was populated. */
    UBaseType_t_core2 uxCurrentPriority_core2;                /* The priority at which the task_core2 was running (may be inherited) when the structure was populated. */
    UBaseType_t_core2 uxBasePriority_core2;                   /* The priority to which the task_core2 will return if the task_core2's current priority has been inherited to avoid unbounded priority inversion when obtaining a mutex.  Only valid if configUSE_MUTEXES_core2 is defined as 1 in FreeRTOSConfig.h. */
    configRUN_TIME_COUNTER_TYPE_core2 ulRunTimeCounter_core2; /* The total run time allocated to the task_core2 so far, as defined by the run time stats clock.  See https://www.FreeRTOS_core2.org/rtos-run-time-stats.html.  Only valid when configGENERATE_RUN_TIME_STATS_core2 is defined as 1 in FreeRTOSConfig.h. */
    StackType_t_core2 * pxStackBase_core2;                    /* Points to the lowest address of the task_core2's stack area. */
    #if ( ( portSTACK_GROWTH_core2 > 0 ) && ( configRECORD_STACK_HIGH_ADDRESS_core2 == 1 ) )
        StackType_t_core2 * pxTopOfStack_core2;               /* Points to the top address of the task_core2's stack area. */
        StackType_t_core2 * pxEndOfStack_core2;               /* Points to the end address of the task_core2's stack area. */
    #endif
    configSTACK_DEPTH_TYPE_core2 usStackHighWaterMark_core2;  /* The minimum amount of stack space that has remained for the task_core2 since the task_core2 was created.  The closer this value is to zero the closer the task_core2 has come to overflowing its stack. */
} TaskStatus_t_core2;

/* Possible return values for eTaskConfirmSleepModeStatus_core2(). */
typedef enum
{
    eAbortSleep_core2 = 0,           /* A task_core2 has been made ready or a context switch pended since portSUPPRESS_TICKS_AND_SLEEP_core2() was called - abort entering a sleep mode. */
    eStandardSleep_core2,            /* Enter a sleep mode that will not last any longer than the expected idle time. */
    #if ( INCLUDE_vTaskSuspend_core2 == 1 )
        eNoTasksWaitingTimeout_core2 /* No tasks are waiting for a timeout so it is safe to enter a sleep mode that can only be exited by an external interrupt. */
    #endif /* INCLUDE_vTaskSuspend_core2 */
} eSleepModeStatus_core2;

/**
 * Defines the priority used by the idle task_core2.  This must not be modified.
 *
 * \ingroup TaskUtils
 */
#define tskIDLE_PRIORITY_core2    ( ( UBaseType_t_core2 ) 0U )

/**
 * task_core2. h
 *
 * Macro for forcing a context switch.
 *
 * \defgroup taskYIELD_core2 taskYIELD_core2
 * \ingroup SchedulerControl
 */
#define taskYIELD_core2()                        portYIELD_core2()

/**
 * task_core2. h
 *
 * Macro to mark the start of a critical code region.  Preemptive context
 * switches cannot occur when in a critical region.
 *
 * NOTE: This may alter the stack (depending on the portable implementation)
 * so must be used with care!
 *
 * \defgroup taskENTER_CRITICAL_core2 taskENTER_CRITICAL_core2
 * \ingroup SchedulerControl
 */
#define taskENTER_CRITICAL_core2()               portENTER_CRITICAL_core2()
#define taskENTER_CRITICAL_FROM_ISR_core2()      portSET_INTERRUPT_MASK_FROM_ISR_core2()

/**
 * task_core2. h
 *
 * Macro to mark the end of a critical code region.  Preemptive context
 * switches cannot occur when in a critical region.
 *
 * NOTE: This may alter the stack (depending on the portable implementation)
 * so must be used with care!
 *
 * \defgroup taskEXIT_CRITICAL_core2 taskEXIT_CRITICAL_core2
 * \ingroup SchedulerControl
 */
#define taskEXIT_CRITICAL_core2()                portEXIT_CRITICAL_core2()
#define taskEXIT_CRITICAL_FROM_ISR_core2( x_core2 )    portCLEAR_INTERRUPT_MASK_FROM_ISR_core2( x_core2 )

/**
 * task_core2. h
 *
 * Macro to disable all maskable interrupts.
 *
 * \defgroup taskDISABLE_INTERRUPTS_core2 taskDISABLE_INTERRUPTS_core2
 * \ingroup SchedulerControl
 */
#define taskDISABLE_INTERRUPTS_core2()           portDISABLE_INTERRUPTS_core2()

/**
 * task_core2. h
 *
 * Macro to enable microcontroller interrupts.
 *
 * \defgroup taskENABLE_INTERRUPTS_core2 taskENABLE_INTERRUPTS_core2
 * \ingroup SchedulerControl
 */
#define taskENABLE_INTERRUPTS_core2()            portENABLE_INTERRUPTS_core2()

/* Definitions returned by xTaskGetSchedulerState_core2().  taskSCHEDULER_SUSPENDED_core2 is
 * 0 to generate more optimal code when configASSERT_core2() is defined as the constant
 * is used in assert() statements. */
#define taskSCHEDULER_SUSPENDED_core2      ( ( BaseType_t_core2 ) 0 )
#define taskSCHEDULER_NOT_STARTED_core2    ( ( BaseType_t_core2 ) 1 )
#define taskSCHEDULER_RUNNING_core2        ( ( BaseType_t_core2 ) 2 )


/*-----------------------------------------------------------
* TASK CREATION API
*----------------------------------------------------------*/

/**
 * task_core2. h
 * @code{c}
 * BaseType_t_core2 xTaskCreate_core2(
 *                            TaskFunction_t_core2 pxTaskCode_core2,
 *                            const char *pcName_core2,
 *                            configSTACK_DEPTH_TYPE_core2 usStackDepth_core2,
 *                            void *pvParameters_core2,
 *                            UBaseType_t_core2 uxPriority_core2,
 *                            TaskHandle_t_core2 *pxCreatedTask_core2
 *                        );
 * @endcode
 *
 * Create a new task_core2 and add it to the list of tasks that are ready to run.
 *
 * Internally, within the FreeRTOS_core2 implementation, tasks use two blocks of
 * memory.  The first block is used to hold the task_core2's data structures.  The
 * second block is used by the task_core2 as its stack.  If a task_core2 is created using
 * xTaskCreate_core2() then both blocks of memory are automatically dynamically
 * allocated inside the xTaskCreate_core2() function.  (see
 * https://www.FreeRTOS_core2.org/a00111.html).  If a task_core2 is created using
 * xTaskCreateStatic_core2() then the application writer must provide the required
 * memory.  xTaskCreateStatic_core2() therefore allows a task_core2 to be created without
 * using any dynamic memory allocation.
 *
 * See xTaskCreateStatic_core2() for a version that does not use any dynamic memory
 * allocation.
 *
 * xTaskCreate_core2() can only be used to create a task_core2 that has unrestricted
 * access to the entire microcontroller memory map.  Systems that include MPU
 * support can alternatively create an MPU constrained task_core2 using
 * xTaskCreateRestricted_core2().
 *
 * @param pxTaskCode_core2 Pointer to the task_core2 entry function.  Tasks
 * must be implemented to never return (i.e. continuous loop).
 *
 * @param pcName_core2 A descriptive name for the task_core2.  This is mainly used to
 * facilitate debugging.  Max length defined by configMAX_TASK_NAME_LEN_core2 - default
 * is 16.
 *
 * @param usStackDepth_core2 The size of the task_core2 stack specified as the number of
 * variables the stack can hold - not the number of bytes.  For example, if
 * the stack is 16 bits wide and usStackDepth_core2 is defined as 100, 200 bytes
 * will be allocated for stack storage.
 *
 * @param pvParameters_core2 Pointer that will be used as the parameter for the task_core2
 * being created.
 *
 * @param uxPriority_core2 The priority at which the task_core2 should run.  Systems that
 * include MPU support can optionally create tasks in a privileged (system)
 * mode by setting bit portPRIVILEGE_BIT_core2 of the priority parameter.  For
 * example, to create a privileged task_core2 at priority 2 the uxPriority_core2 parameter
 * should be set to ( 2 | portPRIVILEGE_BIT_core2 ).
 *
 * @param pxCreatedTask_core2 Used to pass back a handle by which the created task_core2
 * can be referenced.
 *
 * @return pdPASS_core2 if the task_core2 was successfully created and added to a ready
 * list, otherwise an error code defined in the file projdefs.h
 *
 * Example usage:
 * @code{c}
 * // Task to be created.
 * void vTaskCode( void * pvParameters_core2 )
 * {
 *   for( ;; )
 *   {
 *       // Task code goes here.
 *   }
 * }
 *
 * // Function that creates a task_core2.
 * void vOtherFunction( void )
 * {
 * static uint8_t ucParameterToPass;
 * TaskHandle_t_core2 xHandle_core2 = NULL;
 *
 *   // Create the task_core2, storing the handle.  Note that the passed parameter ucParameterToPass
 *   // must exist for the lifetime of the task_core2, so in this case is declared static.  If it was just an
 *   // an automatic stack variable it might no longer exist, or at least have been corrupted, by the time
 *   // the new task_core2 attempts to access it.
 *   xTaskCreate_core2( vTaskCode, "NAME", STACK_SIZE, &ucParameterToPass, tskIDLE_PRIORITY_core2, &xHandle_core2 );
 *   configASSERT_core2( xHandle_core2 );
 *
 *   // Use the handle to delete the task_core2.
 *   if( xHandle_core2 != NULL )
 *   {
 *      vTaskDelete_core2( xHandle_core2 );
 *   }
 * }
 * @endcode
 * \defgroup xTaskCreate_core2 xTaskCreate_core2
 * \ingroup Tasks
 */
#if ( configSUPPORT_DYNAMIC_ALLOCATION_core2 == 1 )
    BaseType_t_core2 xTaskCreate_core2( TaskFunction_t_core2 pxTaskCode_core2,
                            const char * const pcName_core2, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                            const configSTACK_DEPTH_TYPE_core2 usStackDepth_core2,
                            void * const pvParameters_core2,
                            UBaseType_t_core2 uxPriority_core2,
                            TaskHandle_t_core2 * const pxCreatedTask_core2 ) PRIVILEGED_FUNCTION_core2;
#endif

/**
 * task_core2. h
 * @code{c}
 * TaskHandle_t_core2 xTaskCreateStatic_core2( TaskFunction_t_core2 pxTaskCode_core2,
 *                               const char *pcName_core2,
 *                               uint32_t ulStackDepth_core2,
 *                               void *pvParameters_core2,
 *                               UBaseType_t_core2 uxPriority_core2,
 *                               StackType_t_core2 *puxStackBuffer_core2,
 *                               StaticTask_t_core2 *pxTaskBuffer_core2 );
 * @endcode
 *
 * Create a new task_core2 and add it to the list of tasks that are ready to run.
 *
 * Internally, within the FreeRTOS_core2 implementation, tasks use two blocks of
 * memory.  The first block is used to hold the task_core2's data structures.  The
 * second block is used by the task_core2 as its stack.  If a task_core2 is created using
 * xTaskCreate_core2() then both blocks of memory are automatically dynamically
 * allocated inside the xTaskCreate_core2() function.  (see
 * https://www.FreeRTOS_core2.org/a00111.html).  If a task_core2 is created using
 * xTaskCreateStatic_core2() then the application writer must provide the required
 * memory.  xTaskCreateStatic_core2() therefore allows a task_core2 to be created without
 * using any dynamic memory allocation.
 *
 * @param pxTaskCode_core2 Pointer to the task_core2 entry function.  Tasks
 * must be implemented to never return (i.e. continuous loop).
 *
 * @param pcName_core2 A descriptive name for the task_core2.  This is mainly used to
 * facilitate debugging.  The maximum length of the string is defined by
 * configMAX_TASK_NAME_LEN_core2 in FreeRTOSConfig.h.
 *
 * @param ulStackDepth_core2 The size of the task_core2 stack specified as the number of
 * variables the stack can hold - not the number of bytes.  For example, if
 * the stack is 32-bits wide and ulStackDepth_core2 is defined as 100 then 400 bytes
 * will be allocated for stack storage.
 *
 * @param pvParameters_core2 Pointer that will be used as the parameter for the task_core2
 * being created.
 *
 * @param uxPriority_core2 The priority at which the task_core2 will run.
 *
 * @param puxStackBuffer_core2 Must point to a StackType_t_core2 array that has at least
 * ulStackDepth_core2 indexes - the array will then be used as the task_core2's stack,
 * removing the need for the stack to be allocated dynamically.
 *
 * @param pxTaskBuffer_core2 Must point to a variable of type StaticTask_t_core2, which will
 * then be used to hold the task_core2's data structures, removing the need for the
 * memory to be allocated dynamically.
 *
 * @return If neither puxStackBuffer_core2 nor pxTaskBuffer_core2 are NULL, then the task_core2
 * will be created and a handle to the created task_core2 is returned.  If either
 * puxStackBuffer_core2 or pxTaskBuffer_core2 are NULL then the task_core2 will not be created and
 * NULL is returned.
 *
 * Example usage:
 * @code{c}
 *
 *  // Dimensions of the buffer that the task_core2 being created will use as its stack.
 *  // NOTE:  This is the number of words the stack will hold, not the number of
 *  // bytes.  For example, if each stack item is 32-bits, and this is set to 100,
 *  // then 400 bytes (100 * 32-bits) will be allocated.
 #define STACK_SIZE 200
 *
 *  // Structure that will hold the TCB of the task_core2 being created.
 *  StaticTask_t_core2 xTaskBuffer;
 *
 *  // Buffer that the task_core2 being created will use as its stack.  Note this is
 *  // an array of StackType_t_core2 variables.  The size of StackType_t_core2 is dependent on
 *  // the RTOS port.
 *  StackType_t_core2 xStack[ STACK_SIZE ];
 *
 *  // Function that implements the task_core2 being created.
 *  void vTaskCode( void * pvParameters_core2 )
 *  {
 *      // The parameter value is expected to be 1 as 1 is passed in the
 *      // pvParameters_core2 value in the call to xTaskCreateStatic_core2().
 *      configASSERT_core2( ( uint32_t ) pvParameters_core2 == 1UL );
 *
 *      for( ;; )
 *      {
 *          // Task code goes here.
 *      }
 *  }
 *
 *  // Function that creates a task_core2.
 *  void vOtherFunction( void )
 *  {
 *      TaskHandle_t_core2 xHandle_core2 = NULL;
 *
 *      // Create the task_core2 without using any dynamic memory allocation.
 *      xHandle_core2 = xTaskCreateStatic_core2(
 *                    vTaskCode,       // Function that implements the task_core2.
 *                    "NAME",          // Text name for the task_core2.
 *                    STACK_SIZE,      // Stack size in words, not bytes.
 *                    ( void * ) 1,    // Parameter passed into the task_core2.
 *                    tskIDLE_PRIORITY_core2,// Priority at which the task_core2 is created.
 *                    xStack,          // Array to use as the task_core2's stack.
 *                    &xTaskBuffer );  // Variable to hold the task_core2's data structure.
 *
 *      // puxStackBuffer_core2 and pxTaskBuffer_core2 were not NULL, so the task_core2 will have
 *      // been created, and xHandle_core2 will be the task_core2's handle.  Use the handle
 *      // to suspend the task_core2.
 *      vTaskSuspend_core2( xHandle_core2 );
 *  }
 * @endcode
 * \defgroup xTaskCreateStatic_core2 xTaskCreateStatic_core2
 * \ingroup Tasks
 */
#if ( configSUPPORT_STATIC_ALLOCATION_core2 == 1 )
    TaskHandle_t_core2 xTaskCreateStatic_core2( TaskFunction_t_core2 pxTaskCode_core2,
                                    const char * const pcName_core2, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                    const uint32_t ulStackDepth_core2,
                                    void * const pvParameters_core2,
                                    UBaseType_t_core2 uxPriority_core2,
                                    StackType_t_core2 * const puxStackBuffer_core2,
                                    StaticTask_t_core2 * const pxTaskBuffer_core2 ) PRIVILEGED_FUNCTION_core2;
#endif /* configSUPPORT_STATIC_ALLOCATION_core2 */

/**
 * task_core2. h
 * @code{c}
 * BaseType_t_core2 xTaskCreateRestricted_core2( TaskParameters_t_core2 *pxTaskDefinition_core2, TaskHandle_t_core2 *pxCreatedTask_core2 );
 * @endcode
 *
 * Only available when configSUPPORT_DYNAMIC_ALLOCATION_core2 is set to 1.
 *
 * xTaskCreateRestricted_core2() should only be used in systems that include an MPU
 * implementation.
 *
 * Create a new task_core2 and add it to the list of tasks that are ready to run.
 * The function parameters define the memory regions and associated access
 * permissions allocated to the task_core2.
 *
 * See xTaskCreateRestrictedStatic_core2() for a version that does not use any
 * dynamic memory allocation.
 *
 * @param pxTaskDefinition_core2 Pointer to a structure that contains a member
 * for each of the normal xTaskCreate_core2() parameters (see the xTaskCreate_core2() API
 * documentation) plus an optional stack buffer and the memory region
 * definitions.
 *
 * @param pxCreatedTask_core2 Used to pass back a handle by which the created task_core2
 * can be referenced.
 *
 * @return pdPASS_core2 if the task_core2 was successfully created and added to a ready
 * list, otherwise an error code defined in the file projdefs.h
 *
 * Example usage:
 * @code{c}
 * // Create an TaskParameters_t_core2 structure that defines the task_core2 to be created.
 * static const TaskParameters_t_core2 xCheckTaskParameters =
 * {
 *  vATask,     // pvTaskCode_core2 - the function that implements the task_core2.
 *  "ATask",    // pcName_core2 - just a text name for the task_core2 to assist debugging.
 *  100,        // usStackDepth_core2 - the stack size DEFINED IN WORDS.
 *  NULL,       // pvParameters_core2 - passed into the task_core2 function as the function parameters.
 *  ( 1UL | portPRIVILEGE_BIT_core2 ),// uxPriority_core2 - task_core2 priority, set the portPRIVILEGE_BIT_core2 if the task_core2 should run in a privileged state.
 *  cStackBuffer,// puxStackBuffer_core2 - the buffer to be used as the task_core2 stack.
 *
 *  // xRegions_core2 - Allocate up to three separate memory regions for access by
 *  // the task_core2, with appropriate access permissions.  Different processors have
 *  // different memory alignment requirements - refer to the FreeRTOS_core2 documentation
 *  // for full information.
 *  {
 *      // Base address                 Length  Parameters
 *      { cReadWriteArray,              32,     portMPU_REGION_READ_WRITE },
 *      { cReadOnlyArray,               32,     portMPU_REGION_READ_ONLY },
 *      { cPrivilegedOnlyAccessArray,   128,    portMPU_REGION_PRIVILEGED_READ_WRITE }
 *  }
 * };
 *
 * int main( void )
 * {
 * TaskHandle_t_core2 xHandle_core2;
 *
 *  // Create a task_core2 from the const structure defined above.  The task_core2 handle
 *  // is requested (the second parameter is not NULL) but in this case just for
 *  // demonstration purposes as its not actually used.
 *  xTaskCreateRestricted_core2( &xRegTest1Parameters, &xHandle_core2 );
 *
 *  // Start the scheduler.
 *  vTaskStartScheduler_core2();
 *
 *  // Will only get here if there was insufficient memory to create the idle
 *  // and/or timer task_core2.
 *  for( ;; );
 * }
 * @endcode
 * \defgroup xTaskCreateRestricted_core2 xTaskCreateRestricted_core2
 * \ingroup Tasks
 */
#if ( portUSING_MPU_WRAPPERS_core2 == 1 )
    BaseType_t_core2 xTaskCreateRestricted_core2( const TaskParameters_t_core2 * const pxTaskDefinition_core2,
                                      TaskHandle_t_core2 * pxCreatedTask_core2 ) PRIVILEGED_FUNCTION_core2;
#endif

/**
 * task_core2. h
 * @code{c}
 * BaseType_t_core2 xTaskCreateRestrictedStatic_core2( TaskParameters_t_core2 *pxTaskDefinition_core2, TaskHandle_t_core2 *pxCreatedTask_core2 );
 * @endcode
 *
 * Only available when configSUPPORT_STATIC_ALLOCATION_core2 is set to 1.
 *
 * xTaskCreateRestrictedStatic_core2() should only be used in systems that include an
 * MPU implementation.
 *
 * Internally, within the FreeRTOS_core2 implementation, tasks use two blocks of
 * memory.  The first block is used to hold the task_core2's data structures.  The
 * second block is used by the task_core2 as its stack.  If a task_core2 is created using
 * xTaskCreateRestricted_core2() then the stack is provided by the application writer,
 * and the memory used to hold the task_core2's data structure is automatically
 * dynamically allocated inside the xTaskCreateRestricted_core2() function.  If a task_core2
 * is created using xTaskCreateRestrictedStatic_core2() then the application writer
 * must provide the memory used to hold the task_core2's data structures too.
 * xTaskCreateRestrictedStatic_core2() therefore allows a memory protected task_core2 to be
 * created without using any dynamic memory allocation.
 *
 * @param pxTaskDefinition_core2 Pointer to a structure that contains a member
 * for each of the normal xTaskCreate_core2() parameters (see the xTaskCreate_core2() API
 * documentation) plus an optional stack buffer and the memory region
 * definitions.  If configSUPPORT_STATIC_ALLOCATION_core2 is set to 1 the structure
 * contains an additional member, which is used to point to a variable of type
 * StaticTask_t_core2 - which is then used to hold the task_core2's data structure.
 *
 * @param pxCreatedTask_core2 Used to pass back a handle by which the created task_core2
 * can be referenced.
 *
 * @return pdPASS_core2 if the task_core2 was successfully created and added to a ready
 * list, otherwise an error code defined in the file projdefs.h
 *
 * Example usage:
 * @code{c}
 * // Create an TaskParameters_t_core2 structure that defines the task_core2 to be created.
 * // The StaticTask_t_core2 variable is only included in the structure when
 * // configSUPPORT_STATIC_ALLOCATION_core2 is set to 1.  The PRIVILEGED_DATA_core2 macro can
 * // be used to force the variable into the RTOS kernel's privileged data area.
 * static PRIVILEGED_DATA_core2 StaticTask_t_core2 xTaskBuffer;
 * static const TaskParameters_t_core2 xCheckTaskParameters =
 * {
 *  vATask,     // pvTaskCode_core2 - the function that implements the task_core2.
 *  "ATask",    // pcName_core2 - just a text name for the task_core2 to assist debugging.
 *  100,        // usStackDepth_core2 - the stack size DEFINED IN WORDS.
 *  NULL,       // pvParameters_core2 - passed into the task_core2 function as the function parameters.
 *  ( 1UL | portPRIVILEGE_BIT_core2 ),// uxPriority_core2 - task_core2 priority, set the portPRIVILEGE_BIT_core2 if the task_core2 should run in a privileged state.
 *  cStackBuffer,// puxStackBuffer_core2 - the buffer to be used as the task_core2 stack.
 *
 *  // xRegions_core2 - Allocate up to three separate memory regions for access by
 *  // the task_core2, with appropriate access permissions.  Different processors have
 *  // different memory alignment requirements - refer to the FreeRTOS_core2 documentation
 *  // for full information.
 *  {
 *      // Base address                 Length  Parameters
 *      { cReadWriteArray,              32,     portMPU_REGION_READ_WRITE },
 *      { cReadOnlyArray,               32,     portMPU_REGION_READ_ONLY },
 *      { cPrivilegedOnlyAccessArray,   128,    portMPU_REGION_PRIVILEGED_READ_WRITE }
 *  }
 *
 *  &xTaskBuffer; // Holds the task_core2's data structure.
 * };
 *
 * int main( void )
 * {
 * TaskHandle_t_core2 xHandle_core2;
 *
 *  // Create a task_core2 from the const structure defined above.  The task_core2 handle
 *  // is requested (the second parameter is not NULL) but in this case just for
 *  // demonstration purposes as its not actually used.
 *  xTaskCreateRestrictedStatic_core2( &xRegTest1Parameters, &xHandle_core2 );
 *
 *  // Start the scheduler.
 *  vTaskStartScheduler_core2();
 *
 *  // Will only get here if there was insufficient memory to create the idle
 *  // and/or timer task_core2.
 *  for( ;; );
 * }
 * @endcode
 * \defgroup xTaskCreateRestrictedStatic_core2 xTaskCreateRestrictedStatic_core2
 * \ingroup Tasks
 */
#if ( ( portUSING_MPU_WRAPPERS_core2 == 1 ) && ( configSUPPORT_STATIC_ALLOCATION_core2 == 1 ) )
    BaseType_t_core2 xTaskCreateRestrictedStatic_core2( const TaskParameters_t_core2 * const pxTaskDefinition_core2,
                                            TaskHandle_t_core2 * pxCreatedTask_core2 ) PRIVILEGED_FUNCTION_core2;
#endif

/**
 * task_core2. h
 * @code{c}
 * void vTaskAllocateMPURegions_core2( TaskHandle_t_core2 xTask_core2, const MemoryRegion_t_core2 * const pxRegions_core2 );
 * @endcode
 *
 * Memory regions are assigned to a restricted task_core2 when the task_core2 is created by
 * a call to xTaskCreateRestricted_core2().  These regions can be redefined using
 * vTaskAllocateMPURegions_core2().
 *
 * @param xTask_core2 The handle of the task_core2 being updated.
 *
 * @param xRegions_core2 A pointer to a MemoryRegion_t_core2 structure that contains the
 * new memory region definitions.
 *
 * Example usage:
 * @code{c}
 * // Define an array of MemoryRegion_t_core2 structures that configures an MPU region
 * // allowing read/write access for 1024 bytes starting at the beginning of the
 * // ucOneKByte array.  The other two of the maximum 3 definable regions are
 * // unused so set to zero.
 * static const MemoryRegion_t_core2 xAltRegions[ portNUM_CONFIGURABLE_REGIONS_core2 ] =
 * {
 *  // Base address     Length      Parameters
 *  { ucOneKByte,       1024,       portMPU_REGION_READ_WRITE },
 *  { 0,                0,          0 },
 *  { 0,                0,          0 }
 * };
 *
 * void vATask( void *pvParameters_core2 )
 * {
 *  // This task_core2 was created such that it has access to certain regions of
 *  // memory as defined by the MPU configuration.  At some point it is
 *  // desired that these MPU regions are replaced with that defined in the
 *  // xAltRegions const struct above.  Use a call to vTaskAllocateMPURegions_core2()
 *  // for this purpose.  NULL is used as the task_core2 handle to indicate that this
 *  // function should modify the MPU regions of the calling task_core2.
 *  vTaskAllocateMPURegions_core2( NULL, xAltRegions );
 *
 *  // Now the task_core2 can continue its function, but from this point on can only
 *  // access its stack and the ucOneKByte array (unless any other statically
 *  // defined or shared regions have been declared elsewhere).
 * }
 * @endcode
 * \defgroup vTaskAllocateMPURegions_core2 vTaskAllocateMPURegions_core2
 * \ingroup Tasks
 */
void vTaskAllocateMPURegions_core2( TaskHandle_t_core2 xTask_core2,
                              const MemoryRegion_t_core2 * const pxRegions_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * void vTaskDelete_core2( TaskHandle_t_core2 xTaskToDelete_core2 );
 * @endcode
 *
 * INCLUDE_vTaskDelete_core2 must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Remove a task_core2 from the RTOS real time kernel's management.  The task_core2 being
 * deleted will be removed from all ready, blocked, suspended and event lists.
 *
 * NOTE:  The idle task_core2 is responsible for freeing the kernel allocated
 * memory from tasks that have been deleted.  It is therefore important that
 * the idle task_core2 is not starved of microcontroller processing time if your
 * application makes any calls to vTaskDelete_core2 ().  Memory allocated by the
 * task_core2 code is not automatically freed, and should be freed before the task_core2
 * is deleted.
 *
 * See the demo application file death.c for sample code that utilises
 * vTaskDelete_core2 ().
 *
 * @param xTaskToDelete_core2 The handle of the task_core2 to be deleted.  Passing NULL will
 * cause the calling task_core2 to be deleted.
 *
 * Example usage:
 * @code{c}
 * void vOtherFunction( void )
 * {
 * TaskHandle_t_core2 xHandle_core2;
 *
 *   // Create the task_core2, storing the handle.
 *   xTaskCreate_core2( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY_core2, &xHandle_core2 );
 *
 *   // Use the handle to delete the task_core2.
 *   vTaskDelete_core2( xHandle_core2 );
 * }
 * @endcode
 * \defgroup vTaskDelete_core2 vTaskDelete_core2
 * \ingroup Tasks
 */
void vTaskDelete_core2( TaskHandle_t_core2 xTaskToDelete_core2 ) PRIVILEGED_FUNCTION_core2;

/*-----------------------------------------------------------
* TASK CONTROL API
*----------------------------------------------------------*/

/**
 * task_core2. h
 * @code{c}
 * void vTaskDelay_core2( const TickType_t_core2 xTicksToDelay_core2 );
 * @endcode
 *
 * Delay a task_core2 for a given number of ticks.  The actual time that the
 * task_core2 remains blocked depends on the tick rate.  The constant
 * portTICK_PERIOD_MS_core2 can be used to calculate real time from the tick
 * rate - with the resolution of one tick period.
 *
 * INCLUDE_vTaskDelay_core2 must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 *
 * vTaskDelay_core2() specifies a time at which the task_core2 wishes to unblock relative to
 * the time at which vTaskDelay_core2() is called.  For example, specifying a block
 * period of 100 ticks will cause the task_core2 to unblock 100 ticks after
 * vTaskDelay_core2() is called.  vTaskDelay_core2() does not therefore provide a good method
 * of controlling the frequency of a periodic task_core2 as the path taken through the
 * code, as well as other task_core2 and interrupt activity, will affect the frequency
 * at which vTaskDelay_core2() gets called and therefore the time at which the task_core2
 * next executes.  See xTaskDelayUntil_core2() for an alternative API function designed
 * to facilitate fixed frequency execution.  It does this by specifying an
 * absolute time (rather than a relative time) at which the calling task_core2 should
 * unblock.
 *
 * @param xTicksToDelay_core2 The amount of time, in tick periods, that
 * the calling task_core2 should block.
 *
 * Example usage:
 *
 * void vTaskFunction( void * pvParameters_core2 )
 * {
 * // Block for 500ms.
 * const TickType_t_core2 xDelay = 500 / portTICK_PERIOD_MS_core2;
 *
 *   for( ;; )
 *   {
 *       // Simply toggle the LED every 500ms, blocking between each toggle.
 *       vToggleLED();
 *       vTaskDelay_core2( xDelay );
 *   }
 * }
 *
 * \defgroup vTaskDelay_core2 vTaskDelay_core2
 * \ingroup TaskCtrl
 */
void vTaskDelay_core2( const TickType_t_core2 xTicksToDelay_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * BaseType_t_core2 xTaskDelayUntil_core2( TickType_t_core2 *pxPreviousWakeTime_core2, const TickType_t_core2 xTimeIncrement_core2 );
 * @endcode
 *
 * INCLUDE_xTaskDelayUntil_core2 must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Delay a task_core2 until a specified time.  This function can be used by periodic
 * tasks to ensure a constant execution frequency.
 *
 * This function differs from vTaskDelay_core2 () in one important aspect:  vTaskDelay_core2 () will
 * cause a task_core2 to block for the specified number of ticks from the time vTaskDelay_core2 () is
 * called.  It is therefore difficult to use vTaskDelay_core2 () by itself to generate a fixed
 * execution frequency as the time between a task_core2 starting to execute and that task_core2
 * calling vTaskDelay_core2 () may not be fixed [the task_core2 may take a different path though the
 * code between calls, or may get interrupted or preempted a different number of times
 * each time it executes].
 *
 * Whereas vTaskDelay_core2 () specifies a wake time relative to the time at which the function
 * is called, xTaskDelayUntil_core2 () specifies the absolute (exact) time at which it wishes to
 * unblock.
 *
 * The macro pdMS_TO_TICKS_core2() can be used to calculate the number of ticks from a
 * time specified in milliseconds with a resolution of one tick period.
 *
 * @param pxPreviousWakeTime_core2 Pointer to a variable that holds the time at which the
 * task_core2 was last unblocked.  The variable must be initialised with the current time
 * prior to its first use (see the example below).  Following this the variable is
 * automatically updated within xTaskDelayUntil_core2 ().
 *
 * @param xTimeIncrement_core2 The cycle time period.  The task_core2 will be unblocked at
 * time *pxPreviousWakeTime_core2 + xTimeIncrement_core2.  Calling xTaskDelayUntil_core2 with the
 * same xTimeIncrement_core2 parameter value will cause the task_core2 to execute with
 * a fixed interface period.
 *
 * @return Value which can be used to check whether the task_core2 was actually delayed.
 * Will be pdTRUE_core2 if the task_core2 way delayed and pdFALSE_core2 otherwise.  A task_core2 will not
 * be delayed if the next expected wake time is in the past.
 *
 * Example usage:
 * @code{c}
 * // Perform an action every 10 ticks.
 * void vTaskFunction( void * pvParameters_core2 )
 * {
 * TickType_t_core2 xLastWakeTime;
 * const TickType_t_core2 xFrequency = 10;
 * BaseType_t_core2 xWasDelayed;
 *
 *     // Initialise the xLastWakeTime variable with the current time.
 *     xLastWakeTime = xTaskGetTickCount_core2 ();
 *     for( ;; )
 *     {
 *         // Wait for the next cycle.
 *         xWasDelayed = xTaskDelayUntil_core2( &xLastWakeTime, xFrequency );
 *
 *         // Perform action here. xWasDelayed value can be used to determine
 *         // whether a deadline was missed if the code here took too long.
 *     }
 * }
 * @endcode
 * \defgroup xTaskDelayUntil_core2 xTaskDelayUntil_core2
 * \ingroup TaskCtrl
 */
BaseType_t_core2 xTaskDelayUntil_core2( TickType_t_core2 * const pxPreviousWakeTime_core2,
                            const TickType_t_core2 xTimeIncrement_core2 ) PRIVILEGED_FUNCTION_core2;

/*
 * vTaskDelayUntil_core2() is the older version of xTaskDelayUntil_core2() and does not
 * return a value.
 */
#define vTaskDelayUntil_core2( pxPreviousWakeTime_core2, xTimeIncrement_core2 )                   \
    do {                                                                        \
        ( void ) xTaskDelayUntil_core2( ( pxPreviousWakeTime_core2 ), ( xTimeIncrement_core2 ) ); \
    } while( 0 )


/**
 * task_core2. h
 * @code{c}
 * BaseType_t_core2 xTaskAbortDelay_core2( TaskHandle_t_core2 xTask_core2 );
 * @endcode
 *
 * INCLUDE_xTaskAbortDelay_core2 must be defined as 1 in FreeRTOSConfig.h for this
 * function to be available.
 *
 * A task_core2 will enter the Blocked state when it is waiting for an event.  The
 * event it is waiting for can be a temporal event (waiting for a time), such
 * as when vTaskDelay_core2() is called, or an event on an object, such as when
 * xQueueReceive_core2() or ulTaskNotifyTake_core2() is called.  If the handle of a task_core2
 * that is in the Blocked state is used in a call to xTaskAbortDelay_core2() then the
 * task_core2 will leave the Blocked state, and return from whichever function call
 * placed the task_core2 into the Blocked state.
 *
 * There is no 'FromISR' version of this function as an interrupt would need to
 * know which object a task_core2 was blocked on in order to know which actions to
 * take.  For example, if the task_core2 was blocked on a queue the interrupt handler
 * would then need to know if the queue was locked.
 *
 * @param xTask_core2 The handle of the task_core2 to remove from the Blocked state.
 *
 * @return If the task_core2 referenced by xTask_core2 was not in the Blocked state then
 * pdFAIL_core2 is returned.  Otherwise pdPASS_core2 is returned.
 *
 * \defgroup xTaskAbortDelay_core2 xTaskAbortDelay_core2
 * \ingroup TaskCtrl
 */
BaseType_t_core2 xTaskAbortDelay_core2( TaskHandle_t_core2 xTask_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * UBaseType_t_core2 uxTaskPriorityGet_core2( const TaskHandle_t_core2 xTask_core2 );
 * @endcode
 *
 * INCLUDE_uxTaskPriorityGet_core2 must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Obtain the priority of any task_core2.
 *
 * @param xTask_core2 Handle of the task_core2 to be queried.  Passing a NULL
 * handle results in the priority of the calling task_core2 being returned.
 *
 * @return The priority of xTask_core2.
 *
 * Example usage:
 * @code{c}
 * void vAFunction( void )
 * {
 * TaskHandle_t_core2 xHandle_core2;
 *
 *   // Create a task_core2, storing the handle.
 *   xTaskCreate_core2( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY_core2, &xHandle_core2 );
 *
 *   // ...
 *
 *   // Use the handle to obtain the priority of the created task_core2.
 *   // It was created with tskIDLE_PRIORITY_core2, but may have changed
 *   // it itself.
 *   if( uxTaskPriorityGet_core2( xHandle_core2 ) != tskIDLE_PRIORITY_core2 )
 *   {
 *       // The task_core2 has changed it's priority.
 *   }
 *
 *   // ...
 *
 *   // Is our priority higher than the created task_core2?
 *   if( uxTaskPriorityGet_core2( xHandle_core2 ) < uxTaskPriorityGet_core2( NULL ) )
 *   {
 *       // Our priority (obtained using NULL handle) is higher.
 *   }
 * }
 * @endcode
 * \defgroup uxTaskPriorityGet_core2 uxTaskPriorityGet_core2
 * \ingroup TaskCtrl
 */
UBaseType_t_core2 uxTaskPriorityGet_core2( const TaskHandle_t_core2 xTask_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * UBaseType_t_core2 uxTaskPriorityGetFromISR_core2( const TaskHandle_t_core2 xTask_core2 );
 * @endcode
 *
 * A version of uxTaskPriorityGet_core2() that can be used from an ISR.
 */
UBaseType_t_core2 uxTaskPriorityGetFromISR_core2( const TaskHandle_t_core2 xTask_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * eTaskState_core2 eTaskGetState_core2( TaskHandle_t_core2 xTask_core2 );
 * @endcode
 *
 * INCLUDE_eTaskGetState_core2 must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Obtain the state of any task_core2.  States are encoded by the eTaskState_core2
 * enumerated type.
 *
 * @param xTask_core2 Handle of the task_core2 to be queried.
 *
 * @return The state of xTask_core2 at the time the function was called.  Note the
 * state of the task_core2 might change between the function being called, and the
 * functions return value being tested by the calling task_core2.
 */
eTaskState_core2 eTaskGetState_core2( TaskHandle_t_core2 xTask_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * void vTaskGetInfo_core2( TaskHandle_t_core2 xTask_core2, TaskStatus_t_core2 *pxTaskStatus_core2, BaseType_t_core2 xGetFreeStackSpace_core2, eTaskState_core2 eState_core2 );
 * @endcode
 *
 * configUSE_TRACE_FACILITY_core2 must be defined as 1 for this function to be
 * available.  See the configuration section for more information.
 *
 * Populates a TaskStatus_t_core2 structure with information about a task_core2.
 *
 * @param xTask_core2 Handle of the task_core2 being queried.  If xTask_core2 is NULL then
 * information will be returned about the calling task_core2.
 *
 * @param pxTaskStatus_core2 A pointer to the TaskStatus_t_core2 structure that will be
 * filled with information about the task_core2 referenced by the handle passed using
 * the xTask_core2 parameter.
 *
 * @param xGetFreeStackSpace_core2 The TaskStatus_t_core2 structure contains a member to report
 * the stack high water mark of the task_core2 being queried.  Calculating the stack
 * high water mark takes a relatively long time, and can make the system
 * temporarily unresponsive - so the xGetFreeStackSpace_core2 parameter is provided to
 * allow the high water mark checking to be skipped.  The high watermark value
 * will only be written to the TaskStatus_t_core2 structure if xGetFreeStackSpace_core2 is
 * not set to pdFALSE_core2;
 *
 * @param eState_core2 The TaskStatus_t_core2 structure contains a member to report the
 * state of the task_core2 being queried.  Obtaining the task_core2 state is not as fast as
 * a simple assignment - so the eState_core2 parameter is provided to allow the state
 * information to be omitted from the TaskStatus_t_core2 structure.  To obtain state
 * information then set eState_core2 to eInvalid - otherwise the value passed in
 * eState_core2 will be reported as the task_core2 state in the TaskStatus_t_core2 structure.
 *
 * Example usage:
 * @code{c}
 * void vAFunction( void )
 * {
 * TaskHandle_t_core2 xHandle_core2;
 * TaskStatus_t_core2 xTaskDetails;
 *
 *  // Obtain the handle of a task_core2 from its name.
 *  xHandle_core2 = xTaskGetHandle_core2( "Task_Name" );
 *
 *  // Check the handle is not NULL.
 *  configASSERT_core2( xHandle_core2 );
 *
 *  // Use the handle to obtain further information about the task_core2.
 *  vTaskGetInfo_core2( xHandle_core2,
 *                &xTaskDetails,
 *                pdTRUE_core2, // Include the high water mark in xTaskDetails.
 *                eInvalid ); // Include the task_core2 state in xTaskDetails.
 * }
 * @endcode
 * \defgroup vTaskGetInfo_core2 vTaskGetInfo_core2
 * \ingroup TaskCtrl
 */
void vTaskGetInfo_core2( TaskHandle_t_core2 xTask_core2,
                   TaskStatus_t_core2 * pxTaskStatus_core2,
                   BaseType_t_core2 xGetFreeStackSpace_core2,
                   eTaskState_core2 eState_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * void vTaskPrioritySet_core2( TaskHandle_t_core2 xTask_core2, UBaseType_t_core2 uxNewPriority_core2 );
 * @endcode
 *
 * INCLUDE_vTaskPrioritySet_core2 must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Set the priority of any task_core2.
 *
 * A context switch will occur before the function returns if the priority
 * being set is higher than the currently executing task_core2.
 *
 * @param xTask_core2 Handle to the task_core2 for which the priority is being set.
 * Passing a NULL handle results in the priority of the calling task_core2 being set.
 *
 * @param uxNewPriority_core2 The priority to which the task_core2 will be set.
 *
 * Example usage:
 * @code{c}
 * void vAFunction( void )
 * {
 * TaskHandle_t_core2 xHandle_core2;
 *
 *   // Create a task_core2, storing the handle.
 *   xTaskCreate_core2( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY_core2, &xHandle_core2 );
 *
 *   // ...
 *
 *   // Use the handle to raise the priority of the created task_core2.
 *   vTaskPrioritySet_core2( xHandle_core2, tskIDLE_PRIORITY_core2 + 1 );
 *
 *   // ...
 *
 *   // Use a NULL handle to raise our priority to the same value.
 *   vTaskPrioritySet_core2( NULL, tskIDLE_PRIORITY_core2 + 1 );
 * }
 * @endcode
 * \defgroup vTaskPrioritySet_core2 vTaskPrioritySet_core2
 * \ingroup TaskCtrl
 */
void vTaskPrioritySet_core2( TaskHandle_t_core2 xTask_core2,
                       UBaseType_t_core2 uxNewPriority_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * void vTaskSuspend_core2( TaskHandle_t_core2 xTaskToSuspend_core2 );
 * @endcode
 *
 * INCLUDE_vTaskSuspend_core2 must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Suspend any task_core2.  When suspended a task_core2 will never get any microcontroller
 * processing time, no matter what its priority.
 *
 * Calls to vTaskSuspend_core2 are not accumulative -
 * i.e. calling vTaskSuspend_core2 () twice on the same task_core2 still only requires one
 * call to vTaskResume_core2 () to ready the suspended task_core2.
 *
 * @param xTaskToSuspend_core2 Handle to the task_core2 being suspended.  Passing a NULL
 * handle will cause the calling task_core2 to be suspended.
 *
 * Example usage:
 * @code{c}
 * void vAFunction( void )
 * {
 * TaskHandle_t_core2 xHandle_core2;
 *
 *   // Create a task_core2, storing the handle.
 *   xTaskCreate_core2( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY_core2, &xHandle_core2 );
 *
 *   // ...
 *
 *   // Use the handle to suspend the created task_core2.
 *   vTaskSuspend_core2( xHandle_core2 );
 *
 *   // ...
 *
 *   // The created task_core2 will not run during this period, unless
 *   // another task_core2 calls vTaskResume_core2( xHandle_core2 ).
 *
 *   //...
 *
 *
 *   // Suspend ourselves.
 *   vTaskSuspend_core2( NULL );
 *
 *   // We cannot get here unless another task_core2 calls vTaskResume_core2
 *   // with our handle as the parameter.
 * }
 * @endcode
 * \defgroup vTaskSuspend_core2 vTaskSuspend_core2
 * \ingroup TaskCtrl
 */
void vTaskSuspend_core2( TaskHandle_t_core2 xTaskToSuspend_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * void vTaskResume_core2( TaskHandle_t_core2 xTaskToResume_core2 );
 * @endcode
 *
 * INCLUDE_vTaskSuspend_core2 must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Resumes a suspended task_core2.
 *
 * A task_core2 that has been suspended by one or more calls to vTaskSuspend_core2 ()
 * will be made available for running again by a single call to
 * vTaskResume_core2 ().
 *
 * @param xTaskToResume_core2 Handle to the task_core2 being readied.
 *
 * Example usage:
 * @code{c}
 * void vAFunction( void )
 * {
 * TaskHandle_t_core2 xHandle_core2;
 *
 *   // Create a task_core2, storing the handle.
 *   xTaskCreate_core2( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY_core2, &xHandle_core2 );
 *
 *   // ...
 *
 *   // Use the handle to suspend the created task_core2.
 *   vTaskSuspend_core2( xHandle_core2 );
 *
 *   // ...
 *
 *   // The created task_core2 will not run during this period, unless
 *   // another task_core2 calls vTaskResume_core2( xHandle_core2 ).
 *
 *   //...
 *
 *
 *   // Resume the suspended task_core2 ourselves.
 *   vTaskResume_core2( xHandle_core2 );
 *
 *   // The created task_core2 will once again get microcontroller processing
 *   // time in accordance with its priority within the system.
 * }
 * @endcode
 * \defgroup vTaskResume_core2 vTaskResume_core2
 * \ingroup TaskCtrl
 */
void vTaskResume_core2( TaskHandle_t_core2 xTaskToResume_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * void xTaskResumeFromISR_core2( TaskHandle_t_core2 xTaskToResume_core2 );
 * @endcode
 *
 * INCLUDE_xTaskResumeFromISR_core2 must be defined as 1 for this function to be
 * available.  See the configuration section for more information.
 *
 * An implementation of vTaskResume_core2() that can be called from within an ISR.
 *
 * A task_core2 that has been suspended by one or more calls to vTaskSuspend_core2 ()
 * will be made available for running again by a single call to
 * xTaskResumeFromISR_core2 ().
 *
 * xTaskResumeFromISR_core2() should not be used to synchronise a task_core2 with an
 * interrupt if there is a chance that the interrupt could arrive prior to the
 * task_core2 being suspended - as this can lead to interrupts being missed. Use of a
 * semaphore as a synchronisation mechanism would avoid this eventuality.
 *
 * @param xTaskToResume_core2 Handle to the task_core2 being readied.
 *
 * @return pdTRUE_core2 if resuming the task_core2 should result in a context switch,
 * otherwise pdFALSE_core2. This is used by the ISR to determine if a context switch
 * may be required following the ISR.
 *
 * \defgroup vTaskResumeFromISR vTaskResumeFromISR
 * \ingroup TaskCtrl
 */
BaseType_t_core2 xTaskResumeFromISR_core2( TaskHandle_t_core2 xTaskToResume_core2 ) PRIVILEGED_FUNCTION_core2;

/*-----------------------------------------------------------
* SCHEDULER CONTROL
*----------------------------------------------------------*/

/**
 * task_core2. h
 * @code{c}
 * void vTaskStartScheduler_core2( void );
 * @endcode
 *
 * Starts the real time kernel tick processing.  After calling the kernel
 * has control over which tasks are executed and when.
 *
 * See the demo application file main.c for an example of creating
 * tasks and starting the kernel.
 *
 * Example usage:
 * @code{c}
 * void vAFunction( void )
 * {
 *   // Create at least one task_core2 before starting the kernel.
 *   xTaskCreate_core2( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY_core2, NULL );
 *
 *   // Start the real time kernel with preemption.
 *   vTaskStartScheduler_core2 ();
 *
 *   // Will not get here unless a task_core2 calls vTaskEndScheduler_core2 ()
 * }
 * @endcode
 *
 * \defgroup vTaskStartScheduler_core2 vTaskStartScheduler_core2
 * \ingroup SchedulerControl
 */
void vTaskStartScheduler_core2( void ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * void vTaskEndScheduler_core2( void );
 * @endcode
 *
 * NOTE:  At the time of writing only the x86 real mode port, which runs on a PC
 * in place of DOS, implements this function.
 *
 * Stops the real time kernel tick.  All created tasks will be automatically
 * deleted and multitasking (either preemptive or cooperative) will
 * stop.  Execution then resumes from the point where vTaskStartScheduler_core2 ()
 * was called, as if vTaskStartScheduler_core2 () had just returned.
 *
 * See the demo application file main. c in the demo/PC directory for an
 * example that uses vTaskEndScheduler_core2 ().
 *
 * vTaskEndScheduler_core2 () requires an exit function to be defined within the
 * portable layer (see vPortEndScheduler_core2 () in port. c for the PC port).  This
 * performs hardware specific operations such as stopping the kernel tick.
 *
 * vTaskEndScheduler_core2 () will cause all of the resources allocated by the
 * kernel to be freed - but will not free resources allocated by application
 * tasks.
 *
 * Example usage:
 * @code{c}
 * void vTaskCode( void * pvParameters_core2 )
 * {
 *   for( ;; )
 *   {
 *       // Task code goes here.
 *
 *       // At some point we want to end the real time kernel processing
 *       // so call ...
 *       vTaskEndScheduler_core2 ();
 *   }
 * }
 *
 * void vAFunction( void )
 * {
 *   // Create at least one task_core2 before starting the kernel.
 *   xTaskCreate_core2( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY_core2, NULL );
 *
 *   // Start the real time kernel with preemption.
 *   vTaskStartScheduler_core2 ();
 *
 *   // Will only get here when the vTaskCode () task_core2 has called
 *   // vTaskEndScheduler_core2 ().  When we get here we are back to single task_core2
 *   // execution.
 * }
 * @endcode
 *
 * \defgroup vTaskEndScheduler_core2 vTaskEndScheduler_core2
 * \ingroup SchedulerControl
 */
void vTaskEndScheduler_core2( void ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * void vTaskSuspendAll_core2( void );
 * @endcode
 *
 * Suspends the scheduler without disabling interrupts.  Context switches will
 * not occur while the scheduler is suspended.
 *
 * After calling vTaskSuspendAll_core2 () the calling task_core2 will continue to execute
 * without risk of being swapped out until a call to xTaskResumeAll_core2 () has been
 * made.
 *
 * API functions that have the potential to cause a context switch (for example,
 * xTaskDelayUntil_core2(), xQueueSend_core2(), etc.) must not be called while the scheduler
 * is suspended.
 *
 * Example usage:
 * @code{c}
 * void vTask1( void * pvParameters_core2 )
 * {
 *   for( ;; )
 *   {
 *       // Task code goes here.
 *
 *       // ...
 *
 *       // At some point the task_core2 wants to perform a long operation during
 *       // which it does not want to get swapped out.  It cannot use
 *       // taskENTER_CRITICAL_core2 ()/taskEXIT_CRITICAL_core2 () as the length of the
 *       // operation may cause interrupts to be missed - including the
 *       // ticks.
 *
 *       // Prevent the real time kernel swapping out the task_core2.
 *       vTaskSuspendAll_core2 ();
 *
 *       // Perform the operation here.  There is no need to use critical
 *       // sections as we have all the microcontroller processing time.
 *       // During this time interrupts will still operate and the kernel
 *       // tick count will be maintained.
 *
 *       // ...
 *
 *       // The operation is complete.  Restart the kernel.
 *       xTaskResumeAll_core2 ();
 *   }
 * }
 * @endcode
 * \defgroup vTaskSuspendAll_core2 vTaskSuspendAll_core2
 * \ingroup SchedulerControl
 */
void vTaskSuspendAll_core2( void ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * BaseType_t_core2 xTaskResumeAll_core2( void );
 * @endcode
 *
 * Resumes scheduler activity after it was suspended by a call to
 * vTaskSuspendAll_core2().
 *
 * xTaskResumeAll_core2() only resumes the scheduler.  It does not unsuspend tasks
 * that were previously suspended by a call to vTaskSuspend_core2().
 *
 * @return If resuming the scheduler caused a context switch then pdTRUE_core2 is
 *         returned, otherwise pdFALSE_core2 is returned.
 *
 * Example usage:
 * @code{c}
 * void vTask1( void * pvParameters_core2 )
 * {
 *   for( ;; )
 *   {
 *       // Task code goes here.
 *
 *       // ...
 *
 *       // At some point the task_core2 wants to perform a long operation during
 *       // which it does not want to get swapped out.  It cannot use
 *       // taskENTER_CRITICAL_core2 ()/taskEXIT_CRITICAL_core2 () as the length of the
 *       // operation may cause interrupts to be missed - including the
 *       // ticks.
 *
 *       // Prevent the real time kernel swapping out the task_core2.
 *       vTaskSuspendAll_core2 ();
 *
 *       // Perform the operation here.  There is no need to use critical
 *       // sections as we have all the microcontroller processing time.
 *       // During this time interrupts will still operate and the real
 *       // time kernel tick count will be maintained.
 *
 *       // ...
 *
 *       // The operation is complete.  Restart the kernel.  We want to force
 *       // a context switch - but there is no point if resuming the scheduler
 *       // caused a context switch already.
 *       if( !xTaskResumeAll_core2 () )
 *       {
 *            taskYIELD_core2 ();
 *       }
 *   }
 * }
 * @endcode
 * \defgroup xTaskResumeAll_core2 xTaskResumeAll_core2
 * \ingroup SchedulerControl
 */
BaseType_t_core2 xTaskResumeAll_core2( void ) PRIVILEGED_FUNCTION_core2;

/*-----------------------------------------------------------
* TASK UTILITIES
*----------------------------------------------------------*/

/**
 * task_core2. h
 * @code{c}
 * TickType_t_core2 xTaskGetTickCount_core2( void );
 * @endcode
 *
 * @return The count of ticks since vTaskStartScheduler_core2 was called.
 *
 * \defgroup xTaskGetTickCount_core2 xTaskGetTickCount_core2
 * \ingroup TaskUtils
 */
TickType_t_core2 xTaskGetTickCount_core2( void ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * TickType_t_core2 xTaskGetTickCountFromISR_core2( void );
 * @endcode
 *
 * @return The count of ticks since vTaskStartScheduler_core2 was called.
 *
 * This is a version of xTaskGetTickCount_core2() that is safe to be called from an
 * ISR - provided that TickType_t_core2 is the natural word size of the
 * microcontroller being used or interrupt nesting is either not supported or
 * not being used.
 *
 * \defgroup xTaskGetTickCountFromISR_core2 xTaskGetTickCountFromISR_core2
 * \ingroup TaskUtils
 */
TickType_t_core2 xTaskGetTickCountFromISR_core2( void ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * uint16_t uxTaskGetNumberOfTasks_core2( void );
 * @endcode
 *
 * @return The number of tasks that the real time kernel is currently managing.
 * This includes all ready, blocked and suspended tasks.  A task_core2 that
 * has been deleted but not yet freed by the idle task_core2 will also be
 * included in the count.
 *
 * \defgroup uxTaskGetNumberOfTasks_core2 uxTaskGetNumberOfTasks_core2
 * \ingroup TaskUtils
 */
UBaseType_t_core2 uxTaskGetNumberOfTasks_core2( void ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * char *pcTaskGetName_core2( TaskHandle_t_core2 xTaskToQuery_core2 );
 * @endcode
 *
 * @return The text (human readable) name of the task_core2 referenced by the handle
 * xTaskToQuery_core2.  A task_core2 can query its own name by either passing in its own
 * handle, or by setting xTaskToQuery_core2 to NULL.
 *
 * \defgroup pcTaskGetName_core2 pcTaskGetName_core2
 * \ingroup TaskUtils
 */
char * pcTaskGetName_core2( TaskHandle_t_core2 xTaskToQuery_core2 ) PRIVILEGED_FUNCTION_core2; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

/**
 * task_core2. h
 * @code{c}
 * TaskHandle_t_core2 xTaskGetHandle_core2( const char *pcNameToQuery_core2 );
 * @endcode
 *
 * NOTE:  This function takes a relatively long time to complete and should be
 * used sparingly.
 *
 * @return The handle of the task_core2 that has the human readable name pcNameToQuery_core2.
 * NULL is returned if no matching name is found.  INCLUDE_xTaskGetHandle_core2
 * must be set to 1 in FreeRTOSConfig.h for pcTaskGetHandle() to be available.
 *
 * \defgroup pcTaskGetHandle pcTaskGetHandle
 * \ingroup TaskUtils
 */
TaskHandle_t_core2 xTaskGetHandle_core2( const char * pcNameToQuery_core2 ) PRIVILEGED_FUNCTION_core2; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

/**
 * task_core2.h
 * @code{c}
 * UBaseType_t_core2 uxTaskGetStackHighWaterMark_core2( TaskHandle_t_core2 xTask_core2 );
 * @endcode
 *
 * INCLUDE_uxTaskGetStackHighWaterMark_core2 must be set to 1 in FreeRTOSConfig.h for
 * this function to be available.
 *
 * Returns the high water mark of the stack associated with xTask_core2.  That is,
 * the minimum free stack space there has been (in words, so on a 32 bit machine
 * a value of 1 means 4 bytes) since the task_core2 started.  The smaller the returned
 * number the closer the task_core2 has come to overflowing its stack.
 *
 * uxTaskGetStackHighWaterMark_core2() and uxTaskGetStackHighWaterMark2_core2() are the
 * same except for their return type.  Using configSTACK_DEPTH_TYPE_core2 allows the
 * user to determine the return type.  It gets around the problem of the value
 * overflowing on 8-bit types without breaking backward compatibility for
 * applications that expect an 8-bit return type.
 *
 * @param xTask_core2 Handle of the task_core2 associated with the stack to be checked.
 * Set xTask_core2 to NULL to check the stack of the calling task_core2.
 *
 * @return The smallest amount of free stack space there has been (in words, so
 * actual spaces on the stack rather than bytes) since the task_core2 referenced by
 * xTask_core2 was created.
 */
UBaseType_t_core2 uxTaskGetStackHighWaterMark_core2( TaskHandle_t_core2 xTask_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2.h
 * @code{c}
 * configSTACK_DEPTH_TYPE_core2 uxTaskGetStackHighWaterMark2_core2( TaskHandle_t_core2 xTask_core2 );
 * @endcode
 *
 * INCLUDE_uxTaskGetStackHighWaterMark2_core2 must be set to 1 in FreeRTOSConfig.h for
 * this function to be available.
 *
 * Returns the high water mark of the stack associated with xTask_core2.  That is,
 * the minimum free stack space there has been (in words, so on a 32 bit machine
 * a value of 1 means 4 bytes) since the task_core2 started.  The smaller the returned
 * number the closer the task_core2 has come to overflowing its stack.
 *
 * uxTaskGetStackHighWaterMark_core2() and uxTaskGetStackHighWaterMark2_core2() are the
 * same except for their return type.  Using configSTACK_DEPTH_TYPE_core2 allows the
 * user to determine the return type.  It gets around the problem of the value
 * overflowing on 8-bit types without breaking backward compatibility for
 * applications that expect an 8-bit return type.
 *
 * @param xTask_core2 Handle of the task_core2 associated with the stack to be checked.
 * Set xTask_core2 to NULL to check the stack of the calling task_core2.
 *
 * @return The smallest amount of free stack space there has been (in words, so
 * actual spaces on the stack rather than bytes) since the task_core2 referenced by
 * xTask_core2 was created.
 */
configSTACK_DEPTH_TYPE_core2 uxTaskGetStackHighWaterMark2_core2( TaskHandle_t_core2 xTask_core2 ) PRIVILEGED_FUNCTION_core2;

/* When using trace macros it is sometimes necessary to include task_core2.h before
 * FreeRTOS_core2.h.  When this is done TaskHookFunction_t_core2 will not yet have been defined,
 * so the following two prototypes will cause a compilation error.  This can be
 * fixed by simply guarding against the inclusion of these two prototypes unless
 * they are explicitly required by the configUSE_APPLICATION_TASK_TAG_core2 configuration
 * constant. */
#ifdef configUSE_APPLICATION_TASK_TAG_core2
    #if configUSE_APPLICATION_TASK_TAG_core2 == 1

/**
 * task_core2.h
 * @code{c}
 * void vTaskSetApplicationTaskTag_core2( TaskHandle_t_core2 xTask_core2, TaskHookFunction_t_core2 pxHookFunction_core2 );
 * @endcode
 *
 * Sets pxHookFunction_core2 to be the task_core2 hook function used by the task_core2 xTask_core2.
 * Passing xTask_core2 as NULL has the effect of setting the calling tasks hook
 * function.
 */
        void vTaskSetApplicationTaskTag_core2( TaskHandle_t_core2 xTask_core2,
                                         TaskHookFunction_t_core2 pxHookFunction_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2.h
 * @code{c}
 * void xTaskGetApplicationTaskTag_core2( TaskHandle_t_core2 xTask_core2 );
 * @endcode
 *
 * Returns the pxHookFunction_core2 value assigned to the task_core2 xTask_core2.  Do not
 * call from an interrupt service routine - call
 * xTaskGetApplicationTaskTagFromISR_core2() instead.
 */
        TaskHookFunction_t_core2 xTaskGetApplicationTaskTag_core2( TaskHandle_t_core2 xTask_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2.h
 * @code{c}
 * void xTaskGetApplicationTaskTagFromISR_core2( TaskHandle_t_core2 xTask_core2 );
 * @endcode
 *
 * Returns the pxHookFunction_core2 value assigned to the task_core2 xTask_core2.  Can
 * be called from an interrupt service routine.
 */
        TaskHookFunction_t_core2 xTaskGetApplicationTaskTagFromISR_core2( TaskHandle_t_core2 xTask_core2 ) PRIVILEGED_FUNCTION_core2;
    #endif /* configUSE_APPLICATION_TASK_TAG_core2 ==1 */
#endif /* ifdef configUSE_APPLICATION_TASK_TAG_core2 */

#if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS_core2 > 0 )

/* Each task_core2 contains an array of pointers that is dimensioned by the
 * configNUM_THREAD_LOCAL_STORAGE_POINTERS_core2 setting in FreeRTOSConfig.h.  The
 * kernel does not use the pointers itself, so the application writer can use
 * the pointers for any purpose they wish.  The following two functions are
 * used to set and query a pointer respectively. */
    void vTaskSetThreadLocalStoragePointer_core2( TaskHandle_t_core2 xTaskToSet_core2,
                                            BaseType_t_core2 xIndex_core2,
                                            void * pvValue_core2 ) PRIVILEGED_FUNCTION_core2;
    void * pvTaskGetThreadLocalStoragePointer_core2( TaskHandle_t_core2 xTaskToQuery_core2,
                                               BaseType_t_core2 xIndex_core2 ) PRIVILEGED_FUNCTION_core2;

#endif

#if ( configCHECK_FOR_STACK_OVERFLOW_core2 > 0 )

/**
 * task_core2.h
 * @code{c}
 * void vApplicationStackOverflowHook_core2( TaskHandle_t_core2 xTask_core2 char *pcTaskName_core2);
 * @endcode
 *
 * The application stack overflow hook is called when a stack overflow is detected for a task_core2.
 *
 * Details on stack overflow detection can be found here: https://www.FreeRTOS_core2.org/Stacks-and-stack-overflow-checking.html
 *
 * @param xTask_core2 the task_core2 that just exceeded its stack boundaries.
 * @param pcTaskName_core2 A character string containing the name of the offending task_core2.
 */
    void vApplicationStackOverflowHook_core2( TaskHandle_t_core2 xTask_core2,
                                        char * pcTaskName_core2 );

#endif

#if  ( configUSE_TICK_HOOK_core2 > 0 )

/**
 *  task_core2.h
 * @code{c}
 * void vApplicationTickHook_core2( void );
 * @endcode
 *
 * This hook function is called in the system tick handler after any OS work is completed.
 */
    void vApplicationTickHook_core2( void ); /*lint !e526 Symbol not defined as it is an application callback. */

#endif

#if ( configSUPPORT_STATIC_ALLOCATION_core2 == 1 )

/**
 * task_core2.h
 * @code{c}
 * void vApplicationGetIdleTaskMemory_core2( StaticTask_t_core2 ** ppxIdleTaskTCBBuffer_core2, StackType_t_core2 ** ppxIdleTaskStackBuffer_core2, uint32_t *pulIdleTaskStackSize_core2 )
 * @endcode
 *
 * This function is used to provide a statically allocated block of memory to FreeRTOS_core2 to hold the Idle Task TCB.  This function is required when
 * configSUPPORT_STATIC_ALLOCATION_core2 is set.  For more information see this URI: https://www.FreeRTOS_core2.org/a00110.html#configSUPPORT_STATIC_ALLOCATION_core2
 *
 * @param ppxIdleTaskTCBBuffer_core2 A handle to a statically allocated TCB buffer
 * @param ppxIdleTaskStackBuffer_core2 A handle to a statically allocated Stack buffer for the idle task_core2
 * @param pulIdleTaskStackSize_core2 A pointer to the number of elements that will fit in the allocated stack buffer
 */
    void vApplicationGetIdleTaskMemory_core2( StaticTask_t_core2 ** ppxIdleTaskTCBBuffer_core2,
                                        StackType_t_core2 ** ppxIdleTaskStackBuffer_core2,
                                        uint32_t * pulIdleTaskStackSize_core2 ); /*lint !e526 Symbol not defined as it is an application callback. */
#endif

/**
 * task_core2.h
 * @code{c}
 * BaseType_t_core2 xTaskCallApplicationTaskHook_core2( TaskHandle_t_core2 xTask_core2, void *pvParameter_core2 );
 * @endcode
 *
 * Calls the hook function associated with xTask_core2.  Passing xTask_core2 as NULL has
 * the effect of calling the Running tasks (the calling task_core2) hook function.
 *
 * pvParameter_core2 is passed to the hook function for the task_core2 to interpret as it
 * wants.  The return value is the value returned by the task_core2 hook function
 * registered by the user.
 */
BaseType_t_core2 xTaskCallApplicationTaskHook_core2( TaskHandle_t_core2 xTask_core2,
                                         void * pvParameter_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * xTaskGetIdleTaskHandle_core2() is only available if
 * INCLUDE_xTaskGetIdleTaskHandle_core2 is set to 1 in FreeRTOSConfig.h.
 *
 * Simply returns the handle of the idle task_core2.  It is not valid to call
 * xTaskGetIdleTaskHandle_core2() before the scheduler has been started.
 */
TaskHandle_t_core2 xTaskGetIdleTaskHandle_core2( void ) PRIVILEGED_FUNCTION_core2;

/**
 * configUSE_TRACE_FACILITY_core2 must be defined as 1 in FreeRTOSConfig.h for
 * uxTaskGetSystemState_core2() to be available.
 *
 * uxTaskGetSystemState_core2() populates an TaskStatus_t_core2 structure for each task_core2 in
 * the system.  TaskStatus_t_core2 structures contain, among other things, members
 * for the task_core2 handle, task_core2 name, task_core2 priority, task_core2 state, and total amount
 * of run time consumed by the task_core2.  See the TaskStatus_t_core2 structure
 * definition in this file for the full member list.
 *
 * NOTE:  This function is intended for debugging use only as its use results in
 * the scheduler remaining suspended for an extended period.
 *
 * @param pxTaskStatusArray_core2 A pointer to an array of TaskStatus_t_core2 structures.
 * The array must contain at least one TaskStatus_t_core2 structure for each task_core2
 * that is under the control of the RTOS.  The number of tasks under the control
 * of the RTOS can be determined using the uxTaskGetNumberOfTasks_core2() API function.
 *
 * @param uxArraySize_core2 The size of the array pointed to by the pxTaskStatusArray_core2
 * parameter.  The size is specified as the number of indexes in the array, or
 * the number of TaskStatus_t_core2 structures contained in the array, not by the
 * number of bytes in the array.
 *
 * @param pulTotalRunTime_core2 If configGENERATE_RUN_TIME_STATS_core2 is set to 1 in
 * FreeRTOSConfig.h then *pulTotalRunTime_core2 is set by uxTaskGetSystemState_core2() to the
 * total run time (as defined by the run time stats clock, see
 * https://www.FreeRTOS_core2.org/rtos-run-time-stats.html) since the target booted.
 * pulTotalRunTime_core2 can be set to NULL to omit the total run time information.
 *
 * @return The number of TaskStatus_t_core2 structures that were populated by
 * uxTaskGetSystemState_core2().  This should equal the number returned by the
 * uxTaskGetNumberOfTasks_core2() API function, but will be zero if the value passed
 * in the uxArraySize_core2 parameter was too small.
 *
 * Example usage:
 * @code{c}
 *  // This example demonstrates how a human readable table of run time stats
 *  // information is generated from raw data provided by uxTaskGetSystemState_core2().
 *  // The human readable table is written to pcWriteBuffer_core2
 *  void vTaskGetRunTimeStats_core2( char *pcWriteBuffer_core2 )
 *  {
 *  TaskStatus_t_core2 *pxTaskStatusArray_core2;
 *  volatile UBaseType_t_core2 uxArraySize_core2, x_core2;
 *  configRUN_TIME_COUNTER_TYPE_core2 ulTotalRunTime_core2, ulStatsAsPercentage;
 *
 *      // Make sure the write buffer does not contain a string.
 * pcWriteBuffer_core2 = 0x00;
 *
 *      // Take a snapshot of the number of tasks in case it changes while this
 *      // function is executing.
 *      uxArraySize_core2 = uxTaskGetNumberOfTasks_core2();
 *
 *      // Allocate a TaskStatus_t_core2 structure for each task_core2.  An array could be
 *      // allocated statically at compile time.
 *      pxTaskStatusArray_core2 = pvPortMalloc_core2( uxArraySize_core2 * sizeof( TaskStatus_t_core2 ) );
 *
 *      if( pxTaskStatusArray_core2 != NULL )
 *      {
 *          // Generate raw status information about each task_core2.
 *          uxArraySize_core2 = uxTaskGetSystemState_core2( pxTaskStatusArray_core2, uxArraySize_core2, &ulTotalRunTime_core2 );
 *
 *          // For percentage calculations.
 *          ulTotalRunTime_core2 /= 100UL;
 *
 *          // Avoid divide by zero errors.
 *          if( ulTotalRunTime_core2 > 0 )
 *          {
 *              // For each populated position in the pxTaskStatusArray_core2 array,
 *              // format the raw data as human readable ASCII data
 *              for( x_core2 = 0; x_core2 < uxArraySize_core2; x_core2++ )
 *              {
 *                  // What percentage of the total run time has the task_core2 used?
 *                  // This will always be rounded down to the nearest integer.
 *                  // ulTotalRunTimeDiv100 has already been divided by 100.
 *                  ulStatsAsPercentage = pxTaskStatusArray_core2[ x_core2 ].ulRunTimeCounter_core2 / ulTotalRunTime_core2;
 *
 *                  if( ulStatsAsPercentage > 0UL )
 *                  {
 *                      sprintf( pcWriteBuffer_core2, "%s\t\t%lu\t\t%lu%%\r\n", pxTaskStatusArray_core2[ x_core2 ].pcTaskName_core2, pxTaskStatusArray_core2[ x_core2 ].ulRunTimeCounter_core2, ulStatsAsPercentage );
 *                  }
 *                  else
 *                  {
 *                      // If the percentage is zero here then the task_core2 has
 *                      // consumed less than 1% of the total run time.
 *                      sprintf( pcWriteBuffer_core2, "%s\t\t%lu\t\t<1%%\r\n", pxTaskStatusArray_core2[ x_core2 ].pcTaskName_core2, pxTaskStatusArray_core2[ x_core2 ].ulRunTimeCounter_core2 );
 *                  }
 *
 *                  pcWriteBuffer_core2 += strlen( ( char * ) pcWriteBuffer_core2 );
 *              }
 *          }
 *
 *          // The array is no longer needed, free the memory it consumes.
 *          vPortFree_core2( pxTaskStatusArray_core2 );
 *      }
 *  }
 *  @endcode
 */
UBaseType_t_core2 uxTaskGetSystemState_core2( TaskStatus_t_core2 * const pxTaskStatusArray_core2,
                                  const UBaseType_t_core2 uxArraySize_core2,
                                  configRUN_TIME_COUNTER_TYPE_core2 * const pulTotalRunTime_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * void vTaskList_core2( char *pcWriteBuffer_core2 );
 * @endcode
 *
 * configUSE_TRACE_FACILITY_core2 and configUSE_STATS_FORMATTING_FUNCTIONS_core2 must
 * both be defined as 1 for this function to be available.  See the
 * configuration section of the FreeRTOS_core2.org website for more information.
 *
 * NOTE 1: This function will disable interrupts for its duration.  It is
 * not intended for normal application runtime use but as a debug aid.
 *
 * Lists all the current tasks, along with their current state and stack
 * usage high water mark.
 *
 * Tasks are reported as blocked ('B'), ready ('R'), deleted ('D') or
 * suspended ('S').
 *
 * PLEASE NOTE:
 *
 * This function is provided for convenience only, and is used by many of the
 * demo applications.  Do not consider it to be part of the scheduler.
 *
 * vTaskList_core2() calls uxTaskGetSystemState_core2(), then formats part of the
 * uxTaskGetSystemState_core2() output into a human readable table that displays task_core2:
 * names, states, priority, stack usage and task_core2 number.
 * Stack usage specified as the number of unused StackType_t_core2 words stack can hold
 * on top of stack - not the number of bytes.
 *
 * vTaskList_core2() has a dependency on the sprintf() C library function that might
 * bloat the code size, use a lot of stack, and provide different results on
 * different platforms.  An alternative, tiny, third party, and limited
 * functionality implementation of sprintf() is provided in many of the
 * FreeRTOS_core2/Demo sub-directories in a file called printf-stdarg.c (note
 * printf-stdarg.c does not provide a full snprintf() implementation!).
 *
 * It is recommended that production systems call uxTaskGetSystemState_core2()
 * directly to get access to raw stats data, rather than indirectly through a
 * call to vTaskList_core2().
 *
 * @param pcWriteBuffer_core2 A buffer into which the above mentioned details
 * will be written, in ASCII form.  This buffer is assumed to be large
 * enough to contain the generated report.  Approximately 40 bytes per
 * task_core2 should be sufficient.
 *
 * \defgroup vTaskList_core2 vTaskList_core2
 * \ingroup TaskUtils
 */
void vTaskList_core2( char * pcWriteBuffer_core2 ) PRIVILEGED_FUNCTION_core2; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

/**
 * task_core2. h
 * @code{c}
 * void vTaskGetRunTimeStats_core2( char *pcWriteBuffer_core2 );
 * @endcode
 *
 * configGENERATE_RUN_TIME_STATS_core2 and configUSE_STATS_FORMATTING_FUNCTIONS_core2
 * must both be defined as 1 for this function to be available.  The application
 * must also then provide definitions for
 * portCONFIGURE_TIMER_FOR_RUN_TIME_STATS_core2() and portGET_RUN_TIME_COUNTER_VALUE_core2()
 * to configure a peripheral timer/counter and return the timers_core2 current count
 * value respectively.  The counter should be at least 10 times the frequency of
 * the tick count.
 *
 * NOTE 1: This function will disable interrupts for its duration.  It is
 * not intended for normal application runtime use but as a debug aid.
 *
 * Setting configGENERATE_RUN_TIME_STATS_core2 to 1 will result in a total
 * accumulated execution time being stored for each task_core2.  The resolution
 * of the accumulated time value depends on the frequency of the timer
 * configured by the portCONFIGURE_TIMER_FOR_RUN_TIME_STATS_core2() macro.
 * Calling vTaskGetRunTimeStats_core2() writes the total execution time of each
 * task_core2 into a buffer, both as an absolute count value and as a percentage
 * of the total system execution time.
 *
 * NOTE 2:
 *
 * This function is provided for convenience only, and is used by many of the
 * demo applications.  Do not consider it to be part of the scheduler.
 *
 * vTaskGetRunTimeStats_core2() calls uxTaskGetSystemState_core2(), then formats part of the
 * uxTaskGetSystemState_core2() output into a human readable table that displays the
 * amount of time each task_core2 has spent in the Running state in both absolute and
 * percentage terms.
 *
 * vTaskGetRunTimeStats_core2() has a dependency on the sprintf() C library function
 * that might bloat the code size, use a lot of stack, and provide different
 * results on different platforms.  An alternative, tiny, third party, and
 * limited functionality implementation of sprintf() is provided in many of the
 * FreeRTOS_core2/Demo sub-directories in a file called printf-stdarg.c (note
 * printf-stdarg.c does not provide a full snprintf() implementation!).
 *
 * It is recommended that production systems call uxTaskGetSystemState_core2() directly
 * to get access to raw stats data, rather than indirectly through a call to
 * vTaskGetRunTimeStats_core2().
 *
 * @param pcWriteBuffer_core2 A buffer into which the execution times will be
 * written, in ASCII form.  This buffer is assumed to be large enough to
 * contain the generated report.  Approximately 40 bytes per task_core2 should
 * be sufficient.
 *
 * \defgroup vTaskGetRunTimeStats_core2 vTaskGetRunTimeStats_core2
 * \ingroup TaskUtils
 */
void vTaskGetRunTimeStats_core2( char * pcWriteBuffer_core2 ) PRIVILEGED_FUNCTION_core2; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

/**
 * task_core2. h
 * @code{c}
 * configRUN_TIME_COUNTER_TYPE_core2 ulTaskGetIdleRunTimeCounter_core2( void );
 * configRUN_TIME_COUNTER_TYPE_core2 ulTaskGetIdleRunTimePercent_core2( void );
 * @endcode
 *
 * configGENERATE_RUN_TIME_STATS_core2, configUSE_STATS_FORMATTING_FUNCTIONS_core2 and
 * INCLUDE_xTaskGetIdleTaskHandle_core2 must all be defined as 1 for these functions
 * to be available.  The application must also then provide definitions for
 * portCONFIGURE_TIMER_FOR_RUN_TIME_STATS_core2() and portGET_RUN_TIME_COUNTER_VALUE_core2()
 * to configure a peripheral timer/counter and return the timers_core2 current count
 * value respectively.  The counter should be at least 10 times the frequency of
 * the tick count.
 *
 * Setting configGENERATE_RUN_TIME_STATS_core2 to 1 will result in a total
 * accumulated execution time being stored for each task_core2.  The resolution
 * of the accumulated time value depends on the frequency of the timer
 * configured by the portCONFIGURE_TIMER_FOR_RUN_TIME_STATS_core2() macro.
 * While uxTaskGetSystemState_core2() and vTaskGetRunTimeStats_core2() writes the total
 * execution time of each task_core2 into a buffer, ulTaskGetIdleRunTimeCounter_core2()
 * returns the total execution time of just the idle task_core2 and
 * ulTaskGetIdleRunTimePercent_core2() returns the percentage of the CPU time used by
 * just the idle task_core2.
 *
 * Note the amount of idle time is only a good measure of the slack time in a
 * system if there are no other tasks executing at the idle priority, tickless
 * idle is not used, and configIDLE_SHOULD_YIELD_core2 is set to 0.
 *
 * @return The total run time of the idle task_core2 or the percentage of the total
 * run time consumed by the idle task_core2.  This is the amount of time the
 * idle task_core2 has actually been executing.  The unit of time is dependent on the
 * frequency configured using the portCONFIGURE_TIMER_FOR_RUN_TIME_STATS_core2() and
 * portGET_RUN_TIME_COUNTER_VALUE_core2() macros.
 *
 * \defgroup ulTaskGetIdleRunTimeCounter_core2 ulTaskGetIdleRunTimeCounter_core2
 * \ingroup TaskUtils
 */
configRUN_TIME_COUNTER_TYPE_core2 ulTaskGetIdleRunTimeCounter_core2( void ) PRIVILEGED_FUNCTION_core2;
configRUN_TIME_COUNTER_TYPE_core2 ulTaskGetIdleRunTimePercent_core2( void ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2. h
 * @code{c}
 * BaseType_t_core2 xTaskNotifyIndexed_core2( TaskHandle_t_core2 xTaskToNotify_core2, UBaseType_t_core2 uxIndexToNotify_core2, uint32_t ulValue_core2, eNotifyAction_core2 eAction_core2 );
 * BaseType_t_core2 xTaskNotify_core2( TaskHandle_t_core2 xTaskToNotify_core2, uint32_t ulValue_core2, eNotifyAction_core2 eAction_core2 );
 * @endcode
 *
 * See https://www.FreeRTOS_core2.org/RTOS-task_core2-notifications.html for details.
 *
 * configUSE_TASK_NOTIFICATIONS_core2 must be undefined or defined as 1 for these
 * functions to be available.
 *
 * Sends a direct to task_core2 notification to a task_core2, with an optional value and
 * action.
 *
 * Each task_core2 has a private array of "notification values" (or 'notifications'),
 * each of which is a 32-bit unsigned integer (uint32_t).  The constant
 * configTASK_NOTIFICATION_ARRAY_ENTRIES_core2 sets the number of indexes in the
 * array, and (for backward compatibility) defaults to 1 if left undefined.
 * Prior to FreeRTOS_core2 V10.4.0 there was only one notification value per task_core2.
 *
 * Events can be sent to a task_core2 using an intermediary object.  Examples of such
 * objects are queues, semaphores, mutexes and event groups.  Task notifications
 * are a method of sending an event directly to a task_core2 without the need for such
 * an intermediary object.
 *
 * A notification sent to a task_core2 can optionally perform an action, such as
 * update, overwrite or increment one of the task_core2's notification values.  In
 * that way task_core2 notifications can be used to send data to a task_core2, or be used as
 * light weight and fast binary or counting semaphores.
 *
 * A task_core2 can use xTaskNotifyWaitIndexed_core2() or ulTaskNotifyTakeIndexed_core2() to
 * [optionally] block to wait for a notification to be pending.  The task_core2 does
 * not consume any CPU time while it is in the Blocked state.
 *
 * A notification sent to a task_core2 will remain pending until it is cleared by the
 * task_core2 calling xTaskNotifyWaitIndexed_core2() or ulTaskNotifyTakeIndexed_core2() (or their
 * un-indexed equivalents).  If the task_core2 was already in the Blocked state to
 * wait for a notification when the notification arrives then the task_core2 will
 * automatically be removed from the Blocked state (unblocked) and the
 * notification cleared.
 *
 * **NOTE** Each notification within the array operates independently - a task_core2
 * can only block on one notification within the array at a time and will not be
 * unblocked by a notification sent to any other array index.
 *
 * Backward compatibility information:
 * Prior to FreeRTOS_core2 V10.4.0 each task_core2 had a single "notification value", and
 * all task_core2 notification API functions operated on that value. Replacing the
 * single notification value with an array of notification values necessitated a
 * new set of API functions that could address specific notifications within the
 * array.  xTaskNotify_core2() is the original API function, and remains backward
 * compatible by always operating on the notification value at index 0 in the
 * array. Calling xTaskNotify_core2() is equivalent to calling xTaskNotifyIndexed_core2()
 * with the uxIndexToNotify_core2 parameter set to 0.
 *
 * @param xTaskToNotify_core2 The handle of the task_core2 being notified.  The handle to a
 * task_core2 can be returned from the xTaskCreate_core2() API function used to create the
 * task_core2, and the handle of the currently running task_core2 can be obtained by calling
 * xTaskGetCurrentTaskHandle_core2().
 *
 * @param uxIndexToNotify_core2 The index within the target task_core2's array of
 * notification values to which the notification is to be sent.  uxIndexToNotify_core2
 * must be less than configTASK_NOTIFICATION_ARRAY_ENTRIES_core2.  xTaskNotify_core2() does
 * not have this parameter and always sends notifications to index 0.
 *
 * @param ulValue_core2 Data that can be sent with the notification.  How the data is
 * used depends on the value of the eAction_core2 parameter.
 *
 * @param eAction_core2 Specifies how the notification updates the task_core2's notification
 * value, if at all.  Valid values for eAction_core2 are as follows:
 *
 * eSetBits -
 * The target notification value is bitwise ORed with ulValue_core2.
 * xTaskNotifyIndexed_core2() always returns pdPASS_core2 in this case.
 *
 * eIncrement_core2 -
 * The target notification value is incremented.  ulValue_core2 is not used and
 * xTaskNotifyIndexed_core2() always returns pdPASS_core2 in this case.
 *
 * eSetValueWithOverwrite -
 * The target notification value is set to the value of ulValue_core2, even if the
 * task_core2 being notified had not yet processed the previous notification at the
 * same array index (the task_core2 already had a notification pending at that index).
 * xTaskNotifyIndexed_core2() always returns pdPASS_core2 in this case.
 *
 * eSetValueWithoutOverwrite -
 * If the task_core2 being notified did not already have a notification pending at the
 * same array index then the target notification value is set to ulValue_core2 and
 * xTaskNotifyIndexed_core2() will return pdPASS_core2.  If the task_core2 being notified already
 * had a notification pending at the same array index then no action is
 * performed and pdFAIL_core2 is returned.
 *
 * eNoAction_core2 -
 * The task_core2 receives a notification at the specified array index without the
 * notification value at that index being updated.  ulValue_core2 is not used and
 * xTaskNotifyIndexed_core2() always returns pdPASS_core2 in this case.
 *
 * pulPreviousNotificationValue_core2 -
 * Can be used to pass out the subject task_core2's notification value before any
 * bits are modified by the notify function.
 *
 * @return Dependent on the value of eAction_core2.  See the description of the
 * eAction_core2 parameter.
 *
 * \defgroup xTaskNotifyIndexed_core2 xTaskNotifyIndexed_core2
 * \ingroup TaskNotifications
 */
BaseType_t_core2 xTaskGenericNotify_core2( TaskHandle_t_core2 xTaskToNotify_core2,
                               UBaseType_t_core2 uxIndexToNotify_core2,
                               uint32_t ulValue_core2,
                               eNotifyAction_core2 eAction_core2,
                               uint32_t * pulPreviousNotificationValue_core2 ) PRIVILEGED_FUNCTION_core2;
#define xTaskNotify_core2( xTaskToNotify_core2, ulValue_core2, eAction_core2 ) \
    xTaskGenericNotify_core2( ( xTaskToNotify_core2 ), ( tskDEFAULT_INDEX_TO_NOTIFY_core2 ), ( ulValue_core2 ), ( eAction_core2 ), NULL )
#define xTaskNotifyIndexed_core2( xTaskToNotify_core2, uxIndexToNotify_core2, ulValue_core2, eAction_core2 ) \
    xTaskGenericNotify_core2( ( xTaskToNotify_core2 ), ( uxIndexToNotify_core2 ), ( ulValue_core2 ), ( eAction_core2 ), NULL )

/**
 * task_core2. h
 * @code{c}
 * BaseType_t_core2 xTaskNotifyAndQueryIndexed_core2( TaskHandle_t_core2 xTaskToNotify_core2, UBaseType_t_core2 uxIndexToNotify_core2, uint32_t ulValue_core2, eNotifyAction_core2 eAction_core2, uint32_t *pulPreviousNotifyValue_core2 );
 * BaseType_t_core2 xTaskNotifyAndQuery_core2( TaskHandle_t_core2 xTaskToNotify_core2, uint32_t ulValue_core2, eNotifyAction_core2 eAction_core2, uint32_t *pulPreviousNotifyValue_core2 );
 * @endcode
 *
 * See https://www.FreeRTOS_core2.org/RTOS-task_core2-notifications.html for details.
 *
 * xTaskNotifyAndQueryIndexed_core2() performs the same operation as
 * xTaskNotifyIndexed_core2() with the addition that it also returns the subject
 * task_core2's prior notification value (the notification value at the time the
 * function is called rather than when the function returns) in the additional
 * pulPreviousNotifyValue_core2 parameter.
 *
 * xTaskNotifyAndQuery_core2() performs the same operation as xTaskNotify_core2() with the
 * addition that it also returns the subject task_core2's prior notification value
 * (the notification value as it was at the time the function is called, rather
 * than when the function returns) in the additional pulPreviousNotifyValue_core2
 * parameter.
 *
 * \defgroup xTaskNotifyAndQueryIndexed_core2 xTaskNotifyAndQueryIndexed_core2
 * \ingroup TaskNotifications
 */
#define xTaskNotifyAndQuery_core2( xTaskToNotify_core2, ulValue_core2, eAction_core2, pulPreviousNotifyValue_core2 ) \
    xTaskGenericNotify_core2( ( xTaskToNotify_core2 ), ( tskDEFAULT_INDEX_TO_NOTIFY_core2 ), ( ulValue_core2 ), ( eAction_core2 ), ( pulPreviousNotifyValue_core2 ) )
#define xTaskNotifyAndQueryIndexed_core2( xTaskToNotify_core2, uxIndexToNotify_core2, ulValue_core2, eAction_core2, pulPreviousNotifyValue_core2 ) \
    xTaskGenericNotify_core2( ( xTaskToNotify_core2 ), ( uxIndexToNotify_core2 ), ( ulValue_core2 ), ( eAction_core2 ), ( pulPreviousNotifyValue_core2 ) )

/**
 * task_core2. h
 * @code{c}
 * BaseType_t_core2 xTaskNotifyIndexedFromISR_core2( TaskHandle_t_core2 xTaskToNotify_core2, UBaseType_t_core2 uxIndexToNotify_core2, uint32_t ulValue_core2, eNotifyAction_core2 eAction_core2, BaseType_t_core2 *pxHigherPriorityTaskWoken_core2 );
 * BaseType_t_core2 xTaskNotifyFromISR_core2( TaskHandle_t_core2 xTaskToNotify_core2, uint32_t ulValue_core2, eNotifyAction_core2 eAction_core2, BaseType_t_core2 *pxHigherPriorityTaskWoken_core2 );
 * @endcode
 *
 * See https://www.FreeRTOS_core2.org/RTOS-task_core2-notifications.html for details.
 *
 * configUSE_TASK_NOTIFICATIONS_core2 must be undefined or defined as 1 for these
 * functions to be available.
 *
 * A version of xTaskNotifyIndexed_core2() that can be used from an interrupt service
 * routine (ISR).
 *
 * Each task_core2 has a private array of "notification values" (or 'notifications'),
 * each of which is a 32-bit unsigned integer (uint32_t).  The constant
 * configTASK_NOTIFICATION_ARRAY_ENTRIES_core2 sets the number of indexes in the
 * array, and (for backward compatibility) defaults to 1 if left undefined.
 * Prior to FreeRTOS_core2 V10.4.0 there was only one notification value per task_core2.
 *
 * Events can be sent to a task_core2 using an intermediary object.  Examples of such
 * objects are queues, semaphores, mutexes and event groups.  Task notifications
 * are a method of sending an event directly to a task_core2 without the need for such
 * an intermediary object.
 *
 * A notification sent to a task_core2 can optionally perform an action, such as
 * update, overwrite or increment one of the task_core2's notification values.  In
 * that way task_core2 notifications can be used to send data to a task_core2, or be used as
 * light weight and fast binary or counting semaphores.
 *
 * A task_core2 can use xTaskNotifyWaitIndexed_core2() to [optionally] block to wait for a
 * notification to be pending, or ulTaskNotifyTakeIndexed_core2() to [optionally] block
 * to wait for a notification value to have a non-zero value.  The task_core2 does
 * not consume any CPU time while it is in the Blocked state.
 *
 * A notification sent to a task_core2 will remain pending until it is cleared by the
 * task_core2 calling xTaskNotifyWaitIndexed_core2() or ulTaskNotifyTakeIndexed_core2() (or their
 * un-indexed equivalents).  If the task_core2 was already in the Blocked state to
 * wait for a notification when the notification arrives then the task_core2 will
 * automatically be removed from the Blocked state (unblocked) and the
 * notification cleared.
 *
 * **NOTE** Each notification within the array operates independently - a task_core2
 * can only block on one notification within the array at a time and will not be
 * unblocked by a notification sent to any other array index.
 *
 * Backward compatibility information:
 * Prior to FreeRTOS_core2 V10.4.0 each task_core2 had a single "notification value", and
 * all task_core2 notification API functions operated on that value. Replacing the
 * single notification value with an array of notification values necessitated a
 * new set of API functions that could address specific notifications within the
 * array.  xTaskNotifyFromISR_core2() is the original API function, and remains
 * backward compatible by always operating on the notification value at index 0
 * within the array. Calling xTaskNotifyFromISR_core2() is equivalent to calling
 * xTaskNotifyIndexedFromISR_core2() with the uxIndexToNotify_core2 parameter set to 0.
 *
 * @param uxIndexToNotify_core2 The index within the target task_core2's array of
 * notification values to which the notification is to be sent.  uxIndexToNotify_core2
 * must be less than configTASK_NOTIFICATION_ARRAY_ENTRIES_core2.  xTaskNotifyFromISR_core2()
 * does not have this parameter and always sends notifications to index 0.
 *
 * @param xTaskToNotify_core2 The handle of the task_core2 being notified.  The handle to a
 * task_core2 can be returned from the xTaskCreate_core2() API function used to create the
 * task_core2, and the handle of the currently running task_core2 can be obtained by calling
 * xTaskGetCurrentTaskHandle_core2().
 *
 * @param ulValue_core2 Data that can be sent with the notification.  How the data is
 * used depends on the value of the eAction_core2 parameter.
 *
 * @param eAction_core2 Specifies how the notification updates the task_core2's notification
 * value, if at all.  Valid values for eAction_core2 are as follows:
 *
 * eSetBits -
 * The task_core2's notification value is bitwise ORed with ulValue_core2.  xTaskNotify_core2()
 * always returns pdPASS_core2 in this case.
 *
 * eIncrement_core2 -
 * The task_core2's notification value is incremented.  ulValue_core2 is not used and
 * xTaskNotify_core2() always returns pdPASS_core2 in this case.
 *
 * eSetValueWithOverwrite -
 * The task_core2's notification value is set to the value of ulValue_core2, even if the
 * task_core2 being notified had not yet processed the previous notification (the
 * task_core2 already had a notification pending).  xTaskNotify_core2() always returns
 * pdPASS_core2 in this case.
 *
 * eSetValueWithoutOverwrite -
 * If the task_core2 being notified did not already have a notification pending then
 * the task_core2's notification value is set to ulValue_core2 and xTaskNotify_core2() will
 * return pdPASS_core2.  If the task_core2 being notified already had a notification
 * pending then no action is performed and pdFAIL_core2 is returned.
 *
 * eNoAction_core2 -
 * The task_core2 receives a notification without its notification value being
 * updated.  ulValue_core2 is not used and xTaskNotify_core2() always returns pdPASS_core2 in
 * this case.
 *
 * @param pxHigherPriorityTaskWoken_core2  xTaskNotifyFromISR_core2() will set
 * *pxHigherPriorityTaskWoken_core2 to pdTRUE_core2 if sending the notification caused the
 * task_core2 to which the notification was sent to leave the Blocked state, and the
 * unblocked task_core2 has a priority higher than the currently running task_core2.  If
 * xTaskNotifyFromISR_core2() sets this value to pdTRUE_core2 then a context switch should
 * be requested before the interrupt is exited.  How a context switch is
 * requested from an ISR is dependent on the port - see the documentation page
 * for the port in use.
 *
 * @return Dependent on the value of eAction_core2.  See the description of the
 * eAction_core2 parameter.
 *
 * \defgroup xTaskNotifyIndexedFromISR_core2 xTaskNotifyIndexedFromISR_core2
 * \ingroup TaskNotifications
 */
BaseType_t_core2 xTaskGenericNotifyFromISR_core2( TaskHandle_t_core2 xTaskToNotify_core2,
                                      UBaseType_t_core2 uxIndexToNotify_core2,
                                      uint32_t ulValue_core2,
                                      eNotifyAction_core2 eAction_core2,
                                      uint32_t * pulPreviousNotificationValue_core2,
                                      BaseType_t_core2 * pxHigherPriorityTaskWoken_core2 ) PRIVILEGED_FUNCTION_core2;
#define xTaskNotifyFromISR_core2( xTaskToNotify_core2, ulValue_core2, eAction_core2, pxHigherPriorityTaskWoken_core2 ) \
    xTaskGenericNotifyFromISR_core2( ( xTaskToNotify_core2 ), ( tskDEFAULT_INDEX_TO_NOTIFY_core2 ), ( ulValue_core2 ), ( eAction_core2 ), NULL, ( pxHigherPriorityTaskWoken_core2 ) )
#define xTaskNotifyIndexedFromISR_core2( xTaskToNotify_core2, uxIndexToNotify_core2, ulValue_core2, eAction_core2, pxHigherPriorityTaskWoken_core2 ) \
    xTaskGenericNotifyFromISR_core2( ( xTaskToNotify_core2 ), ( uxIndexToNotify_core2 ), ( ulValue_core2 ), ( eAction_core2 ), NULL, ( pxHigherPriorityTaskWoken_core2 ) )

/**
 * task_core2. h
 * @code{c}
 * BaseType_t_core2 xTaskNotifyAndQueryIndexedFromISR_core2( TaskHandle_t_core2 xTaskToNotify_core2, UBaseType_t_core2 uxIndexToNotify_core2, uint32_t ulValue_core2, eNotifyAction_core2 eAction_core2, uint32_t *pulPreviousNotificationValue_core2, BaseType_t_core2 *pxHigherPriorityTaskWoken_core2 );
 * BaseType_t_core2 xTaskNotifyAndQueryFromISR_core2( TaskHandle_t_core2 xTaskToNotify_core2, uint32_t ulValue_core2, eNotifyAction_core2 eAction_core2, uint32_t *pulPreviousNotificationValue_core2, BaseType_t_core2 *pxHigherPriorityTaskWoken_core2 );
 * @endcode
 *
 * See https://www.FreeRTOS_core2.org/RTOS-task_core2-notifications.html for details.
 *
 * xTaskNotifyAndQueryIndexedFromISR_core2() performs the same operation as
 * xTaskNotifyIndexedFromISR_core2() with the addition that it also returns the
 * subject task_core2's prior notification value (the notification value at the time
 * the function is called rather than at the time the function returns) in the
 * additional pulPreviousNotifyValue_core2 parameter.
 *
 * xTaskNotifyAndQueryFromISR_core2() performs the same operation as
 * xTaskNotifyFromISR_core2() with the addition that it also returns the subject
 * task_core2's prior notification value (the notification value at the time the
 * function is called rather than at the time the function returns) in the
 * additional pulPreviousNotifyValue_core2 parameter.
 *
 * \defgroup xTaskNotifyAndQueryIndexedFromISR_core2 xTaskNotifyAndQueryIndexedFromISR_core2
 * \ingroup TaskNotifications
 */
#define xTaskNotifyAndQueryIndexedFromISR_core2( xTaskToNotify_core2, uxIndexToNotify_core2, ulValue_core2, eAction_core2, pulPreviousNotificationValue_core2, pxHigherPriorityTaskWoken_core2 ) \
    xTaskGenericNotifyFromISR_core2( ( xTaskToNotify_core2 ), ( uxIndexToNotify_core2 ), ( ulValue_core2 ), ( eAction_core2 ), ( pulPreviousNotificationValue_core2 ), ( pxHigherPriorityTaskWoken_core2 ) )
#define xTaskNotifyAndQueryFromISR_core2( xTaskToNotify_core2, ulValue_core2, eAction_core2, pulPreviousNotificationValue_core2, pxHigherPriorityTaskWoken_core2 ) \
    xTaskGenericNotifyFromISR_core2( ( xTaskToNotify_core2 ), ( tskDEFAULT_INDEX_TO_NOTIFY_core2 ), ( ulValue_core2 ), ( eAction_core2 ), ( pulPreviousNotificationValue_core2 ), ( pxHigherPriorityTaskWoken_core2 ) )

/**
 * task_core2. h
 * @code{c}
 * BaseType_t_core2 xTaskNotifyWaitIndexed_core2( UBaseType_t_core2 uxIndexToWaitOn_core2, uint32_t ulBitsToClearOnEntry_core2, uint32_t ulBitsToClearOnExit_core2, uint32_t *pulNotificationValue_core2, TickType_t_core2 xTicksToWait_core2 );
 *
 * BaseType_t_core2 xTaskNotifyWait_core2( uint32_t ulBitsToClearOnEntry_core2, uint32_t ulBitsToClearOnExit_core2, uint32_t *pulNotificationValue_core2, TickType_t_core2 xTicksToWait_core2 );
 * @endcode
 *
 * Waits for a direct to task_core2 notification to be pending at a given index within
 * an array of direct to task_core2 notifications.
 *
 * See https://www.FreeRTOS_core2.org/RTOS-task_core2-notifications.html for details.
 *
 * configUSE_TASK_NOTIFICATIONS_core2 must be undefined or defined as 1 for this
 * function to be available.
 *
 * Each task_core2 has a private array of "notification values" (or 'notifications'),
 * each of which is a 32-bit unsigned integer (uint32_t).  The constant
 * configTASK_NOTIFICATION_ARRAY_ENTRIES_core2 sets the number of indexes in the
 * array, and (for backward compatibility) defaults to 1 if left undefined.
 * Prior to FreeRTOS_core2 V10.4.0 there was only one notification value per task_core2.
 *
 * Events can be sent to a task_core2 using an intermediary object.  Examples of such
 * objects are queues, semaphores, mutexes and event groups.  Task notifications
 * are a method of sending an event directly to a task_core2 without the need for such
 * an intermediary object.
 *
 * A notification sent to a task_core2 can optionally perform an action, such as
 * update, overwrite or increment one of the task_core2's notification values.  In
 * that way task_core2 notifications can be used to send data to a task_core2, or be used as
 * light weight and fast binary or counting semaphores.
 *
 * A notification sent to a task_core2 will remain pending until it is cleared by the
 * task_core2 calling xTaskNotifyWaitIndexed_core2() or ulTaskNotifyTakeIndexed_core2() (or their
 * un-indexed equivalents).  If the task_core2 was already in the Blocked state to
 * wait for a notification when the notification arrives then the task_core2 will
 * automatically be removed from the Blocked state (unblocked) and the
 * notification cleared.
 *
 * A task_core2 can use xTaskNotifyWaitIndexed_core2() to [optionally] block to wait for a
 * notification to be pending, or ulTaskNotifyTakeIndexed_core2() to [optionally] block
 * to wait for a notification value to have a non-zero value.  The task_core2 does
 * not consume any CPU time while it is in the Blocked state.
 *
 * **NOTE** Each notification within the array operates independently - a task_core2
 * can only block on one notification within the array at a time and will not be
 * unblocked by a notification sent to any other array index.
 *
 * Backward compatibility information:
 * Prior to FreeRTOS_core2 V10.4.0 each task_core2 had a single "notification value", and
 * all task_core2 notification API functions operated on that value. Replacing the
 * single notification value with an array of notification values necessitated a
 * new set of API functions that could address specific notifications within the
 * array.  xTaskNotifyWait_core2() is the original API function, and remains backward
 * compatible by always operating on the notification value at index 0 in the
 * array. Calling xTaskNotifyWait_core2() is equivalent to calling
 * xTaskNotifyWaitIndexed_core2() with the uxIndexToWaitOn_core2 parameter set to 0.
 *
 * @param uxIndexToWaitOn_core2 The index within the calling task_core2's array of
 * notification values on which the calling task_core2 will wait for a notification to
 * be received.  uxIndexToWaitOn_core2 must be less than
 * configTASK_NOTIFICATION_ARRAY_ENTRIES_core2.  xTaskNotifyWait_core2() does
 * not have this parameter and always waits for notifications on index 0.
 *
 * @param ulBitsToClearOnEntry_core2 Bits that are set in ulBitsToClearOnEntry_core2 value
 * will be cleared in the calling task_core2's notification value before the task_core2
 * checks to see if any notifications are pending, and optionally blocks if no
 * notifications are pending.  Setting ulBitsToClearOnEntry_core2 to ULONG_MAX (if
 * limits.h is included) or 0xffffffffUL (if limits.h is not included) will have
 * the effect of resetting the task_core2's notification value to 0.  Setting
 * ulBitsToClearOnEntry_core2 to 0 will leave the task_core2's notification value unchanged.
 *
 * @param ulBitsToClearOnExit_core2 If a notification is pending or received before
 * the calling task_core2 exits the xTaskNotifyWait_core2() function then the task_core2's
 * notification value (see the xTaskNotify_core2() API function) is passed out using
 * the pulNotificationValue_core2 parameter.  Then any bits that are set in
 * ulBitsToClearOnExit_core2 will be cleared in the task_core2's notification value (note
 * *pulNotificationValue_core2 is set before any bits are cleared).  Setting
 * ulBitsToClearOnExit_core2 to ULONG_MAX (if limits.h is included) or 0xffffffffUL
 * (if limits.h is not included) will have the effect of resetting the task_core2's
 * notification value to 0 before the function exits.  Setting
 * ulBitsToClearOnExit_core2 to 0 will leave the task_core2's notification value unchanged
 * when the function exits (in which case the value passed out in
 * pulNotificationValue_core2 will match the task_core2's notification value).
 *
 * @param pulNotificationValue_core2 Used to pass the task_core2's notification value out
 * of the function.  Note the value passed out will not be effected by the
 * clearing of any bits caused by ulBitsToClearOnExit_core2 being non-zero.
 *
 * @param xTicksToWait_core2 The maximum amount of time that the task_core2 should wait in
 * the Blocked state for a notification to be received, should a notification
 * not already be pending when xTaskNotifyWait_core2() was called.  The task_core2
 * will not consume any processing time while it is in the Blocked state.  This
 * is specified in kernel ticks, the macro pdMS_TO_TICKS_core2( value_in_ms ) can be
 * used to convert a time specified in milliseconds to a time specified in
 * ticks.
 *
 * @return If a notification was received (including notifications that were
 * already pending when xTaskNotifyWait_core2 was called) then pdPASS_core2 is
 * returned.  Otherwise pdFAIL_core2 is returned.
 *
 * \defgroup xTaskNotifyWaitIndexed_core2 xTaskNotifyWaitIndexed_core2
 * \ingroup TaskNotifications
 */
BaseType_t_core2 xTaskGenericNotifyWait_core2( UBaseType_t_core2 uxIndexToWaitOn_core2,
                                   uint32_t ulBitsToClearOnEntry_core2,
                                   uint32_t ulBitsToClearOnExit_core2,
                                   uint32_t * pulNotificationValue_core2,
                                   TickType_t_core2 xTicksToWait_core2 ) PRIVILEGED_FUNCTION_core2;
#define xTaskNotifyWait_core2( ulBitsToClearOnEntry_core2, ulBitsToClearOnExit_core2, pulNotificationValue_core2, xTicksToWait_core2 ) \
    xTaskGenericNotifyWait_core2( tskDEFAULT_INDEX_TO_NOTIFY_core2, ( ulBitsToClearOnEntry_core2 ), ( ulBitsToClearOnExit_core2 ), ( pulNotificationValue_core2 ), ( xTicksToWait_core2 ) )
#define xTaskNotifyWaitIndexed_core2( uxIndexToWaitOn_core2, ulBitsToClearOnEntry_core2, ulBitsToClearOnExit_core2, pulNotificationValue_core2, xTicksToWait_core2 ) \
    xTaskGenericNotifyWait_core2( ( uxIndexToWaitOn_core2 ), ( ulBitsToClearOnEntry_core2 ), ( ulBitsToClearOnExit_core2 ), ( pulNotificationValue_core2 ), ( xTicksToWait_core2 ) )

/**
 * task_core2. h
 * @code{c}
 * BaseType_t_core2 xTaskNotifyGiveIndexed_core2( TaskHandle_t_core2 xTaskToNotify_core2, UBaseType_t_core2 uxIndexToNotify_core2 );
 * BaseType_t_core2 xTaskNotifyGive_core2( TaskHandle_t_core2 xTaskToNotify_core2 );
 * @endcode
 *
 * Sends a direct to task_core2 notification to a particular index in the target
 * task_core2's notification array in a manner similar to giving a counting semaphore.
 *
 * See https://www.FreeRTOS_core2.org/RTOS-task_core2-notifications.html for more details.
 *
 * configUSE_TASK_NOTIFICATIONS_core2 must be undefined or defined as 1 for these
 * macros to be available.
 *
 * Each task_core2 has a private array of "notification values" (or 'notifications'),
 * each of which is a 32-bit unsigned integer (uint32_t).  The constant
 * configTASK_NOTIFICATION_ARRAY_ENTRIES_core2 sets the number of indexes in the
 * array, and (for backward compatibility) defaults to 1 if left undefined.
 * Prior to FreeRTOS_core2 V10.4.0 there was only one notification value per task_core2.
 *
 * Events can be sent to a task_core2 using an intermediary object.  Examples of such
 * objects are queues, semaphores, mutexes and event groups.  Task notifications
 * are a method of sending an event directly to a task_core2 without the need for such
 * an intermediary object.
 *
 * A notification sent to a task_core2 can optionally perform an action, such as
 * update, overwrite or increment one of the task_core2's notification values.  In
 * that way task_core2 notifications can be used to send data to a task_core2, or be used as
 * light weight and fast binary or counting semaphores.
 *
 * xTaskNotifyGiveIndexed_core2() is a helper macro intended for use when task_core2
 * notifications are used as light weight and faster binary or counting
 * semaphore equivalents.  Actual FreeRTOS_core2 semaphores are given using the
 * xSemaphoreGive_core2() API function, the equivalent action that instead uses a task_core2
 * notification is xTaskNotifyGiveIndexed_core2().
 *
 * When task_core2 notifications are being used as a binary or counting semaphore
 * equivalent then the task_core2 being notified should wait for the notification
 * using the ulTaskNotifyTakeIndexed_core2() API function rather than the
 * xTaskNotifyWaitIndexed_core2() API function.
 *
 * **NOTE** Each notification within the array operates independently - a task_core2
 * can only block on one notification within the array at a time and will not be
 * unblocked by a notification sent to any other array index.
 *
 * Backward compatibility information:
 * Prior to FreeRTOS_core2 V10.4.0 each task_core2 had a single "notification value", and
 * all task_core2 notification API functions operated on that value. Replacing the
 * single notification value with an array of notification values necessitated a
 * new set of API functions that could address specific notifications within the
 * array.  xTaskNotifyGive_core2() is the original API function, and remains backward
 * compatible by always operating on the notification value at index 0 in the
 * array. Calling xTaskNotifyGive_core2() is equivalent to calling
 * xTaskNotifyGiveIndexed_core2() with the uxIndexToNotify_core2 parameter set to 0.
 *
 * @param xTaskToNotify_core2 The handle of the task_core2 being notified.  The handle to a
 * task_core2 can be returned from the xTaskCreate_core2() API function used to create the
 * task_core2, and the handle of the currently running task_core2 can be obtained by calling
 * xTaskGetCurrentTaskHandle_core2().
 *
 * @param uxIndexToNotify_core2 The index within the target task_core2's array of
 * notification values to which the notification is to be sent.  uxIndexToNotify_core2
 * must be less than configTASK_NOTIFICATION_ARRAY_ENTRIES_core2.  xTaskNotifyGive_core2()
 * does not have this parameter and always sends notifications to index 0.
 *
 * @return xTaskNotifyGive_core2() is a macro that calls xTaskNotify_core2() with the
 * eAction_core2 parameter set to eIncrement_core2 - so pdPASS_core2 is always returned.
 *
 * \defgroup xTaskNotifyGiveIndexed_core2 xTaskNotifyGiveIndexed_core2
 * \ingroup TaskNotifications
 */
#define xTaskNotifyGive_core2( xTaskToNotify_core2 ) \
    xTaskGenericNotify_core2( ( xTaskToNotify_core2 ), ( tskDEFAULT_INDEX_TO_NOTIFY_core2 ), ( 0 ), eIncrement_core2, NULL )
#define xTaskNotifyGiveIndexed_core2( xTaskToNotify_core2, uxIndexToNotify_core2 ) \
    xTaskGenericNotify_core2( ( xTaskToNotify_core2 ), ( uxIndexToNotify_core2 ), ( 0 ), eIncrement_core2, NULL )

/**
 * task_core2. h
 * @code{c}
 * void vTaskNotifyGiveIndexedFromISR_core2( TaskHandle_t_core2 xTaskHandle, UBaseType_t_core2 uxIndexToNotify_core2, BaseType_t_core2 *pxHigherPriorityTaskWoken_core2 );
 * void vTaskNotifyGiveFromISR_core2( TaskHandle_t_core2 xTaskHandle, BaseType_t_core2 *pxHigherPriorityTaskWoken_core2 );
 * @endcode
 *
 * A version of xTaskNotifyGiveIndexed_core2() that can be called from an interrupt
 * service routine (ISR).
 *
 * See https://www.FreeRTOS_core2.org/RTOS-task_core2-notifications.html for more details.
 *
 * configUSE_TASK_NOTIFICATIONS_core2 must be undefined or defined as 1 for this macro
 * to be available.
 *
 * Each task_core2 has a private array of "notification values" (or 'notifications'),
 * each of which is a 32-bit unsigned integer (uint32_t).  The constant
 * configTASK_NOTIFICATION_ARRAY_ENTRIES_core2 sets the number of indexes in the
 * array, and (for backward compatibility) defaults to 1 if left undefined.
 * Prior to FreeRTOS_core2 V10.4.0 there was only one notification value per task_core2.
 *
 * Events can be sent to a task_core2 using an intermediary object.  Examples of such
 * objects are queues, semaphores, mutexes and event groups.  Task notifications
 * are a method of sending an event directly to a task_core2 without the need for such
 * an intermediary object.
 *
 * A notification sent to a task_core2 can optionally perform an action, such as
 * update, overwrite or increment one of the task_core2's notification values.  In
 * that way task_core2 notifications can be used to send data to a task_core2, or be used as
 * light weight and fast binary or counting semaphores.
 *
 * vTaskNotifyGiveIndexedFromISR_core2() is intended for use when task_core2 notifications
 * are used as light weight and faster binary or counting semaphore equivalents.
 * Actual FreeRTOS_core2 semaphores are given from an ISR using the
 * xSemaphoreGiveFromISR_core2() API function, the equivalent action that instead uses
 * a task_core2 notification is vTaskNotifyGiveIndexedFromISR_core2().
 *
 * When task_core2 notifications are being used as a binary or counting semaphore
 * equivalent then the task_core2 being notified should wait for the notification
 * using the ulTaskNotifyTakeIndexed_core2() API function rather than the
 * xTaskNotifyWaitIndexed_core2() API function.
 *
 * **NOTE** Each notification within the array operates independently - a task_core2
 * can only block on one notification within the array at a time and will not be
 * unblocked by a notification sent to any other array index.
 *
 * Backward compatibility information:
 * Prior to FreeRTOS_core2 V10.4.0 each task_core2 had a single "notification value", and
 * all task_core2 notification API functions operated on that value. Replacing the
 * single notification value with an array of notification values necessitated a
 * new set of API functions that could address specific notifications within the
 * array.  xTaskNotifyFromISR_core2() is the original API function, and remains
 * backward compatible by always operating on the notification value at index 0
 * within the array. Calling xTaskNotifyGiveFromISR() is equivalent to calling
 * xTaskNotifyGiveIndexedFromISR() with the uxIndexToNotify_core2 parameter set to 0.
 *
 * @param xTaskToNotify_core2 The handle of the task_core2 being notified.  The handle to a
 * task_core2 can be returned from the xTaskCreate_core2() API function used to create the
 * task_core2, and the handle of the currently running task_core2 can be obtained by calling
 * xTaskGetCurrentTaskHandle_core2().
 *
 * @param uxIndexToNotify_core2 The index within the target task_core2's array of
 * notification values to which the notification is to be sent.  uxIndexToNotify_core2
 * must be less than configTASK_NOTIFICATION_ARRAY_ENTRIES_core2.
 * xTaskNotifyGiveFromISR() does not have this parameter and always sends
 * notifications to index 0.
 *
 * @param pxHigherPriorityTaskWoken_core2  vTaskNotifyGiveFromISR_core2() will set
 * *pxHigherPriorityTaskWoken_core2 to pdTRUE_core2 if sending the notification caused the
 * task_core2 to which the notification was sent to leave the Blocked state, and the
 * unblocked task_core2 has a priority higher than the currently running task_core2.  If
 * vTaskNotifyGiveFromISR_core2() sets this value to pdTRUE_core2 then a context switch
 * should be requested before the interrupt is exited.  How a context switch is
 * requested from an ISR is dependent on the port - see the documentation page
 * for the port in use.
 *
 * \defgroup vTaskNotifyGiveIndexedFromISR_core2 vTaskNotifyGiveIndexedFromISR_core2
 * \ingroup TaskNotifications
 */
void vTaskGenericNotifyGiveFromISR_core2( TaskHandle_t_core2 xTaskToNotify_core2,
                                    UBaseType_t_core2 uxIndexToNotify_core2,
                                    BaseType_t_core2 * pxHigherPriorityTaskWoken_core2 ) PRIVILEGED_FUNCTION_core2;
#define vTaskNotifyGiveFromISR_core2( xTaskToNotify_core2, pxHigherPriorityTaskWoken_core2 ) \
    vTaskGenericNotifyGiveFromISR_core2( ( xTaskToNotify_core2 ), ( tskDEFAULT_INDEX_TO_NOTIFY_core2 ), ( pxHigherPriorityTaskWoken_core2 ) )
#define vTaskNotifyGiveIndexedFromISR_core2( xTaskToNotify_core2, uxIndexToNotify_core2, pxHigherPriorityTaskWoken_core2 ) \
    vTaskGenericNotifyGiveFromISR_core2( ( xTaskToNotify_core2 ), ( uxIndexToNotify_core2 ), ( pxHigherPriorityTaskWoken_core2 ) )

/**
 * task_core2. h
 * @code{c}
 * uint32_t ulTaskNotifyTakeIndexed_core2( UBaseType_t_core2 uxIndexToWaitOn_core2, BaseType_t_core2 xClearCountOnExit_core2, TickType_t_core2 xTicksToWait_core2 );
 *
 * uint32_t ulTaskNotifyTake_core2( BaseType_t_core2 xClearCountOnExit_core2, TickType_t_core2 xTicksToWait_core2 );
 * @endcode
 *
 * Waits for a direct to task_core2 notification on a particular index in the calling
 * task_core2's notification array in a manner similar to taking a counting semaphore.
 *
 * See https://www.FreeRTOS_core2.org/RTOS-task_core2-notifications.html for details.
 *
 * configUSE_TASK_NOTIFICATIONS_core2 must be undefined or defined as 1 for this
 * function to be available.
 *
 * Each task_core2 has a private array of "notification values" (or 'notifications'),
 * each of which is a 32-bit unsigned integer (uint32_t).  The constant
 * configTASK_NOTIFICATION_ARRAY_ENTRIES_core2 sets the number of indexes in the
 * array, and (for backward compatibility) defaults to 1 if left undefined.
 * Prior to FreeRTOS_core2 V10.4.0 there was only one notification value per task_core2.
 *
 * Events can be sent to a task_core2 using an intermediary object.  Examples of such
 * objects are queues, semaphores, mutexes and event groups.  Task notifications
 * are a method of sending an event directly to a task_core2 without the need for such
 * an intermediary object.
 *
 * A notification sent to a task_core2 can optionally perform an action, such as
 * update, overwrite or increment one of the task_core2's notification values.  In
 * that way task_core2 notifications can be used to send data to a task_core2, or be used as
 * light weight and fast binary or counting semaphores.
 *
 * ulTaskNotifyTakeIndexed_core2() is intended for use when a task_core2 notification is
 * used as a faster and lighter weight binary or counting semaphore alternative.
 * Actual FreeRTOS_core2 semaphores are taken using the xSemaphoreTake_core2() API function,
 * the equivalent action that instead uses a task_core2 notification is
 * ulTaskNotifyTakeIndexed_core2().
 *
 * When a task_core2 is using its notification value as a binary or counting semaphore
 * other tasks should send notifications to it using the xTaskNotifyGiveIndexed_core2()
 * macro, or xTaskNotifyIndex() function with the eAction_core2 parameter set to
 * eIncrement_core2.
 *
 * ulTaskNotifyTakeIndexed_core2() can either clear the task_core2's notification value at
 * the array index specified by the uxIndexToWaitOn_core2 parameter to zero on exit,
 * in which case the notification value acts like a binary semaphore, or
 * decrement the notification value on exit, in which case the notification
 * value acts like a counting semaphore.
 *
 * A task_core2 can use ulTaskNotifyTakeIndexed_core2() to [optionally] block to wait for
 * a notification.  The task_core2 does not consume any CPU time while it is in the
 * Blocked state.
 *
 * Where as xTaskNotifyWaitIndexed_core2() will return when a notification is pending,
 * ulTaskNotifyTakeIndexed_core2() will return when the task_core2's notification value is
 * not zero.
 *
 * **NOTE** Each notification within the array operates independently - a task_core2
 * can only block on one notification within the array at a time and will not be
 * unblocked by a notification sent to any other array index.
 *
 * Backward compatibility information:
 * Prior to FreeRTOS_core2 V10.4.0 each task_core2 had a single "notification value", and
 * all task_core2 notification API functions operated on that value. Replacing the
 * single notification value with an array of notification values necessitated a
 * new set of API functions that could address specific notifications within the
 * array.  ulTaskNotifyTake_core2() is the original API function, and remains backward
 * compatible by always operating on the notification value at index 0 in the
 * array. Calling ulTaskNotifyTake_core2() is equivalent to calling
 * ulTaskNotifyTakeIndexed_core2() with the uxIndexToWaitOn_core2 parameter set to 0.
 *
 * @param uxIndexToWaitOn_core2 The index within the calling task_core2's array of
 * notification values on which the calling task_core2 will wait for a notification to
 * be non-zero.  uxIndexToWaitOn_core2 must be less than
 * configTASK_NOTIFICATION_ARRAY_ENTRIES_core2.  xTaskNotifyTake() does
 * not have this parameter and always waits for notifications on index 0.
 *
 * @param xClearCountOnExit_core2 if xClearCountOnExit_core2 is pdFALSE_core2 then the task_core2's
 * notification value is decremented when the function exits.  In this way the
 * notification value acts like a counting semaphore.  If xClearCountOnExit_core2 is
 * not pdFALSE_core2 then the task_core2's notification value is cleared to zero when the
 * function exits.  In this way the notification value acts like a binary
 * semaphore.
 *
 * @param xTicksToWait_core2 The maximum amount of time that the task_core2 should wait in
 * the Blocked state for the task_core2's notification value to be greater than zero,
 * should the count not already be greater than zero when
 * ulTaskNotifyTake_core2() was called.  The task_core2 will not consume any processing
 * time while it is in the Blocked state.  This is specified in kernel ticks,
 * the macro pdMS_TO_TICKS_core2( value_in_ms ) can be used to convert a time
 * specified in milliseconds to a time specified in ticks.
 *
 * @return The task_core2's notification count before it is either cleared to zero or
 * decremented (see the xClearCountOnExit_core2 parameter).
 *
 * \defgroup ulTaskNotifyTakeIndexed_core2 ulTaskNotifyTakeIndexed_core2
 * \ingroup TaskNotifications
 */
uint32_t ulTaskGenericNotifyTake_core2( UBaseType_t_core2 uxIndexToWaitOn_core2,
                                  BaseType_t_core2 xClearCountOnExit_core2,
                                  TickType_t_core2 xTicksToWait_core2 ) PRIVILEGED_FUNCTION_core2;
#define ulTaskNotifyTake_core2( xClearCountOnExit_core2, xTicksToWait_core2 ) \
    ulTaskGenericNotifyTake_core2( ( tskDEFAULT_INDEX_TO_NOTIFY_core2 ), ( xClearCountOnExit_core2 ), ( xTicksToWait_core2 ) )
#define ulTaskNotifyTakeIndexed_core2( uxIndexToWaitOn_core2, xClearCountOnExit_core2, xTicksToWait_core2 ) \
    ulTaskGenericNotifyTake_core2( ( uxIndexToWaitOn_core2 ), ( xClearCountOnExit_core2 ), ( xTicksToWait_core2 ) )

/**
 * task_core2. h
 * @code{c}
 * BaseType_t_core2 xTaskNotifyStateClearIndexed_core2( TaskHandle_t_core2 xTask_core2, UBaseType_t_core2 uxIndexToCLear );
 *
 * BaseType_t_core2 xTaskNotifyStateClear_core2( TaskHandle_t_core2 xTask_core2 );
 * @endcode
 *
 * See https://www.FreeRTOS_core2.org/RTOS-task_core2-notifications.html for details.
 *
 * configUSE_TASK_NOTIFICATIONS_core2 must be undefined or defined as 1 for these
 * functions to be available.
 *
 * Each task_core2 has a private array of "notification values" (or 'notifications'),
 * each of which is a 32-bit unsigned integer (uint32_t).  The constant
 * configTASK_NOTIFICATION_ARRAY_ENTRIES_core2 sets the number of indexes in the
 * array, and (for backward compatibility) defaults to 1 if left undefined.
 * Prior to FreeRTOS_core2 V10.4.0 there was only one notification value per task_core2.
 *
 * If a notification is sent to an index within the array of notifications then
 * the notification at that index is said to be 'pending' until it is read or
 * explicitly cleared by the receiving task_core2.  xTaskNotifyStateClearIndexed_core2()
 * is the function that clears a pending notification without reading the
 * notification value.  The notification value at the same array index is not
 * altered.  Set xTask_core2 to NULL to clear the notification state of the calling
 * task_core2.
 *
 * Backward compatibility information:
 * Prior to FreeRTOS_core2 V10.4.0 each task_core2 had a single "notification value", and
 * all task_core2 notification API functions operated on that value. Replacing the
 * single notification value with an array of notification values necessitated a
 * new set of API functions that could address specific notifications within the
 * array.  xTaskNotifyStateClear_core2() is the original API function, and remains
 * backward compatible by always operating on the notification value at index 0
 * within the array. Calling xTaskNotifyStateClear_core2() is equivalent to calling
 * xTaskNotifyStateClearIndexed_core2() with the uxIndexToNotify_core2 parameter set to 0.
 *
 * @param xTask_core2 The handle of the RTOS task_core2 that will have a notification state
 * cleared.  Set xTask_core2 to NULL to clear a notification state in the calling
 * task_core2.  To obtain a task_core2's handle create the task_core2 using xTaskCreate_core2() and
 * make use of the pxCreatedTask_core2 parameter, or create the task_core2 using
 * xTaskCreateStatic_core2() and store the returned value, or use the task_core2's name in
 * a call to xTaskGetHandle_core2().
 *
 * @param uxIndexToClear_core2 The index within the target task_core2's array of
 * notification values to act upon.  For example, setting uxIndexToClear_core2 to 1
 * will clear the state of the notification at index 1 within the array.
 * uxIndexToClear_core2 must be less than configTASK_NOTIFICATION_ARRAY_ENTRIES_core2.
 * ulTaskNotifyStateClear() does not have this parameter and always acts on the
 * notification at index 0.
 *
 * @return pdTRUE_core2 if the task_core2's notification state was set to
 * eNotWaitingNotification, otherwise pdFALSE_core2.
 *
 * \defgroup xTaskNotifyStateClearIndexed_core2 xTaskNotifyStateClearIndexed_core2
 * \ingroup TaskNotifications
 */
BaseType_t_core2 xTaskGenericNotifyStateClear_core2( TaskHandle_t_core2 xTask_core2,
                                         UBaseType_t_core2 uxIndexToClear_core2 ) PRIVILEGED_FUNCTION_core2;
#define xTaskNotifyStateClear_core2( xTask_core2 ) \
    xTaskGenericNotifyStateClear_core2( ( xTask_core2 ), ( tskDEFAULT_INDEX_TO_NOTIFY_core2 ) )
#define xTaskNotifyStateClearIndexed_core2( xTask_core2, uxIndexToClear_core2 ) \
    xTaskGenericNotifyStateClear_core2( ( xTask_core2 ), ( uxIndexToClear_core2 ) )

/**
 * task_core2. h
 * @code{c}
 * uint32_t ulTaskNotifyValueClearIndexed_core2( TaskHandle_t_core2 xTask_core2, UBaseType_t_core2 uxIndexToClear_core2, uint32_t ulBitsToClear_core2 );
 *
 * uint32_t ulTaskNotifyValueClear_core2( TaskHandle_t_core2 xTask_core2, uint32_t ulBitsToClear_core2 );
 * @endcode
 *
 * See https://www.FreeRTOS_core2.org/RTOS-task_core2-notifications.html for details.
 *
 * configUSE_TASK_NOTIFICATIONS_core2 must be undefined or defined as 1 for these
 * functions to be available.
 *
 * Each task_core2 has a private array of "notification values" (or 'notifications'),
 * each of which is a 32-bit unsigned integer (uint32_t).  The constant
 * configTASK_NOTIFICATION_ARRAY_ENTRIES_core2 sets the number of indexes in the
 * array, and (for backward compatibility) defaults to 1 if left undefined.
 * Prior to FreeRTOS_core2 V10.4.0 there was only one notification value per task_core2.
 *
 * ulTaskNotifyValueClearIndexed_core2() clears the bits specified by the
 * ulBitsToClear_core2 bit mask in the notification value at array index uxIndexToClear_core2
 * of the task_core2 referenced by xTask_core2.
 *
 * Backward compatibility information:
 * Prior to FreeRTOS_core2 V10.4.0 each task_core2 had a single "notification value", and
 * all task_core2 notification API functions operated on that value. Replacing the
 * single notification value with an array of notification values necessitated a
 * new set of API functions that could address specific notifications within the
 * array.  ulTaskNotifyValueClear_core2() is the original API function, and remains
 * backward compatible by always operating on the notification value at index 0
 * within the array. Calling ulTaskNotifyValueClear_core2() is equivalent to calling
 * ulTaskNotifyValueClearIndexed_core2() with the uxIndexToClear_core2 parameter set to 0.
 *
 * @param xTask_core2 The handle of the RTOS task_core2 that will have bits in one of its
 * notification values cleared. Set xTask_core2 to NULL to clear bits in a
 * notification value of the calling task_core2.  To obtain a task_core2's handle create the
 * task_core2 using xTaskCreate_core2() and make use of the pxCreatedTask_core2 parameter, or
 * create the task_core2 using xTaskCreateStatic_core2() and store the returned value, or
 * use the task_core2's name in a call to xTaskGetHandle_core2().
 *
 * @param uxIndexToClear_core2 The index within the target task_core2's array of
 * notification values in which to clear the bits.  uxIndexToClear_core2
 * must be less than configTASK_NOTIFICATION_ARRAY_ENTRIES_core2.
 * ulTaskNotifyValueClear_core2() does not have this parameter and always clears bits
 * in the notification value at index 0.
 *
 * @param ulBitsToClear_core2 Bit mask of the bits to clear in the notification value of
 * xTask_core2. Set a bit to 1 to clear the corresponding bits in the task_core2's notification
 * value. Set ulBitsToClear_core2 to 0xffffffff (UINT_MAX on 32-bit architectures) to clear
 * the notification value to 0.  Set ulBitsToClear_core2 to 0 to query the task_core2's
 * notification value without clearing any bits.
 *
 *
 * @return The value of the target task_core2's notification value before the bits
 * specified by ulBitsToClear_core2 were cleared.
 * \defgroup ulTaskNotifyValueClear_core2 ulTaskNotifyValueClear_core2
 * \ingroup TaskNotifications
 */
uint32_t ulTaskGenericNotifyValueClear_core2( TaskHandle_t_core2 xTask_core2,
                                        UBaseType_t_core2 uxIndexToClear_core2,
                                        uint32_t ulBitsToClear_core2 ) PRIVILEGED_FUNCTION_core2;
#define ulTaskNotifyValueClear_core2( xTask_core2, ulBitsToClear_core2 ) \
    ulTaskGenericNotifyValueClear_core2( ( xTask_core2 ), ( tskDEFAULT_INDEX_TO_NOTIFY_core2 ), ( ulBitsToClear_core2 ) )
#define ulTaskNotifyValueClearIndexed_core2( xTask_core2, uxIndexToClear_core2, ulBitsToClear_core2 ) \
    ulTaskGenericNotifyValueClear_core2( ( xTask_core2 ), ( uxIndexToClear_core2 ), ( ulBitsToClear_core2 ) )

/**
 * task_core2.h
 * @code{c}
 * void vTaskSetTimeOutState_core2( TimeOut_t_core2 * const pxTimeOut_core2 );
 * @endcode
 *
 * Capture the current time for future use with xTaskCheckForTimeOut_core2().
 *
 * @param pxTimeOut_core2 Pointer to a timeout object into which the current time
 * is to be captured.  The captured time includes the tick count and the number
 * of times the tick count has overflowed since the system first booted.
 * \defgroup vTaskSetTimeOutState_core2 vTaskSetTimeOutState_core2
 * \ingroup TaskCtrl
 */
void vTaskSetTimeOutState_core2( TimeOut_t_core2 * const pxTimeOut_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2.h
 * @code{c}
 * BaseType_t_core2 xTaskCheckForTimeOut_core2( TimeOut_t_core2 * const pxTimeOut_core2, TickType_t_core2 * const pxTicksToWait_core2 );
 * @endcode
 *
 * Determines if pxTicksToWait_core2 ticks has passed since a time was captured
 * using a call to vTaskSetTimeOutState_core2().  The captured time includes the tick
 * count and the number of times the tick count has overflowed.
 *
 * @param pxTimeOut_core2 The time status as captured previously using
 * vTaskSetTimeOutState_core2. If the timeout has not yet occurred, it is updated
 * to reflect the current time status.
 * @param pxTicksToWait_core2 The number of ticks to check for timeout i.e. if
 * pxTicksToWait_core2 ticks have passed since pxTimeOut_core2 was last updated (either by
 * vTaskSetTimeOutState_core2() or xTaskCheckForTimeOut_core2()), the timeout has occurred.
 * If the timeout has not occurred, pxTicksToWait_core2 is updated to reflect the
 * number of remaining ticks.
 *
 * @return If timeout has occurred, pdTRUE_core2 is returned. Otherwise pdFALSE_core2 is
 * returned and pxTicksToWait_core2 is updated to reflect the number of remaining
 * ticks.
 *
 * @see https://www.FreeRTOS_core2.org/xTaskCheckForTimeOut_core2.html
 *
 * Example Usage:
 * @code{c}
 *  // Driver library function used to receive uxWantedBytes from an Rx buffer
 *  // that is filled by a UART interrupt. If there are not enough bytes in the
 *  // Rx buffer then the task_core2 enters the Blocked state until it is notified that
 *  // more data has been placed into the buffer. If there is still not enough
 *  // data then the task_core2 re-enters the Blocked state, and xTaskCheckForTimeOut_core2()
 *  // is used to re-calculate the Block time to ensure the total amount of time
 *  // spent in the Blocked state does not exceed MAX_TIME_TO_WAIT. This
 *  // continues until either the buffer contains at least uxWantedBytes bytes,
 *  // or the total amount of time spent in the Blocked state reaches
 *  // MAX_TIME_TO_WAIT - at which point the task_core2 reads however many bytes are
 *  // available up to a maximum of uxWantedBytes.
 *
 *  size_t xUART_Receive( uint8_t *pucBuffer, size_t uxWantedBytes )
 *  {
 *  size_t uxReceived = 0;
 *  TickType_t_core2 xTicksToWait_core2 = MAX_TIME_TO_WAIT;
 *  TimeOut_t_core2 xTimeOut;
 *
 *      // Initialize xTimeOut.  This records the time at which this function
 *      // was entered.
 *      vTaskSetTimeOutState_core2( &xTimeOut );
 *
 *      // Loop until the buffer contains the wanted number of bytes, or a
 *      // timeout occurs.
 *      while( UART_bytes_in_rx_buffer( pxUARTInstance ) < uxWantedBytes )
 *      {
 *          // The buffer didn't contain enough data so this task_core2 is going to
 *          // enter the Blocked state. Adjusting xTicksToWait_core2 to account for
 *          // any time that has been spent in the Blocked state within this
 *          // function so far to ensure the total amount of time spent in the
 *          // Blocked state does not exceed MAX_TIME_TO_WAIT.
 *          if( xTaskCheckForTimeOut_core2( &xTimeOut, &xTicksToWait_core2 ) != pdFALSE_core2 )
 *          {
 *              //Timed out before the wanted number of bytes were available,
 *              // exit the loop.
 *              break;
 *          }
 *
 *          // Wait for a maximum of xTicksToWait_core2 ticks to be notified that the
 *          // receive interrupt has placed more data into the buffer.
 *          ulTaskNotifyTake_core2( pdTRUE_core2, xTicksToWait_core2 );
 *      }
 *
 *      // Attempt to read uxWantedBytes from the receive buffer into pucBuffer.
 *      // The actual number of bytes read (which might be less than
 *      // uxWantedBytes) is returned.
 *      uxReceived = UART_read_from_receive_buffer( pxUARTInstance,
 *                                                  pucBuffer,
 *                                                  uxWantedBytes );
 *
 *      return uxReceived;
 *  }
 * @endcode
 * \defgroup xTaskCheckForTimeOut_core2 xTaskCheckForTimeOut_core2
 * \ingroup TaskCtrl
 */
BaseType_t_core2 xTaskCheckForTimeOut_core2( TimeOut_t_core2 * const pxTimeOut_core2,
                                 TickType_t_core2 * const pxTicksToWait_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * task_core2.h
 * @code{c}
 * BaseType_t_core2 xTaskCatchUpTicks_core2( TickType_t_core2 xTicksToCatchUp );
 * @endcode
 *
 * This function corrects the tick count value after the application code has held
 * interrupts disabled for an extended period resulting in tick interrupts having
 * been missed.
 *
 * This function is similar to vTaskStepTick_core2(), however, unlike
 * vTaskStepTick_core2(), xTaskCatchUpTicks_core2() may move the tick count forward past a
 * time at which a task_core2 should be removed from the blocked state.  That means
 * tasks may have to be removed from the blocked state as the tick count is
 * moved.
 *
 * @param xTicksToCatchUp The number of tick interrupts that have been missed due to
 * interrupts being disabled.  Its value is not computed automatically, so must be
 * computed by the application writer.
 *
 * @return pdTRUE_core2 if moving the tick count forward resulted in a task_core2 leaving the
 * blocked state and a context switch being performed.  Otherwise pdFALSE_core2.
 *
 * \defgroup xTaskCatchUpTicks_core2 xTaskCatchUpTicks_core2
 * \ingroup TaskCtrl
 */
BaseType_t_core2 xTaskCatchUpTicks_core2( TickType_t_core2 xTicksToCatchUp ) PRIVILEGED_FUNCTION_core2;


/*-----------------------------------------------------------
* SCHEDULER INTERNALS AVAILABLE FOR PORTING PURPOSES
*----------------------------------------------------------*/

/*
 * THIS FUNCTION MUST NOT BE USED FROM APPLICATION CODE.  IT IS ONLY
 * INTENDED FOR USE WHEN IMPLEMENTING A PORT OF THE SCHEDULER AND IS
 * AN INTERFACE WHICH IS FOR THE EXCLUSIVE USE OF THE SCHEDULER.
 *
 * Called from the real time kernel tick (either preemptive or cooperative),
 * this increments the tick count and checks if any tasks that are blocked
 * for a finite period required removing from a blocked list and placing on
 * a ready list.  If a non-zero value is returned then a context switch is
 * required because either:
 *   + A task_core2 was removed from a blocked list because its timeout had expired,
 *     or
 *   + Time slicing is in use and there is a task_core2 of equal priority to the
 *     currently running task_core2.
 */
BaseType_t_core2 xTaskIncrementTick_core2( void ) PRIVILEGED_FUNCTION_core2;

/*
 * THIS FUNCTION MUST NOT BE USED FROM APPLICATION CODE.  IT IS AN
 * INTERFACE WHICH IS FOR THE EXCLUSIVE USE OF THE SCHEDULER.
 *
 * THIS FUNCTION MUST BE CALLED WITH INTERRUPTS DISABLED.
 *
 * Removes the calling task_core2 from the ready list and places it both
 * on the list of tasks waiting for a particular event, and the
 * list of delayed tasks.  The task_core2 will be removed from both lists
 * and replaced on the ready list should either the event occur (and
 * there be no higher priority tasks waiting on the same event) or
 * the delay period expires.
 *
 * The 'unordered' version replaces the event list item value with the
 * xItemValue_core2 value, and inserts the list item at the end of the list.
 *
 * The 'ordered' version uses the existing event list item value (which is the
 * owning task_core2's priority) to insert the list item into the event list in task_core2
 * priority order.
 *
 * @param pxEventList_core2 The list containing tasks that are blocked waiting
 * for the event to occur.
 *
 * @param xItemValue_core2 The item value to use for the event list item when the
 * event list is not ordered by task_core2 priority.
 *
 * @param xTicksToWait_core2 The maximum amount of time that the task_core2 should wait
 * for the event to occur.  This is specified in kernel ticks, the constant
 * portTICK_PERIOD_MS_core2 can be used to convert kernel ticks into a real time
 * period.
 */
void vTaskPlaceOnEventList_core2( List_t_core2 * const pxEventList_core2,
                            const TickType_t_core2 xTicksToWait_core2 ) PRIVILEGED_FUNCTION_core2;
void vTaskPlaceOnUnorderedEventList_core2( List_t_core2 * pxEventList_core2,
                                     const TickType_t_core2 xItemValue_core2,
                                     const TickType_t_core2 xTicksToWait_core2 ) PRIVILEGED_FUNCTION_core2;

/*
 * THIS FUNCTION MUST NOT BE USED FROM APPLICATION CODE.  IT IS AN
 * INTERFACE WHICH IS FOR THE EXCLUSIVE USE OF THE SCHEDULER.
 *
 * THIS FUNCTION MUST BE CALLED WITH INTERRUPTS DISABLED.
 *
 * This function performs nearly the same function as vTaskPlaceOnEventList_core2().
 * The difference being that this function does not permit tasks to block
 * indefinitely, whereas vTaskPlaceOnEventList_core2() does.
 *
 */
void vTaskPlaceOnEventListRestricted_core2( List_t_core2 * const pxEventList_core2,
                                      TickType_t_core2 xTicksToWait_core2,
                                      const BaseType_t_core2 xWaitIndefinitely_core2 ) PRIVILEGED_FUNCTION_core2;

/*
 * THIS FUNCTION MUST NOT BE USED FROM APPLICATION CODE.  IT IS AN
 * INTERFACE WHICH IS FOR THE EXCLUSIVE USE OF THE SCHEDULER.
 *
 * THIS FUNCTION MUST BE CALLED WITH INTERRUPTS DISABLED.
 *
 * Removes a task_core2 from both the specified event list and the list of blocked
 * tasks, and places it on a ready queue.
 *
 * xTaskRemoveFromEventList_core2()/vTaskRemoveFromUnorderedEventList_core2() will be called
 * if either an event occurs to unblock a task_core2, or the block timeout period
 * expires.
 *
 * xTaskRemoveFromEventList_core2() is used when the event list is in task_core2 priority
 * order.  It removes the list item from the head of the event list as that will
 * have the highest priority owning task_core2 of all the tasks on the event list.
 * vTaskRemoveFromUnorderedEventList_core2() is used when the event list is not
 * ordered and the event list items hold something other than the owning tasks
 * priority.  In this case the event list item value is updated to the value
 * passed in the xItemValue_core2 parameter.
 *
 * @return pdTRUE_core2 if the task_core2 being removed has a higher priority than the task_core2
 * making the call, otherwise pdFALSE_core2.
 */
BaseType_t_core2 xTaskRemoveFromEventList_core2( const List_t_core2 * const pxEventList_core2 ) PRIVILEGED_FUNCTION_core2;
void vTaskRemoveFromUnorderedEventList_core2( ListItem_t_core2 * pxEventListItem_core2,
                                        const TickType_t_core2 xItemValue_core2 ) PRIVILEGED_FUNCTION_core2;

/*
 * THIS FUNCTION MUST NOT BE USED FROM APPLICATION CODE.  IT IS ONLY
 * INTENDED FOR USE WHEN IMPLEMENTING A PORT OF THE SCHEDULER AND IS
 * AN INTERFACE WHICH IS FOR THE EXCLUSIVE USE OF THE SCHEDULER.
 *
 * Sets the pointer to the current TCB to the TCB of the highest priority task_core2
 * that is ready to run.
 */
portDONT_DISCARD_core2 void vTaskSwitchContext_core2( void ) PRIVILEGED_FUNCTION_core2;

/*
 * THESE FUNCTIONS MUST NOT BE USED FROM APPLICATION CODE.  THEY ARE USED BY
 * THE EVENT BITS MODULE.
 */
TickType_t_core2 uxTaskResetEventItemValue_core2( void ) PRIVILEGED_FUNCTION_core2;

/*
 * Return the handle of the calling task_core2.
 */
TaskHandle_t_core2 xTaskGetCurrentTaskHandle_core2( void ) PRIVILEGED_FUNCTION_core2;

/*
 * Shortcut used by the queue implementation to prevent unnecessary call to
 * taskYIELD_core2();
 */
void vTaskMissedYield_core2( void ) PRIVILEGED_FUNCTION_core2;

/*
 * Returns the scheduler state as taskSCHEDULER_RUNNING_core2,
 * taskSCHEDULER_NOT_STARTED_core2 or taskSCHEDULER_SUSPENDED_core2.
 */
BaseType_t_core2 xTaskGetSchedulerState_core2( void ) PRIVILEGED_FUNCTION_core2;

/*
 * Raises the priority of the mutex holder to that of the calling task_core2 should
 * the mutex holder have a priority less than the calling task_core2.
 */
BaseType_t_core2 xTaskPriorityInherit_core2( TaskHandle_t_core2 const pxMutexHolder_core2 ) PRIVILEGED_FUNCTION_core2;

/*
 * Set the priority of a task_core2 back to its proper priority in the case that it
 * inherited a higher priority while it was holding a semaphore.
 */
BaseType_t_core2 xTaskPriorityDisinherit_core2( TaskHandle_t_core2 const pxMutexHolder_core2 ) PRIVILEGED_FUNCTION_core2;

/*
 * If a higher priority task_core2 attempting to obtain a mutex caused a lower
 * priority task_core2 to inherit the higher priority task_core2's priority - but the higher
 * priority task_core2 then timed out without obtaining the mutex, then the lower
 * priority task_core2 will disinherit the priority again - but only down as far as
 * the highest priority task_core2 that is still waiting for the mutex (if there were
 * more than one task_core2 waiting for the mutex).
 */
void vTaskPriorityDisinheritAfterTimeout_core2( TaskHandle_t_core2 const pxMutexHolder_core2,
                                          UBaseType_t_core2 uxHighestPriorityWaitingTask_core2 ) PRIVILEGED_FUNCTION_core2;

/*
 * Get the uxTaskNumber_core2 assigned to the task_core2 referenced by the xTask_core2 parameter.
 */
UBaseType_t_core2 uxTaskGetTaskNumber_core2( TaskHandle_t_core2 xTask_core2 ) PRIVILEGED_FUNCTION_core2;

/*
 * Set the uxTaskNumber_core2 of the task_core2 referenced by the xTask_core2 parameter to
 * uxHandle_core2.
 */
void vTaskSetTaskNumber_core2( TaskHandle_t_core2 xTask_core2,
                         const UBaseType_t_core2 uxHandle_core2 ) PRIVILEGED_FUNCTION_core2;

/*
 * Only available when configUSE_TICKLESS_IDLE_core2 is set to 1.
 * If tickless mode is being used, or a low power mode is implemented, then
 * the tick interrupt will not execute during idle periods.  When this is the
 * case, the tick count value maintained by the scheduler needs to be kept up
 * to date with the actual execution time by being skipped forward by a time
 * equal to the idle period.
 */
void vTaskStepTick_core2( TickType_t_core2 xTicksToJump_core2 ) PRIVILEGED_FUNCTION_core2;

/*
 * Only available when configUSE_TICKLESS_IDLE_core2 is set to 1.
 * Provided for use within portSUPPRESS_TICKS_AND_SLEEP_core2() to allow the port
 * specific sleep function to determine if it is ok to proceed with the sleep,
 * and if it is ok to proceed, if it is ok to sleep indefinitely.
 *
 * This function is necessary because portSUPPRESS_TICKS_AND_SLEEP_core2() is only
 * called with the scheduler suspended, not from within a critical section.  It
 * is therefore possible for an interrupt to request a context switch between
 * portSUPPRESS_TICKS_AND_SLEEP_core2() and the low power mode actually being
 * entered.  eTaskConfirmSleepModeStatus_core2() should be called from a short
 * critical section between the timer being stopped and the sleep mode being
 * entered to ensure it is ok to proceed into the sleep mode.
 */
eSleepModeStatus_core2 eTaskConfirmSleepModeStatus_core2( void ) PRIVILEGED_FUNCTION_core2;

/*
 * For internal use only.  Increment the mutex held count when a mutex is
 * taken and return the handle of the task_core2 that has taken the mutex.
 */
TaskHandle_t_core2 pvTaskIncrementMutexHeldCount_core2( void ) PRIVILEGED_FUNCTION_core2;

/*
 * For internal use only.  Same as vTaskSetTimeOutState_core2(), but without a critical
 * section.
 */
void vTaskInternalSetTimeOutState_core2( TimeOut_t_core2 * const pxTimeOut_core2 ) PRIVILEGED_FUNCTION_core2;


/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */
#endif /* INC_TASK_H */
