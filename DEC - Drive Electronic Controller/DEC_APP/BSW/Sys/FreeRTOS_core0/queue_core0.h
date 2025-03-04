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


#ifndef QUEUE_CORE0_H
#define QUEUE_CORE0_H

#ifndef INC_FREERTOS_CORE0_H
    #error "include FreeRTOS_core0.h" must appear in source files before "include queue.h"
#endif

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

#include "task_core0.h"

/**
 * Type by which queues are referenced.  For example, a call to xQueueCreate_core0()
 * returns an QueueHandle_t_core0 variable that can then be used as a parameter to
 * xQueueSend_core0(), xQueueReceive_core0(), etc.
 */
struct QueueDefinition; /* Using old naming convention so as not to break kernel aware debuggers. */
typedef struct QueueDefinition_core0   * QueueHandle_t_core0;

/**
 * Type by which queue sets are referenced.  For example, a call to
 * xQueueCreateSet_core0() returns an xQueueSet_core0 variable that can then be used as a
 * parameter to xQueueSelectFromSet_core0(), xQueueAddToSet_core0(), etc.
 */
typedef struct QueueDefinition_core0   * QueueSetHandle_t_core0;

/**
 * Queue sets can contain both queues and semaphores, so the
 * QueueSetMemberHandle_t_core0 is defined as a type to be used where a parameter or
 * return value can be either an QueueHandle_t_core0 or an SemaphoreHandle_t.
 */
typedef struct QueueDefinition_core0   * QueueSetMemberHandle_t_core0;

/* For internal use only. */
#define queueSEND_TO_BACK_core0                     ( ( BaseType_t_core0 ) 0 )
#define queueSEND_TO_FRONT_core0                    ( ( BaseType_t_core0 ) 1 )
#define queueOVERWRITE_core0                        ( ( BaseType_t_core0 ) 2 )

/* For internal use only.  These definitions *must* match those in queue.c. */
#define queueQUEUE_TYPE_BASE_core0                  ( ( uint8_t ) 0U )
#define queueQUEUE_TYPE_SET_core0                   ( ( uint8_t ) 0U )
#define queueQUEUE_TYPE_MUTEX_core0                 ( ( uint8_t ) 1U )
#define queueQUEUE_TYPE_COUNTING_SEMAPHORE_core0    ( ( uint8_t ) 2U )
#define queueQUEUE_TYPE_BINARY_SEMAPHORE_core0      ( ( uint8_t ) 3U )
#define queueQUEUE_TYPE_RECURSIVE_MUTEX_core0       ( ( uint8_t ) 4U )

/**
 * queue. h
 * @code{c}
 * QueueHandle_t_core0 xQueueCreate_core0(
 *                            UBaseType_t_core0 uxQueueLength_core0,
 *                            UBaseType_t_core0 uxItemSize_core0
 *                        );
 * @endcode
 *
 * Creates a new queue instance, and returns a handle by which the new queue
 * can be referenced.
 *
 * Internally, within the FreeRTOS_core0 implementation, queues use two blocks of
 * memory.  The first block is used to hold the queue's data structures.  The
 * second block is used to hold items placed into the queue.  If a queue is
 * created using xQueueCreate_core0() then both blocks of memory are automatically
 * dynamically allocated inside the xQueueCreate_core0() function.  (see
 * https://www.FreeRTOS_core0.org/a00111.html).  If a queue is created using
 * xQueueCreateStatic_core0() then the application writer must provide the memory that
 * will get used by the queue.  xQueueCreateStatic_core0() therefore allows a queue to
 * be created without using any dynamic memory allocation.
 *
 * https://www.FreeRTOS_core0.org/Embedded-RTOS-Queues.html
 *
 * @param uxQueueLength_core0 The maximum number of items that the queue can contain.
 *
 * @param uxItemSize_core0 The number of bytes each item in the queue will require.
 * Items are queued by copy, not by reference, so this is the number of bytes
 * that will be copied for each posted item.  Each item on the queue must be
 * the same size.
 *
 * @return If the queue is successfully create then a handle to the newly
 * created queue is returned.  If the queue cannot be created then 0 is
 * returned.
 *
 * Example usage:
 * @code{c}
 * struct AMessage
 * {
 *  char ucMessageID;
 *  char ucData[ 20 ];
 * };
 *
 * void vATask( void *pvParameters_core0 )
 * {
 * QueueHandle_t_core0 xQueue1, xQueue2;
 *
 *  // Create a queue capable of containing 10 uint32_t values.
 *  xQueue1 = xQueueCreate_core0( 10, sizeof( uint32_t ) );
 *  if( xQueue1 == 0 )
 *  {
 *      // Queue was not created and must not be used.
 *  }
 *
 *  // Create a queue capable of containing 10 pointers to AMessage structures.
 *  // These should be passed by pointer as they contain a lot of data.
 *  xQueue2 = xQueueCreate_core0( 10, sizeof( struct AMessage * ) );
 *  if( xQueue2 == 0 )
 *  {
 *      // Queue was not created and must not be used.
 *  }
 *
 *  // ... Rest of task_core0 code.
 * }
 * @endcode
 * \defgroup xQueueCreate_core0 xQueueCreate_core0
 * \ingroup QueueManagement
 */
#if ( configSUPPORT_DYNAMIC_ALLOCATION_core0 == 1 )
    #define xQueueCreate_core0( uxQueueLength_core0, uxItemSize_core0 )    xQueueGenericCreate_core0( ( uxQueueLength_core0 ), ( uxItemSize_core0 ), ( queueQUEUE_TYPE_BASE_core0 ) )
#endif

/**
 * queue. h
 * @code{c}
 * QueueHandle_t_core0 xQueueCreateStatic_core0(
 *                            UBaseType_t_core0 uxQueueLength_core0,
 *                            UBaseType_t_core0 uxItemSize_core0,
 *                            uint8_t *pucQueueStorage_core0,
 *                            StaticQueue_t_core0 *pxQueueBuffer
 *                        );
 * @endcode
 *
 * Creates a new queue instance, and returns a handle by which the new queue
 * can be referenced.
 *
 * Internally, within the FreeRTOS_core0 implementation, queues use two blocks of
 * memory.  The first block is used to hold the queue's data structures.  The
 * second block is used to hold items placed into the queue.  If a queue is
 * created using xQueueCreate_core0() then both blocks of memory are automatically
 * dynamically allocated inside the xQueueCreate_core0() function.  (see
 * https://www.FreeRTOS_core0.org/a00111.html).  If a queue is created using
 * xQueueCreateStatic_core0() then the application writer must provide the memory that
 * will get used by the queue.  xQueueCreateStatic_core0() therefore allows a queue to
 * be created without using any dynamic memory allocation.
 *
 * https://www.FreeRTOS_core0.org/Embedded-RTOS-Queues.html
 *
 * @param uxQueueLength_core0 The maximum number of items that the queue can contain.
 *
 * @param uxItemSize_core0 The number of bytes each item in the queue will require.
 * Items are queued by copy, not by reference, so this is the number of bytes
 * that will be copied for each posted item.  Each item on the queue must be
 * the same size.
 *
 * @param pucQueueStorage_core0 If uxItemSize_core0 is not zero then
 * pucQueueStorage_core0 must point to a uint8_t array that is at least large
 * enough to hold the maximum number of items that can be in the queue at any
 * one time - which is ( uxQueueLength_core0 * uxItemsSize ) bytes.  If uxItemSize_core0 is
 * zero then pucQueueStorage_core0 can be NULL.
 *
 * @param pxQueueBuffer Must point to a variable of type StaticQueue_t_core0, which
 * will be used to hold the queue's data structure.
 *
 * @return If the queue is created then a handle to the created queue is
 * returned.  If pxQueueBuffer is NULL then NULL is returned.
 *
 * Example usage:
 * @code{c}
 * struct AMessage
 * {
 *  char ucMessageID;
 *  char ucData[ 20 ];
 * };
 *
 #define QUEUE_LENGTH 10
 #define ITEM_SIZE sizeof( uint32_t )
 *
 * // xQueueBuffer will hold the queue structure.
 * StaticQueue_t_core0 xQueueBuffer;
 *
 * // ucQueueStorage will hold the items posted to the queue.  Must be at least
 * // [(queue length) * ( queue item size)] bytes long.
 * uint8_t ucQueueStorage[ QUEUE_LENGTH * ITEM_SIZE ];
 *
 * void vATask( void *pvParameters_core0 )
 * {
 *  QueueHandle_t_core0 xQueue1;
 *
 *  // Create a queue capable of containing 10 uint32_t values.
 *  xQueue1 = xQueueCreate_core0( QUEUE_LENGTH, // The number of items the queue can hold.
 *                          ITEM_SIZE     // The size of each item in the queue
 *                          &( ucQueueStorage[ 0 ] ), // The buffer that will hold the items in the queue.
 *                          &xQueueBuffer ); // The buffer that will hold the queue structure.
 *
 *  // The queue is guaranteed to be created successfully as no dynamic memory
 *  // allocation is used.  Therefore xQueue1 is now a handle to a valid queue.
 *
 *  // ... Rest of task_core0 code.
 * }
 * @endcode
 * \defgroup xQueueCreateStatic_core0 xQueueCreateStatic_core0
 * \ingroup QueueManagement
 */
#if ( configSUPPORT_STATIC_ALLOCATION_core0 == 1 )
    #define xQueueCreateStatic_core0( uxQueueLength_core0, uxItemSize_core0, pucQueueStorage_core0, pxQueueBuffer_core0 )    xQueueGenericCreateStatic_core0( ( uxQueueLength_core0 ), ( uxItemSize_core0 ), ( pucQueueStorage_core0 ), ( pxQueueBuffer_core0 ), ( queueQUEUE_TYPE_BASE_core0 ) )
#endif /* configSUPPORT_STATIC_ALLOCATION_core0 */

/**
 * queue. h
 * @code{c}
 * BaseType_t_core0 xQueueSendToToFront(
 *                                 QueueHandle_t_core0    xQueue_core0,
 *                                 const void       *pvItemToQueue_core0,
 *                                 TickType_t_core0       xTicksToWait_core0
 *                             );
 * @endcode
 *
 * Post an item to the front of a queue.  The item is queued by copy, not by
 * reference.  This function must not be called from an interrupt service
 * routine.  See xQueueSendFromISR_core0 () for an alternative which may be used
 * in an ISR.
 *
 * @param xQueue_core0 The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue_core0 A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue_core0
 * into the queue storage area.
 *
 * @param xTicksToWait_core0 The maximum amount of time the task_core0 should block
 * waiting for space to become available on the queue, should it already
 * be full.  The call will return immediately if this is set to 0 and the
 * queue is full.  The time is defined in tick periods so the constant
 * portTICK_PERIOD_MS_core0 should be used to convert to real time if this is required.
 *
 * @return pdTRUE_core0 if the item was successfully posted, otherwise errQUEUE_FULL_core0.
 *
 * Example usage:
 * @code{c}
 * struct AMessage
 * {
 *  char ucMessageID;
 *  char ucData[ 20 ];
 * } xMessage_core0;
 *
 * uint32_t ulVar = 10UL;
 *
 * void vATask( void *pvParameters_core0 )
 * {
 * QueueHandle_t_core0 xQueue1, xQueue2;
 * struct AMessage *pxMessage;
 *
 *  // Create a queue capable of containing 10 uint32_t values.
 *  xQueue1 = xQueueCreate_core0( 10, sizeof( uint32_t ) );
 *
 *  // Create a queue capable of containing 10 pointers to AMessage structures.
 *  // These should be passed by pointer as they contain a lot of data.
 *  xQueue2 = xQueueCreate_core0( 10, sizeof( struct AMessage * ) );
 *
 *  // ...
 *
 *  if( xQueue1 != 0 )
 *  {
 *      // Send an uint32_t.  Wait for 10 ticks for space to become
 *      // available if necessary.
 *      if( xQueueSendToFront_core0( xQueue1, ( void * ) &ulVar, ( TickType_t_core0 ) 10 ) != pdPASS_core0 )
 *      {
 *          // Failed to post the message, even after 10 ticks.
 *      }
 *  }
 *
 *  if( xQueue2 != 0 )
 *  {
 *      // Send a pointer to a struct AMessage object.  Don't block if the
 *      // queue is already full.
 *      pxMessage = & xMessage_core0;
 *      xQueueSendToFront_core0( xQueue2, ( void * ) &pxMessage, ( TickType_t_core0 ) 0 );
 *  }
 *
 *  // ... Rest of task_core0 code.
 * }
 * @endcode
 * \defgroup xQueueSend_core0 xQueueSend_core0
 * \ingroup QueueManagement
 */
#define xQueueSendToFront_core0( xQueue_core0, pvItemToQueue_core0, xTicksToWait_core0 ) \
    xQueueGenericSend_core0( ( xQueue_core0 ), ( pvItemToQueue_core0 ), ( xTicksToWait_core0 ), queueSEND_TO_FRONT_core0 )

/**
 * queue. h
 * @code{c}
 * BaseType_t_core0 xQueueSendToBack_core0(
 *                                 QueueHandle_t_core0    xQueue_core0,
 *                                 const void       *pvItemToQueue_core0,
 *                                 TickType_t_core0       xTicksToWait_core0
 *                             );
 * @endcode
 *
 * This is a macro that calls xQueueGenericSend_core0().
 *
 * Post an item to the back of a queue.  The item is queued by copy, not by
 * reference.  This function must not be called from an interrupt service
 * routine.  See xQueueSendFromISR_core0 () for an alternative which may be used
 * in an ISR.
 *
 * @param xQueue_core0 The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue_core0 A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue_core0
 * into the queue storage area.
 *
 * @param xTicksToWait_core0 The maximum amount of time the task_core0 should block
 * waiting for space to become available on the queue, should it already
 * be full.  The call will return immediately if this is set to 0 and the queue
 * is full.  The  time is defined in tick periods so the constant
 * portTICK_PERIOD_MS_core0 should be used to convert to real time if this is required.
 *
 * @return pdTRUE_core0 if the item was successfully posted, otherwise errQUEUE_FULL_core0.
 *
 * Example usage:
 * @code{c}
 * struct AMessage
 * {
 *  char ucMessageID;
 *  char ucData[ 20 ];
 * } xMessage_core0;
 *
 * uint32_t ulVar = 10UL;
 *
 * void vATask( void *pvParameters_core0 )
 * {
 * QueueHandle_t_core0 xQueue1, xQueue2;
 * struct AMessage *pxMessage;
 *
 *  // Create a queue capable of containing 10 uint32_t values.
 *  xQueue1 = xQueueCreate_core0( 10, sizeof( uint32_t ) );
 *
 *  // Create a queue capable of containing 10 pointers to AMessage structures.
 *  // These should be passed by pointer as they contain a lot of data.
 *  xQueue2 = xQueueCreate_core0( 10, sizeof( struct AMessage * ) );
 *
 *  // ...
 *
 *  if( xQueue1 != 0 )
 *  {
 *      // Send an uint32_t.  Wait for 10 ticks for space to become
 *      // available if necessary.
 *      if( xQueueSendToBack_core0( xQueue1, ( void * ) &ulVar, ( TickType_t_core0 ) 10 ) != pdPASS_core0 )
 *      {
 *          // Failed to post the message, even after 10 ticks.
 *      }
 *  }
 *
 *  if( xQueue2 != 0 )
 *  {
 *      // Send a pointer to a struct AMessage object.  Don't block if the
 *      // queue is already full.
 *      pxMessage = & xMessage_core0;
 *      xQueueSendToBack_core0( xQueue2, ( void * ) &pxMessage, ( TickType_t_core0 ) 0 );
 *  }
 *
 *  // ... Rest of task_core0 code.
 * }
 * @endcode
 * \defgroup xQueueSend_core0 xQueueSend_core0
 * \ingroup QueueManagement
 */
#define xQueueSendToBack_core0( xQueue_core0, pvItemToQueue_core0, xTicksToWait_core0 ) \
    xQueueGenericSend_core0( ( xQueue_core0 ), ( pvItemToQueue_core0 ), ( xTicksToWait_core0 ), queueSEND_TO_BACK_core0 )

/**
 * queue. h
 * @code{c}
 * BaseType_t_core0 xQueueSend_core0(
 *                            QueueHandle_t_core0 xQueue_core0,
 *                            const void * pvItemToQueue_core0,
 *                            TickType_t_core0 xTicksToWait_core0
 *                       );
 * @endcode
 *
 * This is a macro that calls xQueueGenericSend_core0().  It is included for
 * backward compatibility with versions of FreeRTOS_core0.org that did not
 * include the xQueueSendToFront_core0() and xQueueSendToBack_core0() macros.  It is
 * equivalent to xQueueSendToBack_core0().
 *
 * Post an item on a queue.  The item is queued by copy, not by reference.
 * This function must not be called from an interrupt service routine.
 * See xQueueSendFromISR_core0 () for an alternative which may be used in an ISR.
 *
 * @param xQueue_core0 The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue_core0 A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue_core0
 * into the queue storage area.
 *
 * @param xTicksToWait_core0 The maximum amount of time the task_core0 should block
 * waiting for space to become available on the queue, should it already
 * be full.  The call will return immediately if this is set to 0 and the
 * queue is full.  The time is defined in tick periods so the constant
 * portTICK_PERIOD_MS_core0 should be used to convert to real time if this is required.
 *
 * @return pdTRUE_core0 if the item was successfully posted, otherwise errQUEUE_FULL_core0.
 *
 * Example usage:
 * @code{c}
 * struct AMessage
 * {
 *  char ucMessageID;
 *  char ucData[ 20 ];
 * } xMessage_core0;
 *
 * uint32_t ulVar = 10UL;
 *
 * void vATask( void *pvParameters_core0 )
 * {
 * QueueHandle_t_core0 xQueue1, xQueue2;
 * struct AMessage *pxMessage;
 *
 *  // Create a queue capable of containing 10 uint32_t values.
 *  xQueue1 = xQueueCreate_core0( 10, sizeof( uint32_t ) );
 *
 *  // Create a queue capable of containing 10 pointers to AMessage structures.
 *  // These should be passed by pointer as they contain a lot of data.
 *  xQueue2 = xQueueCreate_core0( 10, sizeof( struct AMessage * ) );
 *
 *  // ...
 *
 *  if( xQueue1 != 0 )
 *  {
 *      // Send an uint32_t.  Wait for 10 ticks for space to become
 *      // available if necessary.
 *      if( xQueueSend_core0( xQueue1, ( void * ) &ulVar, ( TickType_t_core0 ) 10 ) != pdPASS_core0 )
 *      {
 *          // Failed to post the message, even after 10 ticks.
 *      }
 *  }
 *
 *  if( xQueue2 != 0 )
 *  {
 *      // Send a pointer to a struct AMessage object.  Don't block if the
 *      // queue is already full.
 *      pxMessage = & xMessage_core0;
 *      xQueueSend_core0( xQueue2, ( void * ) &pxMessage, ( TickType_t_core0 ) 0 );
 *  }
 *
 *  // ... Rest of task_core0 code.
 * }
 * @endcode
 * \defgroup xQueueSend_core0 xQueueSend_core0
 * \ingroup QueueManagement
 */
#define xQueueSend_core0( xQueue_core0, pvItemToQueue_core0, xTicksToWait_core0 ) \
    xQueueGenericSend_core0( ( xQueue_core0 ), ( pvItemToQueue_core0 ), ( xTicksToWait_core0 ), queueSEND_TO_BACK_core0 )

/**
 * queue. h
 * @code{c}
 * BaseType_t_core0 xQueueOverwrite_core0(
 *                            QueueHandle_t_core0 xQueue_core0,
 *                            const void * pvItemToQueue_core0
 *                       );
 * @endcode
 *
 * Only for use with queues that have a length of one - so the queue is either
 * empty or full.
 *
 * Post an item on a queue.  If the queue is already full then overwrite the
 * value held in the queue.  The item is queued by copy, not by reference.
 *
 * This function must not be called from an interrupt service routine.
 * See xQueueOverwriteFromISR_core0 () for an alternative which may be used in an ISR.
 *
 * @param xQueue_core0 The handle of the queue to which the data is being sent.
 *
 * @param pvItemToQueue_core0 A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue_core0
 * into the queue storage area.
 *
 * @return xQueueOverwrite_core0() is a macro that calls xQueueGenericSend_core0(), and
 * therefore has the same return values as xQueueSendToFront_core0().  However, pdPASS_core0
 * is the only value that can be returned because xQueueOverwrite_core0() will write
 * to the queue even when the queue is already full.
 *
 * Example usage:
 * @code{c}
 *
 * void vFunction_core0( void *pvParameters_core0 )
 * {
 * QueueHandle_t_core0 xQueue_core0;
 * uint32_t ulVarToSend, ulValReceived;
 *
 *  // Create a queue to hold one uint32_t value.  It is strongly
 *  // recommended *not* to use xQueueOverwrite_core0() on queues that can
 *  // contain more than one value, and doing so will trigger an assertion
 *  // if configASSERT_core0() is defined.
 *  xQueue_core0 = xQueueCreate_core0( 1, sizeof( uint32_t ) );
 *
 *  // Write the value 10 to the queue using xQueueOverwrite_core0().
 *  ulVarToSend = 10;
 *  xQueueOverwrite_core0( xQueue_core0, &ulVarToSend );
 *
 *  // Peeking the queue should now return 10, but leave the value 10 in
 *  // the queue.  A block time of zero is used as it is known that the
 *  // queue holds a value.
 *  ulValReceived = 0;
 *  xQueuePeek_core0( xQueue_core0, &ulValReceived, 0 );
 *
 *  if( ulValReceived != 10 )
 *  {
 *      // Error unless the item was removed by a different task_core0.
 *  }
 *
 *  // The queue is still full.  Use xQueueOverwrite_core0() to overwrite the
 *  // value held in the queue with 100.
 *  ulVarToSend = 100;
 *  xQueueOverwrite_core0( xQueue_core0, &ulVarToSend );
 *
 *  // This time read from the queue, leaving the queue empty once more.
 *  // A block time of 0 is used again.
 *  xQueueReceive_core0( xQueue_core0, &ulValReceived, 0 );
 *
 *  // The value read should be the last value written, even though the
 *  // queue was already full when the value was written.
 *  if( ulValReceived != 100 )
 *  {
 *      // Error!
 *  }
 *
 *  // ...
 * }
 * @endcode
 * \defgroup xQueueOverwrite_core0 xQueueOverwrite_core0
 * \ingroup QueueManagement
 */
#define xQueueOverwrite_core0( xQueue_core0, pvItemToQueue_core0 ) \
    xQueueGenericSend_core0( ( xQueue_core0 ), ( pvItemToQueue_core0 ), 0, queueOVERWRITE_core0 )


/**
 * queue. h
 * @code{c}
 * BaseType_t_core0 xQueueGenericSend_core0(
 *                                  QueueHandle_t_core0 xQueue_core0,
 *                                  const void * pvItemToQueue_core0,
 *                                  TickType_t_core0 xTicksToWait_core0
 *                                  BaseType_t_core0 xCopyPosition_core0
 *                              );
 * @endcode
 *
 * It is preferred that the macros xQueueSend_core0(), xQueueSendToFront_core0() and
 * xQueueSendToBack_core0() are used in place of calling this function directly.
 *
 * Post an item on a queue.  The item is queued by copy, not by reference.
 * This function must not be called from an interrupt service routine.
 * See xQueueSendFromISR_core0 () for an alternative which may be used in an ISR.
 *
 * @param xQueue_core0 The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue_core0 A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue_core0
 * into the queue storage area.
 *
 * @param xTicksToWait_core0 The maximum amount of time the task_core0 should block
 * waiting for space to become available on the queue, should it already
 * be full.  The call will return immediately if this is set to 0 and the
 * queue is full.  The time is defined in tick periods so the constant
 * portTICK_PERIOD_MS_core0 should be used to convert to real time if this is required.
 *
 * @param xCopyPosition_core0 Can take the value queueSEND_TO_BACK_core0 to place the
 * item at the back of the queue, or queueSEND_TO_FRONT_core0 to place the item
 * at the front of the queue (for high priority messages).
 *
 * @return pdTRUE_core0 if the item was successfully posted, otherwise errQUEUE_FULL_core0.
 *
 * Example usage:
 * @code{c}
 * struct AMessage
 * {
 *  char ucMessageID;
 *  char ucData[ 20 ];
 * } xMessage_core0;
 *
 * uint32_t ulVar = 10UL;
 *
 * void vATask( void *pvParameters_core0 )
 * {
 * QueueHandle_t_core0 xQueue1, xQueue2;
 * struct AMessage *pxMessage;
 *
 *  // Create a queue capable of containing 10 uint32_t values.
 *  xQueue1 = xQueueCreate_core0( 10, sizeof( uint32_t ) );
 *
 *  // Create a queue capable of containing 10 pointers to AMessage structures.
 *  // These should be passed by pointer as they contain a lot of data.
 *  xQueue2 = xQueueCreate_core0( 10, sizeof( struct AMessage * ) );
 *
 *  // ...
 *
 *  if( xQueue1 != 0 )
 *  {
 *      // Send an uint32_t.  Wait for 10 ticks for space to become
 *      // available if necessary.
 *      if( xQueueGenericSend_core0( xQueue1, ( void * ) &ulVar, ( TickType_t_core0 ) 10, queueSEND_TO_BACK_core0 ) != pdPASS_core0 )
 *      {
 *          // Failed to post the message, even after 10 ticks.
 *      }
 *  }
 *
 *  if( xQueue2 != 0 )
 *  {
 *      // Send a pointer to a struct AMessage object.  Don't block if the
 *      // queue is already full.
 *      pxMessage = & xMessage_core0;
 *      xQueueGenericSend_core0( xQueue2, ( void * ) &pxMessage, ( TickType_t_core0 ) 0, queueSEND_TO_BACK_core0 );
 *  }
 *
 *  // ... Rest of task_core0 code.
 * }
 * @endcode
 * \defgroup xQueueSend_core0 xQueueSend_core0
 * \ingroup QueueManagement
 */
BaseType_t_core0 xQueueGenericSend_core0( QueueHandle_t_core0 xQueue_core0,
                              const void * const pvItemToQueue_core0,
                              TickType_t_core0 xTicksToWait_core0,
                              const BaseType_t_core0 xCopyPosition_core0 ) PRIVILEGED_FUNCTION_core0;

/**
 * queue. h
 * @code{c}
 * BaseType_t_core0 xQueuePeek_core0(
 *                           QueueHandle_t_core0 xQueue_core0,
 *                           void * const pvBuffer_core0,
 *                           TickType_t_core0 xTicksToWait_core0
 *                       );
 * @endcode
 *
 * Receive an item from a queue without removing the item from the queue.
 * The item is received by copy so a buffer of adequate size must be
 * provided.  The number of bytes copied into the buffer was defined when
 * the queue was created.
 *
 * Successfully received items remain on the queue so will be returned again
 * by the next call, or a call to xQueueReceive_core0().
 *
 * This macro must not be used in an interrupt service routine.  See
 * xQueuePeekFromISR_core0() for an alternative that can be called from an interrupt
 * service routine.
 *
 * @param xQueue_core0 The handle to the queue from which the item is to be
 * received.
 *
 * @param pvBuffer_core0 Pointer to the buffer into which the received item will
 * be copied.
 *
 * @param xTicksToWait_core0 The maximum amount of time the task_core0 should block
 * waiting for an item to receive should the queue be empty at the time
 * of the call. The time is defined in tick periods so the constant
 * portTICK_PERIOD_MS_core0 should be used to convert to real time if this is required.
 * xQueuePeek_core0() will return immediately if xTicksToWait_core0 is 0 and the queue
 * is empty.
 *
 * @return pdTRUE_core0 if an item was successfully received from the queue,
 * otherwise pdFALSE_core0.
 *
 * Example usage:
 * @code{c}
 * struct AMessage
 * {
 *  char ucMessageID;
 *  char ucData[ 20 ];
 * } xMessage_core0;
 *
 * QueueHandle_t_core0 xQueue_core0;
 *
 * // Task to create a queue and post a value.
 * void vATask( void *pvParameters_core0 )
 * {
 * struct AMessage *pxMessage;
 *
 *  // Create a queue capable of containing 10 pointers to AMessage structures.
 *  // These should be passed by pointer as they contain a lot of data.
 *  xQueue_core0 = xQueueCreate_core0( 10, sizeof( struct AMessage * ) );
 *  if( xQueue_core0 == 0 )
 *  {
 *      // Failed to create the queue.
 *  }
 *
 *  // ...
 *
 *  // Send a pointer to a struct AMessage object.  Don't block if the
 *  // queue is already full.
 *  pxMessage = & xMessage_core0;
 *  xQueueSend_core0( xQueue_core0, ( void * ) &pxMessage, ( TickType_t_core0 ) 0 );
 *
 *  // ... Rest of task_core0 code.
 * }
 *
 * // Task to peek the data from the queue.
 * void vADifferentTask( void *pvParameters_core0 )
 * {
 * struct AMessage *pxRxedMessage;
 *
 *  if( xQueue_core0 != 0 )
 *  {
 *      // Peek a message on the created queue.  Block for 10 ticks if a
 *      // message is not immediately available.
 *      if( xQueuePeek_core0( xQueue_core0, &( pxRxedMessage ), ( TickType_t_core0 ) 10 ) )
 *      {
 *          // pcRxedMessage now points to the struct AMessage variable posted
 *          // by vATask, but the item still remains on the queue.
 *      }
 *  }
 *
 *  // ... Rest of task_core0 code.
 * }
 * @endcode
 * \defgroup xQueuePeek_core0 xQueuePeek_core0
 * \ingroup QueueManagement
 */
BaseType_t_core0 xQueuePeek_core0( QueueHandle_t_core0 xQueue_core0,
                       void * const pvBuffer_core0,
                       TickType_t_core0 xTicksToWait_core0 ) PRIVILEGED_FUNCTION_core0;

/**
 * queue. h
 * @code{c}
 * BaseType_t_core0 xQueuePeekFromISR_core0(
 *                                  QueueHandle_t_core0 xQueue_core0,
 *                                  void *pvBuffer_core0,
 *                              );
 * @endcode
 *
 * A version of xQueuePeek_core0() that can be called from an interrupt service
 * routine (ISR).
 *
 * Receive an item from a queue without removing the item from the queue.
 * The item is received by copy so a buffer of adequate size must be
 * provided.  The number of bytes copied into the buffer was defined when
 * the queue was created.
 *
 * Successfully received items remain on the queue so will be returned again
 * by the next call, or a call to xQueueReceive_core0().
 *
 * @param xQueue_core0 The handle to the queue from which the item is to be
 * received.
 *
 * @param pvBuffer_core0 Pointer to the buffer into which the received item will
 * be copied.
 *
 * @return pdTRUE_core0 if an item was successfully received from the queue,
 * otherwise pdFALSE_core0.
 *
 * \defgroup xQueuePeekFromISR_core0 xQueuePeekFromISR_core0
 * \ingroup QueueManagement
 */
BaseType_t_core0 xQueuePeekFromISR_core0( QueueHandle_t_core0 xQueue_core0,
                              void * const pvBuffer_core0 ) PRIVILEGED_FUNCTION_core0;

/**
 * queue. h
 * @code{c}
 * BaseType_t_core0 xQueueReceive_core0(
 *                               QueueHandle_t_core0 xQueue_core0,
 *                               void *pvBuffer_core0,
 *                               TickType_t_core0 xTicksToWait_core0
 *                          );
 * @endcode
 *
 * Receive an item from a queue.  The item is received by copy so a buffer of
 * adequate size must be provided.  The number of bytes copied into the buffer
 * was defined when the queue was created.
 *
 * Successfully received items are removed from the queue.
 *
 * This function must not be used in an interrupt service routine.  See
 * xQueueReceiveFromISR_core0 for an alternative that can.
 *
 * @param xQueue_core0 The handle to the queue from which the item is to be
 * received.
 *
 * @param pvBuffer_core0 Pointer to the buffer into which the received item will
 * be copied.
 *
 * @param xTicksToWait_core0 The maximum amount of time the task_core0 should block
 * waiting for an item to receive should the queue be empty at the time
 * of the call. xQueueReceive_core0() will return immediately if xTicksToWait_core0
 * is zero and the queue is empty.  The time is defined in tick periods so the
 * constant portTICK_PERIOD_MS_core0 should be used to convert to real time if this is
 * required.
 *
 * @return pdTRUE_core0 if an item was successfully received from the queue,
 * otherwise pdFALSE_core0.
 *
 * Example usage:
 * @code{c}
 * struct AMessage
 * {
 *  char ucMessageID;
 *  char ucData[ 20 ];
 * } xMessage_core0;
 *
 * QueueHandle_t_core0 xQueue_core0;
 *
 * // Task to create a queue and post a value.
 * void vATask( void *pvParameters_core0 )
 * {
 * struct AMessage *pxMessage;
 *
 *  // Create a queue capable of containing 10 pointers to AMessage structures.
 *  // These should be passed by pointer as they contain a lot of data.
 *  xQueue_core0 = xQueueCreate_core0( 10, sizeof( struct AMessage * ) );
 *  if( xQueue_core0 == 0 )
 *  {
 *      // Failed to create the queue.
 *  }
 *
 *  // ...
 *
 *  // Send a pointer to a struct AMessage object.  Don't block if the
 *  // queue is already full.
 *  pxMessage = & xMessage_core0;
 *  xQueueSend_core0( xQueue_core0, ( void * ) &pxMessage, ( TickType_t_core0 ) 0 );
 *
 *  // ... Rest of task_core0 code.
 * }
 *
 * // Task to receive from the queue.
 * void vADifferentTask( void *pvParameters_core0 )
 * {
 * struct AMessage *pxRxedMessage;
 *
 *  if( xQueue_core0 != 0 )
 *  {
 *      // Receive a message on the created queue.  Block for 10 ticks if a
 *      // message is not immediately available.
 *      if( xQueueReceive_core0( xQueue_core0, &( pxRxedMessage ), ( TickType_t_core0 ) 10 ) )
 *      {
 *          // pcRxedMessage now points to the struct AMessage variable posted
 *          // by vATask.
 *      }
 *  }
 *
 *  // ... Rest of task_core0 code.
 * }
 * @endcode
 * \defgroup xQueueReceive_core0 xQueueReceive_core0
 * \ingroup QueueManagement
 */
BaseType_t_core0 xQueueReceive_core0( QueueHandle_t_core0 xQueue_core0,
                          void * const pvBuffer_core0,
                          TickType_t_core0 xTicksToWait_core0 ) PRIVILEGED_FUNCTION_core0;

/**
 * queue. h
 * @code{c}
 * UBaseType_t_core0 uxQueueMessagesWaiting_core0( const QueueHandle_t_core0 xQueue_core0 );
 * @endcode
 *
 * Return the number of messages stored in a queue.
 *
 * @param xQueue_core0 A handle to the queue being queried.
 *
 * @return The number of messages available in the queue.
 *
 * \defgroup uxQueueMessagesWaiting_core0 uxQueueMessagesWaiting_core0
 * \ingroup QueueManagement
 */
UBaseType_t_core0 uxQueueMessagesWaiting_core0( const QueueHandle_t_core0 xQueue_core0 ) PRIVILEGED_FUNCTION_core0;

/**
 * queue. h
 * @code{c}
 * UBaseType_t_core0 uxQueueSpacesAvailable_core0( const QueueHandle_t_core0 xQueue_core0 );
 * @endcode
 *
 * Return the number of free spaces available in a queue.  This is equal to the
 * number of items that can be sent to the queue before the queue becomes full
 * if no items are removed.
 *
 * @param xQueue_core0 A handle to the queue being queried.
 *
 * @return The number of spaces available in the queue.
 *
 * \defgroup uxQueueMessagesWaiting_core0 uxQueueMessagesWaiting_core0
 * \ingroup QueueManagement
 */
UBaseType_t_core0 uxQueueSpacesAvailable_core0( const QueueHandle_t_core0 xQueue_core0 ) PRIVILEGED_FUNCTION_core0;

/**
 * queue. h
 * @code{c}
 * void vQueueDelete_core0( QueueHandle_t_core0 xQueue_core0 );
 * @endcode
 *
 * Delete a queue - freeing all the memory allocated for storing of items
 * placed on the queue.
 *
 * @param xQueue_core0 A handle to the queue to be deleted.
 *
 * \defgroup vQueueDelete_core0 vQueueDelete_core0
 * \ingroup QueueManagement
 */
void vQueueDelete_core0( QueueHandle_t_core0 xQueue_core0 ) PRIVILEGED_FUNCTION_core0;

/**
 * queue. h
 * @code{c}
 * BaseType_t_core0 xQueueSendToFrontFromISR_core0(
 *                                       QueueHandle_t_core0 xQueue_core0,
 *                                       const void *pvItemToQueue_core0,
 *                                       BaseType_t_core0 *pxHigherPriorityTaskWoken_core0
 *                                    );
 * @endcode
 *
 * This is a macro that calls xQueueGenericSendFromISR_core0().
 *
 * Post an item to the front of a queue.  It is safe to use this macro from
 * within an interrupt service routine.
 *
 * Items are queued by copy not reference so it is preferable to only
 * queue small items, especially when called from an ISR.  In most cases
 * it would be preferable to store a pointer to the item being queued.
 *
 * @param xQueue_core0 The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue_core0 A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue_core0
 * into the queue storage area.
 *
 * @param pxHigherPriorityTaskWoken_core0 xQueueSendToFrontFromISR_core0() will set
 * *pxHigherPriorityTaskWoken_core0 to pdTRUE_core0 if sending to the queue caused a task_core0
 * to unblock, and the unblocked task_core0 has a priority higher than the currently
 * running task_core0.  If xQueueSendToFromFromISR() sets this value to pdTRUE_core0 then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE_core0 if the data was successfully sent to the queue, otherwise
 * errQUEUE_FULL_core0.
 *
 * Example usage for buffered IO (where the ISR can obtain more than one value
 * per call):
 * @code{c}
 * void vBufferISR( void )
 * {
 * char cIn;
 * BaseType_t_core0 xHigherPriorityTaskWoken_core0;
 *
 *  // We have not woken a task_core0 at the start of the ISR.
 *  xHigherPriorityTaskWoken_core0 = pdFALSE_core0;
 *
 *  // Loop until the buffer is empty.
 *  do
 *  {
 *      // Obtain a byte from the buffer.
 *      cIn = portINPUT_BYTE( RX_REGISTER_ADDRESS );
 *
 *      // Post the byte.
 *      xQueueSendToFrontFromISR_core0( xRxQueue, &cIn, &xHigherPriorityTaskWoken_core0 );
 *
 *  } while( portINPUT_BYTE( BUFFER_COUNT ) );
 *
 *  // Now the buffer is empty we can switch context if necessary.
 *  if( xHigherPriorityTaskWoken_core0 )
 *  {
 *      taskYIELD ();
 *  }
 * }
 * @endcode
 *
 * \defgroup xQueueSendFromISR_core0 xQueueSendFromISR_core0
 * \ingroup QueueManagement
 */
#define xQueueSendToFrontFromISR_core0( xQueue_core0, pvItemToQueue_core0, pxHigherPriorityTaskWoken_core0 ) \
    xQueueGenericSendFromISR_core0( ( xQueue_core0 ), ( pvItemToQueue_core0 ), ( pxHigherPriorityTaskWoken_core0 ), queueSEND_TO_FRONT_core0 )


/**
 * queue. h
 * @code{c}
 * BaseType_t_core0 xQueueSendToBackFromISR_core0(
 *                                       QueueHandle_t_core0 xQueue_core0,
 *                                       const void *pvItemToQueue_core0,
 *                                       BaseType_t_core0 *pxHigherPriorityTaskWoken_core0
 *                                    );
 * @endcode
 *
 * This is a macro that calls xQueueGenericSendFromISR_core0().
 *
 * Post an item to the back of a queue.  It is safe to use this macro from
 * within an interrupt service routine.
 *
 * Items are queued by copy not reference so it is preferable to only
 * queue small items, especially when called from an ISR.  In most cases
 * it would be preferable to store a pointer to the item being queued.
 *
 * @param xQueue_core0 The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue_core0 A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue_core0
 * into the queue storage area.
 *
 * @param pxHigherPriorityTaskWoken_core0 xQueueSendToBackFromISR_core0() will set
 * *pxHigherPriorityTaskWoken_core0 to pdTRUE_core0 if sending to the queue caused a task_core0
 * to unblock, and the unblocked task_core0 has a priority higher than the currently
 * running task_core0.  If xQueueSendToBackFromISR_core0() sets this value to pdTRUE_core0 then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE_core0 if the data was successfully sent to the queue, otherwise
 * errQUEUE_FULL_core0.
 *
 * Example usage for buffered IO (where the ISR can obtain more than one value
 * per call):
 * @code{c}
 * void vBufferISR( void )
 * {
 * char cIn;
 * BaseType_t_core0 xHigherPriorityTaskWoken_core0;
 *
 *  // We have not woken a task_core0 at the start of the ISR.
 *  xHigherPriorityTaskWoken_core0 = pdFALSE_core0;
 *
 *  // Loop until the buffer is empty.
 *  do
 *  {
 *      // Obtain a byte from the buffer.
 *      cIn = portINPUT_BYTE( RX_REGISTER_ADDRESS );
 *
 *      // Post the byte.
 *      xQueueSendToBackFromISR_core0( xRxQueue, &cIn, &xHigherPriorityTaskWoken_core0 );
 *
 *  } while( portINPUT_BYTE( BUFFER_COUNT ) );
 *
 *  // Now the buffer is empty we can switch context if necessary.
 *  if( xHigherPriorityTaskWoken_core0 )
 *  {
 *      taskYIELD ();
 *  }
 * }
 * @endcode
 *
 * \defgroup xQueueSendFromISR_core0 xQueueSendFromISR_core0
 * \ingroup QueueManagement
 */
#define xQueueSendToBackFromISR_core0( xQueue_core0, pvItemToQueue_core0, pxHigherPriorityTaskWoken_core0 ) \
    xQueueGenericSendFromISR_core0( ( xQueue_core0 ), ( pvItemToQueue_core0 ), ( pxHigherPriorityTaskWoken_core0 ), queueSEND_TO_BACK_core0 )

/**
 * queue. h
 * @code{c}
 * BaseType_t_core0 xQueueOverwriteFromISR_core0(
 *                            QueueHandle_t_core0 xQueue_core0,
 *                            const void * pvItemToQueue_core0,
 *                            BaseType_t_core0 *pxHigherPriorityTaskWoken_core0
 *                       );
 * @endcode
 *
 * A version of xQueueOverwrite_core0() that can be used in an interrupt service
 * routine (ISR).
 *
 * Only for use with queues that can hold a single item - so the queue is either
 * empty or full.
 *
 * Post an item on a queue.  If the queue is already full then overwrite the
 * value held in the queue.  The item is queued by copy, not by reference.
 *
 * @param xQueue_core0 The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue_core0 A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue_core0
 * into the queue storage area.
 *
 * @param pxHigherPriorityTaskWoken_core0 xQueueOverwriteFromISR_core0() will set
 * *pxHigherPriorityTaskWoken_core0 to pdTRUE_core0 if sending to the queue caused a task_core0
 * to unblock, and the unblocked task_core0 has a priority higher than the currently
 * running task_core0.  If xQueueOverwriteFromISR_core0() sets this value to pdTRUE_core0 then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return xQueueOverwriteFromISR_core0() is a macro that calls
 * xQueueGenericSendFromISR_core0(), and therefore has the same return values as
 * xQueueSendToFrontFromISR_core0().  However, pdPASS_core0 is the only value that can be
 * returned because xQueueOverwriteFromISR_core0() will write to the queue even when
 * the queue is already full.
 *
 * Example usage:
 * @code{c}
 *
 * QueueHandle_t_core0 xQueue_core0;
 *
 * void vFunction_core0( void *pvParameters_core0 )
 * {
 *  // Create a queue to hold one uint32_t value.  It is strongly
 *  // recommended *not* to use xQueueOverwriteFromISR_core0() on queues that can
 *  // contain more than one value, and doing so will trigger an assertion
 *  // if configASSERT_core0() is defined.
 *  xQueue_core0 = xQueueCreate_core0( 1, sizeof( uint32_t ) );
 * }
 *
 * void vAnInterruptHandler( void )
 * {
 * // xHigherPriorityTaskWoken_core0 must be set to pdFALSE_core0 before it is used.
 * BaseType_t_core0 xHigherPriorityTaskWoken_core0 = pdFALSE_core0;
 * uint32_t ulVarToSend, ulValReceived;
 *
 *  // Write the value 10 to the queue using xQueueOverwriteFromISR_core0().
 *  ulVarToSend = 10;
 *  xQueueOverwriteFromISR_core0( xQueue_core0, &ulVarToSend, &xHigherPriorityTaskWoken_core0 );
 *
 *  // The queue is full, but calling xQueueOverwriteFromISR_core0() again will still
 *  // pass because the value held in the queue will be overwritten with the
 *  // new value.
 *  ulVarToSend = 100;
 *  xQueueOverwriteFromISR_core0( xQueue_core0, &ulVarToSend, &xHigherPriorityTaskWoken_core0 );
 *
 *  // Reading from the queue will now return 100.
 *
 *  // ...
 *
 *  if( xHigherPrioritytaskWoken == pdTRUE_core0 )
 *  {
 *      // Writing to the queue caused a task_core0 to unblock and the unblocked task_core0
 *      // has a priority higher than or equal to the priority of the currently
 *      // executing task_core0 (the task_core0 this interrupt interrupted).  Perform a context
 *      // switch so this interrupt returns directly to the unblocked task_core0.
 *      portYIELD_FROM_ISR_core0(); // or portEND_SWITCHING_ISR() depending on the port.
 *  }
 * }
 * @endcode
 * \defgroup xQueueOverwriteFromISR_core0 xQueueOverwriteFromISR_core0
 * \ingroup QueueManagement
 */
#define xQueueOverwriteFromISR_core0( xQueue_core0, pvItemToQueue_core0, pxHigherPriorityTaskWoken_core0 ) \
    xQueueGenericSendFromISR_core0( ( xQueue_core0 ), ( pvItemToQueue_core0 ), ( pxHigherPriorityTaskWoken_core0 ), queueOVERWRITE_core0 )

/**
 * queue. h
 * @code{c}
 * BaseType_t_core0 xQueueSendFromISR_core0(
 *                                   QueueHandle_t_core0 xQueue_core0,
 *                                   const void *pvItemToQueue_core0,
 *                                   BaseType_t_core0 *pxHigherPriorityTaskWoken_core0
 *                              );
 * @endcode
 *
 * This is a macro that calls xQueueGenericSendFromISR_core0().  It is included
 * for backward compatibility with versions of FreeRTOS_core0.org that did not
 * include the xQueueSendToBackFromISR_core0() and xQueueSendToFrontFromISR_core0()
 * macros.
 *
 * Post an item to the back of a queue.  It is safe to use this function from
 * within an interrupt service routine.
 *
 * Items are queued by copy not reference so it is preferable to only
 * queue small items, especially when called from an ISR.  In most cases
 * it would be preferable to store a pointer to the item being queued.
 *
 * @param xQueue_core0 The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue_core0 A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue_core0
 * into the queue storage area.
 *
 * @param pxHigherPriorityTaskWoken_core0 xQueueSendFromISR_core0() will set
 * *pxHigherPriorityTaskWoken_core0 to pdTRUE_core0 if sending to the queue caused a task_core0
 * to unblock, and the unblocked task_core0 has a priority higher than the currently
 * running task_core0.  If xQueueSendFromISR_core0() sets this value to pdTRUE_core0 then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE_core0 if the data was successfully sent to the queue, otherwise
 * errQUEUE_FULL_core0.
 *
 * Example usage for buffered IO (where the ISR can obtain more than one value
 * per call):
 * @code{c}
 * void vBufferISR( void )
 * {
 * char cIn;
 * BaseType_t_core0 xHigherPriorityTaskWoken_core0;
 *
 *  // We have not woken a task_core0 at the start of the ISR.
 *  xHigherPriorityTaskWoken_core0 = pdFALSE_core0;
 *
 *  // Loop until the buffer is empty.
 *  do
 *  {
 *      // Obtain a byte from the buffer.
 *      cIn = portINPUT_BYTE( RX_REGISTER_ADDRESS );
 *
 *      // Post the byte.
 *      xQueueSendFromISR_core0( xRxQueue, &cIn, &xHigherPriorityTaskWoken_core0 );
 *
 *  } while( portINPUT_BYTE( BUFFER_COUNT ) );
 *
 *  // Now the buffer is empty we can switch context if necessary.
 *  if( xHigherPriorityTaskWoken_core0 )
 *  {
 *      // Actual macro used here is port specific.
 *      portYIELD_FROM_ISR_core0 ();
 *  }
 * }
 * @endcode
 *
 * \defgroup xQueueSendFromISR_core0 xQueueSendFromISR_core0
 * \ingroup QueueManagement
 */
#define xQueueSendFromISR_core0( xQueue_core0, pvItemToQueue_core0, pxHigherPriorityTaskWoken_core0 ) \
    xQueueGenericSendFromISR_core0( ( xQueue_core0 ), ( pvItemToQueue_core0 ), ( pxHigherPriorityTaskWoken_core0 ), queueSEND_TO_BACK_core0 )

/**
 * queue. h
 * @code{c}
 * BaseType_t_core0 xQueueGenericSendFromISR_core0(
 *                                         QueueHandle_t_core0    xQueue_core0,
 *                                         const    void    *pvItemToQueue_core0,
 *                                         BaseType_t_core0  *pxHigherPriorityTaskWoken_core0,
 *                                         BaseType_t_core0  xCopyPosition_core0
 *                                     );
 * @endcode
 *
 * It is preferred that the macros xQueueSendFromISR_core0(),
 * xQueueSendToFrontFromISR_core0() and xQueueSendToBackFromISR_core0() be used in place
 * of calling this function directly.  xQueueGiveFromISR() is an
 * equivalent for use by semaphores that don't actually copy any data.
 *
 * Post an item on a queue.  It is safe to use this function from within an
 * interrupt service routine.
 *
 * Items are queued by copy not reference so it is preferable to only
 * queue small items, especially when called from an ISR.  In most cases
 * it would be preferable to store a pointer to the item being queued.
 *
 * @param xQueue_core0 The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue_core0 A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue_core0
 * into the queue storage area.
 *
 * @param pxHigherPriorityTaskWoken_core0 xQueueGenericSendFromISR_core0() will set
 * *pxHigherPriorityTaskWoken_core0 to pdTRUE_core0 if sending to the queue caused a task_core0
 * to unblock, and the unblocked task_core0 has a priority higher than the currently
 * running task_core0.  If xQueueGenericSendFromISR_core0() sets this value to pdTRUE_core0 then
 * a context switch should be requested before the interrupt is exited.
 *
 * @param xCopyPosition_core0 Can take the value queueSEND_TO_BACK_core0 to place the
 * item at the back of the queue, or queueSEND_TO_FRONT_core0 to place the item
 * at the front of the queue (for high priority messages).
 *
 * @return pdTRUE_core0 if the data was successfully sent to the queue, otherwise
 * errQUEUE_FULL_core0.
 *
 * Example usage for buffered IO (where the ISR can obtain more than one value
 * per call):
 * @code{c}
 * void vBufferISR( void )
 * {
 * char cIn;
 * BaseType_t_core0 xHigherPriorityTaskWokenByPost;
 *
 *  // We have not woken a task_core0 at the start of the ISR.
 *  xHigherPriorityTaskWokenByPost = pdFALSE_core0;
 *
 *  // Loop until the buffer is empty.
 *  do
 *  {
 *      // Obtain a byte from the buffer.
 *      cIn = portINPUT_BYTE( RX_REGISTER_ADDRESS );
 *
 *      // Post each byte.
 *      xQueueGenericSendFromISR_core0( xRxQueue, &cIn, &xHigherPriorityTaskWokenByPost, queueSEND_TO_BACK_core0 );
 *
 *  } while( portINPUT_BYTE( BUFFER_COUNT ) );
 *
 *  // Now the buffer is empty we can switch context if necessary.  Note that the
 *  // name of the yield function required is port specific.
 *  if( xHigherPriorityTaskWokenByPost )
 *  {
 *      portYIELD_FROM_ISR_core0();
 *  }
 * }
 * @endcode
 *
 * \defgroup xQueueSendFromISR_core0 xQueueSendFromISR_core0
 * \ingroup QueueManagement
 */
BaseType_t_core0 xQueueGenericSendFromISR_core0( QueueHandle_t_core0 xQueue_core0,
                                     const void * const pvItemToQueue_core0,
                                     BaseType_t_core0 * const pxHigherPriorityTaskWoken_core0,
                                     const BaseType_t_core0 xCopyPosition_core0 ) PRIVILEGED_FUNCTION_core0;
BaseType_t_core0 xQueueGiveFromISR( QueueHandle_t_core0 xQueue_core0,
                              BaseType_t_core0 * const pxHigherPriorityTaskWoken_core0 ) PRIVILEGED_FUNCTION_core0;

/**
 * queue. h
 * @code{c}
 * BaseType_t_core0 xQueueReceiveFromISR_core0(
 *                                     QueueHandle_t_core0    xQueue_core0,
 *                                     void             *pvBuffer_core0,
 *                                     BaseType_t_core0       *pxTaskWoken
 *                                 );
 * @endcode
 *
 * Receive an item from a queue.  It is safe to use this function from within an
 * interrupt service routine.
 *
 * @param xQueue_core0 The handle to the queue from which the item is to be
 * received.
 *
 * @param pvBuffer_core0 Pointer to the buffer into which the received item will
 * be copied.
 *
 * @param pxTaskWoken A task_core0 may be blocked waiting for space to become
 * available on the queue.  If xQueueReceiveFromISR_core0 causes such a task_core0 to
 * unblock *pxTaskWoken will get set to pdTRUE_core0, otherwise *pxTaskWoken will
 * remain unchanged.
 *
 * @return pdTRUE_core0 if an item was successfully received from the queue,
 * otherwise pdFALSE_core0.
 *
 * Example usage:
 * @code{c}
 *
 * QueueHandle_t_core0 xQueue_core0;
 *
 * // Function to create a queue and post some values.
 * void vAFunction( void *pvParameters_core0 )
 * {
 * char cValueToPost;
 * const TickType_t_core0 xTicksToWait_core0 = ( TickType_t_core0 )0xff;
 *
 *  // Create a queue capable of containing 10 characters.
 *  xQueue_core0 = xQueueCreate_core0( 10, sizeof( char ) );
 *  if( xQueue_core0 == 0 )
 *  {
 *      // Failed to create the queue.
 *  }
 *
 *  // ...
 *
 *  // Post some characters that will be used within an ISR.  If the queue
 *  // is full then this task_core0 will block for xTicksToWait_core0 ticks.
 *  cValueToPost = 'a';
 *  xQueueSend_core0( xQueue_core0, ( void * ) &cValueToPost, xTicksToWait_core0 );
 *  cValueToPost = 'b';
 *  xQueueSend_core0( xQueue_core0, ( void * ) &cValueToPost, xTicksToWait_core0 );
 *
 *  // ... keep posting characters ... this task_core0 may block when the queue
 *  // becomes full.
 *
 *  cValueToPost = 'c';
 *  xQueueSend_core0( xQueue_core0, ( void * ) &cValueToPost, xTicksToWait_core0 );
 * }
 *
 * // ISR that outputs all the characters received on the queue.
 * void vISR_Routine( void )
 * {
 * BaseType_t_core0 xTaskWokenByReceive = pdFALSE_core0;
 * char cRxedChar;
 *
 *  while( xQueueReceiveFromISR_core0( xQueue_core0, ( void * ) &cRxedChar, &xTaskWokenByReceive) )
 *  {
 *      // A character was received.  Output the character now.
 *      vOutputCharacter( cRxedChar );
 *
 *      // If removing the character from the queue woke the task_core0 that was
 *      // posting onto the queue xTaskWokenByReceive will have been set to
 *      // pdTRUE_core0.  No matter how many times this loop iterates only one
 *      // task_core0 will be woken.
 *  }
 *
 *  if( xTaskWokenByReceive != ( char ) pdFALSE_core0;
 *  {
 *      taskYIELD ();
 *  }
 * }
 * @endcode
 * \defgroup xQueueReceiveFromISR_core0 xQueueReceiveFromISR_core0
 * \ingroup QueueManagement
 */
BaseType_t_core0 xQueueReceiveFromISR_core0( QueueHandle_t_core0 xQueue_core0,
                                 void * const pvBuffer_core0,
                                 BaseType_t_core0 * const pxHigherPriorityTaskWoken_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * Utilities to query queues that are safe to use from an ISR.  These utilities
 * should be used only from within an ISR, or within a critical section.
 */
BaseType_t_core0 xQueueIsQueueEmptyFromISR_core0( const QueueHandle_t_core0 xQueue_core0 ) PRIVILEGED_FUNCTION_core0;
BaseType_t_core0 xQueueIsQueueFullFromISR_core0( const QueueHandle_t_core0 xQueue_core0 ) PRIVILEGED_FUNCTION_core0;
UBaseType_t_core0 uxQueueMessagesWaitingFromISR_core0( const QueueHandle_t_core0 xQueue_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * The functions defined above are for passing data to and from tasks.  The
 * functions below are the equivalents for passing data to and from
 * co-routines.
 *
 * These functions are called from the co-routine macro implementation and
 * should not be called directly from application code.  Instead use the macro
 * wrappers defined within croutine.h.
 */
BaseType_t_core0 xQueueCRSendFromISR_core0( QueueHandle_t_core0 xQueue_core0,
                                const void * pvItemToQueue_core0,
                                BaseType_t_core0 xCoRoutinePreviouslyWoken_core0 );
BaseType_t_core0 xQueueCRReceiveFromISR_core0( QueueHandle_t_core0 xQueue_core0,
                                   void * pvBuffer_core0,
                                   BaseType_t_core0 * pxTaskWoken );
BaseType_t_core0 xQueueCRSend_core0( QueueHandle_t_core0 xQueue_core0,
                         const void * pvItemToQueue_core0,
                         TickType_t_core0 xTicksToWait_core0 );
BaseType_t_core0 xQueueCRReceive_core0( QueueHandle_t_core0 xQueue_core0,
                            void * pvBuffer_core0,
                            TickType_t_core0 xTicksToWait_core0 );

/*
 * For internal use only.  Use xSemaphoreCreateMutex(),
 * xSemaphoreCreateCounting() or xSemaphoreGetMutexHolder() instead of calling
 * these functions directly.
 */
QueueHandle_t_core0 xQueueCreateMutex_core0( const uint8_t ucQueueType_core0 ) PRIVILEGED_FUNCTION_core0;
QueueHandle_t_core0 xQueueCreateMutexStatic_core0( const uint8_t ucQueueType_core0,
                                       StaticQueue_t_core0 * pxStaticQueue_core0 ) PRIVILEGED_FUNCTION_core0;
QueueHandle_t_core0 xQueueCreateCountingSemaphore_core0( const UBaseType_t_core0 uxMaxCount_core0,
                                             const UBaseType_t_core0 uxInitialCount_core0 ) PRIVILEGED_FUNCTION_core0;
QueueHandle_t_core0 xQueueCreateCountingSemaphoreStatic_core0( const UBaseType_t_core0 uxMaxCount_core0,
                                                   const UBaseType_t_core0 uxInitialCount_core0,
                                                   StaticQueue_t_core0 * pxStaticQueue_core0 ) PRIVILEGED_FUNCTION_core0;
BaseType_t_core0 xQueueSemaphoreTake_core0( QueueHandle_t_core0 xQueue_core0,
                                TickType_t_core0 xTicksToWait_core0 ) PRIVILEGED_FUNCTION_core0;
TaskHandle_t_core0 xQueueGetMutexHolder_core0( QueueHandle_t_core0 xSemaphore_core0 ) PRIVILEGED_FUNCTION_core0;
TaskHandle_t_core0 xQueueGetMutexHolderFromISR_core0( QueueHandle_t_core0 xSemaphore_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * For internal use only.  Use xSemaphoreTakeMutexRecursive() or
 * xSemaphoreGiveMutexRecursive() instead of calling these functions directly.
 */
BaseType_t_core0 xQueueTakeMutexRecursive_core0( QueueHandle_t_core0 xMutex_core0,
                                     TickType_t_core0 xTicksToWait_core0 ) PRIVILEGED_FUNCTION_core0;
BaseType_t_core0 xQueueGiveMutexRecursive_core0( QueueHandle_t_core0 xMutex_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * Reset a queue back to its original empty state.  The return value is now
 * obsolete and is always set to pdPASS_core0.
 */
#define xQueueReset_core0( xQueue_core0 )    xQueueGenericReset_core0( ( xQueue_core0 ), pdFALSE_core0 )

/*
 * The registry is provided as a means for kernel aware debuggers to
 * locate queues, semaphores and mutexes.  Call vQueueAddToRegistry_core0() add
 * a queue, semaphore or mutex handle to the registry if you want the handle
 * to be available to a kernel aware debugger.  If you are not using a kernel
 * aware debugger then this function can be ignored.
 *
 * configQUEUE_REGISTRY_SIZE_core0 defines the maximum number of handles the
 * registry can hold.  configQUEUE_REGISTRY_SIZE_core0 must be greater than 0
 * within FreeRTOSConfig.h for the registry to be available.  Its value
 * does not affect the number of queues, semaphores and mutexes that can be
 * created - just the number that the registry can hold.
 *
 * If vQueueAddToRegistry_core0 is called more than once with the same xQueue_core0
 * parameter, the registry will store the pcQueueName_core0 parameter from the
 * most recent call to vQueueAddToRegistry_core0.
 *
 * @param xQueue_core0 The handle of the queue being added to the registry.  This
 * is the handle returned by a call to xQueueCreate_core0().  Semaphore and mutex
 * handles can also be passed in here.
 *
 * @param pcQueueName_core0 The name to be associated with the handle.  This is the
 * name that the kernel aware debugger will display.  The queue registry only
 * stores a pointer to the string - so the string must be persistent (global or
 * preferably in ROM/Flash), not on the stack.
 */
#if ( configQUEUE_REGISTRY_SIZE_core0 > 0 )
    void vQueueAddToRegistry_core0( QueueHandle_t_core0 xQueue_core0,
                              const char * pcQueueName_core0 ) PRIVILEGED_FUNCTION_core0; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
#endif

/*
 * The registry is provided as a means for kernel aware debuggers to
 * locate queues, semaphores and mutexes.  Call vQueueAddToRegistry_core0() add
 * a queue, semaphore or mutex handle to the registry if you want the handle
 * to be available to a kernel aware debugger, and vQueueUnregisterQueue_core0() to
 * remove the queue, semaphore or mutex from the register.  If you are not using
 * a kernel aware debugger then this function can be ignored.
 *
 * @param xQueue_core0 The handle of the queue being removed from the registry.
 */
#if ( configQUEUE_REGISTRY_SIZE_core0 > 0 )
    void vQueueUnregisterQueue_core0( QueueHandle_t_core0 xQueue_core0 ) PRIVILEGED_FUNCTION_core0;
#endif

/*
 * The queue registry is provided as a means for kernel aware debuggers to
 * locate queues, semaphores and mutexes.  Call pcQueueGetName_core0() to look
 * up and return the name of a queue in the queue registry from the queue's
 * handle.
 *
 * @param xQueue_core0 The handle of the queue the name of which will be returned.
 * @return If the queue is in the registry then a pointer to the name of the
 * queue is returned.  If the queue is not in the registry then NULL is
 * returned.
 */
#if ( configQUEUE_REGISTRY_SIZE_core0 > 0 )
    const char * pcQueueGetName_core0( QueueHandle_t_core0 xQueue_core0 ) PRIVILEGED_FUNCTION_core0; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
#endif

/*
 * Generic version of the function used to create a queue using dynamic memory
 * allocation.  This is called by other functions and macros that create other
 * RTOS objects that use the queue structure as their base.
 */
#if ( configSUPPORT_DYNAMIC_ALLOCATION_core0 == 1 )
    QueueHandle_t_core0 xQueueGenericCreate_core0( const UBaseType_t_core0 uxQueueLength_core0,
                                       const UBaseType_t_core0 uxItemSize_core0,
                                       const uint8_t ucQueueType_core0 ) PRIVILEGED_FUNCTION_core0;
#endif

/*
 * Generic version of the function used to create a queue using dynamic memory
 * allocation.  This is called by other functions and macros that create other
 * RTOS objects that use the queue structure as their base.
 */
#if ( configSUPPORT_STATIC_ALLOCATION_core0 == 1 )
    QueueHandle_t_core0 xQueueGenericCreateStatic_core0( const UBaseType_t_core0 uxQueueLength_core0,
                                             const UBaseType_t_core0 uxItemSize_core0,
                                             uint8_t * pucQueueStorage_core0,
                                             StaticQueue_t_core0 * pxStaticQueue_core0,
                                             const uint8_t ucQueueType_core0 ) PRIVILEGED_FUNCTION_core0;
#endif

/*
 * Queue sets provide a mechanism to allow a task_core0 to block (pend) on a read
 * operation from multiple queues or semaphores simultaneously.
 *
 * See FreeRTOS_core0/Source/Demo/Common/Minimal/QueueSet.c for an example using this
 * function.
 *
 * A queue set must be explicitly created using a call to xQueueCreateSet_core0()
 * before it can be used.  Once created, standard FreeRTOS_core0 queues and semaphores
 * can be added to the set using calls to xQueueAddToSet_core0().
 * xQueueSelectFromSet_core0() is then used to determine which, if any, of the queues
 * or semaphores contained in the set is in a state where a queue read or
 * semaphore take operation would be successful.
 *
 * Note 1:  See the documentation on https://www.FreeRTOS_core0.org/RTOS-queue-sets.html
 * for reasons why queue sets are very rarely needed in practice as there are
 * simpler methods of blocking on multiple objects.
 *
 * Note 2:  Blocking on a queue set that contains a mutex will not cause the
 * mutex holder to inherit the priority of the blocked task_core0.
 *
 * Note 3:  An additional 4 bytes of RAM is required for each space in a every
 * queue added to a queue set.  Therefore counting semaphores that have a high
 * maximum count value should not be added to a queue set.
 *
 * Note 4:  A receive (in the case of a queue) or take (in the case of a
 * semaphore) operation must not be performed on a member of a queue set unless
 * a call to xQueueSelectFromSet_core0() has first returned a handle to that set member.
 *
 * @param uxEventQueueLength_core0 Queue sets store events that occur on
 * the queues and semaphores contained in the set.  uxEventQueueLength_core0 specifies
 * the maximum number of events that can be queued at once.  To be absolutely
 * certain that events are not lost uxEventQueueLength_core0 should be set to the
 * total sum of the length of the queues added to the set, where binary
 * semaphores and mutexes have a length of 1, and counting semaphores have a
 * length set by their maximum count value.  Examples:
 *  + If a queue set is to hold a queue of length 5, another queue of length 12,
 *    and a binary semaphore, then uxEventQueueLength_core0 should be set to
 *    (5 + 12 + 1), or 18.
 *  + If a queue set is to hold three binary semaphores then uxEventQueueLength_core0
 *    should be set to (1 + 1 + 1 ), or 3.
 *  + If a queue set is to hold a counting semaphore that has a maximum count of
 *    5, and a counting semaphore that has a maximum count of 3, then
 *    uxEventQueueLength_core0 should be set to (5 + 3), or 8.
 *
 * @return If the queue set is created successfully then a handle to the created
 * queue set is returned.  Otherwise NULL is returned.
 */
QueueSetHandle_t_core0 xQueueCreateSet_core0( const UBaseType_t_core0 uxEventQueueLength_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * Adds a queue or semaphore to a queue set that was previously created by a
 * call to xQueueCreateSet_core0().
 *
 * See FreeRTOS_core0/Source/Demo/Common/Minimal/QueueSet.c for an example using this
 * function.
 *
 * Note 1:  A receive (in the case of a queue) or take (in the case of a
 * semaphore) operation must not be performed on a member of a queue set unless
 * a call to xQueueSelectFromSet_core0() has first returned a handle to that set member.
 *
 * @param xQueueOrSemaphore_core0 The handle of the queue or semaphore being added to
 * the queue set (cast to an QueueSetMemberHandle_t_core0 type).
 *
 * @param xQueueSet_core0 The handle of the queue set to which the queue or semaphore
 * is being added.
 *
 * @return If the queue or semaphore was successfully added to the queue set
 * then pdPASS_core0 is returned.  If the queue could not be successfully added to the
 * queue set because it is already a member of a different queue set then pdFAIL_core0
 * is returned.
 */
BaseType_t_core0 xQueueAddToSet_core0( QueueSetMemberHandle_t_core0 xQueueOrSemaphore_core0,
                           QueueSetHandle_t_core0 xQueueSet_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * Removes a queue or semaphore from a queue set.  A queue or semaphore can only
 * be removed from a set if the queue or semaphore is empty.
 *
 * See FreeRTOS_core0/Source/Demo/Common/Minimal/QueueSet.c for an example using this
 * function.
 *
 * @param xQueueOrSemaphore_core0 The handle of the queue or semaphore being removed
 * from the queue set (cast to an QueueSetMemberHandle_t_core0 type).
 *
 * @param xQueueSet_core0 The handle of the queue set in which the queue or semaphore
 * is included.
 *
 * @return If the queue or semaphore was successfully removed from the queue set
 * then pdPASS_core0 is returned.  If the queue was not in the queue set, or the
 * queue (or semaphore) was not empty, then pdFAIL_core0 is returned.
 */
BaseType_t_core0 xQueueRemoveFromSet_core0( QueueSetMemberHandle_t_core0 xQueueOrSemaphore_core0,
                                QueueSetHandle_t_core0 xQueueSet_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * xQueueSelectFromSet_core0() selects from the members of a queue set a queue or
 * semaphore that either contains data (in the case of a queue) or is available
 * to take (in the case of a semaphore).  xQueueSelectFromSet_core0() effectively
 * allows a task_core0 to block (pend) on a read operation on all the queues and
 * semaphores in a queue set simultaneously.
 *
 * See FreeRTOS_core0/Source/Demo/Common/Minimal/QueueSet.c for an example using this
 * function.
 *
 * Note 1:  See the documentation on https://www.FreeRTOS_core0.org/RTOS-queue-sets.html
 * for reasons why queue sets are very rarely needed in practice as there are
 * simpler methods of blocking on multiple objects.
 *
 * Note 2:  Blocking on a queue set that contains a mutex will not cause the
 * mutex holder to inherit the priority of the blocked task_core0.
 *
 * Note 3:  A receive (in the case of a queue) or take (in the case of a
 * semaphore) operation must not be performed on a member of a queue set unless
 * a call to xQueueSelectFromSet_core0() has first returned a handle to that set member.
 *
 * @param xQueueSet_core0 The queue set on which the task_core0 will (potentially) block.
 *
 * @param xTicksToWait_core0 The maximum time, in ticks, that the calling task_core0 will
 * remain in the Blocked state (with other tasks executing) to wait for a member
 * of the queue set to be ready for a successful queue read or semaphore take
 * operation.
 *
 * @return xQueueSelectFromSet_core0() will return the handle of a queue (cast to
 * a QueueSetMemberHandle_t_core0 type) contained in the queue set that contains data,
 * or the handle of a semaphore (cast to a QueueSetMemberHandle_t_core0 type) contained
 * in the queue set that is available, or NULL if no such queue or semaphore
 * exists before before the specified block time expires.
 */
QueueSetMemberHandle_t_core0 xQueueSelectFromSet_core0( QueueSetHandle_t_core0 xQueueSet_core0,
                                            const TickType_t_core0 xTicksToWait_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * A version of xQueueSelectFromSet_core0() that can be used from an ISR.
 */
QueueSetMemberHandle_t_core0 xQueueSelectFromSetFromISR_core0( QueueSetHandle_t_core0 xQueueSet_core0 ) PRIVILEGED_FUNCTION_core0;

/* Not public API functions. */
void vQueueWaitForMessageRestricted_core0( QueueHandle_t_core0 xQueue_core0,
                                     TickType_t_core0 xTicksToWait_core0,
                                     const BaseType_t_core0 xWaitIndefinitely_core0 ) PRIVILEGED_FUNCTION_core0;
BaseType_t_core0 xQueueGenericReset_core0( QueueHandle_t_core0 xQueue_core0,
                               BaseType_t_core0 xNewQueue_core0 ) PRIVILEGED_FUNCTION_core0;
void vQueueSetQueueNumber( QueueHandle_t_core0 xQueue_core0,
                           UBaseType_t_core0 uxQueueNumber_core0 ) PRIVILEGED_FUNCTION_core0;
UBaseType_t_core0 uxQueueGetQueueNumber( QueueHandle_t_core0 xQueue_core0 ) PRIVILEGED_FUNCTION_core0;
uint8_t ucQueueGetQueueType( QueueHandle_t_core0 xQueue_core0 ) PRIVILEGED_FUNCTION_core0;


/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* QUEUE_H */
