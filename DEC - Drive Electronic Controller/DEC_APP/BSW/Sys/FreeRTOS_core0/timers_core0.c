/*
 * FreeRTOS_core0 Kernel V10.5.1
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
 * https://www.FreeRTOS_core0.org
 * https://github.com/FreeRTOS_core0
 *
 */

/* Standard includes. */
#include <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE_core0 prevents task_core0.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task_core0.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE_core0

#include "FreeRTOS_core0.h"
#include "task_core0.h"
#include "queue_core0.h"
#include "timers_core0.h"

#if ( INCLUDE_xTimerPendFunctionCall_core0 == 1 ) && ( configUSE_TIMERS_core0 == 0 )
    #error configUSE_TIMERS_core0 must be set to 1 to make the xTimerPendFunctionCall_core0() function available.
#endif

/* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE_core0 to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE_core0 /*lint !e9021 !e961 !e750. */


/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  This #if is closed at the very bottom
 * of this file.  If you want to include software timer functionality then ensure
 * configUSE_TIMERS_core0 is set to 1 in FreeRTOSConfig.h. */
#if ( configUSE_TIMERS_core0 == 1 )

/* Misc definitions. */
    #define tmrNO_DELAY_core0                    ( ( TickType_t_core0 ) 0U )
    #define tmrMAX_TIME_BEFORE_OVERFLOW_core0    ( ( TickType_t_core0 ) -1 )

/* The name assigned to the timer service task_core0.  This can be overridden by
 * defining trmTIMER_SERVICE_TASK_NAME in FreeRTOSConfig.h. */
    #ifndef configTIMER_SERVICE_TASK_NAME_core0
        #define configTIMER_SERVICE_TASK_NAME_core0    "Tmr Svc_core0"
    #endif

/* Bit definitions used in the ucStatus_core0 member of a timer structure. */
    #define tmrSTATUS_IS_ACTIVE_core0                  ( ( uint8_t ) 0x01 )
    #define tmrSTATUS_IS_STATICALLY_ALLOCATED_core0    ( ( uint8_t ) 0x02 )
    #define tmrSTATUS_IS_AUTORELOAD_core0              ( ( uint8_t ) 0x04 )

/* The definition of the timers_core0 themselves. */
    typedef struct tmrTimerControl_core0                  /* The old naming convention is used to prevent breaking kernel aware debuggers. */
    {
        const char * pcTimerName_core0;                   /*<< Text name.  This is not used by the kernel, it is included simply to make debugging easier. */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        ListItem_t_core0 xTimerListItem_core0;                  /*<< Standard linked list item as used by all kernel features for event management. */
        TickType_t_core0 xTimerPeriodInTicks_core0;             /*<< How quickly and often the timer expires. */
        void * pvTimerID_core0;                           /*<< An ID to identify the timer.  This allows the timer to be identified when the same callback is used for multiple timers_core0. */
        TimerCallbackFunction_t_core0 pxCallbackFunction_core0; /*<< The function that will be called when the timer expires. */
        #if ( configUSE_TRACE_FACILITY_core0 == 1 )
            UBaseType_t_core0 uxTimerNumber_core0;              /*<< An ID assigned by trace tools such as FreeRTOS_core0+Trace */
        #endif
        uint8_t ucStatus_core0;                           /*<< Holds bits to say if the timer was statically allocated or not, and if it is active or not. */
    } xTIMER_core0;

/* The old xTIMER_core0 name is maintained above then typedefed to the new Timer_t_core0
 * name below to enable the use of older kernel aware debuggers. */
    typedef xTIMER_core0 Timer_t_core0;

/* The definition of messages that can be sent and received on the timer queue.
 * Two types of message can be queued - messages that manipulate a software timer,
 * and messages that request the execution of a non-timer related callback.  The
 * two message types are defined in two separate structures, xTimerParametersType
 * and xCallbackParametersType respectively. */
    typedef struct tmrTimerParameters_core0
    {
        TickType_t_core0 xMessageValue_core0; /*<< An optional value used by a subset of commands, for example, when changing the period of a timer. */
        Timer_t_core0 * pxTimer_core0;        /*<< The timer to which the command will be applied. */
    } TimerParameter_t_core0;


    typedef struct tmrCallbackParameters_core0
    {
        PendedFunction_t_core0 pxCallbackFunction_core0; /* << The callback function to execute. */
        void * pvParameter1_core0;                 /* << The value that will be used as the callback functions first parameter. */
        uint32_t ulParameter2_core0;               /* << The value that will be used as the callback functions second parameter. */
    } CallbackParameters_t_core0;

/* The structure that contains the two message types, along with an identifier
 * that is used to determine which message type is valid. */
    typedef struct tmrTimerQueueMessage_core0
    {
        BaseType_t_core0 xMessageID_core0; /*<< The command being sent to the timer service task_core0. */
        union
        {
            TimerParameter_t_core0 xTimerParameters_core0;

            /* Don't include xCallbackParameters_core0 if it is not going to be used as
             * it makes the structure (and therefore the timer queue) larger. */
            #if ( INCLUDE_xTimerPendFunctionCall_core0 == 1 )
                CallbackParameters_t_core0 xCallbackParameters_core0;
            #endif /* INCLUDE_xTimerPendFunctionCall_core0 */
        } u;
    } DaemonTaskMessage_t_core0;

/*lint -save -e956 A manual analysis and inspection has been used to determine
 * which static variables must be declared volatile. */

/* The list in which active timers_core0 are stored.  Timers are referenced in expire
 * time order, with the nearest expiry time at the front of the list.  Only the
 * timer service task_core0 is allowed to access these lists.
 * xActiveTimerList1_core0 and xActiveTimerList2_core0 could be at function scope but that
 * breaks some kernel aware debuggers, and debuggers that reply on removing the
 * static qualifier. */
    PRIVILEGED_DATA_core0 static List_t_core0 xActiveTimerList1_core0;
    PRIVILEGED_DATA_core0 static List_t_core0 xActiveTimerList2_core0;
    PRIVILEGED_DATA_core0 static List_t_core0 * pxCurrentTimerList_core0;
    PRIVILEGED_DATA_core0 static List_t_core0 * pxOverflowTimerList_core0;

/* A queue that is used to send commands to the timer service task_core0. */
    PRIVILEGED_DATA_core0 static QueueHandle_t_core0 xTimerQueue_core0 = NULL;
    PRIVILEGED_DATA_core0 static TaskHandle_t_core0 xTimerTaskHandle_core0 = NULL;

/*lint -restore */

/*-----------------------------------------------------------*/

/*
 * Initialise the infrastructure used by the timer service task_core0 if it has not
 * been initialised already.
 */
    static void prvCheckForValidListAndQueue_core0( void ) PRIVILEGED_FUNCTION_core0;

/*
 * The timer service task_core0 (daemon).  Timer functionality is controlled by this
 * task_core0.  Other tasks communicate with the timer service task_core0 using the
 * xTimerQueue_core0 queue.
 */
    static portTASK_FUNCTION_PROTO_core0( prvTimerTask_core0, pvParameters_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * Called by the timer service task_core0 to interpret and process a command it
 * received on the timer queue.
 */
    static void prvProcessReceivedCommands_core0( void ) PRIVILEGED_FUNCTION_core0;

/*
 * Insert the timer into either xActiveTimerList1_core0, or xActiveTimerList2_core0,
 * depending on if the expire time causes a timer counter overflow.
 */
    static BaseType_t_core0 prvInsertTimerInActiveList_core0( Timer_t_core0 * const pxTimer_core0,
                                                  const TickType_t_core0 xNextExpiryTime_core0,
                                                  const TickType_t_core0 xTimeNow_core0,
                                                  const TickType_t_core0 xCommandTime_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * Reload the specified auto-reload timer.  If the reloading is backlogged,
 * clear the backlog, calling the callback for each additional reload.  When
 * this function returns, the next expiry time is after xTimeNow_core0.
 */
    static void prvReloadTimer_core0( Timer_t_core0 * const pxTimer_core0,
                                TickType_t_core0 xExpiredTime_core0,
                                const TickType_t_core0 xTimeNow_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * An active timer has reached its expire time.  Reload the timer if it is an
 * auto-reload timer, then call its callback.
 */
    static void prvProcessExpiredTimer_core0( const TickType_t_core0 xNextExpireTime_core0,
                                        const TickType_t_core0 xTimeNow_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * The tick count has overflowed.  Switch the timer lists after ensuring the
 * current timer list does not still reference some timers_core0.
 */
    static void prvSwitchTimerLists_core0( void ) PRIVILEGED_FUNCTION_core0;

/*
 * Obtain the current tick count, setting *pxTimerListsWereSwitched_core0 to pdTRUE_core0
 * if a tick count overflow occurred since prvSampleTimeNow_core0() was last called.
 */
    static TickType_t_core0 prvSampleTimeNow_core0( BaseType_t_core0 * const pxTimerListsWereSwitched_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * If the timer list contains any active timers_core0 then return the expire time of
 * the timer that will expire first and set *pxListWasEmpty_core0 to false.  If the
 * timer list does not contain any timers_core0 then return 0 and set *pxListWasEmpty_core0
 * to pdTRUE_core0.
 */
    static TickType_t_core0 prvGetNextExpireTime_core0( BaseType_t_core0 * const pxListWasEmpty_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * If a timer has expired, process it.  Otherwise, block the timer service task_core0
 * until either a timer does expire or a command is received.
 */
    static void prvProcessTimerOrBlockTask_core0( const TickType_t_core0 xNextExpireTime_core0,
                                            BaseType_t_core0 xListWasEmpty_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * Called after a Timer_t_core0 structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
    static void prvInitialiseNewTimer_core0( const char * const pcTimerName_core0, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                       const TickType_t_core0 xTimerPeriodInTicks_core0,
                                       const BaseType_t_core0 xAutoReload_core0,
                                       void * const pvTimerID_core0,
                                       TimerCallbackFunction_t_core0 pxCallbackFunction_core0,
                                       Timer_t_core0 * pxNewTimer_core0 ) PRIVILEGED_FUNCTION_core0;
/*-----------------------------------------------------------*/

    BaseType_t_core0 xTimerCreateTimerTask_core0( void )
    {
        BaseType_t_core0 xReturn_core0 = pdFAIL_core0;

        /* This function is called when the scheduler is started if
         * configUSE_TIMERS_core0 is set to 1.  Check that the infrastructure used by the
         * timer service task_core0 has been created/initialised.  If timers_core0 have already
         * been created then the initialisation will already have been performed. */
        prvCheckForValidListAndQueue_core0();

        if( xTimerQueue_core0 != NULL )
        {
            #if ( configSUPPORT_STATIC_ALLOCATION_core0 == 1 )
            {
                StaticTask_t_core0 * pxTimerTaskTCBBuffer_core0 = NULL;
                StackType_t_core0 * pxTimerTaskStackBuffer_core0 = NULL;
                uint32_t ulTimerTaskStackSize_core0;

                vApplicationGetTimerTaskMemory_core0( &pxTimerTaskTCBBuffer_core0, &pxTimerTaskStackBuffer_core0, &ulTimerTaskStackSize_core0 );
                xTimerTaskHandle_core0 = xTaskCreateStatic_core0( prvTimerTask_core0,
                                                      configTIMER_SERVICE_TASK_NAME_core0,
                                                      ulTimerTaskStackSize_core0,
                                                      NULL,
                                                      ( ( UBaseType_t_core0 ) configTIMER_TASK_PRIORITY_core0 ) | portPRIVILEGE_BIT_core0,
                                                      pxTimerTaskStackBuffer_core0,
                                                      pxTimerTaskTCBBuffer_core0 );

                if( xTimerTaskHandle_core0 != NULL )
                {
                    xReturn_core0 = pdPASS_core0;
                }
            }
            #else /* if ( configSUPPORT_STATIC_ALLOCATION_core0 == 1 ) */
            {
                xReturn_core0 = xTaskCreate_core0( prvTimerTask_core0,
                                       configTIMER_SERVICE_TASK_NAME_core0,
                                       configTIMER_TASK_STACK_DEPTH_core0,
                                       NULL,
                                       ( ( UBaseType_t_core0 ) configTIMER_TASK_PRIORITY_core0 ) | portPRIVILEGE_BIT_core0,
                                       &xTimerTaskHandle_core0 );
            }
            #endif /* configSUPPORT_STATIC_ALLOCATION_core0 */
        }
        else
        {
            //mtCOVERAGE_TEST_MARKER_core0();
        }

        //configASSERT_core0( xReturn_core0 );
        return xReturn_core0;
    }
/*-----------------------------------------------------------*/

    #if ( configSUPPORT_DYNAMIC_ALLOCATION_core0 == 1 )

        TimerHandle_t_core0 xTimerCreate_core0( const char * const pcTimerName_core0, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                    const TickType_t_core0 xTimerPeriodInTicks_core0,
                                    const BaseType_t_core0 xAutoReload_core0,
                                    void * const pvTimerID_core0,
                                    TimerCallbackFunction_t_core0 pxCallbackFunction_core0 )
        {
            Timer_t_core0 * pxNewTimer_core0;

            pxNewTimer_core0 = ( Timer_t_core0 * ) pvPortMalloc_core0( sizeof( Timer_t_core0 ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc_core0() have at least the alignment required by the MCU's stack, and the first member of Timer_t_core0 is always a pointer to the timer's mame. */

            if( pxNewTimer_core0 != NULL )
            {
                /* Status is thus far zero as the timer is not created statically
                 * and has not been started.  The auto-reload bit may get set in
                 * prvInitialiseNewTimer_core0. */
                pxNewTimer_core0->ucStatus_core0 = 0x00;
                prvInitialiseNewTimer_core0( pcTimerName_core0, xTimerPeriodInTicks_core0, xAutoReload_core0, pvTimerID_core0, pxCallbackFunction_core0, pxNewTimer_core0 );
            }

            return pxNewTimer_core0;
        }

    #endif /* configSUPPORT_DYNAMIC_ALLOCATION_core0 */
/*-----------------------------------------------------------*/

    #if ( configSUPPORT_STATIC_ALLOCATION_core0 == 1 )

        TimerHandle_t_core0 xTimerCreateStatic_core0( const char * const pcTimerName_core0, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                          const TickType_t_core0 xTimerPeriodInTicks_core0,
                                          const BaseType_t_core0 xAutoReload_core0,
                                          void * const pvTimerID_core0,
                                          TimerCallbackFunction_t_core0 pxCallbackFunction_core0,
                                          StaticTimer_t_core0 * pxTimerBuffer_core0 )
        {
            Timer_t_core0 * pxNewTimer_core0;

            #if ( configASSERT_DEFINED_core0 == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTimer_t_core0 equals the size of the real timer
                 * structure. */
                volatile size_t xSize_core0 = sizeof( StaticTimer_t_core0 );
                configASSERT_core0( xSize_core0 == sizeof( Timer_t_core0 ) );
                ( void ) xSize_core0; /* Keeps lint quiet when configASSERT_core0() is not defined. */
            }
            #endif /* configASSERT_DEFINED_core0 */

            /* A pointer to a StaticTimer_t_core0 structure MUST be provided, use it. */
            configASSERT_core0( pxTimerBuffer_core0 );
            pxNewTimer_core0 = ( Timer_t_core0 * ) pxTimerBuffer_core0; /*lint !e740 !e9087 StaticTimer_t_core0 is a pointer to a Timer_t_core0, so guaranteed to be aligned and sized correctly (checked by an assert()), so this is safe. */

            if( pxNewTimer_core0 != NULL )
            {
                /* Timers can be created statically or dynamically so note this
                 * timer was created statically in case it is later deleted.  The
                 * auto-reload bit may get set in prvInitialiseNewTimer_core0(). */
                pxNewTimer_core0->ucStatus_core0 = tmrSTATUS_IS_STATICALLY_ALLOCATED_core0;

                prvInitialiseNewTimer_core0( pcTimerName_core0, xTimerPeriodInTicks_core0, xAutoReload_core0, pvTimerID_core0, pxCallbackFunction_core0, pxNewTimer_core0 );
            }

            return pxNewTimer_core0;
        }

    #endif /* configSUPPORT_STATIC_ALLOCATION_core0 */
/*-----------------------------------------------------------*/

    static void prvInitialiseNewTimer_core0( const char * const pcTimerName_core0, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                       const TickType_t_core0 xTimerPeriodInTicks_core0,
                                       const BaseType_t_core0 xAutoReload_core0,
                                       void * const pvTimerID_core0,
                                       TimerCallbackFunction_t_core0 pxCallbackFunction_core0,
                                       Timer_t_core0 * pxNewTimer_core0 )
    {
        /* 0 is not a valid value for xTimerPeriodInTicks_core0. */
        configASSERT_core0( ( xTimerPeriodInTicks_core0 > 0 ) );

        /* Ensure the infrastructure used by the timer service task_core0 has been
         * created/initialised. */
        prvCheckForValidListAndQueue_core0();

        /* Initialise the timer structure members using the function
         * parameters. */
        pxNewTimer_core0->pcTimerName_core0 = pcTimerName_core0;
        pxNewTimer_core0->xTimerPeriodInTicks_core0 = xTimerPeriodInTicks_core0;
        pxNewTimer_core0->pvTimerID_core0 = pvTimerID_core0;
        pxNewTimer_core0->pxCallbackFunction_core0 = pxCallbackFunction_core0;
        vListInitialiseItem_core0( &( pxNewTimer_core0->xTimerListItem_core0 ) );

        if( xAutoReload_core0 != pdFALSE_core0 )
        {
            pxNewTimer_core0->ucStatus_core0 |= tmrSTATUS_IS_AUTORELOAD_core0;
        }

        traceTIMER_CREATE_core0( pxNewTimer_core0 );
    }
/*-----------------------------------------------------------*/

    BaseType_t_core0 xTimerGenericCommand_core0( TimerHandle_t_core0 xTimer_core0,
                                     const BaseType_t_core0 xCommandID_core0,
                                     const TickType_t_core0 xOptionalValue_core0,
                                     BaseType_t_core0 * const pxHigherPriorityTaskWoken_core0,
                                     const TickType_t_core0 xTicksToWait_core0 )
    {
        BaseType_t_core0 xReturn_core0 = pdFAIL_core0;
        DaemonTaskMessage_t_core0 xMessage_core0;

        configASSERT_core0( xTimer_core0 );

        /* Send a message to the timer service task_core0 to perform a particular action
         * on a particular timer definition. */
        if( xTimerQueue_core0 != NULL )
        {
            /* Send a command to the timer service task_core0 to start the xTimer_core0 timer. */
            xMessage_core0.xMessageID_core0 = xCommandID_core0;
            xMessage_core0.u.xTimerParameters_core0.xMessageValue_core0 = xOptionalValue_core0;
            xMessage_core0.u.xTimerParameters_core0.pxTimer_core0 = xTimer_core0;

            if( xCommandID_core0 < tmrFIRST_FROM_ISR_COMMAND_core0 )
            {
                if( xTaskGetSchedulerState_core0() == taskSCHEDULER_RUNNING_core0 )
                {
                    xReturn_core0 = xQueueSendToBack_core0( xTimerQueue_core0, &xMessage_core0, xTicksToWait_core0 );
                }
                else
                {
                    xReturn_core0 = xQueueSendToBack_core0( xTimerQueue_core0, &xMessage_core0, tmrNO_DELAY_core0 );
                }
            }
            else
            {
                xReturn_core0 = xQueueSendToBackFromISR_core0( xTimerQueue_core0, &xMessage_core0, pxHigherPriorityTaskWoken_core0 );
            }

            traceTIMER_COMMAND_SEND_core0( xTimer_core0, xCommandID_core0, xOptionalValue_core0, xReturn_core0 );
        }
        else
        {
            mtCOVERAGE_TEST_MARKER_core0();
        }

        return xReturn_core0;
    }
/*-----------------------------------------------------------*/

    TaskHandle_t_core0 xTimerGetTimerDaemonTaskHandle_core0( void )
    {
        /* If xTimerGetTimerDaemonTaskHandle_core0() is called before the scheduler has been
         * started, then xTimerTaskHandle_core0 will be NULL. */
        configASSERT_core0( ( xTimerTaskHandle_core0 != NULL ) );
        return xTimerTaskHandle_core0;
    }
/*-----------------------------------------------------------*/

    TickType_t_core0 xTimerGetPeriod_core0( TimerHandle_t_core0 xTimer_core0 )
    {
        Timer_t_core0 * pxTimer_core0 = xTimer_core0;

        configASSERT_core0( xTimer_core0 );
        return pxTimer_core0->xTimerPeriodInTicks_core0;
    }
/*-----------------------------------------------------------*/

    void vTimerSetReloadMode_core0( TimerHandle_t_core0 xTimer_core0,
                              const BaseType_t_core0 xAutoReload_core0 )
    {
        Timer_t_core0 * pxTimer_core0 = xTimer_core0;

        configASSERT_core0( xTimer_core0 );
        taskENTER_CRITICAL_core0();
        {
            if( xAutoReload_core0 != pdFALSE_core0 )
            {
                pxTimer_core0->ucStatus_core0 |= tmrSTATUS_IS_AUTORELOAD_core0;
            }
            else
            {
                pxTimer_core0->ucStatus_core0 &= ( ( uint8_t ) ~tmrSTATUS_IS_AUTORELOAD_core0 );
            }
        }
        taskEXIT_CRITICAL_core0();
    }
/*-----------------------------------------------------------*/

    BaseType_t_core0 xTimerGetReloadMode_core0( TimerHandle_t_core0 xTimer_core0 )
    {
        Timer_t_core0 * pxTimer_core0 = xTimer_core0;
        BaseType_t_core0 xReturn_core0;

        configASSERT_core0( xTimer_core0 );
        taskENTER_CRITICAL_core0();
        {
            if( ( pxTimer_core0->ucStatus_core0 & tmrSTATUS_IS_AUTORELOAD_core0 ) == 0 )
            {
                /* Not an auto-reload timer. */
                xReturn_core0 = pdFALSE_core0;
            }
            else
            {
                /* Is an auto-reload timer. */
                xReturn_core0 = pdTRUE_core0;
            }
        }
        taskEXIT_CRITICAL_core0();

        return xReturn_core0;
    }

    UBaseType_t_core0 uxTimerGetReloadMode_core0( TimerHandle_t_core0 xTimer_core0 )
    {
        return ( UBaseType_t_core0 ) xTimerGetReloadMode_core0( xTimer_core0 );
    }
/*-----------------------------------------------------------*/

    TickType_t_core0 xTimerGetExpiryTime_core0( TimerHandle_t_core0 xTimer_core0 )
    {
        Timer_t_core0 * pxTimer_core0 = xTimer_core0;
        TickType_t_core0 xReturn_core0;

        configASSERT_core0( xTimer_core0 );
        xReturn_core0 = listGET_LIST_ITEM_VALUE_core0( &( pxTimer_core0->xTimerListItem_core0 ) );
        return xReturn_core0;
    }
/*-----------------------------------------------------------*/

    const char * pcTimerGetName_core0( TimerHandle_t_core0 xTimer_core0 ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        Timer_t_core0 * pxTimer_core0 = xTimer_core0;

        configASSERT_core0( xTimer_core0 );
        return pxTimer_core0->pcTimerName_core0;
    }
/*-----------------------------------------------------------*/

    static void prvReloadTimer_core0( Timer_t_core0 * const pxTimer_core0,
                                TickType_t_core0 xExpiredTime_core0,
                                const TickType_t_core0 xTimeNow_core0 )
    {
        /* Insert the timer into the appropriate list for the next expiry time.
         * If the next expiry time has already passed, advance the expiry time,
         * call the callback function, and try again. */
        while( prvInsertTimerInActiveList_core0( pxTimer_core0, ( xExpiredTime_core0 + pxTimer_core0->xTimerPeriodInTicks_core0 ), xTimeNow_core0, xExpiredTime_core0 ) != pdFALSE_core0 )
        {
            /* Advance the expiry time. */
            xExpiredTime_core0 += pxTimer_core0->xTimerPeriodInTicks_core0;

            /* Call the timer callback. */
            traceTIMER_EXPIRED_core0( pxTimer_core0 );
            pxTimer_core0->pxCallbackFunction_core0( ( TimerHandle_t_core0 ) pxTimer_core0 );
        }
    }
/*-----------------------------------------------------------*/

    static void prvProcessExpiredTimer_core0( const TickType_t_core0 xNextExpireTime_core0,
                                        const TickType_t_core0 xTimeNow_core0 )
    {
        Timer_t_core0 * const pxTimer_core0 = ( Timer_t_core0 * ) listGET_OWNER_OF_HEAD_ENTRY_core0( pxCurrentTimerList_core0 ); /*lint !e9087 !e9079 void * is used as this macro is used with tasks and co-routines too.  Alignment is known to be fine as the type of the pointer stored and retrieved is the same. */

        /* Remove the timer from the list of active timers_core0.  A check has already
         * been performed to ensure the list is not empty. */

        ( void ) uxListRemove_core0( &( pxTimer_core0->xTimerListItem_core0 ) );

        /* If the timer is an auto-reload timer then calculate the next
         * expiry time and re-insert the timer in the list of active timers_core0. */
        if( ( pxTimer_core0->ucStatus_core0 & tmrSTATUS_IS_AUTORELOAD_core0 ) != 0 )
        {
            prvReloadTimer_core0( pxTimer_core0, xNextExpireTime_core0, xTimeNow_core0 );
        }
        else
        {
            pxTimer_core0->ucStatus_core0 &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE_core0 );
        }

        /* Call the timer callback. */
        traceTIMER_EXPIRED_core0( pxTimer_core0 );
        pxTimer_core0->pxCallbackFunction_core0( ( TimerHandle_t_core0 ) pxTimer_core0 );
    }
/*-----------------------------------------------------------*/

    static portTASK_FUNCTION_core0( prvTimerTask_core0, pvParameters_core0 )
    {
        TickType_t_core0 xNextExpireTime_core0;
        BaseType_t_core0 xListWasEmpty_core0;

        /* Just to avoid compiler warnings. */
        ( void ) pvParameters_core0;

        #if ( configUSE_DAEMON_TASK_STARTUP_HOOK_core0 == 1 )
        {
            extern void vApplicationDaemonTaskStartupHook_core0( void );

            /* Allow the application writer to execute some code in the context of
             * this task_core0 at the point the task_core0 starts executing.  This is useful if the
             * application includes initialisation code that would benefit from
             * executing after the scheduler has been started. */
            vApplicationDaemonTaskStartupHook_core0();
        }
        #endif /* configUSE_DAEMON_TASK_STARTUP_HOOK_core0 */

        for( ; ; )
        {
            /* Query the timers_core0 list to see if it contains any timers_core0, and if so,
             * obtain the time at which the next timer will expire. */
            xNextExpireTime_core0 = prvGetNextExpireTime_core0( &xListWasEmpty_core0 );

            /* If a timer has expired, process it.  Otherwise, block this task_core0
             * until either a timer does expire, or a command is received. */
            prvProcessTimerOrBlockTask_core0( xNextExpireTime_core0, xListWasEmpty_core0 );

            /* Empty the command queue. */
            prvProcessReceivedCommands_core0();
        }
    }
/*-----------------------------------------------------------*/

    static void prvProcessTimerOrBlockTask_core0( const TickType_t_core0 xNextExpireTime_core0,
                                            BaseType_t_core0 xListWasEmpty_core0 )
    {
        TickType_t_core0 xTimeNow_core0;
        BaseType_t_core0 xTimerListsWereSwitched_core0;

        vTaskSuspendAll_core0();
        {
            /* Obtain the time now to make an assessment as to whether the timer
             * has expired or not.  If obtaining the time causes the lists to switch
             * then don't process this timer as any timers_core0 that remained in the list
             * when the lists were switched will have been processed within the
             * prvSampleTimeNow_core0() function. */
            xTimeNow_core0 = prvSampleTimeNow_core0( &xTimerListsWereSwitched_core0 );

            if( xTimerListsWereSwitched_core0 == pdFALSE_core0 )
            {
                /* The tick count has not overflowed, has the timer expired? */
                if( ( xListWasEmpty_core0 == pdFALSE_core0 ) && ( xNextExpireTime_core0 <= xTimeNow_core0 ) )
                {
                    ( void ) xTaskResumeAll_core0();
                    prvProcessExpiredTimer_core0( xNextExpireTime_core0, xTimeNow_core0 );
                }
                else
                {
                    /* The tick count has not overflowed, and the next expire
                     * time has not been reached yet.  This task_core0 should therefore
                     * block to wait for the next expire time or a command to be
                     * received - whichever comes first.  The following line cannot
                     * be reached unless xNextExpireTime_core0 > xTimeNow_core0, except in the
                     * case when the current timer list is empty. */
                    if( xListWasEmpty_core0 != pdFALSE_core0 )
                    {
                        /* The current timer list is empty - is the overflow list
                         * also empty? */
                        xListWasEmpty_core0 = listLIST_IS_EMPTY_core0( pxOverflowTimerList_core0 );
                    }

                    vQueueWaitForMessageRestricted_core0( xTimerQueue_core0, ( xNextExpireTime_core0 - xTimeNow_core0 ), xListWasEmpty_core0 );

                    if( xTaskResumeAll_core0() == pdFALSE_core0 )
                    {
                        /* Yield to wait for either a command to arrive, or the
                         * block time to expire.  If a command arrived between the
                         * critical section being exited and this yield then the yield
                         * will not cause the task_core0 to block. */
                        portYIELD_WITHIN_API_core0();
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
            }
            else
            {
                ( void ) xTaskResumeAll_core0();
            }
        }
    }
/*-----------------------------------------------------------*/

    static TickType_t_core0 prvGetNextExpireTime_core0( BaseType_t_core0 * const pxListWasEmpty_core0 )
    {
        TickType_t_core0 xNextExpireTime_core0;

        /* Timers are listed in expiry time order, with the head of the list
         * referencing the task_core0 that will expire first.  Obtain the time at which
         * the timer with the nearest expiry time will expire.  If there are no
         * active timers_core0 then just set the next expire time to 0.  That will cause
         * this task_core0 to unblock when the tick count overflows, at which point the
         * timer lists will be switched and the next expiry time can be
         * re-assessed.  */
        *pxListWasEmpty_core0 = listLIST_IS_EMPTY_core0( pxCurrentTimerList_core0 );

        if( *pxListWasEmpty_core0 == pdFALSE_core0 )
        {
            xNextExpireTime_core0 = listGET_ITEM_VALUE_OF_HEAD_ENTRY_core0( pxCurrentTimerList_core0 );
        }
        else
        {
            /* Ensure the task_core0 unblocks when the tick count rolls over. */
            xNextExpireTime_core0 = ( TickType_t_core0 ) 0U;
        }

        return xNextExpireTime_core0;
    }
/*-----------------------------------------------------------*/

    static TickType_t_core0 prvSampleTimeNow_core0( BaseType_t_core0 * const pxTimerListsWereSwitched_core0 )
    {
        TickType_t_core0 xTimeNow_core0;
        PRIVILEGED_DATA_core0 static TickType_t_core0 xLastTime_core0 = ( TickType_t_core0 ) 0U; /*lint !e956 Variable is only accessible to one task_core0. */

        xTimeNow_core0 = xTaskGetTickCount_core0();

        if( xTimeNow_core0 < xLastTime_core0 )
        {
            prvSwitchTimerLists_core0();
            *pxTimerListsWereSwitched_core0 = pdTRUE_core0;
        }
        else
        {
            *pxTimerListsWereSwitched_core0 = pdFALSE_core0;
        }

        xLastTime_core0 = xTimeNow_core0;

        return xTimeNow_core0;
    }
/*-----------------------------------------------------------*/

    static BaseType_t_core0 prvInsertTimerInActiveList_core0( Timer_t_core0 * const pxTimer_core0,
                                                  const TickType_t_core0 xNextExpiryTime_core0,
                                                  const TickType_t_core0 xTimeNow_core0,
                                                  const TickType_t_core0 xCommandTime_core0 )
    {
        BaseType_t_core0 xProcessTimerNow_core0 = pdFALSE_core0;

        listSET_LIST_ITEM_VALUE_core0( &( pxTimer_core0->xTimerListItem_core0 ), xNextExpiryTime_core0 );
        listSET_LIST_ITEM_OWNER_core0( &( pxTimer_core0->xTimerListItem_core0 ), pxTimer_core0 );

        if( xNextExpiryTime_core0 <= xTimeNow_core0 )
        {
            /* Has the expiry time elapsed between the command to start/reset a
             * timer was issued, and the time the command was processed? */
            if( ( ( TickType_t_core0 ) ( xTimeNow_core0 - xCommandTime_core0 ) ) >= pxTimer_core0->xTimerPeriodInTicks_core0 ) /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
            {
                /* The time between a command being issued and the command being
                 * processed actually exceeds the timers_core0 period.  */
                xProcessTimerNow_core0 = pdTRUE_core0;
            }
            else
            {
                vListInsert_core0( pxOverflowTimerList_core0, &( pxTimer_core0->xTimerListItem_core0 ) );
            }
        }
        else
        {
            if( ( xTimeNow_core0 < xCommandTime_core0 ) && ( xNextExpiryTime_core0 >= xCommandTime_core0 ) )
            {
                /* If, since the command was issued, the tick count has overflowed
                 * but the expiry time has not, then the timer must have already passed
                 * its expiry time and should be processed immediately. */
                xProcessTimerNow_core0 = pdTRUE_core0;
            }
            else
            {
                vListInsert_core0( pxCurrentTimerList_core0, &( pxTimer_core0->xTimerListItem_core0 ) );
            }
        }

        return xProcessTimerNow_core0;
    }
/*-----------------------------------------------------------*/

    static void prvProcessReceivedCommands_core0( void )
    {
        DaemonTaskMessage_t_core0 xMessage_core0;
        Timer_t_core0 * pxTimer_core0;
        BaseType_t_core0 xTimerListsWereSwitched_core0;
        TickType_t_core0 xTimeNow_core0;

        while( xQueueReceive_core0( xTimerQueue_core0, &xMessage_core0, tmrNO_DELAY_core0 ) != pdFAIL_core0 ) /*lint !e603 xMessage_core0 does not have to be initialised as it is passed out, not in, and it is not used unless xQueueReceive_core0() returns pdTRUE_core0. */
        {
            #if ( INCLUDE_xTimerPendFunctionCall_core0 == 1 )
            {
                /* Negative commands are pended function calls rather than timer
                 * commands. */
                if( xMessage_core0.xMessageID_core0 < ( BaseType_t_core0 ) 0 )
                {
                    const CallbackParameters_t_core0 * const pxCallback_core0 = &( xMessage_core0.u.xCallbackParameters_core0 );

                    /* The timer uses the xCallbackParameters_core0 member to request a
                     * callback be executed.  Check the callback is not NULL. */
                    configASSERT_core0( pxCallback_core0 );

                    /* Call the function. */
                    pxCallback_core0->pxCallbackFunction_core0( pxCallback_core0->pvParameter1_core0, pxCallback_core0->ulParameter2_core0 );
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }
            }
            #endif /* INCLUDE_xTimerPendFunctionCall_core0 */

            /* Commands that are positive are timer commands rather than pended
             * function calls. */
            if( xMessage_core0.xMessageID_core0 >= ( BaseType_t_core0 ) 0 )
            {
                /* The messages uses the xTimerParameters_core0 member to work on a
                 * software timer. */
                pxTimer_core0 = xMessage_core0.u.xTimerParameters_core0.pxTimer_core0;

                if( listIS_CONTAINED_WITHIN_core0( NULL, &( pxTimer_core0->xTimerListItem_core0 ) ) == pdFALSE_core0 ) /*lint !e961. The cast is only redundant when NULL is passed into the macro. */
                {
                    /* The timer is in a list, remove it. */
                    ( void ) uxListRemove_core0( &( pxTimer_core0->xTimerListItem_core0 ) );
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }

                traceTIMER_COMMAND_RECEIVED_core0( pxTimer_core0, xMessage_core0.xMessageID_core0, xMessage_core0.u.xTimerParameters_core0.xMessageValue_core0 );

                /* In this case the xTimerListsWereSwitched_core0 parameter is not used, but
                 *  it must be present in the function call.  prvSampleTimeNow_core0() must be
                 *  called after the message is received from xTimerQueue_core0 so there is no
                 *  possibility of a higher priority task_core0 adding a message to the message
                 *  queue with a time that is ahead of the timer daemon task_core0 (because it
                 *  pre-empted the timer daemon task_core0 after the xTimeNow_core0 value was set). */
                xTimeNow_core0 = prvSampleTimeNow_core0( &xTimerListsWereSwitched_core0 );

                switch( xMessage_core0.xMessageID_core0 )
                {
                    case tmrCOMMAND_START_core0:
                    case tmrCOMMAND_START_FROM_ISR_core0:
                    case tmrCOMMAND_RESET_core0:
                    case tmrCOMMAND_RESET_FROM_ISR_core0:
                        /* Start or restart a timer. */
                        pxTimer_core0->ucStatus_core0 |= tmrSTATUS_IS_ACTIVE_core0;

                        if( prvInsertTimerInActiveList_core0( pxTimer_core0, xMessage_core0.u.xTimerParameters_core0.xMessageValue_core0 + pxTimer_core0->xTimerPeriodInTicks_core0, xTimeNow_core0, xMessage_core0.u.xTimerParameters_core0.xMessageValue_core0 ) != pdFALSE_core0 )
                        {
                            /* The timer expired before it was added to the active
                             * timer list.  Process it now. */
                            if( ( pxTimer_core0->ucStatus_core0 & tmrSTATUS_IS_AUTORELOAD_core0 ) != 0 )
                            {
                                prvReloadTimer_core0( pxTimer_core0, xMessage_core0.u.xTimerParameters_core0.xMessageValue_core0 + pxTimer_core0->xTimerPeriodInTicks_core0, xTimeNow_core0 );
                            }
                            else
                            {
                                pxTimer_core0->ucStatus_core0 &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE_core0 );
                            }

                            /* Call the timer callback. */
                            traceTIMER_EXPIRED_core0( pxTimer_core0 );
                            pxTimer_core0->pxCallbackFunction_core0( ( TimerHandle_t_core0 ) pxTimer_core0 );
                        }
                        else
                        {
                            mtCOVERAGE_TEST_MARKER_core0();
                        }

                        break;

                    case tmrCOMMAND_STOP_core0:
                    case tmrCOMMAND_STOP_FROM_ISR_core0:
                        /* The timer has already been removed from the active list. */
                        pxTimer_core0->ucStatus_core0 &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE_core0 );
                        break;

                    case tmrCOMMAND_CHANGE_PERIOD_core0:
                    case tmrCOMMAND_CHANGE_PERIOD_FROM_ISR_core0:
                        pxTimer_core0->ucStatus_core0 |= tmrSTATUS_IS_ACTIVE_core0;
                        pxTimer_core0->xTimerPeriodInTicks_core0 = xMessage_core0.u.xTimerParameters_core0.xMessageValue_core0;
                        configASSERT_core0( ( pxTimer_core0->xTimerPeriodInTicks_core0 > 0 ) );

                        /* The new period does not really have a reference, and can
                         * be longer or shorter than the old one.  The command time is
                         * therefore set to the current time, and as the period cannot
                         * be zero the next expiry time can only be in the future,
                         * meaning (unlike for the xTimerStart_core0() case above) there is
                         * no fail case that needs to be handled here. */
                        ( void ) prvInsertTimerInActiveList_core0( pxTimer_core0, ( xTimeNow_core0 + pxTimer_core0->xTimerPeriodInTicks_core0 ), xTimeNow_core0, xTimeNow_core0 );
                        break;

                    case tmrCOMMAND_DELETE_core0:
                        #if ( configSUPPORT_DYNAMIC_ALLOCATION_core0 == 1 )
                        {
                            /* The timer has already been removed from the active list,
                             * just free up the memory if the memory was dynamically
                             * allocated. */
                            if( ( pxTimer_core0->ucStatus_core0 & tmrSTATUS_IS_STATICALLY_ALLOCATED_core0 ) == ( uint8_t ) 0 )
                            {
                                vPortFree_core0( pxTimer_core0 );
                            }
                            else
                            {
                                pxTimer_core0->ucStatus_core0 &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE_core0 );
                            }
                        }
                        #else /* if ( configSUPPORT_DYNAMIC_ALLOCATION_core0 == 1 ) */
                        {
                            /* If dynamic allocation is not enabled, the memory
                             * could not have been dynamically allocated. So there is
                             * no need to free the memory - just mark the timer as
                             * "not active". */
                            pxTimer_core0->ucStatus_core0 &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE_core0 );
                        }
                        #endif /* configSUPPORT_DYNAMIC_ALLOCATION_core0 */
                        break;

                    default:
                        /* Don't expect to get here. */
                        break;
                }
            }
        }
    }
/*-----------------------------------------------------------*/

    static void prvSwitchTimerLists_core0( void )
    {
        TickType_t_core0 xNextExpireTime_core0;
        List_t_core0 * pxTemp_core0;

        /* The tick count has overflowed.  The timer lists must be switched.
         * If there are any timers_core0 still referenced from the current timer list
         * then they must have expired and should be processed before the lists
         * are switched. */
        while( listLIST_IS_EMPTY_core0( pxCurrentTimerList_core0 ) == pdFALSE_core0 )
        {
            xNextExpireTime_core0 = listGET_ITEM_VALUE_OF_HEAD_ENTRY_core0( pxCurrentTimerList_core0 );

            /* Process the expired timer.  For auto-reload timers_core0, be careful to
             * process only expirations that occur on the current list.  Further
             * expirations must wait until after the lists are switched. */
            prvProcessExpiredTimer_core0( xNextExpireTime_core0, tmrMAX_TIME_BEFORE_OVERFLOW_core0 );
        }

        pxTemp_core0 = pxCurrentTimerList_core0;
        pxCurrentTimerList_core0 = pxOverflowTimerList_core0;
        pxOverflowTimerList_core0 = pxTemp_core0;
    }
/*-----------------------------------------------------------*/

    static void prvCheckForValidListAndQueue_core0( void )
    {
        /* Check that the list from which active timers_core0 are referenced, and the
         * queue used to communicate with the timer service, have been
         * initialised. */
        taskENTER_CRITICAL_core0();
        {
            if( xTimerQueue_core0 == NULL )
            {
                vListInitialise_core0( &xActiveTimerList1_core0 );
                vListInitialise_core0( &xActiveTimerList2_core0 );
                pxCurrentTimerList_core0 = &xActiveTimerList1_core0;
                pxOverflowTimerList_core0 = &xActiveTimerList2_core0;

                #if ( configSUPPORT_STATIC_ALLOCATION_core0 == 1 )
                {
                    /* The timer queue is allocated statically in case
                     * configSUPPORT_DYNAMIC_ALLOCATION_core0 is 0. */
                    PRIVILEGED_DATA_core0 static StaticQueue_t_core0 xStaticTimerQueue_core0;                                                                          /*lint !e956 Ok to declare in this manner to prevent additional conditional compilation guards in other locations. */
                    PRIVILEGED_DATA_core0 static uint8_t ucStaticTimerQueueStorage_core0[ ( size_t ) configTIMER_QUEUE_LENGTH_core0 * sizeof( DaemonTaskMessage_t_core0 ) ]; /*lint !e956 Ok to declare in this manner to prevent additional conditional compilation guards in other locations. */

                    xTimerQueue_core0 = xQueueCreateStatic_core0( ( UBaseType_t_core0 ) configTIMER_QUEUE_LENGTH_core0, ( UBaseType_t_core0 ) sizeof( DaemonTaskMessage_t_core0 ), &( ucStaticTimerQueueStorage_core0[ 0 ] ), &xStaticTimerQueue_core0 );
                }
                #else
                {
                    xTimerQueue_core0 = xQueueCreate_core0( ( UBaseType_t_core0 ) configTIMER_QUEUE_LENGTH_core0, sizeof( DaemonTaskMessage_t_core0 ) );
                }
                #endif /* if ( configSUPPORT_STATIC_ALLOCATION_core0 == 1 ) */

                #if ( configQUEUE_REGISTRY_SIZE_core0 > 0 )
                {
                    if( xTimerQueue_core0 != NULL )
                    {
                        vQueueAddToRegistry_core0( xTimerQueue_core0, "TmrQ_core0" );
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                #endif /* configQUEUE_REGISTRY_SIZE_core0 */
            }
            else
            {
                mtCOVERAGE_TEST_MARKER_core0();
            }
        }
        taskEXIT_CRITICAL_core0();
    }
/*-----------------------------------------------------------*/

    BaseType_t_core0 xTimerIsTimerActive_core0( TimerHandle_t_core0 xTimer_core0 )
    {
        BaseType_t_core0 xReturn_core0;
        Timer_t_core0 * pxTimer_core0 = xTimer_core0;

        configASSERT_core0( xTimer_core0 );

        /* Is the timer in the list of active timers_core0? */
        taskENTER_CRITICAL_core0();
        {
            if( ( pxTimer_core0->ucStatus_core0 & tmrSTATUS_IS_ACTIVE_core0 ) == 0 )
            {
                xReturn_core0 = pdFALSE_core0;
            }
            else
            {
                xReturn_core0 = pdTRUE_core0;
            }
        }
        taskEXIT_CRITICAL_core0();

        return xReturn_core0;
    } /*lint !e818 Can't be pointer to const due to the typedef. */
/*-----------------------------------------------------------*/

    void * pvTimerGetTimerID_core0( const TimerHandle_t_core0 xTimer_core0 )
    {
        Timer_t_core0 * const pxTimer_core0 = xTimer_core0;
        void * pvReturn_core0;

        configASSERT_core0( xTimer_core0 );

        taskENTER_CRITICAL_core0();
        {
            pvReturn_core0 = pxTimer_core0->pvTimerID_core0;
        }
        taskEXIT_CRITICAL_core0();

        return pvReturn_core0;
    }
/*-----------------------------------------------------------*/

    void vTimerSetTimerID_core0( TimerHandle_t_core0 xTimer_core0,
                           void * pvNewID_core0 )
    {
        Timer_t_core0 * const pxTimer_core0 = xTimer_core0;

        configASSERT_core0( xTimer_core0 );

        taskENTER_CRITICAL_core0();
        {
            pxTimer_core0->pvTimerID_core0 = pvNewID_core0;
        }
        taskEXIT_CRITICAL_core0();
    }
/*-----------------------------------------------------------*/

    #if ( INCLUDE_xTimerPendFunctionCall_core0 == 1 )

        BaseType_t_core0 xTimerPendFunctionCallFromISR_core0( PendedFunction_t_core0 xFunctionToPend_core0,
                                                  void * pvParameter1_core0,
                                                  uint32_t ulParameter2_core0,
                                                  BaseType_t_core0 * pxHigherPriorityTaskWoken_core0 )
        {
            DaemonTaskMessage_t_core0 xMessage_core0;
            BaseType_t_core0 xReturn_core0;

            /* Complete the message with the function parameters and post it to the
             * daemon task_core0. */
            xMessage_core0.xMessageID_core0 = tmrCOMMAND_EXECUTE_CALLBACK_FROM_ISR_core0;
            xMessage_core0.u.xCallbackParameters_core0.pxCallbackFunction_core0 = xFunctionToPend_core0;
            xMessage_core0.u.xCallbackParameters_core0.pvParameter1_core0 = pvParameter1_core0;
            xMessage_core0.u.xCallbackParameters_core0.ulParameter2_core0 = ulParameter2_core0;

            xReturn_core0 = xQueueSendFromISR_core0( xTimerQueue_core0, &xMessage_core0, pxHigherPriorityTaskWoken_core0 );

            tracePEND_FUNC_CALL_FROM_ISR_core0( xFunctionToPend_core0, pvParameter1_core0, ulParameter2_core0, xReturn_core0 );

            return xReturn_core0;
        }

    #endif /* INCLUDE_xTimerPendFunctionCall_core0 */
/*-----------------------------------------------------------*/

    #if ( INCLUDE_xTimerPendFunctionCall_core0 == 1 )

        BaseType_t_core0 xTimerPendFunctionCall_core0( PendedFunction_t_core0 xFunctionToPend_core0,
                                           void * pvParameter1_core0,
                                           uint32_t ulParameter2_core0,
                                           TickType_t_core0 xTicksToWait_core0 )
        {
            DaemonTaskMessage_t_core0 xMessage_core0;
            BaseType_t_core0 xReturn_core0;

            /* This function can only be called after a timer has been created or
             * after the scheduler has been started because, until then, the timer
             * queue does not exist. */
            configASSERT_core0( xTimerQueue_core0 );

            /* Complete the message with the function parameters and post it to the
             * daemon task_core0. */
            xMessage_core0.xMessageID_core0 = tmrCOMMAND_EXECUTE_CALLBACK_core0;
            xMessage_core0.u.xCallbackParameters_core0.pxCallbackFunction_core0 = xFunctionToPend_core0;
            xMessage_core0.u.xCallbackParameters_core0.pvParameter1_core0 = pvParameter1_core0;
            xMessage_core0.u.xCallbackParameters_core0.ulParameter2_core0 = ulParameter2_core0;

            xReturn_core0 = xQueueSendToBack_core0( xTimerQueue_core0, &xMessage_core0, xTicksToWait_core0 );

            tracePEND_FUNC_CALL_core0( xFunctionToPend_core0, pvParameter1_core0, ulParameter2_core0, xReturn_core0 );

            return xReturn_core0;
        }

    #endif /* INCLUDE_xTimerPendFunctionCall_core0 */
/*-----------------------------------------------------------*/

    #if ( configUSE_TRACE_FACILITY_core0 == 1 )

        UBaseType_t_core0 uxTimerGetTimerNumber_core0( TimerHandle_t_core0 xTimer_core0 )
        {
            return ( ( Timer_t_core0 * ) xTimer_core0 )->uxTimerNumber_core0;
        }

    #endif /* configUSE_TRACE_FACILITY_core0 */
/*-----------------------------------------------------------*/

    #if ( configUSE_TRACE_FACILITY_core0 == 1 )

        void vTimerSetTimerNumber_core0( TimerHandle_t_core0 xTimer_core0,
                                   UBaseType_t_core0 uxTimerNumber_core0 )
        {
            ( ( Timer_t_core0 * ) xTimer_core0 )->uxTimerNumber_core0 = uxTimerNumber_core0;
        }

    #endif /* configUSE_TRACE_FACILITY_core0 */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  If you want to include software timer
 * functionality then ensure configUSE_TIMERS_core0 is set to 1 in FreeRTOSConfig.h. */
#endif /* configUSE_TIMERS_core0 == 1 */
