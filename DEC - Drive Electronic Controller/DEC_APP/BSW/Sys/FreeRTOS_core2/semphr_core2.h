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

#ifndef SEMAPHORE_CORE2_H
#define SEMAPHORE_CORE2_H

#ifndef INC_FREERTOS_CORE2_H
    #error "include FreeRTOS_core2.h" must appear in source files before "include semphr.h"
#endif

#include "queue_core2.h"

typedef QueueHandle_t_core2 SemaphoreHandle_t_core2;

#define semBINARY_SEMAPHORE_QUEUE_LENGTH_core2    ( ( uint8_t ) 1U )
#define semSEMAPHORE_QUEUE_ITEM_LENGTH_core2      ( ( uint8_t ) 0U )
#define semGIVE_BLOCK_TIME_core2                  ( ( TickType_t_core2 ) 0U )


/**
 * semphr. h
 * @code{c}
 * vSemaphoreCreateBinary_core2( SemaphoreHandle_t_core2 xSemaphore_core2 );
 * @endcode
 *
 * In many usage scenarios it is faster and more memory efficient to use a
 * direct to task_core2 notification in place of a binary semaphore!
 * https://www.FreeRTOS_core2.org/RTOS-task_core2-notifications.html
 *
 * This old vSemaphoreCreateBinary_core2() macro is now deprecated in favour of the
 * xSemaphoreCreateBinary_core2() function.  Note that binary semaphores created using
 * the vSemaphoreCreateBinary_core2() macro are created in a state such that the
 * first call to 'take' the semaphore would pass, whereas binary semaphores
 * created using xSemaphoreCreateBinary_core2() are created in a state such that the
 * the semaphore must first be 'given' before it can be 'taken'.
 *
 * <i>Macro</i> that implements a semaphore by using the existing queue mechanism.
 * The queue length is 1 as this is a binary semaphore.  The data size is 0
 * as we don't want to actually store any data - we just want to know if the
 * queue is empty or full.
 *
 * This type of semaphore can be used for pure synchronisation between tasks or
 * between an interrupt and a task_core2.  The semaphore need not be given back once
 * obtained, so one task_core2/interrupt can continuously 'give' the semaphore while
 * another continuously 'takes' the semaphore.  For this reason this type of
 * semaphore does not use a priority inheritance mechanism.  For an alternative
 * that does use priority inheritance see xSemaphoreCreateMutex_core2().
 *
 * @param xSemaphore_core2 Handle to the created semaphore.  Should be of type SemaphoreHandle_t_core2.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphore_core2 = NULL;
 *
 * void vATask( void * pvParameters_core2 )
 * {
 *  // Semaphore cannot be used before a call to vSemaphoreCreateBinary_core2 ().
 *  // This is a macro so pass the variable in directly.
 *  vSemaphoreCreateBinary_core2( xSemaphore_core2 );
 *
 *  if( xSemaphore_core2 != NULL )
 *  {
 *      // The semaphore was created successfully.
 *      // The semaphore can now be used.
 *  }
 * }
 * @endcode
 * \defgroup vSemaphoreCreateBinary_core2 vSemaphoreCreateBinary_core2
 * \ingroup Semaphores
 */
#if ( configSUPPORT_DYNAMIC_ALLOCATION_core2 == 1 )
    #define vSemaphoreCreateBinary_core2( xSemaphore_core2 )                                                                                     \
    {                                                                                                                                \
        ( xSemaphore_core2 ) = xQueueGenericCreate_core2( ( UBaseType_t_core2 ) 1, semSEMAPHORE_QUEUE_ITEM_LENGTH_core2, queueQUEUE_TYPE_BINARY_SEMAPHORE_core2 ); \
        if( ( xSemaphore_core2 ) != NULL )                                                                                                 \
        {                                                                                                                            \
            ( void ) xSemaphoreGive_core2( ( xSemaphore_core2 ) );                                                                               \
        }                                                                                                                            \
    }
#endif

/**
 * semphr. h
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphoreCreateBinary_core2( void );
 * @endcode
 *
 * Creates a new binary semaphore instance, and returns a handle by which the
 * new semaphore can be referenced.
 *
 * In many usage scenarios it is faster and more memory efficient to use a
 * direct to task_core2 notification in place of a binary semaphore!
 * https://www.FreeRTOS_core2.org/RTOS-task_core2-notifications.html
 *
 * Internally, within the FreeRTOS_core2 implementation, binary semaphores use a block
 * of memory, in which the semaphore structure is stored.  If a binary semaphore
 * is created using xSemaphoreCreateBinary_core2() then the required memory is
 * automatically dynamically allocated inside the xSemaphoreCreateBinary_core2()
 * function.  (see https://www.FreeRTOS_core2.org/a00111.html).  If a binary semaphore
 * is created using xSemaphoreCreateBinaryStatic_core2() then the application writer
 * must provide the memory.  xSemaphoreCreateBinaryStatic_core2() therefore allows a
 * binary semaphore to be created without using any dynamic memory allocation.
 *
 * The old vSemaphoreCreateBinary_core2() macro is now deprecated in favour of this
 * xSemaphoreCreateBinary_core2() function.  Note that binary semaphores created using
 * the vSemaphoreCreateBinary_core2() macro are created in a state such that the
 * first call to 'take' the semaphore would pass, whereas binary semaphores
 * created using xSemaphoreCreateBinary_core2() are created in a state such that the
 * the semaphore must first be 'given' before it can be 'taken'.
 *
 * This type of semaphore can be used for pure synchronisation between tasks or
 * between an interrupt and a task_core2.  The semaphore need not be given back once
 * obtained, so one task_core2/interrupt can continuously 'give' the semaphore while
 * another continuously 'takes' the semaphore.  For this reason this type of
 * semaphore does not use a priority inheritance mechanism.  For an alternative
 * that does use priority inheritance see xSemaphoreCreateMutex_core2().
 *
 * @return Handle to the created semaphore, or NULL if the memory required to
 * hold the semaphore's data structures could not be allocated.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphore_core2 = NULL;
 *
 * void vATask( void * pvParameters_core2 )
 * {
 *  // Semaphore cannot be used before a call to xSemaphoreCreateBinary_core2().
 *  // This is a macro so pass the variable in directly.
 *  xSemaphore_core2 = xSemaphoreCreateBinary_core2();
 *
 *  if( xSemaphore_core2 != NULL )
 *  {
 *      // The semaphore was created successfully.
 *      // The semaphore can now be used.
 *  }
 * }
 * @endcode
 * \defgroup xSemaphoreCreateBinary_core2 xSemaphoreCreateBinary_core2
 * \ingroup Semaphores
 */
#if ( configSUPPORT_DYNAMIC_ALLOCATION_core2 == 1 )
    #define xSemaphoreCreateBinary_core2()    xQueueGenericCreate_core2( ( UBaseType_t_core2 ) 1, semSEMAPHORE_QUEUE_ITEM_LENGTH_core2, queueQUEUE_TYPE_BINARY_SEMAPHORE_core2 )
#endif

/**
 * semphr. h
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphoreCreateBinaryStatic_core2( StaticSemaphore_t_core2 *pxSemaphoreBuffer_core2 );
 * @endcode
 *
 * Creates a new binary semaphore instance, and returns a handle by which the
 * new semaphore can be referenced.
 *
 * NOTE: In many usage scenarios it is faster and more memory efficient to use a
 * direct to task_core2 notification in place of a binary semaphore!
 * https://www.FreeRTOS_core2.org/RTOS-task_core2-notifications.html
 *
 * Internally, within the FreeRTOS_core2 implementation, binary semaphores use a block
 * of memory, in which the semaphore structure is stored.  If a binary semaphore
 * is created using xSemaphoreCreateBinary_core2() then the required memory is
 * automatically dynamically allocated inside the xSemaphoreCreateBinary_core2()
 * function.  (see https://www.FreeRTOS_core2.org/a00111.html).  If a binary semaphore
 * is created using xSemaphoreCreateBinaryStatic_core2() then the application writer
 * must provide the memory.  xSemaphoreCreateBinaryStatic_core2() therefore allows a
 * binary semaphore to be created without using any dynamic memory allocation.
 *
 * This type of semaphore can be used for pure synchronisation between tasks or
 * between an interrupt and a task_core2.  The semaphore need not be given back once
 * obtained, so one task_core2/interrupt can continuously 'give' the semaphore while
 * another continuously 'takes' the semaphore.  For this reason this type of
 * semaphore does not use a priority inheritance mechanism.  For an alternative
 * that does use priority inheritance see xSemaphoreCreateMutex_core2().
 *
 * @param pxSemaphoreBuffer_core2 Must point to a variable of type StaticSemaphore_t_core2,
 * which will then be used to hold the semaphore's data structure, removing the
 * need for the memory to be allocated dynamically.
 *
 * @return If the semaphore is created then a handle to the created semaphore is
 * returned.  If pxSemaphoreBuffer_core2 is NULL then NULL is returned.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphore_core2 = NULL;
 * StaticSemaphore_t_core2 xSemaphoreBuffer;
 *
 * void vATask( void * pvParameters_core2 )
 * {
 *  // Semaphore cannot be used before a call to xSemaphoreCreateBinary_core2().
 *  // The semaphore's data structures will be placed in the xSemaphoreBuffer
 *  // variable, the address of which is passed into the function.  The
 *  // function's parameter is not NULL, so the function will not attempt any
 *  // dynamic memory allocation, and therefore the function will not return
 *  // return NULL.
 *  xSemaphore_core2 = xSemaphoreCreateBinary_core2( &xSemaphoreBuffer );
 *
 *  // Rest of task_core2 code goes here.
 * }
 * @endcode
 * \defgroup xSemaphoreCreateBinaryStatic_core2 xSemaphoreCreateBinaryStatic_core2
 * \ingroup Semaphores
 */
#if ( configSUPPORT_STATIC_ALLOCATION_core2 == 1 )
    #define xSemaphoreCreateBinaryStatic_core2( pxStaticSemaphore_core2 )    xQueueGenericCreateStatic_core2( ( UBaseType_t_core2 ) 1, semSEMAPHORE_QUEUE_ITEM_LENGTH_core2, NULL, ( pxStaticSemaphore_core2 ), queueQUEUE_TYPE_BINARY_SEMAPHORE_core2 )
#endif /* configSUPPORT_STATIC_ALLOCATION_core2 */

/**
 * semphr. h
 * @code{c}
 * xSemaphoreTake_core2(
 *                   SemaphoreHandle_t_core2 xSemaphore_core2,
 *                   TickType_t_core2 xBlockTime_core2
 *               );
 * @endcode
 *
 * <i>Macro</i> to obtain a semaphore.  The semaphore must have previously been
 * created with a call to xSemaphoreCreateBinary_core2(), xSemaphoreCreateMutex_core2() or
 * xSemaphoreCreateCounting_core2().
 *
 * @param xSemaphore_core2 A handle to the semaphore being taken - obtained when
 * the semaphore was created.
 *
 * @param xBlockTime_core2 The time in ticks to wait for the semaphore to become
 * available.  The macro portTICK_PERIOD_MS_core2 can be used to convert this to a
 * real time.  A block time of zero can be used to poll the semaphore.  A block
 * time of portMAX_DELAY_core2 can be used to block indefinitely (provided
 * INCLUDE_vTaskSuspend_core2 is set to 1 in FreeRTOSConfig.h).
 *
 * @return pdTRUE_core2 if the semaphore was obtained.  pdFALSE_core2
 * if xBlockTime_core2 expired without the semaphore becoming available.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphore_core2 = NULL;
 *
 * // A task_core2 that creates a semaphore.
 * void vATask( void * pvParameters_core2 )
 * {
 *  // Create the semaphore to guard a shared resource.
 *  xSemaphore_core2 = xSemaphoreCreateBinary_core2();
 * }
 *
 * // A task_core2 that uses the semaphore.
 * void vAnotherTask( void * pvParameters_core2 )
 * {
 *  // ... Do other things.
 *
 *  if( xSemaphore_core2 != NULL )
 *  {
 *      // See if we can obtain the semaphore.  If the semaphore is not available
 *      // wait 10 ticks to see if it becomes free.
 *      if( xSemaphoreTake_core2( xSemaphore_core2, ( TickType_t_core2 ) 10 ) == pdTRUE_core2 )
 *      {
 *          // We were able to obtain the semaphore and can now access the
 *          // shared resource.
 *
 *          // ...
 *
 *          // We have finished accessing the shared resource.  Release the
 *          // semaphore.
 *          xSemaphoreGive_core2( xSemaphore_core2 );
 *      }
 *      else
 *      {
 *          // We could not obtain the semaphore and can therefore not access
 *          // the shared resource safely.
 *      }
 *  }
 * }
 * @endcode
 * \defgroup xSemaphoreTake_core2 xSemaphoreTake_core2
 * \ingroup Semaphores
 */
#define xSemaphoreTake_core2( xSemaphore_core2, xBlockTime_core2 )    xQueueSemaphoreTake_core2( ( xSemaphore_core2 ), ( xBlockTime_core2 ) )

/**
 * semphr. h
 * @code{c}
 * xSemaphoreTakeRecursive_core2(
 *                          SemaphoreHandle_t_core2 xMutex_core2,
 *                          TickType_t_core2 xBlockTime_core2
 *                        );
 * @endcode
 *
 * <i>Macro</i> to recursively obtain, or 'take', a mutex type semaphore.
 * The mutex must have previously been created using a call to
 * xSemaphoreCreateRecursiveMutex_core2();
 *
 * configUSE_RECURSIVE_MUTEXES_core2 must be set to 1 in FreeRTOSConfig.h for this
 * macro to be available.
 *
 * This macro must not be used on mutexes created using xSemaphoreCreateMutex_core2().
 *
 * A mutex used recursively can be 'taken' repeatedly by the owner. The mutex
 * doesn't become available again until the owner has called
 * xSemaphoreGiveRecursive_core2() for each successful 'take' request.  For example,
 * if a task_core2 successfully 'takes' the same mutex 5 times then the mutex will
 * not be available to any other task_core2 until it has also  'given' the mutex back
 * exactly five times.
 *
 * @param xMutex_core2 A handle to the mutex being obtained.  This is the
 * handle returned by xSemaphoreCreateRecursiveMutex_core2();
 *
 * @param xBlockTime_core2 The time in ticks to wait for the semaphore to become
 * available.  The macro portTICK_PERIOD_MS_core2 can be used to convert this to a
 * real time.  A block time of zero can be used to poll the semaphore.  If
 * the task_core2 already owns the semaphore then xSemaphoreTakeRecursive_core2() will
 * return immediately no matter what the value of xBlockTime_core2.
 *
 * @return pdTRUE_core2 if the semaphore was obtained.  pdFALSE_core2 if xBlockTime_core2
 * expired without the semaphore becoming available.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core2 xMutex_core2 = NULL;
 *
 * // A task_core2 that creates a mutex.
 * void vATask( void * pvParameters_core2 )
 * {
 *  // Create the mutex to guard a shared resource.
 *  xMutex_core2 = xSemaphoreCreateRecursiveMutex_core2();
 * }
 *
 * // A task_core2 that uses the mutex.
 * void vAnotherTask( void * pvParameters_core2 )
 * {
 *  // ... Do other things.
 *
 *  if( xMutex_core2 != NULL )
 *  {
 *      // See if we can obtain the mutex.  If the mutex is not available
 *      // wait 10 ticks to see if it becomes free.
 *      if( xSemaphoreTakeRecursive_core2( xSemaphore_core2, ( TickType_t_core2 ) 10 ) == pdTRUE_core2 )
 *      {
 *          // We were able to obtain the mutex and can now access the
 *          // shared resource.
 *
 *          // ...
 *          // For some reason due to the nature of the code further calls to
 *          // xSemaphoreTakeRecursive_core2() are made on the same mutex.  In real
 *          // code these would not be just sequential calls as this would make
 *          // no sense.  Instead the calls are likely to be buried inside
 *          // a more complex call structure.
 *          xSemaphoreTakeRecursive_core2( xMutex_core2, ( TickType_t_core2 ) 10 );
 *          xSemaphoreTakeRecursive_core2( xMutex_core2, ( TickType_t_core2 ) 10 );
 *
 *          // The mutex has now been 'taken' three times, so will not be
 *          // available to another task_core2 until it has also been given back
 *          // three times.  Again it is unlikely that real code would have
 *          // these calls sequentially, but instead buried in a more complex
 *          // call structure.  This is just for illustrative purposes.
 *          xSemaphoreGiveRecursive_core2( xMutex_core2 );
 *          xSemaphoreGiveRecursive_core2( xMutex_core2 );
 *          xSemaphoreGiveRecursive_core2( xMutex_core2 );
 *
 *          // Now the mutex can be taken by other tasks.
 *      }
 *      else
 *      {
 *          // We could not obtain the mutex and can therefore not access
 *          // the shared resource safely.
 *      }
 *  }
 * }
 * @endcode
 * \defgroup xSemaphoreTakeRecursive_core2 xSemaphoreTakeRecursive_core2
 * \ingroup Semaphores
 */
#if ( configUSE_RECURSIVE_MUTEXES_core2 == 1 )
    #define xSemaphoreTakeRecursive_core2( xMutex_core2, xBlockTime_core2 )    xQueueTakeMutexRecursive_core2( ( xMutex_core2 ), ( xBlockTime_core2 ) )
#endif

/**
 * semphr. h
 * @code{c}
 * xSemaphoreGive_core2( SemaphoreHandle_t_core2 xSemaphore_core2 );
 * @endcode
 *
 * <i>Macro</i> to release a semaphore.  The semaphore must have previously been
 * created with a call to xSemaphoreCreateBinary_core2(), xSemaphoreCreateMutex_core2() or
 * xSemaphoreCreateCounting_core2(). and obtained using sSemaphoreTake().
 *
 * This macro must not be used from an ISR.  See xSemaphoreGiveFromISR_core2 () for
 * an alternative which can be used from an ISR.
 *
 * This macro must also not be used on semaphores created using
 * xSemaphoreCreateRecursiveMutex_core2().
 *
 * @param xSemaphore_core2 A handle to the semaphore being released.  This is the
 * handle returned when the semaphore was created.
 *
 * @return pdTRUE_core2 if the semaphore was released.  pdFALSE_core2 if an error occurred.
 * Semaphores are implemented using queues.  An error can occur if there is
 * no space on the queue to post a message - indicating that the
 * semaphore was not first obtained correctly.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphore_core2 = NULL;
 *
 * void vATask( void * pvParameters_core2 )
 * {
 *  // Create the semaphore to guard a shared resource.
 *  xSemaphore_core2 = vSemaphoreCreateBinary_core2();
 *
 *  if( xSemaphore_core2 != NULL )
 *  {
 *      if( xSemaphoreGive_core2( xSemaphore_core2 ) != pdTRUE_core2 )
 *      {
 *          // We would expect this call to fail because we cannot give
 *          // a semaphore without first "taking" it!
 *      }
 *
 *      // Obtain the semaphore - don't block if the semaphore is not
 *      // immediately available.
 *      if( xSemaphoreTake_core2( xSemaphore_core2, ( TickType_t_core2 ) 0 ) )
 *      {
 *          // We now have the semaphore and can access the shared resource.
 *
 *          // ...
 *
 *          // We have finished accessing the shared resource so can free the
 *          // semaphore.
 *          if( xSemaphoreGive_core2( xSemaphore_core2 ) != pdTRUE_core2 )
 *          {
 *              // We would not expect this call to fail because we must have
 *              // obtained the semaphore to get here.
 *          }
 *      }
 *  }
 * }
 * @endcode
 * \defgroup xSemaphoreGive_core2 xSemaphoreGive_core2
 * \ingroup Semaphores
 */
#define xSemaphoreGive_core2( xSemaphore_core2 )    xQueueGenericSend_core2( ( QueueHandle_t_core2 ) ( xSemaphore_core2 ), NULL, semGIVE_BLOCK_TIME_core2, queueSEND_TO_BACK_core2 )

/**
 * semphr. h
 * @code{c}
 * xSemaphoreGiveRecursive_core2( SemaphoreHandle_t_core2 xMutex_core2 );
 * @endcode
 *
 * <i>Macro</i> to recursively release, or 'give', a mutex type semaphore.
 * The mutex must have previously been created using a call to
 * xSemaphoreCreateRecursiveMutex_core2();
 *
 * configUSE_RECURSIVE_MUTEXES_core2 must be set to 1 in FreeRTOSConfig.h for this
 * macro to be available.
 *
 * This macro must not be used on mutexes created using xSemaphoreCreateMutex_core2().
 *
 * A mutex used recursively can be 'taken' repeatedly by the owner. The mutex
 * doesn't become available again until the owner has called
 * xSemaphoreGiveRecursive_core2() for each successful 'take' request.  For example,
 * if a task_core2 successfully 'takes' the same mutex 5 times then the mutex will
 * not be available to any other task_core2 until it has also  'given' the mutex back
 * exactly five times.
 *
 * @param xMutex_core2 A handle to the mutex being released, or 'given'.  This is the
 * handle returned by xSemaphoreCreateMutex_core2();
 *
 * @return pdTRUE_core2 if the semaphore was given.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core2 xMutex_core2 = NULL;
 *
 * // A task_core2 that creates a mutex.
 * void vATask( void * pvParameters_core2 )
 * {
 *  // Create the mutex to guard a shared resource.
 *  xMutex_core2 = xSemaphoreCreateRecursiveMutex_core2();
 * }
 *
 * // A task_core2 that uses the mutex.
 * void vAnotherTask( void * pvParameters_core2 )
 * {
 *  // ... Do other things.
 *
 *  if( xMutex_core2 != NULL )
 *  {
 *      // See if we can obtain the mutex.  If the mutex is not available
 *      // wait 10 ticks to see if it becomes free.
 *      if( xSemaphoreTakeRecursive_core2( xMutex_core2, ( TickType_t_core2 ) 10 ) == pdTRUE_core2 )
 *      {
 *          // We were able to obtain the mutex and can now access the
 *          // shared resource.
 *
 *          // ...
 *          // For some reason due to the nature of the code further calls to
 *          // xSemaphoreTakeRecursive_core2() are made on the same mutex.  In real
 *          // code these would not be just sequential calls as this would make
 *          // no sense.  Instead the calls are likely to be buried inside
 *          // a more complex call structure.
 *          xSemaphoreTakeRecursive_core2( xMutex_core2, ( TickType_t_core2 ) 10 );
 *          xSemaphoreTakeRecursive_core2( xMutex_core2, ( TickType_t_core2 ) 10 );
 *
 *          // The mutex has now been 'taken' three times, so will not be
 *          // available to another task_core2 until it has also been given back
 *          // three times.  Again it is unlikely that real code would have
 *          // these calls sequentially, it would be more likely that the calls
 *          // to xSemaphoreGiveRecursive_core2() would be called as a call stack
 *          // unwound.  This is just for demonstrative purposes.
 *          xSemaphoreGiveRecursive_core2( xMutex_core2 );
 *          xSemaphoreGiveRecursive_core2( xMutex_core2 );
 *          xSemaphoreGiveRecursive_core2( xMutex_core2 );
 *
 *          // Now the mutex can be taken by other tasks.
 *      }
 *      else
 *      {
 *          // We could not obtain the mutex and can therefore not access
 *          // the shared resource safely.
 *      }
 *  }
 * }
 * @endcode
 * \defgroup xSemaphoreGiveRecursive_core2 xSemaphoreGiveRecursive_core2
 * \ingroup Semaphores
 */
#if ( configUSE_RECURSIVE_MUTEXES_core2 == 1 )
    #define xSemaphoreGiveRecursive_core2( xMutex_core2 )    xQueueGiveMutexRecursive_core2( ( xMutex_core2 ) )
#endif

/**
 * semphr. h
 * @code{c}
 * xSemaphoreGiveFromISR_core2(
 *                        SemaphoreHandle_t_core2 xSemaphore_core2,
 *                        BaseType_t_core2 *pxHigherPriorityTaskWoken_core2
 *                    );
 * @endcode
 *
 * <i>Macro</i> to  release a semaphore.  The semaphore must have previously been
 * created with a call to xSemaphoreCreateBinary_core2() or xSemaphoreCreateCounting_core2().
 *
 * Mutex type semaphores (those created using a call to xSemaphoreCreateMutex_core2())
 * must not be used with this macro.
 *
 * This macro can be used from an ISR.
 *
 * @param xSemaphore_core2 A handle to the semaphore being released.  This is the
 * handle returned when the semaphore was created.
 *
 * @param pxHigherPriorityTaskWoken_core2 xSemaphoreGiveFromISR_core2() will set
 * *pxHigherPriorityTaskWoken_core2 to pdTRUE_core2 if giving the semaphore caused a task_core2
 * to unblock, and the unblocked task_core2 has a priority higher than the currently
 * running task_core2.  If xSemaphoreGiveFromISR_core2() sets this value to pdTRUE_core2 then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE_core2 if the semaphore was successfully given, otherwise errQUEUE_FULL_core2.
 *
 * Example usage:
 * @code{c}
 \#define LONG_TIME 0xffff
 \#define TICKS_TO_WAIT 10
 * SemaphoreHandle_t_core2 xSemaphore_core2 = NULL;
 *
 * // Repetitive task_core2.
 * void vATask( void * pvParameters_core2 )
 * {
 *  for( ;; )
 *  {
 *      // We want this task_core2 to run every 10 ticks of a timer.  The semaphore
 *      // was created before this task_core2 was started.
 *
 *      // Block waiting for the semaphore to become available.
 *      if( xSemaphoreTake_core2( xSemaphore_core2, LONG_TIME ) == pdTRUE_core2 )
 *      {
 *          // It is time to execute.
 *
 *          // ...
 *
 *          // We have finished our task_core2.  Return to the top of the loop where
 *          // we will block on the semaphore until it is time to execute
 *          // again.  Note when using the semaphore for synchronisation with an
 *          // ISR in this manner there is no need to 'give' the semaphore back.
 *      }
 *  }
 * }
 *
 * // Timer ISR
 * void vTimerISR( void * pvParameters_core2 )
 * {
 * static uint8_t ucLocalTickCount = 0;
 * static BaseType_t_core2 xHigherPriorityTaskWoken_core2;
 *
 *  // A timer tick has occurred.
 *
 *  // ... Do other time functions.
 *
 *  // Is it time for vATask () to run?
 *  xHigherPriorityTaskWoken_core2 = pdFALSE_core2;
 *  ucLocalTickCount++;
 *  if( ucLocalTickCount >= TICKS_TO_WAIT )
 *  {
 *      // Unblock the task_core2 by releasing the semaphore.
 *      xSemaphoreGiveFromISR_core2( xSemaphore_core2, &xHigherPriorityTaskWoken_core2 );
 *
 *      // Reset the count so we release the semaphore again in 10 ticks time.
 *      ucLocalTickCount = 0;
 *  }
 *
 *  if( xHigherPriorityTaskWoken_core2 != pdFALSE_core2 )
 *  {
 *      // We can force a context switch here.  Context switching from an
 *      // ISR uses port specific syntax.  Check the demo task_core2 for your port
 *      // to find the syntax required.
 *  }
 * }
 * @endcode
 * \defgroup xSemaphoreGiveFromISR_core2 xSemaphoreGiveFromISR_core2
 * \ingroup Semaphores
 */
#define xSemaphoreGiveFromISR_core2( xSemaphore_core2, pxHigherPriorityTaskWoken_core2 )    xQueueGiveFromISR_core2( ( QueueHandle_t_core2 ) ( xSemaphore_core2 ), ( pxHigherPriorityTaskWoken_core2 ) )

/**
 * semphr. h
 * @code{c}
 * xSemaphoreTakeFromISR_core2(
 *                        SemaphoreHandle_t_core2 xSemaphore_core2,
 *                        BaseType_t_core2 *pxHigherPriorityTaskWoken_core2
 *                    );
 * @endcode
 *
 * <i>Macro</i> to  take a semaphore from an ISR.  The semaphore must have
 * previously been created with a call to xSemaphoreCreateBinary_core2() or
 * xSemaphoreCreateCounting_core2().
 *
 * Mutex type semaphores (those created using a call to xSemaphoreCreateMutex_core2())
 * must not be used with this macro.
 *
 * This macro can be used from an ISR, however taking a semaphore from an ISR
 * is not a common operation.  It is likely to only be useful when taking a
 * counting semaphore when an interrupt is obtaining an object from a resource
 * pool (when the semaphore count indicates the number of resources available).
 *
 * @param xSemaphore_core2 A handle to the semaphore being taken.  This is the
 * handle returned when the semaphore was created.
 *
 * @param pxHigherPriorityTaskWoken_core2 xSemaphoreTakeFromISR_core2() will set
 * *pxHigherPriorityTaskWoken_core2 to pdTRUE_core2 if taking the semaphore caused a task_core2
 * to unblock, and the unblocked task_core2 has a priority higher than the currently
 * running task_core2.  If xSemaphoreTakeFromISR_core2() sets this value to pdTRUE_core2 then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE_core2 if the semaphore was successfully taken, otherwise
 * pdFALSE_core2
 */
#define xSemaphoreTakeFromISR_core2( xSemaphore_core2, pxHigherPriorityTaskWoken_core2 )    xQueueReceiveFromISR_core2( ( QueueHandle_t_core2 ) ( xSemaphore_core2 ), NULL, ( pxHigherPriorityTaskWoken_core2 ) )

/**
 * semphr. h
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphoreCreateMutex_core2( void );
 * @endcode
 *
 * Creates a new mutex type semaphore instance, and returns a handle by which
 * the new mutex can be referenced.
 *
 * Internally, within the FreeRTOS_core2 implementation, mutex semaphores use a block
 * of memory, in which the mutex structure is stored.  If a mutex is created
 * using xSemaphoreCreateMutex_core2() then the required memory is automatically
 * dynamically allocated inside the xSemaphoreCreateMutex_core2() function.  (see
 * https://www.FreeRTOS_core2.org/a00111.html).  If a mutex is created using
 * xSemaphoreCreateMutexStatic_core2() then the application writer must provided the
 * memory.  xSemaphoreCreateMutexStatic_core2() therefore allows a mutex to be created
 * without using any dynamic memory allocation.
 *
 * Mutexes created using this function can be accessed using the xSemaphoreTake_core2()
 * and xSemaphoreGive_core2() macros.  The xSemaphoreTakeRecursive_core2() and
 * xSemaphoreGiveRecursive_core2() macros must not be used.
 *
 * This type of semaphore uses a priority inheritance mechanism so a task_core2
 * 'taking' a semaphore MUST ALWAYS 'give' the semaphore back once the
 * semaphore it is no longer required.
 *
 * Mutex type semaphores cannot be used from within interrupt service routines.
 *
 * See xSemaphoreCreateBinary_core2() for an alternative implementation that can be
 * used for pure synchronisation (where one task_core2 or interrupt always 'gives' the
 * semaphore and another always 'takes' the semaphore) and from within interrupt
 * service routines.
 *
 * @return If the mutex was successfully created then a handle to the created
 * semaphore is returned.  If there was not enough heap to allocate the mutex
 * data structures then NULL is returned.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphore_core2;
 *
 * void vATask( void * pvParameters_core2 )
 * {
 *  // Semaphore cannot be used before a call to xSemaphoreCreateMutex_core2().
 *  // This is a macro so pass the variable in directly.
 *  xSemaphore_core2 = xSemaphoreCreateMutex_core2();
 *
 *  if( xSemaphore_core2 != NULL )
 *  {
 *      // The semaphore was created successfully.
 *      // The semaphore can now be used.
 *  }
 * }
 * @endcode
 * \defgroup xSemaphoreCreateMutex_core2 xSemaphoreCreateMutex_core2
 * \ingroup Semaphores
 */
#if ( ( configSUPPORT_DYNAMIC_ALLOCATION_core2 == 1 ) && ( configUSE_MUTEXES_core2 == 1 ) )
    #define xSemaphoreCreateMutex_core2()    xQueueCreateMutex_core2( queueQUEUE_TYPE_MUTEX_core2 )
#endif

/**
 * semphr. h
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphoreCreateMutexStatic_core2( StaticSemaphore_t_core2 *pxMutexBuffer_core2 );
 * @endcode
 *
 * Creates a new mutex type semaphore instance, and returns a handle by which
 * the new mutex can be referenced.
 *
 * Internally, within the FreeRTOS_core2 implementation, mutex semaphores use a block
 * of memory, in which the mutex structure is stored.  If a mutex is created
 * using xSemaphoreCreateMutex_core2() then the required memory is automatically
 * dynamically allocated inside the xSemaphoreCreateMutex_core2() function.  (see
 * https://www.FreeRTOS_core2.org/a00111.html).  If a mutex is created using
 * xSemaphoreCreateMutexStatic_core2() then the application writer must provided the
 * memory.  xSemaphoreCreateMutexStatic_core2() therefore allows a mutex to be created
 * without using any dynamic memory allocation.
 *
 * Mutexes created using this function can be accessed using the xSemaphoreTake_core2()
 * and xSemaphoreGive_core2() macros.  The xSemaphoreTakeRecursive_core2() and
 * xSemaphoreGiveRecursive_core2() macros must not be used.
 *
 * This type of semaphore uses a priority inheritance mechanism so a task_core2
 * 'taking' a semaphore MUST ALWAYS 'give' the semaphore back once the
 * semaphore it is no longer required.
 *
 * Mutex type semaphores cannot be used from within interrupt service routines.
 *
 * See xSemaphoreCreateBinary_core2() for an alternative implementation that can be
 * used for pure synchronisation (where one task_core2 or interrupt always 'gives' the
 * semaphore and another always 'takes' the semaphore) and from within interrupt
 * service routines.
 *
 * @param pxMutexBuffer_core2 Must point to a variable of type StaticSemaphore_t_core2,
 * which will be used to hold the mutex's data structure, removing the need for
 * the memory to be allocated dynamically.
 *
 * @return If the mutex was successfully created then a handle to the created
 * mutex is returned.  If pxMutexBuffer_core2 was NULL then NULL is returned.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphore_core2;
 * StaticSemaphore_t_core2 xMutexBuffer;
 *
 * void vATask( void * pvParameters_core2 )
 * {
 *  // A mutex cannot be used before it has been created.  xMutexBuffer is
 *  // into xSemaphoreCreateMutexStatic_core2() so no dynamic memory allocation is
 *  // attempted.
 *  xSemaphore_core2 = xSemaphoreCreateMutexStatic_core2( &xMutexBuffer );
 *
 *  // As no dynamic memory allocation was performed, xSemaphore_core2 cannot be NULL,
 *  // so there is no need to check it.
 * }
 * @endcode
 * \defgroup xSemaphoreCreateMutexStatic_core2 xSemaphoreCreateMutexStatic_core2
 * \ingroup Semaphores
 */
#if ( ( configSUPPORT_STATIC_ALLOCATION_core2 == 1 ) && ( configUSE_MUTEXES_core2 == 1 ) )
    #define xSemaphoreCreateMutexStatic_core2( pxMutexBuffer_core2 )    xQueueCreateMutexStatic_core2( queueQUEUE_TYPE_MUTEX_core2, ( pxMutexBuffer_core2 ) )
#endif


/**
 * semphr. h
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphoreCreateRecursiveMutex_core2( void );
 * @endcode
 *
 * Creates a new recursive mutex type semaphore instance, and returns a handle
 * by which the new recursive mutex can be referenced.
 *
 * Internally, within the FreeRTOS_core2 implementation, recursive mutexes use a block
 * of memory, in which the mutex structure is stored.  If a recursive mutex is
 * created using xSemaphoreCreateRecursiveMutex_core2() then the required memory is
 * automatically dynamically allocated inside the
 * xSemaphoreCreateRecursiveMutex_core2() function.  (see
 * https://www.FreeRTOS_core2.org/a00111.html).  If a recursive mutex is created using
 * xSemaphoreCreateRecursiveMutexStatic_core2() then the application writer must
 * provide the memory that will get used by the mutex.
 * xSemaphoreCreateRecursiveMutexStatic_core2() therefore allows a recursive mutex to
 * be created without using any dynamic memory allocation.
 *
 * Mutexes created using this macro can be accessed using the
 * xSemaphoreTakeRecursive_core2() and xSemaphoreGiveRecursive_core2() macros.  The
 * xSemaphoreTake_core2() and xSemaphoreGive_core2() macros must not be used.
 *
 * A mutex used recursively can be 'taken' repeatedly by the owner. The mutex
 * doesn't become available again until the owner has called
 * xSemaphoreGiveRecursive_core2() for each successful 'take' request.  For example,
 * if a task_core2 successfully 'takes' the same mutex 5 times then the mutex will
 * not be available to any other task_core2 until it has also  'given' the mutex back
 * exactly five times.
 *
 * This type of semaphore uses a priority inheritance mechanism so a task_core2
 * 'taking' a semaphore MUST ALWAYS 'give' the semaphore back once the
 * semaphore it is no longer required.
 *
 * Mutex type semaphores cannot be used from within interrupt service routines.
 *
 * See xSemaphoreCreateBinary_core2() for an alternative implementation that can be
 * used for pure synchronisation (where one task_core2 or interrupt always 'gives' the
 * semaphore and another always 'takes' the semaphore) and from within interrupt
 * service routines.
 *
 * @return xSemaphore_core2 Handle to the created mutex semaphore.  Should be of type
 * SemaphoreHandle_t_core2.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphore_core2;
 *
 * void vATask( void * pvParameters_core2 )
 * {
 *  // Semaphore cannot be used before a call to xSemaphoreCreateMutex_core2().
 *  // This is a macro so pass the variable in directly.
 *  xSemaphore_core2 = xSemaphoreCreateRecursiveMutex_core2();
 *
 *  if( xSemaphore_core2 != NULL )
 *  {
 *      // The semaphore was created successfully.
 *      // The semaphore can now be used.
 *  }
 * }
 * @endcode
 * \defgroup xSemaphoreCreateRecursiveMutex_core2 xSemaphoreCreateRecursiveMutex_core2
 * \ingroup Semaphores
 */
#if ( ( configSUPPORT_DYNAMIC_ALLOCATION_core2 == 1 ) && ( configUSE_RECURSIVE_MUTEXES_core2 == 1 ) )
    #define xSemaphoreCreateRecursiveMutex_core2()    xQueueCreateMutex_core2( queueQUEUE_TYPE_RECURSIVE_MUTEX_core2 )
#endif

/**
 * semphr. h
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphoreCreateRecursiveMutexStatic_core2( StaticSemaphore_t_core2 *pxMutexBuffer_core2 );
 * @endcode
 *
 * Creates a new recursive mutex type semaphore instance, and returns a handle
 * by which the new recursive mutex can be referenced.
 *
 * Internally, within the FreeRTOS_core2 implementation, recursive mutexes use a block
 * of memory, in which the mutex structure is stored.  If a recursive mutex is
 * created using xSemaphoreCreateRecursiveMutex_core2() then the required memory is
 * automatically dynamically allocated inside the
 * xSemaphoreCreateRecursiveMutex_core2() function.  (see
 * https://www.FreeRTOS_core2.org/a00111.html).  If a recursive mutex is created using
 * xSemaphoreCreateRecursiveMutexStatic_core2() then the application writer must
 * provide the memory that will get used by the mutex.
 * xSemaphoreCreateRecursiveMutexStatic_core2() therefore allows a recursive mutex to
 * be created without using any dynamic memory allocation.
 *
 * Mutexes created using this macro can be accessed using the
 * xSemaphoreTakeRecursive_core2() and xSemaphoreGiveRecursive_core2() macros.  The
 * xSemaphoreTake_core2() and xSemaphoreGive_core2() macros must not be used.
 *
 * A mutex used recursively can be 'taken' repeatedly by the owner. The mutex
 * doesn't become available again until the owner has called
 * xSemaphoreGiveRecursive_core2() for each successful 'take' request.  For example,
 * if a task_core2 successfully 'takes' the same mutex 5 times then the mutex will
 * not be available to any other task_core2 until it has also  'given' the mutex back
 * exactly five times.
 *
 * This type of semaphore uses a priority inheritance mechanism so a task_core2
 * 'taking' a semaphore MUST ALWAYS 'give' the semaphore back once the
 * semaphore it is no longer required.
 *
 * Mutex type semaphores cannot be used from within interrupt service routines.
 *
 * See xSemaphoreCreateBinary_core2() for an alternative implementation that can be
 * used for pure synchronisation (where one task_core2 or interrupt always 'gives' the
 * semaphore and another always 'takes' the semaphore) and from within interrupt
 * service routines.
 *
 * @param pxMutexBuffer_core2 Must point to a variable of type StaticSemaphore_t_core2,
 * which will then be used to hold the recursive mutex's data structure,
 * removing the need for the memory to be allocated dynamically.
 *
 * @return If the recursive mutex was successfully created then a handle to the
 * created recursive mutex is returned.  If pxMutexBuffer_core2 was NULL then NULL is
 * returned.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphore_core2;
 * StaticSemaphore_t_core2 xMutexBuffer;
 *
 * void vATask( void * pvParameters_core2 )
 * {
 *  // A recursive semaphore cannot be used before it is created.  Here a
 *  // recursive mutex is created using xSemaphoreCreateRecursiveMutexStatic_core2().
 *  // The address of xMutexBuffer is passed into the function, and will hold
 *  // the mutexes data structures - so no dynamic memory allocation will be
 *  // attempted.
 *  xSemaphore_core2 = xSemaphoreCreateRecursiveMutexStatic_core2( &xMutexBuffer );
 *
 *  // As no dynamic memory allocation was performed, xSemaphore_core2 cannot be NULL,
 *  // so there is no need to check it.
 * }
 * @endcode
 * \defgroup xSemaphoreCreateRecursiveMutexStatic_core2 xSemaphoreCreateRecursiveMutexStatic_core2
 * \ingroup Semaphores
 */
#if ( ( configSUPPORT_STATIC_ALLOCATION_core2 == 1 ) && ( configUSE_RECURSIVE_MUTEXES_core2 == 1 ) )
    #define xSemaphoreCreateRecursiveMutexStatic_core2( pxStaticSemaphore_core2 )    xQueueCreateMutexStatic_core2( queueQUEUE_TYPE_RECURSIVE_MUTEX_core2, ( pxStaticSemaphore_core2 ) )
#endif /* configSUPPORT_STATIC_ALLOCATION_core2 */

/**
 * semphr. h
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphoreCreateCounting_core2( UBaseType_t_core2 uxMaxCount_core2, UBaseType_t_core2 uxInitialCount_core2 );
 * @endcode
 *
 * Creates a new counting semaphore instance, and returns a handle by which the
 * new counting semaphore can be referenced.
 *
 * In many usage scenarios it is faster and more memory efficient to use a
 * direct to task_core2 notification in place of a counting semaphore!
 * https://www.FreeRTOS_core2.org/RTOS-task_core2-notifications.html
 *
 * Internally, within the FreeRTOS_core2 implementation, counting semaphores use a
 * block of memory, in which the counting semaphore structure is stored.  If a
 * counting semaphore is created using xSemaphoreCreateCounting_core2() then the
 * required memory is automatically dynamically allocated inside the
 * xSemaphoreCreateCounting_core2() function.  (see
 * https://www.FreeRTOS_core2.org/a00111.html).  If a counting semaphore is created
 * using xSemaphoreCreateCountingStatic_core2() then the application writer can
 * instead optionally provide the memory that will get used by the counting
 * semaphore.  xSemaphoreCreateCountingStatic_core2() therefore allows a counting
 * semaphore to be created without using any dynamic memory allocation.
 *
 * Counting semaphores are typically used for two things:
 *
 * 1) Counting events.
 *
 *    In this usage scenario an event handler will 'give' a semaphore each time
 *    an event occurs (incrementing the semaphore count value), and a handler
 *    task_core2 will 'take' a semaphore each time it processes an event
 *    (decrementing the semaphore count value).  The count value is therefore
 *    the difference between the number of events that have occurred and the
 *    number that have been processed.  In this case it is desirable for the
 *    initial count value to be zero.
 *
 * 2) Resource management.
 *
 *    In this usage scenario the count value indicates the number of resources
 *    available.  To obtain control of a resource a task_core2 must first obtain a
 *    semaphore - decrementing the semaphore count value.  When the count value
 *    reaches zero there are no free resources.  When a task_core2 finishes with the
 *    resource it 'gives' the semaphore back - incrementing the semaphore count
 *    value.  In this case it is desirable for the initial count value to be
 *    equal to the maximum count value, indicating that all resources are free.
 *
 * @param uxMaxCount_core2 The maximum count value that can be reached.  When the
 *        semaphore reaches this value it can no longer be 'given'.
 *
 * @param uxInitialCount_core2 The count value assigned to the semaphore when it is
 *        created.
 *
 * @return Handle to the created semaphore.  Null if the semaphore could not be
 *         created.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphore_core2;
 *
 * void vATask( void * pvParameters_core2 )
 * {
 * SemaphoreHandle_t_core2 xSemaphore_core2 = NULL;
 *
 *  // Semaphore cannot be used before a call to xSemaphoreCreateCounting_core2().
 *  // The max value to which the semaphore can count should be 10, and the
 *  // initial value assigned to the count should be 0.
 *  xSemaphore_core2 = xSemaphoreCreateCounting_core2( 10, 0 );
 *
 *  if( xSemaphore_core2 != NULL )
 *  {
 *      // The semaphore was created successfully.
 *      // The semaphore can now be used.
 *  }
 * }
 * @endcode
 * \defgroup xSemaphoreCreateCounting_core2 xSemaphoreCreateCounting_core2
 * \ingroup Semaphores
 */
#if ( configSUPPORT_DYNAMIC_ALLOCATION_core2 == 1 )
    #define xSemaphoreCreateCounting_core2( uxMaxCount_core2, uxInitialCount_core2 )    xQueueCreateCountingSemaphore_core2( ( uxMaxCount_core2 ), ( uxInitialCount_core2 ) )
#endif

/**
 * semphr. h
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphoreCreateCountingStatic_core2( UBaseType_t_core2 uxMaxCount_core2, UBaseType_t_core2 uxInitialCount_core2, StaticSemaphore_t_core2 *pxSemaphoreBuffer_core2 );
 * @endcode
 *
 * Creates a new counting semaphore instance, and returns a handle by which the
 * new counting semaphore can be referenced.
 *
 * In many usage scenarios it is faster and more memory efficient to use a
 * direct to task_core2 notification in place of a counting semaphore!
 * https://www.FreeRTOS_core2.org/RTOS-task_core2-notifications.html
 *
 * Internally, within the FreeRTOS_core2 implementation, counting semaphores use a
 * block of memory, in which the counting semaphore structure is stored.  If a
 * counting semaphore is created using xSemaphoreCreateCounting_core2() then the
 * required memory is automatically dynamically allocated inside the
 * xSemaphoreCreateCounting_core2() function.  (see
 * https://www.FreeRTOS_core2.org/a00111.html).  If a counting semaphore is created
 * using xSemaphoreCreateCountingStatic_core2() then the application writer must
 * provide the memory.  xSemaphoreCreateCountingStatic_core2() therefore allows a
 * counting semaphore to be created without using any dynamic memory allocation.
 *
 * Counting semaphores are typically used for two things:
 *
 * 1) Counting events.
 *
 *    In this usage scenario an event handler will 'give' a semaphore each time
 *    an event occurs (incrementing the semaphore count value), and a handler
 *    task_core2 will 'take' a semaphore each time it processes an event
 *    (decrementing the semaphore count value).  The count value is therefore
 *    the difference between the number of events that have occurred and the
 *    number that have been processed.  In this case it is desirable for the
 *    initial count value to be zero.
 *
 * 2) Resource management.
 *
 *    In this usage scenario the count value indicates the number of resources
 *    available.  To obtain control of a resource a task_core2 must first obtain a
 *    semaphore - decrementing the semaphore count value.  When the count value
 *    reaches zero there are no free resources.  When a task_core2 finishes with the
 *    resource it 'gives' the semaphore back - incrementing the semaphore count
 *    value.  In this case it is desirable for the initial count value to be
 *    equal to the maximum count value, indicating that all resources are free.
 *
 * @param uxMaxCount_core2 The maximum count value that can be reached.  When the
 *        semaphore reaches this value it can no longer be 'given'.
 *
 * @param uxInitialCount_core2 The count value assigned to the semaphore when it is
 *        created.
 *
 * @param pxSemaphoreBuffer_core2 Must point to a variable of type StaticSemaphore_t_core2,
 * which will then be used to hold the semaphore's data structure, removing the
 * need for the memory to be allocated dynamically.
 *
 * @return If the counting semaphore was successfully created then a handle to
 * the created counting semaphore is returned.  If pxSemaphoreBuffer_core2 was NULL
 * then NULL is returned.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core2 xSemaphore_core2;
 * StaticSemaphore_t_core2 xSemaphoreBuffer;
 *
 * void vATask( void * pvParameters_core2 )
 * {
 * SemaphoreHandle_t_core2 xSemaphore_core2 = NULL;
 *
 *  // Counting semaphore cannot be used before they have been created.  Create
 *  // a counting semaphore using xSemaphoreCreateCountingStatic_core2().  The max
 *  // value to which the semaphore can count is 10, and the initial value
 *  // assigned to the count will be 0.  The address of xSemaphoreBuffer is
 *  // passed in and will be used to hold the semaphore structure, so no dynamic
 *  // memory allocation will be used.
 *  xSemaphore_core2 = xSemaphoreCreateCounting_core2( 10, 0, &xSemaphoreBuffer );
 *
 *  // No memory allocation was attempted so xSemaphore_core2 cannot be NULL, so there
 *  // is no need to check its value.
 * }
 * @endcode
 * \defgroup xSemaphoreCreateCountingStatic_core2 xSemaphoreCreateCountingStatic_core2
 * \ingroup Semaphores
 */
#if ( configSUPPORT_STATIC_ALLOCATION_core2 == 1 )
    #define xSemaphoreCreateCountingStatic_core2( uxMaxCount_core2, uxInitialCount_core2, pxSemaphoreBuffer_core2 )    xQueueCreateCountingSemaphoreStatic_core2( ( uxMaxCount_core2 ), ( uxInitialCount_core2 ), ( pxSemaphoreBuffer_core2 ) )
#endif /* configSUPPORT_STATIC_ALLOCATION_core2 */

/**
 * semphr. h
 * @code{c}
 * void vSemaphoreDelete_core2( SemaphoreHandle_t_core2 xSemaphore_core2 );
 * @endcode
 *
 * Delete a semaphore.  This function must be used with care.  For example,
 * do not delete a mutex type semaphore if the mutex is held by a task_core2.
 *
 * @param xSemaphore_core2 A handle to the semaphore to be deleted.
 *
 * \defgroup vSemaphoreDelete_core2 vSemaphoreDelete_core2
 * \ingroup Semaphores
 */
#define vSemaphoreDelete_core2( xSemaphore_core2 )    vQueueDelete_core2( ( QueueHandle_t_core2 ) ( xSemaphore_core2 ) )

/**
 * semphr.h
 * @code{c}
 * TaskHandle_t_core2 xSemaphoreGetMutexHolder_core2( SemaphoreHandle_t_core2 xMutex_core2 );
 * @endcode
 *
 * If xMutex_core2 is indeed a mutex type semaphore, return the current mutex holder.
 * If xMutex_core2 is not a mutex type semaphore, or the mutex is available (not held
 * by a task_core2), return NULL.
 *
 * Note: This is a good way of determining if the calling task_core2 is the mutex
 * holder, but not a good way of determining the identity of the mutex holder as
 * the holder may change between the function exiting and the returned value
 * being tested.
 */
#if ( ( configUSE_MUTEXES_core2 == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder_core2 == 1 ) )
    #define xSemaphoreGetMutexHolder_core2( xSemaphore_core2 )    xQueueGetMutexHolder_core2( ( xSemaphore_core2 ) )
#endif

/**
 * semphr.h
 * @code{c}
 * TaskHandle_t_core2 xSemaphoreGetMutexHolderFromISR_core2( SemaphoreHandle_t_core2 xMutex_core2 );
 * @endcode
 *
 * If xMutex_core2 is indeed a mutex type semaphore, return the current mutex holder.
 * If xMutex_core2 is not a mutex type semaphore, or the mutex is available (not held
 * by a task_core2), return NULL.
 *
 */
#if ( ( configUSE_MUTEXES_core2 == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder_core2 == 1 ) )
    #define xSemaphoreGetMutexHolderFromISR_core2( xSemaphore_core2 )    xQueueGetMutexHolderFromISR_core2( ( xSemaphore_core2 ) )
#endif

/**
 * semphr.h
 * @code{c}
 * UBaseType_t_core2 uxSemaphoreGetCount_core2( SemaphoreHandle_t_core2 xSemaphore_core2 );
 * @endcode
 *
 * If the semaphore is a counting semaphore then uxSemaphoreGetCount_core2() returns
 * its current count value.  If the semaphore is a binary semaphore then
 * uxSemaphoreGetCount_core2() returns 1 if the semaphore is available, and 0 if the
 * semaphore is not available.
 *
 */
#define uxSemaphoreGetCount_core2( xSemaphore_core2 )           uxQueueMessagesWaiting_core2( ( QueueHandle_t_core2 ) ( xSemaphore_core2 ) )

/**
 * semphr.h
 * @code{c}
 * UBaseType_t_core2 uxSemaphoreGetCountFromISR_core2( SemaphoreHandle_t_core2 xSemaphore_core2 );
 * @endcode
 *
 * If the semaphore is a counting semaphore then uxSemaphoreGetCountFromISR_core2() returns
 * its current count value.  If the semaphore is a binary semaphore then
 * uxSemaphoreGetCountFromISR_core2() returns 1 if the semaphore is available, and 0 if the
 * semaphore is not available.
 *
 */
#define uxSemaphoreGetCountFromISR_core2( xSemaphore_core2 )    uxQueueMessagesWaitingFromISR_core2( ( QueueHandle_t_core2 ) ( xSemaphore_core2 ) )

#endif /* SEMAPHORE_H */
