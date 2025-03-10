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

/*
 * When the MPU is used the standard (non MPU) API functions are mapped to
 * equivalents that start "MPU_", the prototypes for which are defined in this
 * header files.  This will cause the application code to call the MPU_ version
 * which wraps the non-MPU version with privilege promoting then demoting code,
 * so the kernel code always runs will full privileges.
 */


#ifndef MPU_PROTOTYPES_CORE0_H
#define MPU_PROTOTYPES_CORE0_H

#include "FreeRTOS_core0.h"
#include "task_core0.h"

/* MPU versions of task_core0.h API functions. */
BaseType_t_core0 MPU_xTaskCreate_core0( TaskFunction_t_core0 pxTaskCode_core0,
                            const char * const pcName_core0,
                            const uint16_t usStackDepth_core0,
                            void * const pvParameters_core0,
                            UBaseType_t_core0 uxPriority_core0,
                            TaskHandle_t_core0 * const pxCreatedTask_core0 ) FREERTOS_SYSTEM_CALL_core0;
TaskHandle_t_core0 MPU_xTaskCreateStatic_core0( TaskFunction_t_core0 pxTaskCode_core0,
                                    const char * const pcName_core0,
                                    const uint32_t ulStackDepth_core0,
                                    void * const pvParameters_core0,
                                    UBaseType_t_core0 uxPriority_core0,
                                    StackType_t_core0 * const puxStackBuffer_core0,
                                    StaticTask_t_core0 * const pxTaskBuffer_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vTaskDelete_core0( TaskHandle_t_core0 xTaskToDelete_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vTaskDelay_core0( const TickType_t_core0 xTicksToDelay_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xTaskDelayUntil_core0( TickType_t_core0 * const pxPreviousWakeTime_core0,
                                const TickType_t_core0 xTimeIncrement_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xTaskAbortDelay_core0( TaskHandle_t_core0 xTask_core0 ) FREERTOS_SYSTEM_CALL_core0;
UBaseType_t_core0 MPU_uxTaskPriorityGet_core0( const TaskHandle_t_core0 xTask_core0 ) FREERTOS_SYSTEM_CALL_core0;
eTaskState_core0 MPU_eTaskGetState_core0( TaskHandle_t_core0 xTask_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vTaskGetInfo_core0( TaskHandle_t_core0 xTask_core0,
                       TaskStatus_t_core0 * pxTaskStatus_core0,
                       BaseType_t_core0 xGetFreeStackSpace,
                       eTaskState_core0 eState_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vTaskPrioritySet_core0( TaskHandle_t_core0 xTask_core0,
                           UBaseType_t_core0 uxNewPriority_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vTaskSuspend_core0( TaskHandle_t_core0 xTaskToSuspend_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vTaskResume_core0( TaskHandle_t_core0 xTaskToResume_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vTaskStartScheduler_core0( void ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vTaskSuspendAll_core0( void ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xTaskResumeAll_core0( void ) FREERTOS_SYSTEM_CALL_core0;
TickType_t_core0 MPU_xTaskGetTickCount_core0( void ) FREERTOS_SYSTEM_CALL_core0;
UBaseType_t_core0 MPU_uxTaskGetNumberOfTasks_core0( void ) FREERTOS_SYSTEM_CALL_core0;
char * MPU_pcTaskGetName_core0( TaskHandle_t_core0 xTaskToQuery_core0 ) FREERTOS_SYSTEM_CALL_core0;
TaskHandle_t_core0 MPU_xTaskGetHandle_core0( const char * pcNameToQuery_core0 ) FREERTOS_SYSTEM_CALL_core0;
UBaseType_t_core0 MPU_uxTaskGetStackHighWaterMark_core0( TaskHandle_t_core0 xTask_core0 ) FREERTOS_SYSTEM_CALL_core0;
configSTACK_DEPTH_TYPE_core0 MPU_uxTaskGetStackHighWaterMark2_core0( TaskHandle_t_core0 xTask_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vTaskSetApplicationTaskTag_core0( TaskHandle_t_core0 xTask_core0,
                                     TaskHookFunction_t_core0 pxHookFunction ) FREERTOS_SYSTEM_CALL_core0;
TaskHookFunction_t_core0 MPU_xTaskGetApplicationTaskTag_core0( TaskHandle_t_core0 xTask_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vTaskSetThreadLocalStoragePointer_core0( TaskHandle_t_core0 xTaskToSet_core0,
                                            BaseType_t_core0 xIndex_core0,
                                            void * pvValue ) FREERTOS_SYSTEM_CALL_core0;
void * MPU_pvTaskGetThreadLocalStoragePointer_core0( TaskHandle_t_core0 xTaskToQuery_core0,
                                               BaseType_t_core0 xIndex_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xTaskCallApplicationTaskHook_core0( TaskHandle_t_core0 xTask_core0,
                                             void * pvParameter_core0 ) FREERTOS_SYSTEM_CALL_core0;
TaskHandle_t_core0 MPU_xTaskGetIdleTaskHandle_core0( void ) FREERTOS_SYSTEM_CALL_core0;
UBaseType_t_core0 MPU_uxTaskGetSystemState_core0( TaskStatus_t_core0 * const pxTaskStatusArray_core0,
                                      const UBaseType_t_core0 uxArraySize_core0,
                                      configRUN_TIME_COUNTER_TYPE_core0 * const pulTotalRunTime_core0 ) FREERTOS_SYSTEM_CALL_core0;
configRUN_TIME_COUNTER_TYPE_core0 MPU_ulTaskGetIdleRunTimeCounter_core0( void ) FREERTOS_SYSTEM_CALL_core0;
configRUN_TIME_COUNTER_TYPE_core0 MPU_ulTaskGetIdleRunTimePercent_core0( void ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vTaskList_core0( char * pcWriteBuffer_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vTaskGetRunTimeStats_core0( char * pcWriteBuffer_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xTaskGenericNotify_core0( TaskHandle_t_core0 xTaskToNotify_core0,
                                   UBaseType_t_core0 uxIndexToNotify_core0,
                                   uint32_t ulValue_core0,
                                   eNotifyAction_core0 eAction_core0,
                                   uint32_t * pulPreviousNotificationValue_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xTaskGenericNotifyWait_core0( UBaseType_t_core0 uxIndexToWaitOn_core0,
                                       uint32_t ulBitsToClearOnEntry_core0,
                                       uint32_t ulBitsToClearOnExit_core0,
                                       uint32_t * pulNotificationValue_core0,
                                       TickType_t_core0 xTicksToWait_core0 ) FREERTOS_SYSTEM_CALL_core0;
uint32_t MPU_ulTaskGenericNotifyTake_core0( UBaseType_t_core0 uxIndexToWaitOn_core0,
                                      BaseType_t_core0 xClearCountOnExit_core0,
                                      TickType_t_core0 xTicksToWait_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xTaskGenericNotifyStateClear_core0( TaskHandle_t_core0 xTask_core0,
                                             UBaseType_t_core0 uxIndexToClear_core0 ) FREERTOS_SYSTEM_CALL_core0;
uint32_t MPU_ulTaskGenericNotifyValueClear_core0( TaskHandle_t_core0 xTask_core0,
                                            UBaseType_t_core0 uxIndexToClear_core0,
                                            uint32_t ulBitsToClear_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xTaskIncrementTick_core0( void ) FREERTOS_SYSTEM_CALL_core0;
TaskHandle_t_core0 MPU_xTaskGetCurrentTaskHandle_core0( void ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vTaskSetTimeOutState_core0( TimeOut_t_core0 * const pxTimeOut_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xTaskCheckForTimeOut_core0( TimeOut_t_core0 * const pxTimeOut_core0,
                                     TickType_t_core0 * const pxTicksToWait_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vTaskMissedYield_core0( void ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xTaskGetSchedulerState_core0( void ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xTaskCatchUpTicks_core0( TickType_t_core0 xTicksToCatchUp ) FREERTOS_SYSTEM_CALL_core0;

/* MPU versions of queue.h API functions. */
BaseType_t_core0 MPU_xQueueGenericSend_core0( QueueHandle_t_core0 xQueue_core0,
                                  const void * const pvItemToQueue_core0,
                                  TickType_t_core0 xTicksToWait_core0,
                                  const BaseType_t_core0 xCopyPosition_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xQueueReceive_core0( QueueHandle_t_core0 xQueue_core0,
                              void * const pvBuffer_core0,
                              TickType_t_core0 xTicksToWait_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xQueuePeek_core0( QueueHandle_t_core0 xQueue_core0,
                           void * const pvBuffer_core0,
                           TickType_t_core0 xTicksToWait_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xQueueSemaphoreTake_core0( QueueHandle_t_core0 xQueue_core0,
                                    TickType_t_core0 xTicksToWait_core0 ) FREERTOS_SYSTEM_CALL_core0;
UBaseType_t_core0 MPU_uxQueueMessagesWaiting_core0( const QueueHandle_t_core0 xQueue_core0 ) FREERTOS_SYSTEM_CALL_core0;
UBaseType_t_core0 MPU_uxQueueSpacesAvailable_core0( const QueueHandle_t_core0 xQueue_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vQueueDelete_core0( QueueHandle_t_core0 xQueue_core0 ) FREERTOS_SYSTEM_CALL_core0;
QueueHandle_t_core0 MPU_xQueueCreateMutex_core0( const uint8_t ucQueueType_core0 ) FREERTOS_SYSTEM_CALL_core0;
QueueHandle_t_core0 MPU_xQueueCreateMutexStatic_core0( const uint8_t ucQueueType_core0,
                                           StaticQueue_t_core0 * pxStaticQueue_core0 ) FREERTOS_SYSTEM_CALL_core0;
QueueHandle_t_core0 MPU_xQueueCreateCountingSemaphore_core0( const UBaseType_t_core0 uxMaxCount_core0,
                                                 const UBaseType_t_core0 uxInitialCount_core0 ) FREERTOS_SYSTEM_CALL_core0;
QueueHandle_t_core0 MPU_xQueueCreateCountingSemaphoreStatic_core0( const UBaseType_t_core0 uxMaxCount_core0,
                                                       const UBaseType_t_core0 uxInitialCount_core0,
                                                       StaticQueue_t_core0 * pxStaticQueue_core0 ) FREERTOS_SYSTEM_CALL_core0;
TaskHandle_t_core0 MPU_xQueueGetMutexHolder_core0( QueueHandle_t_core0 xSemaphore_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xQueueTakeMutexRecursive_core0( QueueHandle_t_core0 xMutex_core0,
                                         TickType_t_core0 xTicksToWait_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xQueueGiveMutexRecursive_core0( QueueHandle_t_core0 pxMutex_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vQueueAddToRegistry_core0( QueueHandle_t_core0 xQueue_core0,
                              const char * pcName_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vQueueUnregisterQueue_core0( QueueHandle_t_core0 xQueue_core0 ) FREERTOS_SYSTEM_CALL_core0;
const char * MPU_pcQueueGetName_core0( QueueHandle_t_core0 xQueue_core0 ) FREERTOS_SYSTEM_CALL_core0;
QueueHandle_t_core0 MPU_xQueueGenericCreate_core0( const UBaseType_t_core0 uxQueueLength_core0,
                                       const UBaseType_t_core0 uxItemSize_core0,
                                       const uint8_t ucQueueType_core0 ) FREERTOS_SYSTEM_CALL_core0;
QueueHandle_t_core0 MPU_xQueueGenericCreateStatic_core0( const UBaseType_t_core0 uxQueueLength_core0,
                                             const UBaseType_t_core0 uxItemSize_core0,
                                             uint8_t * pucQueueStorage_core0,
                                             StaticQueue_t_core0 * pxStaticQueue_core0,
                                             const uint8_t ucQueueType_core0 ) FREERTOS_SYSTEM_CALL_core0;
QueueSetHandle_t_core0 MPU_xQueueCreateSet_core0( const UBaseType_t_core0 uxEventQueueLength_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xQueueAddToSet_core0( QueueSetMemberHandle_t_core0 xQueueOrSemaphore_core0,
                               QueueSetHandle_t_core0 xQueueSet_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xQueueRemoveFromSet_core0( QueueSetMemberHandle_t_core0 xQueueOrSemaphore_core0,
                                    QueueSetHandle_t_core0 xQueueSet_core0 ) FREERTOS_SYSTEM_CALL_core0;
QueueSetMemberHandle_t_core0 MPU_xQueueSelectFromSet_core0( QueueSetHandle_t_core0 xQueueSet_core0,
                                                const TickType_t_core0 xTicksToWait_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xQueueGenericReset_core0( QueueHandle_t_core0 xQueue_core0,
                                   BaseType_t_core0 xNewQueue_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vQueueSetQueueNumber_core0( QueueHandle_t_core0 xQueue_core0,
                               UBaseType_t_core0 uxQueueNumber_core0 ) FREERTOS_SYSTEM_CALL_core0;
UBaseType_t_core0 MPU_uxQueueGetQueueNumber_core0( QueueHandle_t_core0 xQueue_core0 ) FREERTOS_SYSTEM_CALL_core0;
uint8_t MPU_ucQueueGetQueueType_core0( QueueHandle_t_core0 xQueue_core0 ) FREERTOS_SYSTEM_CALL_core0;

/* MPU versions of timers_core0.h API functions. */
TimerHandle_t_core0 MPU_xTimerCreate_core0( const char * const pcTimerName_core0,
                                const TickType_t_core0 xTimerPeriodInTicks_core0,
                                const UBaseType_t_core0 uxAutoReload_core0,
                                void * const pvTimerID_core0,
                                TimerCallbackFunction_t_core0 pxCallbackFunction_core0 ) FREERTOS_SYSTEM_CALL_core0;
TimerHandle_t_core0 MPU_xTimerCreateStatic_core0( const char * const pcTimerName_core0,
                                      const TickType_t_core0 xTimerPeriodInTicks_core0,
                                      const UBaseType_t_core0 uxAutoReload_core0,
                                      void * const pvTimerID_core0,
                                      TimerCallbackFunction_t_core0 pxCallbackFunction_core0,
                                      StaticTimer_t_core0 * pxTimerBuffer_core0 ) FREERTOS_SYSTEM_CALL_core0;
void * MPU_pvTimerGetTimerID_core0( const TimerHandle_t_core0 xTimer_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vTimerSetTimerID_core0( TimerHandle_t_core0 xTimer_core0,
                           void * pvNewID_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xTimerIsTimerActive_core0( TimerHandle_t_core0 xTimer_core0 ) FREERTOS_SYSTEM_CALL_core0;
TaskHandle_t_core0 MPU_xTimerGetTimerDaemonTaskHandle_core0( void ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xTimerPendFunctionCall_core0( PendedFunction_t_core0 xFunctionToPend_core0,
                                       void * pvParameter1_core0,
                                       uint32_t ulParameter2_core0,
                                       TickType_t_core0 xTicksToWait_core0 ) FREERTOS_SYSTEM_CALL_core0;
const char * MPU_pcTimerGetName_core0( TimerHandle_t_core0 xTimer_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vTimerSetReloadMode_core0( TimerHandle_t_core0 xTimer_core0,
                              const UBaseType_t_core0 uxAutoReload_core0 ) FREERTOS_SYSTEM_CALL_core0;
UBaseType_t_core0 MPU_uxTimerGetReloadMode_core0( TimerHandle_t_core0 xTimer_core0 ) FREERTOS_SYSTEM_CALL_core0;
TickType_t_core0 MPU_xTimerGetPeriod_core0( TimerHandle_t_core0 xTimer_core0 ) FREERTOS_SYSTEM_CALL_core0;
TickType_t_core0 MPU_xTimerGetExpiryTime_core0( TimerHandle_t_core0 xTimer_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xTimerCreateTimerTask_core0( void ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xTimerGenericCommand_core0( TimerHandle_t_core0 xTimer_core0,
                                     const BaseType_t_core0 xCommandID_core0,
                                     const TickType_t_core0 xOptionalValue_core0,
                                     BaseType_t_core0 * const pxHigherPriorityTaskWoken_core0,
                                     const TickType_t_core0 xTicksToWait_core0 ) FREERTOS_SYSTEM_CALL_core0;

/* MPU versions of event_group.h API functions. */
EventGroupHandle_t_core0 MPU_xEventGroupCreate_core0( void ) FREERTOS_SYSTEM_CALL_core0;
EventGroupHandle_t_core0 MPU_xEventGroupCreateStatic_core0( StaticEventGroup_t_core0 * pxEventGroupBuffer_core0 ) FREERTOS_SYSTEM_CALL_core0;
EventBits_t_core0 MPU_xEventGroupWaitBits_core0( EventGroupHandle_t_core0 xEventGroup_core0,
                                     const EventBits_t_core0 uxBitsToWaitFor_core0,
                                     const BaseType_t_core0 xClearOnExit_core0,
                                     const BaseType_t_core0 xWaitForAllBits_core0,
                                     TickType_t_core0 xTicksToWait_core0 ) FREERTOS_SYSTEM_CALL_core0;
EventBits_t_core0 MPU_xEventGroupClearBits_core0( EventGroupHandle_t_core0 xEventGroup_core0,
                                      const EventBits_t_core0 uxBitsToClear_core0 ) FREERTOS_SYSTEM_CALL_core0;
EventBits_t_core0 MPU_xEventGroupSetBits_core0( EventGroupHandle_t_core0 xEventGroup_core0,
                                    const EventBits_t_core0 uxBitsToSet_core0 ) FREERTOS_SYSTEM_CALL_core0;
EventBits_t_core0 MPU_xEventGroupSync_core0( EventGroupHandle_t_core0 xEventGroup_core0,
                                 const EventBits_t_core0 uxBitsToSet_core0,
                                 const EventBits_t_core0 uxBitsToWaitFor_core0,
                                 TickType_t_core0 xTicksToWait_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vEventGroupDelete_core0( EventGroupHandle_t_core0 xEventGroup_core0 ) FREERTOS_SYSTEM_CALL_core0;
UBaseType_t_core0 MPU_uxEventGroupGetNumber_core0( void * xEventGroup_core0 ) FREERTOS_SYSTEM_CALL_core0;

/* MPU versions of message/stream_buffer_core0.h API functions. */
size_t MPU_xStreamBufferSend_core0( StreamBufferHandle_t_core0 xStreamBuffer_core0,
                              const void * pvTxData_core0,
                              size_t xDataLengthBytes_core0,
                              TickType_t_core0 xTicksToWait_core0 ) FREERTOS_SYSTEM_CALL_core0;
size_t MPU_xStreamBufferReceive_core0( StreamBufferHandle_t_core0 xStreamBuffer_core0,
                                 void * pvRxData_core0,
                                 size_t xBufferLengthBytes_core0,
                                 TickType_t_core0 xTicksToWait_core0 ) FREERTOS_SYSTEM_CALL_core0;
size_t MPU_xStreamBufferNextMessageLengthBytes_core0( StreamBufferHandle_t_core0 xStreamBuffer_core0 ) FREERTOS_SYSTEM_CALL_core0;
void MPU_vStreamBufferDelete_core0( StreamBufferHandle_t_core0 xStreamBuffer_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xStreamBufferIsFull_core0( StreamBufferHandle_t_core0 xStreamBuffer_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xStreamBufferIsEmpty_core0( StreamBufferHandle_t_core0 xStreamBuffer_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xStreamBufferReset_core0( StreamBufferHandle_t_core0 xStreamBuffer_core0 ) FREERTOS_SYSTEM_CALL_core0;
size_t MPU_xStreamBufferSpacesAvailable_core0( StreamBufferHandle_t_core0 xStreamBuffer_core0 ) FREERTOS_SYSTEM_CALL_core0;
size_t MPU_xStreamBufferBytesAvailable_core0( StreamBufferHandle_t_core0 xStreamBuffer_core0 ) FREERTOS_SYSTEM_CALL_core0;
BaseType_t_core0 MPU_xStreamBufferSetTriggerLevel_core0( StreamBufferHandle_t_core0 xStreamBuffer_core0,
                                             size_t xTriggerLevel_core0 ) FREERTOS_SYSTEM_CALL_core0;
StreamBufferHandle_t_core0 MPU_xStreamBufferGenericCreate_core0( size_t xBufferSizeBytes_core0,
                                                     size_t xTriggerLevelBytes_core0,
                                                     BaseType_t_core0 xIsMessageBuffer_core0,
                                                     StreamBufferCallbackFunction_t_core0 pxSendCompletedCallback_core0,
                                                     StreamBufferCallbackFunction_t_core0 pxReceiveCompletedCallback_core0 ) FREERTOS_SYSTEM_CALL_core0;
StreamBufferHandle_t_core0 MPU_xStreamBufferGenericCreateStatic_core0( size_t xBufferSizeBytes_core0,
                                                           size_t xTriggerLevelBytes_core0,
                                                           BaseType_t_core0 xIsMessageBuffer_core0,
                                                           uint8_t * const pucStreamBufferStorageArea_core0,
                                                           StaticStreamBuffer_t_core0 * const pxStaticStreamBuffer_core0,
                                                           StreamBufferCallbackFunction_t_core0 pxSendCompletedCallback_core0,
                                                           StreamBufferCallbackFunction_t_core0 pxReceiveCompletedCallback_core0 ) FREERTOS_SYSTEM_CALL_core0;



#endif /* MPU_PROTOTYPES_H */
