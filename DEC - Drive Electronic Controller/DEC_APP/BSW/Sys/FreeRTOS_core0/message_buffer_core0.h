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
 * Message buffers build functionality on top of FreeRTOS_core0 stream buffers.
 * Whereas stream buffers are used to send a continuous stream of data from one
 * task_core0 or interrupt to another, message buffers are used to send variable
 * length discrete messages from one task_core0 or interrupt to another.  Their
 * implementation is light weight, making them particularly suited for interrupt
 * to task_core0 and core to core communication scenarios.
 *
 * ***NOTE***:  Uniquely among FreeRTOS_core0 objects, the stream buffer
 * implementation (so also the message buffer implementation, as message buffers
 * are built on top of stream buffers) assumes there is only one task_core0 or
 * interrupt that will write to the buffer (the writer), and only one task_core0 or
 * interrupt that will read from the buffer (the reader).  It is safe for the
 * writer and reader to be different tasks or interrupts, but, unlike other
 * FreeRTOS_core0 objects, it is not safe to have multiple different writers or
 * multiple different readers.  If there are to be multiple different writers
 * then the application writer must place each call to a writing API function
 * (such as xMessageBufferSend_core0()) inside a critical section and set the send
 * block time to 0.  Likewise, if there are to be multiple different readers
 * then the application writer must place each call to a reading API function
 * (such as xMessageBufferRead()) inside a critical section and set the receive
 * timeout to 0.
 *
 * Message buffers hold variable length messages.  To enable that, when a
 * message is written to the message buffer an additional sizeof( size_t ) bytes
 * are also written to store the message's length (that happens internally, with
 * the API function).  sizeof( size_t ) is typically 4 bytes on a 32-bit
 * architecture, so writing a 10 byte message to a message buffer on a 32-bit
 * architecture will actually reduce the available space in the message buffer
 * by 14 bytes (10 byte are used by the message, and 4 bytes to hold the length
 * of the message).
 */

#ifndef FREERTOS_MESSAGE_BUFFER_CORE0_H
#define FREERTOS_MESSAGE_BUFFER_CORE0_H

#ifndef INC_FREERTOS_CORE0_H
    #error "include FreeRTOS_core0.h must appear in source files before include message_buffer.h"
#endif

/* Message buffers are built onto of stream buffers. */
#include "stream_buffer_core0.h"

/* *INDENT-OFF* */
#if defined( __cplusplus )
    extern "C" {
#endif
/* *INDENT-ON* */

/**
 * Type by which message buffers are referenced.  For example, a call to
 * xMessageBufferCreate_core0() returns an MessageBufferHandle_t_core0 variable that can
 * then be used as a parameter to xMessageBufferSend_core0(), xMessageBufferReceive_core0(),
 * etc. Message buffer is essentially built as a stream buffer hence its handle
 * is also set to same type as a stream buffer handle.
 */
typedef StreamBufferHandle_t_core0 MessageBufferHandle_t_core0;

/*-----------------------------------------------------------*/

/**
 * message_buffer.h
 *
 * @code{c}
 * MessageBufferHandle_t_core0 xMessageBufferCreate_core0( size_t xBufferSizeBytes_core0 );
 * @endcode
 *
 * Creates a new message buffer using dynamically allocated memory.  See
 * xMessageBufferCreateStatic_core0() for a version that uses statically allocated
 * memory (memory that is allocated at compile time).
 *
 * configSUPPORT_DYNAMIC_ALLOCATION_core0 must be set to 1 or left undefined in
 * FreeRTOSConfig.h for xMessageBufferCreate_core0() to be available.
 *
 * @param xBufferSizeBytes_core0 The total number of bytes (not messages) the message
 * buffer will be able to hold at any one time.  When a message is written to
 * the message buffer an additional sizeof( size_t ) bytes are also written to
 * store the message's length.  sizeof( size_t ) is typically 4 bytes on a
 * 32-bit architecture, so on most 32-bit architectures a 10 byte message will
 * take up 14 bytes of message buffer space.
 *
 * @param pxSendCompletedCallback_core0 Callback invoked when a send operation to the
 * message buffer is complete. If the parameter is NULL or xMessageBufferCreate_core0()
 * is called without the parameter, then it will use the default implementation
 * provided by sbSEND_COMPLETED macro. To enable the callback,
 * configUSE_SB_COMPLETED_CALLBACK_core0 must be set to 1 in FreeRTOSConfig.h.
 *
 * @param pxReceiveCompletedCallback_core0 Callback invoked when a receive operation from
 * the message buffer is complete. If the parameter is NULL or xMessageBufferCreate_core0()
 * is called without the parameter, it will use the default implementation provided
 * by sbRECEIVE_COMPLETED macro. To enable the callback,
 * configUSE_SB_COMPLETED_CALLBACK_core0 must be set to 1 in FreeRTOSConfig.h.
 *
 * @return If NULL is returned, then the message buffer cannot be created
 * because there is insufficient heap memory available for FreeRTOS_core0 to allocate
 * the message buffer data structures and storage area.  A non-NULL value being
 * returned indicates that the message buffer has been created successfully -
 * the returned value should be stored as the handle to the created message
 * buffer.
 *
 * Example use:
 * @code{c}
 *
 * void vAFunction( void )
 * {
 * MessageBufferHandle_t_core0 xMessageBuffer_core0;
 * const size_t xMessageBufferSizeBytes = 100;
 *
 *  // Create a message buffer that can hold 100 bytes.  The memory used to hold
 *  // both the message buffer structure and the messages themselves is allocated
 *  // dynamically.  Each message added to the buffer consumes an additional 4
 *  // bytes which are used to hold the length of the message.
 *  xMessageBuffer_core0 = xMessageBufferCreate_core0( xMessageBufferSizeBytes );
 *
 *  if( xMessageBuffer_core0 == NULL )
 *  {
 *      // There was not enough heap memory space available to create the
 *      // message buffer.
 *  }
 *  else
 *  {
 *      // The message buffer was created successfully and can now be used.
 *  }
 *
 * @endcode
 * \defgroup xMessageBufferCreate_core0 xMessageBufferCreate_core0
 * \ingroup MessageBufferManagement
 */
#define xMessageBufferCreate_core0( xBufferSizeBytes_core0 ) \
    xStreamBufferGenericCreate_core0( ( xBufferSizeBytes_core0 ), ( size_t ) 0, pdTRUE_core0, NULL, NULL )

#if ( configUSE_SB_COMPLETED_CALLBACK_core0 == 1 )
    #define xMessageBufferCreateWithCallback_core0( xBufferSizeBytes_core0, pxSendCompletedCallback_core0, pxReceiveCompletedCallback_core0 ) \
    xStreamBufferGenericCreate_core0( ( xBufferSizeBytes_core0 ), ( size_t ) 0, pdTRUE_core0, ( pxSendCompletedCallback_core0 ), ( pxReceiveCompletedCallback_core0 ) )
#endif

/**
 * message_buffer.h
 *
 * @code{c}
 * MessageBufferHandle_t_core0 xMessageBufferCreateStatic_core0( size_t xBufferSizeBytes_core0,
 *                                                uint8_t *pucMessageBufferStorageArea_core0,
 *                                                StaticMessageBuffer_t *pxStaticMessageBuffer_core0 );
 * @endcode
 * Creates a new message buffer using statically allocated memory.  See
 * xMessageBufferCreate_core0() for a version that uses dynamically allocated memory.
 *
 * @param xBufferSizeBytes_core0 The size, in bytes, of the buffer pointed to by the
 * pucMessageBufferStorageArea_core0 parameter.  When a message is written to the
 * message buffer an additional sizeof( size_t ) bytes are also written to store
 * the message's length.  sizeof( size_t ) is typically 4 bytes on a 32-bit
 * architecture, so on most 32-bit architecture a 10 byte message will take up
 * 14 bytes of message buffer space.  The maximum number of bytes that can be
 * stored in the message buffer is actually (xBufferSizeBytes_core0 - 1).
 *
 * @param pucMessageBufferStorageArea_core0 Must point to a uint8_t array that is at
 * least xBufferSizeBytes_core0 big.  This is the array to which messages are
 * copied when they are written to the message buffer.
 *
 * @param pxStaticMessageBuffer_core0 Must point to a variable of type
 * StaticMessageBuffer_t, which will be used to hold the message buffer's data
 * structure.
 *
 * @param pxSendCompletedCallback_core0 Callback invoked when a new message is sent to the message buffer.
 * If the parameter is NULL or xMessageBufferCreate_core0() is called without the parameter, then it will use the default
 * implementation provided by sbSEND_COMPLETED macro. To enable the callback,
 * configUSE_SB_COMPLETED_CALLBACK_core0 must be set to 1 in FreeRTOSConfig.h.
 *
 * @param pxReceiveCompletedCallback_core0 Callback invoked when a message is read from a
 * message buffer. If the parameter is NULL or xMessageBufferCreate_core0() is called without the parameter, it will
 * use the default implementation provided by sbRECEIVE_COMPLETED macro. To enable the callback,
 * configUSE_SB_COMPLETED_CALLBACK_core0 must be set to 1 in FreeRTOSConfig.h.
 *
 * @return If the message buffer is created successfully then a handle to the
 * created message buffer is returned. If either pucMessageBufferStorageArea_core0 or
 * pxStaticmessageBuffer are NULL then NULL is returned.
 *
 * Example use:
 * @code{c}
 *
 * // Used to dimension the array used to hold the messages.  The available space
 * // will actually be one less than this, so 999.
 #define STORAGE_SIZE_BYTES 1000
 *
 * // Defines the memory that will actually hold the messages within the message
 * // buffer.
 * static uint8_t ucStorageBuffer[ STORAGE_SIZE_BYTES ];
 *
 * // The variable used to hold the message buffer structure.
 * StaticMessageBuffer_t xMessageBufferStruct;
 *
 * void MyFunction( void )
 * {
 * MessageBufferHandle_t_core0 xMessageBuffer_core0;
 *
 *  xMessageBuffer_core0 = xMessageBufferCreateStatic_core0( sizeof( ucStorageBuffer ),
 *                                               ucStorageBuffer,
 *                                               &xMessageBufferStruct );
 *
 *  // As neither the pucMessageBufferStorageArea_core0 or pxStaticMessageBuffer_core0
 *  // parameters were NULL, xMessageBuffer_core0 will not be NULL, and can be used to
 *  // reference the created message buffer in other message buffer API calls.
 *
 *  // Other code that uses the message buffer can go here.
 * }
 *
 * @endcode
 * \defgroup xMessageBufferCreateStatic_core0 xMessageBufferCreateStatic_core0
 * \ingroup MessageBufferManagement
 */
#define xMessageBufferCreateStatic_core0( xBufferSizeBytes_core0, pucMessageBufferStorageArea_core0, pxStaticMessageBuffer_core0 ) \
    xStreamBufferGenericCreateStatic_core0( ( xBufferSizeBytes_core0 ), 0, pdTRUE_core0, ( pucMessageBufferStorageArea_core0 ), ( pxStaticMessageBuffer_core0 ), NULL, NULL )

#if ( configUSE_SB_COMPLETED_CALLBACK_core0 == 1 )
    #define xMessageBufferCreateStaticWithCallback_core0( xBufferSizeBytes_core0, pucMessageBufferStorageArea_core0, pxStaticMessageBuffer_core0, pxSendCompletedCallback_core0, pxReceiveCompletedCallback_core0 ) \
    xStreamBufferGenericCreateStatic_core0( ( xBufferSizeBytes_core0 ), 0, pdTRUE_core0, ( pucMessageBufferStorageArea_core0 ), ( pxStaticMessageBuffer_core0 ), ( pxSendCompletedCallback_core0 ), ( pxReceiveCompletedCallback_core0 ) )
#endif

/**
 * message_buffer.h
 *
 * @code{c}
 * size_t xMessageBufferSend_core0( MessageBufferHandle_t_core0 xMessageBuffer_core0,
 *                         const void *pvTxData_core0,
 *                         size_t xDataLengthBytes_core0,
 *                         TickType_t_core0 xTicksToWait_core0 );
 * @endcode
 *
 * Sends a discrete message to the message buffer.  The message can be any
 * length that fits within the buffer's free space, and is copied into the
 * buffer.
 *
 * ***NOTE***:  Uniquely among FreeRTOS_core0 objects, the stream buffer
 * implementation (so also the message buffer implementation, as message buffers
 * are built on top of stream buffers) assumes there is only one task_core0 or
 * interrupt that will write to the buffer (the writer), and only one task_core0 or
 * interrupt that will read from the buffer (the reader).  It is safe for the
 * writer and reader to be different tasks or interrupts, but, unlike other
 * FreeRTOS_core0 objects, it is not safe to have multiple different writers or
 * multiple different readers.  If there are to be multiple different writers
 * then the application writer must place each call to a writing API function
 * (such as xMessageBufferSend_core0()) inside a critical section and set the send
 * block time to 0.  Likewise, if there are to be multiple different readers
 * then the application writer must place each call to a reading API function
 * (such as xMessageBufferRead()) inside a critical section and set the receive
 * block time to 0.
 *
 * Use xMessageBufferSend_core0() to write to a message buffer from a task_core0.  Use
 * xMessageBufferSendFromISR_core0() to write to a message buffer from an interrupt
 * service routine (ISR).
 *
 * @param xMessageBuffer_core0 The handle of the message buffer to which a message is
 * being sent.
 *
 * @param pvTxData_core0 A pointer to the message that is to be copied into the
 * message buffer.
 *
 * @param xDataLengthBytes_core0 The length of the message.  That is, the number of
 * bytes to copy from pvTxData_core0 into the message buffer.  When a message is
 * written to the message buffer an additional sizeof( size_t ) bytes are also
 * written to store the message's length.  sizeof( size_t ) is typically 4 bytes
 * on a 32-bit architecture, so on most 32-bit architecture setting
 * xDataLengthBytes_core0 to 20 will reduce the free space in the message buffer by 24
 * bytes (20 bytes of message data and 4 bytes to hold the message length).
 *
 * @param xTicksToWait_core0 The maximum amount of time the calling task_core0 should remain
 * in the Blocked state to wait for enough space to become available in the
 * message buffer, should the message buffer have insufficient space when
 * xMessageBufferSend_core0() is called.  The calling task_core0 will never block if
 * xTicksToWait_core0 is zero.  The block time is specified in tick periods, so the
 * absolute time it represents is dependent on the tick frequency.  The macro
 * pdMS_TO_TICKS() can be used to convert a time specified in milliseconds into
 * a time specified in ticks.  Setting xTicksToWait_core0 to portMAX_DELAY_core0 will cause
 * the task_core0 to wait indefinitely (without timing out), provided
 * INCLUDE_vTaskSuspend_core0 is set to 1 in FreeRTOSConfig.h.  Tasks do not use any
 * CPU time when they are in the Blocked state.
 *
 * @return The number of bytes written to the message buffer.  If the call to
 * xMessageBufferSend_core0() times out before there was enough space to write the
 * message into the message buffer then zero is returned.  If the call did not
 * time out then xDataLengthBytes_core0 is returned.
 *
 * Example use:
 * @code{c}
 * void vAFunction( MessageBufferHandle_t_core0 xMessageBuffer_core0 )
 * {
 * size_t xBytesSent_core0;
 * uint8_t ucArrayToSend[] = { 0, 1, 2, 3 };
 * char *pcStringToSend = "String to send";
 * const TickType_t_core0 x100ms = pdMS_TO_TICKS( 100 );
 *
 *  // Send an array to the message buffer, blocking for a maximum of 100ms to
 *  // wait for enough space to be available in the message buffer.
 *  xBytesSent_core0 = xMessageBufferSend_core0( xMessageBuffer_core0, ( void * ) ucArrayToSend, sizeof( ucArrayToSend ), x100ms );
 *
 *  if( xBytesSent_core0 != sizeof( ucArrayToSend ) )
 *  {
 *      // The call to xMessageBufferSend_core0() times out before there was enough
 *      // space in the buffer for the data to be written.
 *  }
 *
 *  // Send the string to the message buffer.  Return immediately if there is
 *  // not enough space in the buffer.
 *  xBytesSent_core0 = xMessageBufferSend_core0( xMessageBuffer_core0, ( void * ) pcStringToSend, strlen( pcStringToSend ), 0 );
 *
 *  if( xBytesSent_core0 != strlen( pcStringToSend ) )
 *  {
 *      // The string could not be added to the message buffer because there was
 *      // not enough free space in the buffer.
 *  }
 * }
 * @endcode
 * \defgroup xMessageBufferSend_core0 xMessageBufferSend_core0
 * \ingroup MessageBufferManagement
 */
#define xMessageBufferSend_core0( xMessageBuffer_core0, pvTxData_core0, xDataLengthBytes_core0, xTicksToWait_core0 ) \
    xStreamBufferSend_core0( ( xMessageBuffer_core0 ), ( pvTxData_core0 ), ( xDataLengthBytes_core0 ), ( xTicksToWait_core0 ) )

/**
 * message_buffer.h
 *
 * @code{c}
 * size_t xMessageBufferSendFromISR_core0( MessageBufferHandle_t_core0 xMessageBuffer_core0,
 *                                const void *pvTxData_core0,
 *                                size_t xDataLengthBytes_core0,
 *                                BaseType_t_core0 *pxHigherPriorityTaskWoken_core0 );
 * @endcode
 *
 * Interrupt safe version of the API function that sends a discrete message to
 * the message buffer.  The message can be any length that fits within the
 * buffer's free space, and is copied into the buffer.
 *
 * ***NOTE***:  Uniquely among FreeRTOS_core0 objects, the stream buffer
 * implementation (so also the message buffer implementation, as message buffers
 * are built on top of stream buffers) assumes there is only one task_core0 or
 * interrupt that will write to the buffer (the writer), and only one task_core0 or
 * interrupt that will read from the buffer (the reader).  It is safe for the
 * writer and reader to be different tasks or interrupts, but, unlike other
 * FreeRTOS_core0 objects, it is not safe to have multiple different writers or
 * multiple different readers.  If there are to be multiple different writers
 * then the application writer must place each call to a writing API function
 * (such as xMessageBufferSend_core0()) inside a critical section and set the send
 * block time to 0.  Likewise, if there are to be multiple different readers
 * then the application writer must place each call to a reading API function
 * (such as xMessageBufferRead()) inside a critical section and set the receive
 * block time to 0.
 *
 * Use xMessageBufferSend_core0() to write to a message buffer from a task_core0.  Use
 * xMessageBufferSendFromISR_core0() to write to a message buffer from an interrupt
 * service routine (ISR).
 *
 * @param xMessageBuffer_core0 The handle of the message buffer to which a message is
 * being sent.
 *
 * @param pvTxData_core0 A pointer to the message that is to be copied into the
 * message buffer.
 *
 * @param xDataLengthBytes_core0 The length of the message.  That is, the number of
 * bytes to copy from pvTxData_core0 into the message buffer.  When a message is
 * written to the message buffer an additional sizeof( size_t ) bytes are also
 * written to store the message's length.  sizeof( size_t ) is typically 4 bytes
 * on a 32-bit architecture, so on most 32-bit architecture setting
 * xDataLengthBytes_core0 to 20 will reduce the free space in the message buffer by 24
 * bytes (20 bytes of message data and 4 bytes to hold the message length).
 *
 * @param pxHigherPriorityTaskWoken_core0  It is possible that a message buffer will
 * have a task_core0 blocked on it waiting for data.  Calling
 * xMessageBufferSendFromISR_core0() can make data available, and so cause a task_core0 that
 * was waiting for data to leave the Blocked state.  If calling
 * xMessageBufferSendFromISR_core0() causes a task_core0 to leave the Blocked state, and the
 * unblocked task_core0 has a priority higher than the currently executing task_core0 (the
 * task_core0 that was interrupted), then, internally, xMessageBufferSendFromISR_core0()
 * will set *pxHigherPriorityTaskWoken_core0 to pdTRUE_core0.  If
 * xMessageBufferSendFromISR_core0() sets this value to pdTRUE_core0, then normally a
 * context switch should be performed before the interrupt is exited.  This will
 * ensure that the interrupt returns directly to the highest priority Ready
 * state task_core0.  *pxHigherPriorityTaskWoken_core0 should be set to pdFALSE_core0 before it
 * is passed into the function.  See the code example below for an example.
 *
 * @return The number of bytes actually written to the message buffer.  If the
 * message buffer didn't have enough free space for the message to be stored
 * then 0 is returned, otherwise xDataLengthBytes_core0 is returned.
 *
 * Example use:
 * @code{c}
 * // A message buffer that has already been created.
 * MessageBufferHandle_t_core0 xMessageBuffer_core0;
 *
 * void vAnInterruptServiceRoutine( void )
 * {
 * size_t xBytesSent_core0;
 * char *pcStringToSend = "String to send";
 * BaseType_t_core0 xHigherPriorityTaskWoken_core0 = pdFALSE_core0; // Initialised to pdFALSE_core0.
 *
 *  // Attempt to send the string to the message buffer.
 *  xBytesSent_core0 = xMessageBufferSendFromISR_core0( xMessageBuffer_core0,
 *                                          ( void * ) pcStringToSend,
 *                                          strlen( pcStringToSend ),
 *                                          &xHigherPriorityTaskWoken_core0 );
 *
 *  if( xBytesSent_core0 != strlen( pcStringToSend ) )
 *  {
 *      // The string could not be added to the message buffer because there was
 *      // not enough free space in the buffer.
 *  }
 *
 *  // If xHigherPriorityTaskWoken_core0 was set to pdTRUE_core0 inside
 *  // xMessageBufferSendFromISR_core0() then a task_core0 that has a priority above the
 *  // priority of the currently executing task_core0 was unblocked and a context
 *  // switch should be performed to ensure the ISR returns to the unblocked
 *  // task_core0.  In most FreeRTOS_core0 ports this is done by simply passing
 *  // xHigherPriorityTaskWoken_core0 into portYIELD_FROM_ISR_core0(), which will test the
 *  // variables value, and perform the context switch if necessary.  Check the
 *  // documentation for the port in use for port specific instructions.
 *  portYIELD_FROM_ISR_core0( xHigherPriorityTaskWoken_core0 );
 * }
 * @endcode
 * \defgroup xMessageBufferSendFromISR_core0 xMessageBufferSendFromISR_core0
 * \ingroup MessageBufferManagement
 */
#define xMessageBufferSendFromISR_core0( xMessageBuffer_core0, pvTxData_core0, xDataLengthBytes_core0, pxHigherPriorityTaskWoken_core0 ) \
    xStreamBufferSendFromISR_core0( ( xMessageBuffer_core0 ), ( pvTxData_core0 ), ( xDataLengthBytes_core0 ), ( pxHigherPriorityTaskWoken_core0 ) )

/**
 * message_buffer.h
 *
 * @code{c}
 * size_t xMessageBufferReceive_core0( MessageBufferHandle_t_core0 xMessageBuffer_core0,
 *                            void *pvRxData_core0,
 *                            size_t xBufferLengthBytes_core0,
 *                            TickType_t_core0 xTicksToWait_core0 );
 * @endcode
 *
 * Receives a discrete message from a message buffer.  Messages can be of
 * variable length and are copied out of the buffer.
 *
 * ***NOTE***:  Uniquely among FreeRTOS_core0 objects, the stream buffer
 * implementation (so also the message buffer implementation, as message buffers
 * are built on top of stream buffers) assumes there is only one task_core0 or
 * interrupt that will write to the buffer (the writer), and only one task_core0 or
 * interrupt that will read from the buffer (the reader).  It is safe for the
 * writer and reader to be different tasks or interrupts, but, unlike other
 * FreeRTOS_core0 objects, it is not safe to have multiple different writers or
 * multiple different readers.  If there are to be multiple different writers
 * then the application writer must place each call to a writing API function
 * (such as xMessageBufferSend_core0()) inside a critical section and set the send
 * block time to 0.  Likewise, if there are to be multiple different readers
 * then the application writer must place each call to a reading API function
 * (such as xMessageBufferRead()) inside a critical section and set the receive
 * block time to 0.
 *
 * Use xMessageBufferReceive_core0() to read from a message buffer from a task_core0.  Use
 * xMessageBufferReceiveFromISR_core0() to read from a message buffer from an
 * interrupt service routine (ISR).
 *
 * @param xMessageBuffer_core0 The handle of the message buffer from which a message
 * is being received.
 *
 * @param pvRxData_core0 A pointer to the buffer into which the received message is
 * to be copied.
 *
 * @param xBufferLengthBytes_core0 The length of the buffer pointed to by the pvRxData_core0
 * parameter.  This sets the maximum length of the message that can be received.
 * If xBufferLengthBytes_core0 is too small to hold the next message then the message
 * will be left in the message buffer and 0 will be returned.
 *
 * @param xTicksToWait_core0 The maximum amount of time the task_core0 should remain in the
 * Blocked state to wait for a message, should the message buffer be empty.
 * xMessageBufferReceive_core0() will return immediately if xTicksToWait_core0 is zero and
 * the message buffer is empty.  The block time is specified in tick periods, so
 * the absolute time it represents is dependent on the tick frequency.  The
 * macro pdMS_TO_TICKS() can be used to convert a time specified in milliseconds
 * into a time specified in ticks.  Setting xTicksToWait_core0 to portMAX_DELAY_core0 will
 * cause the task_core0 to wait indefinitely (without timing out), provided
 * INCLUDE_vTaskSuspend_core0 is set to 1 in FreeRTOSConfig.h.  Tasks do not use any
 * CPU time when they are in the Blocked state.
 *
 * @return The length, in bytes, of the message read from the message buffer, if
 * any.  If xMessageBufferReceive_core0() times out before a message became available
 * then zero is returned.  If the length of the message is greater than
 * xBufferLengthBytes_core0 then the message will be left in the message buffer and
 * zero is returned.
 *
 * Example use:
 * @code{c}
 * void vAFunction( MessageBuffer_t xMessageBuffer_core0 )
 * {
 * uint8_t ucRxData[ 20 ];
 * size_t xReceivedBytes;
 * const TickType_t_core0 xBlockTime = pdMS_TO_TICKS( 20 );
 *
 *  // Receive the next message from the message buffer.  Wait in the Blocked
 *  // state (so not using any CPU processing time) for a maximum of 100ms for
 *  // a message to become available.
 *  xReceivedBytes = xMessageBufferReceive_core0( xMessageBuffer_core0,
 *                                          ( void * ) ucRxData,
 *                                          sizeof( ucRxData ),
 *                                          xBlockTime );
 *
 *  if( xReceivedBytes > 0 )
 *  {
 *      // A ucRxData contains a message that is xReceivedBytes long.  Process
 *      // the message here....
 *  }
 * }
 * @endcode
 * \defgroup xMessageBufferReceive_core0 xMessageBufferReceive_core0
 * \ingroup MessageBufferManagement
 */
#define xMessageBufferReceive_core0( xMessageBuffer_core0, pvRxData_core0, xBufferLengthBytes_core0, xTicksToWait_core0 ) \
    xStreamBufferReceive_core0( ( xMessageBuffer_core0 ), ( pvRxData_core0 ), ( xBufferLengthBytes_core0 ), ( xTicksToWait_core0 ) )


/**
 * message_buffer.h
 *
 * @code{c}
 * size_t xMessageBufferReceiveFromISR_core0( MessageBufferHandle_t_core0 xMessageBuffer_core0,
 *                                   void *pvRxData_core0,
 *                                   size_t xBufferLengthBytes_core0,
 *                                   BaseType_t_core0 *pxHigherPriorityTaskWoken_core0 );
 * @endcode
 *
 * An interrupt safe version of the API function that receives a discrete
 * message from a message buffer.  Messages can be of variable length and are
 * copied out of the buffer.
 *
 * ***NOTE***:  Uniquely among FreeRTOS_core0 objects, the stream buffer
 * implementation (so also the message buffer implementation, as message buffers
 * are built on top of stream buffers) assumes there is only one task_core0 or
 * interrupt that will write to the buffer (the writer), and only one task_core0 or
 * interrupt that will read from the buffer (the reader).  It is safe for the
 * writer and reader to be different tasks or interrupts, but, unlike other
 * FreeRTOS_core0 objects, it is not safe to have multiple different writers or
 * multiple different readers.  If there are to be multiple different writers
 * then the application writer must place each call to a writing API function
 * (such as xMessageBufferSend_core0()) inside a critical section and set the send
 * block time to 0.  Likewise, if there are to be multiple different readers
 * then the application writer must place each call to a reading API function
 * (such as xMessageBufferRead()) inside a critical section and set the receive
 * block time to 0.
 *
 * Use xMessageBufferReceive_core0() to read from a message buffer from a task_core0.  Use
 * xMessageBufferReceiveFromISR_core0() to read from a message buffer from an
 * interrupt service routine (ISR).
 *
 * @param xMessageBuffer_core0 The handle of the message buffer from which a message
 * is being received.
 *
 * @param pvRxData_core0 A pointer to the buffer into which the received message is
 * to be copied.
 *
 * @param xBufferLengthBytes_core0 The length of the buffer pointed to by the pvRxData_core0
 * parameter.  This sets the maximum length of the message that can be received.
 * If xBufferLengthBytes_core0 is too small to hold the next message then the message
 * will be left in the message buffer and 0 will be returned.
 *
 * @param pxHigherPriorityTaskWoken_core0  It is possible that a message buffer will
 * have a task_core0 blocked on it waiting for space to become available.  Calling
 * xMessageBufferReceiveFromISR_core0() can make space available, and so cause a task_core0
 * that is waiting for space to leave the Blocked state.  If calling
 * xMessageBufferReceiveFromISR_core0() causes a task_core0 to leave the Blocked state, and
 * the unblocked task_core0 has a priority higher than the currently executing task_core0
 * (the task_core0 that was interrupted), then, internally,
 * xMessageBufferReceiveFromISR_core0() will set *pxHigherPriorityTaskWoken_core0 to pdTRUE_core0.
 * If xMessageBufferReceiveFromISR_core0() sets this value to pdTRUE_core0, then normally a
 * context switch should be performed before the interrupt is exited.  That will
 * ensure the interrupt returns directly to the highest priority Ready state
 * task_core0.  *pxHigherPriorityTaskWoken_core0 should be set to pdFALSE_core0 before it is
 * passed into the function.  See the code example below for an example.
 *
 * @return The length, in bytes, of the message read from the message buffer, if
 * any.
 *
 * Example use:
 * @code{c}
 * // A message buffer that has already been created.
 * MessageBuffer_t xMessageBuffer_core0;
 *
 * void vAnInterruptServiceRoutine( void )
 * {
 * uint8_t ucRxData[ 20 ];
 * size_t xReceivedBytes;
 * BaseType_t_core0 xHigherPriorityTaskWoken_core0 = pdFALSE_core0;  // Initialised to pdFALSE_core0.
 *
 *  // Receive the next message from the message buffer.
 *  xReceivedBytes = xMessageBufferReceiveFromISR_core0( xMessageBuffer_core0,
 *                                                ( void * ) ucRxData,
 *                                                sizeof( ucRxData ),
 *                                                &xHigherPriorityTaskWoken_core0 );
 *
 *  if( xReceivedBytes > 0 )
 *  {
 *      // A ucRxData contains a message that is xReceivedBytes long.  Process
 *      // the message here....
 *  }
 *
 *  // If xHigherPriorityTaskWoken_core0 was set to pdTRUE_core0 inside
 *  // xMessageBufferReceiveFromISR_core0() then a task_core0 that has a priority above the
 *  // priority of the currently executing task_core0 was unblocked and a context
 *  // switch should be performed to ensure the ISR returns to the unblocked
 *  // task_core0.  In most FreeRTOS_core0 ports this is done by simply passing
 *  // xHigherPriorityTaskWoken_core0 into portYIELD_FROM_ISR_core0(), which will test the
 *  // variables value, and perform the context switch if necessary.  Check the
 *  // documentation for the port in use for port specific instructions.
 *  portYIELD_FROM_ISR_core0( xHigherPriorityTaskWoken_core0 );
 * }
 * @endcode
 * \defgroup xMessageBufferReceiveFromISR_core0 xMessageBufferReceiveFromISR_core0
 * \ingroup MessageBufferManagement
 */
#define xMessageBufferReceiveFromISR_core0( xMessageBuffer_core0, pvRxData_core0, xBufferLengthBytes_core0, pxHigherPriorityTaskWoken_core0 ) \
    xStreamBufferReceiveFromISR_core0( ( xMessageBuffer_core0 ), ( pvRxData_core0 ), ( xBufferLengthBytes_core0 ), ( pxHigherPriorityTaskWoken_core0 ) )

/**
 * message_buffer.h
 *
 * @code{c}
 * void vMessageBufferDelete_core0( MessageBufferHandle_t_core0 xMessageBuffer_core0 );
 * @endcode
 *
 * Deletes a message buffer that was previously created using a call to
 * xMessageBufferCreate_core0() or xMessageBufferCreateStatic_core0().  If the message
 * buffer was created using dynamic memory (that is, by xMessageBufferCreate_core0()),
 * then the allocated memory is freed.
 *
 * A message buffer handle must not be used after the message buffer has been
 * deleted.
 *
 * @param xMessageBuffer_core0 The handle of the message buffer to be deleted.
 *
 */
#define vMessageBufferDelete_core0( xMessageBuffer_core0 ) \
    vStreamBufferDelete_core0( xMessageBuffer_core0 )

/**
 * message_buffer.h
 * @code{c}
 * BaseType_t_core0 xMessageBufferIsFull_core0( MessageBufferHandle_t_core0 xMessageBuffer_core0 );
 * @endcode
 *
 * Tests to see if a message buffer is full.  A message buffer is full if it
 * cannot accept any more messages, of any size, until space is made available
 * by a message being removed from the message buffer.
 *
 * @param xMessageBuffer_core0 The handle of the message buffer being queried.
 *
 * @return If the message buffer referenced by xMessageBuffer_core0 is full then
 * pdTRUE_core0 is returned.  Otherwise pdFALSE_core0 is returned.
 */
#define xMessageBufferIsFull_core0( xMessageBuffer_core0 ) \
    xStreamBufferIsFull_core0( xMessageBuffer_core0 )

/**
 * message_buffer.h
 * @code{c}
 * BaseType_t_core0 xMessageBufferIsEmpty_core0( MessageBufferHandle_t_core0 xMessageBuffer_core0 );
 * @endcode
 *
 * Tests to see if a message buffer is empty (does not contain any messages).
 *
 * @param xMessageBuffer_core0 The handle of the message buffer being queried.
 *
 * @return If the message buffer referenced by xMessageBuffer_core0 is empty then
 * pdTRUE_core0 is returned.  Otherwise pdFALSE_core0 is returned.
 *
 */
#define xMessageBufferIsEmpty_core0( xMessageBuffer_core0 ) \
    xStreamBufferIsEmpty_core0( xMessageBuffer_core0 )

/**
 * message_buffer.h
 * @code{c}
 * BaseType_t_core0 xMessageBufferReset_core0( MessageBufferHandle_t_core0 xMessageBuffer_core0 );
 * @endcode
 *
 * Resets a message buffer to its initial empty state, discarding any message it
 * contained.
 *
 * A message buffer can only be reset if there are no tasks blocked on it.
 *
 * @param xMessageBuffer_core0 The handle of the message buffer being reset.
 *
 * @return If the message buffer was reset then pdPASS_core0 is returned.  If the
 * message buffer could not be reset because either there was a task_core0 blocked on
 * the message queue to wait for space to become available, or to wait for a
 * a message to be available, then pdFAIL_core0 is returned.
 *
 * \defgroup xMessageBufferReset_core0 xMessageBufferReset_core0
 * \ingroup MessageBufferManagement
 */
#define xMessageBufferReset_core0( xMessageBuffer_core0 ) \
    xStreamBufferReset_core0( xMessageBuffer_core0 )


/**
 * message_buffer.h
 * @code{c}
 * size_t xMessageBufferSpaceAvailable_core0( MessageBufferHandle_t_core0 xMessageBuffer_core0 );
 * @endcode
 * Returns the number of bytes of free space in the message buffer.
 *
 * @param xMessageBuffer_core0 The handle of the message buffer being queried.
 *
 * @return The number of bytes that can be written to the message buffer before
 * the message buffer would be full.  When a message is written to the message
 * buffer an additional sizeof( size_t ) bytes are also written to store the
 * message's length.  sizeof( size_t ) is typically 4 bytes on a 32-bit
 * architecture, so if xMessageBufferSpacesAvailable_core0() returns 10, then the size
 * of the largest message that can be written to the message buffer is 6 bytes.
 *
 * \defgroup xMessageBufferSpaceAvailable_core0 xMessageBufferSpaceAvailable_core0
 * \ingroup MessageBufferManagement
 */
#define xMessageBufferSpaceAvailable_core0( xMessageBuffer_core0 ) \
    xStreamBufferSpacesAvailable_core0( xMessageBuffer_core0 )
#define xMessageBufferSpacesAvailable_core0( xMessageBuffer_core0 ) \
    xStreamBufferSpacesAvailable_core0( xMessageBuffer_core0 ) /* Corrects typo in original macro name. */

/**
 * message_buffer.h
 * @code{c}
 * size_t xMessageBufferNextLengthBytes_core0( MessageBufferHandle_t_core0 xMessageBuffer_core0 );
 * @endcode
 * Returns the length (in bytes) of the next message in a message buffer.
 * Useful if xMessageBufferReceive_core0() returned 0 because the size of the buffer
 * passed into xMessageBufferReceive_core0() was too small to hold the next message.
 *
 * @param xMessageBuffer_core0 The handle of the message buffer being queried.
 *
 * @return The length (in bytes) of the next message in the message buffer, or 0
 * if the message buffer is empty.
 *
 * \defgroup xMessageBufferNextLengthBytes_core0 xMessageBufferNextLengthBytes_core0
 * \ingroup MessageBufferManagement
 */
#define xMessageBufferNextLengthBytes_core0( xMessageBuffer_core0 ) \
    xStreamBufferNextMessageLengthBytes_core0( xMessageBuffer_core0 ) PRIVILEGED_FUNCTION_core0;

/**
 * message_buffer.h
 *
 * @code{c}
 * BaseType_t_core0 xMessageBufferSendCompletedFromISR_core0( MessageBufferHandle_t_core0 xMessageBuffer_core0, BaseType_t_core0 *pxHigherPriorityTaskWoken_core0 );
 * @endcode
 *
 * For advanced users only.
 *
 * The sbSEND_COMPLETED() macro is called from within the FreeRTOS_core0 APIs when
 * data is sent to a message buffer or stream buffer.  If there was a task_core0 that
 * was blocked on the message or stream buffer waiting for data to arrive then
 * the sbSEND_COMPLETED() macro sends a notification to the task_core0 to remove it
 * from the Blocked state.  xMessageBufferSendCompletedFromISR_core0() does the same
 * thing.  It is provided to enable application writers to implement their own
 * version of sbSEND_COMPLETED(), and MUST NOT BE USED AT ANY OTHER TIME.
 *
 * See the example implemented in FreeRTOS_core0/Demo/Minimal/MessageBufferAMP.c for
 * additional information.
 *
 * @param xMessageBuffer_core0 The handle of the stream buffer to which data was
 * written.
 *
 * @param pxHigherPriorityTaskWoken_core0 *pxHigherPriorityTaskWoken_core0 should be
 * initialised to pdFALSE_core0 before it is passed into
 * xMessageBufferSendCompletedFromISR_core0().  If calling
 * xMessageBufferSendCompletedFromISR_core0() removes a task_core0 from the Blocked state,
 * and the task_core0 has a priority above the priority of the currently running task_core0,
 * then *pxHigherPriorityTaskWoken_core0 will get set to pdTRUE_core0 indicating that a
 * context switch should be performed before exiting the ISR.
 *
 * @return If a task_core0 was removed from the Blocked state then pdTRUE_core0 is returned.
 * Otherwise pdFALSE_core0 is returned.
 *
 * \defgroup xMessageBufferSendCompletedFromISR_core0 xMessageBufferSendCompletedFromISR_core0
 * \ingroup StreamBufferManagement
 */
#define xMessageBufferSendCompletedFromISR_core0( xMessageBuffer_core0, pxHigherPriorityTaskWoken_core0 ) \
    xStreamBufferSendCompletedFromISR_core0( ( xMessageBuffer_core0 ), ( pxHigherPriorityTaskWoken_core0 ) )

/**
 * message_buffer.h
 *
 * @code{c}
 * BaseType_t_core0 xMessageBufferReceiveCompletedFromISR_core0( MessageBufferHandle_t_core0 xMessageBuffer_core0, BaseType_t_core0 *pxHigherPriorityTaskWoken_core0 );
 * @endcode
 *
 * For advanced users only.
 *
 * The sbRECEIVE_COMPLETED() macro is called from within the FreeRTOS_core0 APIs when
 * data is read out of a message buffer or stream buffer.  If there was a task_core0
 * that was blocked on the message or stream buffer waiting for data to arrive
 * then the sbRECEIVE_COMPLETED() macro sends a notification to the task_core0 to
 * remove it from the Blocked state.  xMessageBufferReceiveCompletedFromISR_core0()
 * does the same thing.  It is provided to enable application writers to
 * implement their own version of sbRECEIVE_COMPLETED(), and MUST NOT BE USED AT
 * ANY OTHER TIME.
 *
 * See the example implemented in FreeRTOS_core0/Demo/Minimal/MessageBufferAMP.c for
 * additional information.
 *
 * @param xMessageBuffer_core0 The handle of the stream buffer from which data was
 * read.
 *
 * @param pxHigherPriorityTaskWoken_core0 *pxHigherPriorityTaskWoken_core0 should be
 * initialised to pdFALSE_core0 before it is passed into
 * xMessageBufferReceiveCompletedFromISR_core0().  If calling
 * xMessageBufferReceiveCompletedFromISR_core0() removes a task_core0 from the Blocked state,
 * and the task_core0 has a priority above the priority of the currently running task_core0,
 * then *pxHigherPriorityTaskWoken_core0 will get set to pdTRUE_core0 indicating that a
 * context switch should be performed before exiting the ISR.
 *
 * @return If a task_core0 was removed from the Blocked state then pdTRUE_core0 is returned.
 * Otherwise pdFALSE_core0 is returned.
 *
 * \defgroup xMessageBufferReceiveCompletedFromISR_core0 xMessageBufferReceiveCompletedFromISR_core0
 * \ingroup StreamBufferManagement
 */
#define xMessageBufferReceiveCompletedFromISR_core0( xMessageBuffer_core0, pxHigherPriorityTaskWoken_core0 ) \
    xStreamBufferReceiveCompletedFromISR_core0( ( xMessageBuffer_core0 ), ( pxHigherPriorityTaskWoken_core0 ) )

/* *INDENT-OFF* */
#if defined( __cplusplus )
    } /* extern "C" */
#endif
/* *INDENT-ON* */

#endif /* !defined( FREERTOS_MESSAGE_BUFFER_H ) */
