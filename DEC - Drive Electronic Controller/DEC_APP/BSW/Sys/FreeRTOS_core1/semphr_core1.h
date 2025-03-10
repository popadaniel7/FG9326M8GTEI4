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

#ifndef SEMAPHORE_CORE1_H
#define SEMAPHORE_CORE1_H

#ifndef INC_FREERTOS_H
    #error "include FreeRTOS_core1.h" must appear in source files before "include semphr.h"
#endif

#include "queue_core1.h"

typedef QueueHandle_t_core1 SemaphoreHandle_t_core1;

#define semBINARY_SEMAPHORE_QUEUE_LENGTH_core1    ( ( uint8_t ) 1U )
#define semSEMAPHORE_QUEUE_ITEM_LENGTH_core1      ( ( uint8_t ) 0U )
#define semGIVE_BLOCK_TIME_core1                  ( ( TickType_t_core1 ) 0U )


/**
 * semphr. h
 * @code{c}
 * vSemaphoreCreateBinary_core1( SemaphoreHandle_t_core1 xSemaphore_core1 );
 * @endcode
 *
 * In many usage scenarios it is faster and more memory efficient to use a
 * direct to task_core1 notification in place of a binary semaphore!
 * https://www.FreeRTOS_core1.org/RTOS-task_core1-notifications.html
 *
 * This old vSemaphoreCreateBinary_core1() macro is now deprecated in favour of the
 * xSemaphoreCreateBinary_core1() function.  Note that binary semaphores created using
 * the vSemaphoreCreateBinary_core1() macro are created in a state such that the
 * first call to 'take' the semaphore would pass, whereas binary semaphores
 * created using xSemaphoreCreateBinary_core1() are created in a state such that the
 * the semaphore must first be 'given' before it can be 'taken'.
 *
 * <i>Macro</i> that implements a semaphore by using the existing queue mechanism.
 * The queue length is 1 as this is a binary semaphore.  The data size is 0
 * as we don't want to actually store any data - we just want to know if the
 * queue is empty or full.
 *
 * This type of semaphore can be used for pure synchronisation between tasks or
 * between an interrupt and a task_core1.  The semaphore need not be given back once
 * obtained, so one task_core1/interrupt can continuously 'give' the semaphore while
 * another continuously 'takes' the semaphore.  For this reason this type of
 * semaphore does not use a priority inheritance mechanism.  For an alternative
 * that does use priority inheritance see xSemaphoreCreateMutex_core1().
 *
 * @param xSemaphore_core1 Handle to the created semaphore.  Should be of type SemaphoreHandle_t_core1.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphore_core1 = NULL;
 *
 * void vATask( void * pvParameters_core1 )
 * {
 *  // Semaphore cannot be used before a call to vSemaphoreCreateBinary_core1 ().
 *  // This is a macro so pass the variable in directly.
 *  vSemaphoreCreateBinary_core1( xSemaphore_core1 );
 *
 *  if( xSemaphore_core1 != NULL )
 *  {
 *      // The semaphore was created successfully.
 *      // The semaphore can now be used.
 *  }
 * }
 * @endcode
 * \defgroup vSemaphoreCreateBinary_core1 vSemaphoreCreateBinary_core1
 * \ingroup Semaphores
 */
#if ( configSUPPORT_DYNAMIC_ALLOCATION_core1 == 1 )
    #define vSemaphoreCreateBinary_core1( xSemaphore_core1 )                                                                                     \
    {                                                                                                                                \
        ( xSemaphore_core1 ) = xQueueGenericCreate_core1( ( UBaseType_t_core1 ) 1, semSEMAPHORE_QUEUE_ITEM_LENGTH_core1, queueQUEUE_TYPE_BINARY_SEMAPHORE_core1 ); \
        if( ( xSemaphore_core1 ) != NULL )                                                                                                 \
        {                                                                                                                            \
            ( void ) xSemaphoreGive_core1( ( xSemaphore_core1 ) );                                                                               \
        }                                                                                                                            \
    }
#endif

/**
 * semphr. h
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphoreCreateBinary_core1( void );
 * @endcode
 *
 * Creates a new binary semaphore instance, and returns a handle by which the
 * new semaphore can be referenced.
 *
 * In many usage scenarios it is faster and more memory efficient to use a
 * direct to task_core1 notification in place of a binary semaphore!
 * https://www.FreeRTOS_core1.org/RTOS-task_core1-notifications.html
 *
 * Internally, within the FreeRTOS_core1 implementation, binary semaphores use a block
 * of memory, in which the semaphore structure is stored.  If a binary semaphore
 * is created using xSemaphoreCreateBinary_core1() then the required memory is
 * automatically dynamically allocated inside the xSemaphoreCreateBinary_core1()
 * function.  (see https://www.FreeRTOS_core1.org/a00111.html).  If a binary semaphore
 * is created using xSemaphoreCreateBinaryStatic_core1() then the application writer
 * must provide the memory.  xSemaphoreCreateBinaryStatic_core1() therefore allows a
 * binary semaphore to be created without using any dynamic memory allocation.
 *
 * The old vSemaphoreCreateBinary_core1() macro is now deprecated in favour of this
 * xSemaphoreCreateBinary_core1() function.  Note that binary semaphores created using
 * the vSemaphoreCreateBinary_core1() macro are created in a state such that the
 * first call to 'take' the semaphore would pass, whereas binary semaphores
 * created using xSemaphoreCreateBinary_core1() are created in a state such that the
 * the semaphore must first be 'given' before it can be 'taken'.
 *
 * This type of semaphore can be used for pure synchronisation between tasks or
 * between an interrupt and a task_core1.  The semaphore need not be given back once
 * obtained, so one task_core1/interrupt can continuously 'give' the semaphore while
 * another continuously 'takes' the semaphore.  For this reason this type of
 * semaphore does not use a priority inheritance mechanism.  For an alternative
 * that does use priority inheritance see xSemaphoreCreateMutex_core1().
 *
 * @return Handle to the created semaphore, or NULL if the memory required to
 * hold the semaphore's data structures could not be allocated.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphore_core1 = NULL;
 *
 * void vATask( void * pvParameters_core1 )
 * {
 *  // Semaphore cannot be used before a call to xSemaphoreCreateBinary_core1().
 *  // This is a macro so pass the variable in directly.
 *  xSemaphore_core1 = xSemaphoreCreateBinary_core1();
 *
 *  if( xSemaphore_core1 != NULL )
 *  {
 *      // The semaphore was created successfully.
 *      // The semaphore can now be used.
 *  }
 * }
 * @endcode
 * \defgroup xSemaphoreCreateBinary_core1 xSemaphoreCreateBinary_core1
 * \ingroup Semaphores
 */
#if ( configSUPPORT_DYNAMIC_ALLOCATION_core1 == 1 )
    #define xSemaphoreCreateBinary_core1()    xQueueGenericCreate_core1( ( UBaseType_t_core1 ) 1, semSEMAPHORE_QUEUE_ITEM_LENGTH_core1, queueQUEUE_TYPE_BINARY_SEMAPHORE_core1 )
#endif

/**
 * semphr. h
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphoreCreateBinaryStatic_core1( StaticSemaphore_t_core1 *pxSemaphoreBuffer_core1 );
 * @endcode
 *
 * Creates a new binary semaphore instance, and returns a handle by which the
 * new semaphore can be referenced.
 *
 * NOTE: In many usage scenarios it is faster and more memory efficient to use a
 * direct to task_core1 notification in place of a binary semaphore!
 * https://www.FreeRTOS_core1.org/RTOS-task_core1-notifications.html
 *
 * Internally, within the FreeRTOS_core1 implementation, binary semaphores use a block
 * of memory, in which the semaphore structure is stored.  If a binary semaphore
 * is created using xSemaphoreCreateBinary_core1() then the required memory is
 * automatically dynamically allocated inside the xSemaphoreCreateBinary_core1()
 * function.  (see https://www.FreeRTOS_core1.org/a00111.html).  If a binary semaphore
 * is created using xSemaphoreCreateBinaryStatic_core1() then the application writer
 * must provide the memory.  xSemaphoreCreateBinaryStatic_core1() therefore allows a
 * binary semaphore to be created without using any dynamic memory allocation.
 *
 * This type of semaphore can be used for pure synchronisation between tasks or
 * between an interrupt and a task_core1.  The semaphore need not be given back once
 * obtained, so one task_core1/interrupt can continuously 'give' the semaphore while
 * another continuously 'takes' the semaphore.  For this reason this type of
 * semaphore does not use a priority inheritance mechanism.  For an alternative
 * that does use priority inheritance see xSemaphoreCreateMutex_core1().
 *
 * @param pxSemaphoreBuffer_core1 Must point to a variable of type StaticSemaphore_t_core1,
 * which will then be used to hold the semaphore's data structure, removing the
 * need for the memory to be allocated dynamically.
 *
 * @return If the semaphore is created then a handle to the created semaphore is
 * returned.  If pxSemaphoreBuffer_core1 is NULL then NULL is returned.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphore_core1 = NULL;
 * StaticSemaphore_t_core1 xSemaphoreBuffer;
 *
 * void vATask( void * pvParameters_core1 )
 * {
 *  // Semaphore cannot be used before a call to xSemaphoreCreateBinary_core1().
 *  // The semaphore's data structures will be placed in the xSemaphoreBuffer
 *  // variable, the address of which is passed into the function.  The
 *  // function's parameter is not NULL, so the function will not attempt any
 *  // dynamic memory allocation, and therefore the function will not return
 *  // return NULL.
 *  xSemaphore_core1 = xSemaphoreCreateBinary_core1( &xSemaphoreBuffer );
 *
 *  // Rest of task_core1 code goes here.
 * }
 * @endcode
 * \defgroup xSemaphoreCreateBinaryStatic_core1 xSemaphoreCreateBinaryStatic_core1
 * \ingroup Semaphores
 */
#if ( configSUPPORT_STATIC_ALLOCATION_core1 == 1 )
    #define xSemaphoreCreateBinaryStatic_core1( pxStaticSemaphore_core1 )    xQueueGenericCreateStatic_core1( ( UBaseType_t_core1 ) 1, semSEMAPHORE_QUEUE_ITEM_LENGTH_core1, NULL, ( pxStaticSemaphore_core1 ), queueQUEUE_TYPE_BINARY_SEMAPHORE_core1 )
#endif /* configSUPPORT_STATIC_ALLOCATION_core1 */

/**
 * semphr. h
 * @code{c}
 * xSemaphoreTake_core1(
 *                   SemaphoreHandle_t_core1 xSemaphore_core1,
 *                   TickType_t_core1 xBlockTime_core1
 *               );
 * @endcode
 *
 * <i>Macro</i> to obtain a semaphore.  The semaphore must have previously been
 * created with a call to xSemaphoreCreateBinary_core1(), xSemaphoreCreateMutex_core1() or
 * xSemaphoreCreateCounting_core1().
 *
 * @param xSemaphore_core1 A handle to the semaphore being taken - obtained when
 * the semaphore was created.
 *
 * @param xBlockTime_core1 The time in ticks to wait for the semaphore to become
 * available.  The macro portTICK_PERIOD_MS_core1 can be used to convert this to a
 * real time.  A block time of zero can be used to poll the semaphore.  A block
 * time of portMAX_DELAY_core1 can be used to block indefinitely (provided
 * INCLUDE_vTaskSuspend_core1 is set to 1 in FreeRTOSConfig.h).
 *
 * @return pdTRUE_core1 if the semaphore was obtained.  pdFALSE_core1
 * if xBlockTime_core1 expired without the semaphore becoming available.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphore_core1 = NULL;
 *
 * // A task_core1 that creates a semaphore.
 * void vATask( void * pvParameters_core1 )
 * {
 *  // Create the semaphore to guard a shared resource.
 *  xSemaphore_core1 = xSemaphoreCreateBinary_core1();
 * }
 *
 * // A task_core1 that uses the semaphore.
 * void vAnotherTask( void * pvParameters_core1 )
 * {
 *  // ... Do other things.
 *
 *  if( xSemaphore_core1 != NULL )
 *  {
 *      // See if we can obtain the semaphore.  If the semaphore is not available
 *      // wait 10 ticks to see if it becomes free.
 *      if( xSemaphoreTake_core1( xSemaphore_core1, ( TickType_t_core1 ) 10 ) == pdTRUE_core1 )
 *      {
 *          // We were able to obtain the semaphore and can now access the
 *          // shared resource.
 *
 *          // ...
 *
 *          // We have finished accessing the shared resource.  Release the
 *          // semaphore.
 *          xSemaphoreGive_core1( xSemaphore_core1 );
 *      }
 *      else
 *      {
 *          // We could not obtain the semaphore and can therefore not access
 *          // the shared resource safely.
 *      }
 *  }
 * }
 * @endcode
 * \defgroup xSemaphoreTake_core1 xSemaphoreTake_core1
 * \ingroup Semaphores
 */
#define xSemaphoreTake_core1( xSemaphore_core1, xBlockTime_core1 )    xQueueSemaphoreTake_core1( ( xSemaphore_core1 ), ( xBlockTime_core1 ) )

/**
 * semphr. h
 * @code{c}
 * xSemaphoreTakeRecursive_core1(
 *                          SemaphoreHandle_t_core1 xMutex_core1,
 *                          TickType_t_core1 xBlockTime_core1
 *                        );
 * @endcode
 *
 * <i>Macro</i> to recursively obtain, or 'take', a mutex type semaphore.
 * The mutex must have previously been created using a call to
 * xSemaphoreCreateRecursiveMutex_core1();
 *
 * configUSE_RECURSIVE_MUTEXES_core1 must be set to 1 in FreeRTOSConfig.h for this
 * macro to be available.
 *
 * This macro must not be used on mutexes created using xSemaphoreCreateMutex_core1().
 *
 * A mutex used recursively can be 'taken' repeatedly by the owner. The mutex
 * doesn't become available again until the owner has called
 * xSemaphoreGiveRecursive_core1() for each successful 'take' request.  For example,
 * if a task_core1 successfully 'takes' the same mutex 5 times then the mutex will
 * not be available to any other task_core1 until it has also  'given' the mutex back
 * exactly five times.
 *
 * @param xMutex_core1 A handle to the mutex being obtained.  This is the
 * handle returned by xSemaphoreCreateRecursiveMutex_core1();
 *
 * @param xBlockTime_core1 The time in ticks to wait for the semaphore to become
 * available.  The macro portTICK_PERIOD_MS_core1 can be used to convert this to a
 * real time.  A block time of zero can be used to poll the semaphore.  If
 * the task_core1 already owns the semaphore then xSemaphoreTakeRecursive_core1() will
 * return immediately no matter what the value of xBlockTime_core1.
 *
 * @return pdTRUE_core1 if the semaphore was obtained.  pdFALSE_core1 if xBlockTime_core1
 * expired without the semaphore becoming available.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core1 xMutex_core1 = NULL;
 *
 * // A task_core1 that creates a mutex.
 * void vATask( void * pvParameters_core1 )
 * {
 *  // Create the mutex to guard a shared resource.
 *  xMutex_core1 = xSemaphoreCreateRecursiveMutex_core1();
 * }
 *
 * // A task_core1 that uses the mutex.
 * void vAnotherTask( void * pvParameters_core1 )
 * {
 *  // ... Do other things.
 *
 *  if( xMutex_core1 != NULL )
 *  {
 *      // See if we can obtain the mutex.  If the mutex is not available
 *      // wait 10 ticks to see if it becomes free.
 *      if( xSemaphoreTakeRecursive_core1( xSemaphore_core1, ( TickType_t_core1 ) 10 ) == pdTRUE_core1 )
 *      {
 *          // We were able to obtain the mutex and can now access the
 *          // shared resource.
 *
 *          // ...
 *          // For some reason due to the nature of the code further calls to
 *          // xSemaphoreTakeRecursive_core1() are made on the same mutex.  In real
 *          // code these would not be just sequential calls as this would make
 *          // no sense.  Instead the calls are likely to be buried inside
 *          // a more complex call structure.
 *          xSemaphoreTakeRecursive_core1( xMutex_core1, ( TickType_t_core1 ) 10 );
 *          xSemaphoreTakeRecursive_core1( xMutex_core1, ( TickType_t_core1 ) 10 );
 *
 *          // The mutex has now been 'taken' three times, so will not be
 *          // available to another task_core1 until it has also been given back
 *          // three times.  Again it is unlikely that real code would have
 *          // these calls sequentially, but instead buried in a more complex
 *          // call structure.  This is just for illustrative purposes.
 *          xSemaphoreGiveRecursive_core1( xMutex_core1 );
 *          xSemaphoreGiveRecursive_core1( xMutex_core1 );
 *          xSemaphoreGiveRecursive_core1( xMutex_core1 );
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
 * \defgroup xSemaphoreTakeRecursive_core1 xSemaphoreTakeRecursive_core1
 * \ingroup Semaphores
 */
#if ( configUSE_RECURSIVE_MUTEXES_core1 == 1 )
    #define xSemaphoreTakeRecursive_core1( xMutex_core1, xBlockTime_core1 )    xQueueTakeMutexRecursive_core1( ( xMutex_core1 ), ( xBlockTime_core1 ) )
#endif

/**
 * semphr. h
 * @code{c}
 * xSemaphoreGive_core1( SemaphoreHandle_t_core1 xSemaphore_core1 );
 * @endcode
 *
 * <i>Macro</i> to release a semaphore.  The semaphore must have previously been
 * created with a call to xSemaphoreCreateBinary_core1(), xSemaphoreCreateMutex_core1() or
 * xSemaphoreCreateCounting_core1(). and obtained using sSemaphoreTake().
 *
 * This macro must not be used from an ISR.  See xSemaphoreGiveFromISR_core1 () for
 * an alternative which can be used from an ISR.
 *
 * This macro must also not be used on semaphores created using
 * xSemaphoreCreateRecursiveMutex_core1().
 *
 * @param xSemaphore_core1 A handle to the semaphore being released.  This is the
 * handle returned when the semaphore was created.
 *
 * @return pdTRUE_core1 if the semaphore was released.  pdFALSE_core1 if an error occurred.
 * Semaphores are implemented using queues.  An error can occur if there is
 * no space on the queue to post a message - indicating that the
 * semaphore was not first obtained correctly.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphore_core1 = NULL;
 *
 * void vATask( void * pvParameters_core1 )
 * {
 *  // Create the semaphore to guard a shared resource.
 *  xSemaphore_core1 = vSemaphoreCreateBinary_core1();
 *
 *  if( xSemaphore_core1 != NULL )
 *  {
 *      if( xSemaphoreGive_core1( xSemaphore_core1 ) != pdTRUE_core1 )
 *      {
 *          // We would expect this call to fail because we cannot give
 *          // a semaphore without first "taking" it!
 *      }
 *
 *      // Obtain the semaphore - don't block if the semaphore is not
 *      // immediately available.
 *      if( xSemaphoreTake_core1( xSemaphore_core1, ( TickType_t_core1 ) 0 ) )
 *      {
 *          // We now have the semaphore and can access the shared resource.
 *
 *          // ...
 *
 *          // We have finished accessing the shared resource so can free the
 *          // semaphore.
 *          if( xSemaphoreGive_core1( xSemaphore_core1 ) != pdTRUE_core1 )
 *          {
 *              // We would not expect this call to fail because we must have
 *              // obtained the semaphore to get here.
 *          }
 *      }
 *  }
 * }
 * @endcode
 * \defgroup xSemaphoreGive_core1 xSemaphoreGive_core1
 * \ingroup Semaphores
 */
#define xSemaphoreGive_core1( xSemaphore_core1 )    xQueueGenericSend_core1( ( QueueHandle_t_core1 ) ( xSemaphore_core1 ), NULL, semGIVE_BLOCK_TIME_core1, queueSEND_TO_BACK_core1 )

/**
 * semphr. h
 * @code{c}
 * xSemaphoreGiveRecursive_core1( SemaphoreHandle_t_core1 xMutex_core1 );
 * @endcode
 *
 * <i>Macro</i> to recursively release, or 'give', a mutex type semaphore.
 * The mutex must have previously been created using a call to
 * xSemaphoreCreateRecursiveMutex_core1();
 *
 * configUSE_RECURSIVE_MUTEXES_core1 must be set to 1 in FreeRTOSConfig.h for this
 * macro to be available.
 *
 * This macro must not be used on mutexes created using xSemaphoreCreateMutex_core1().
 *
 * A mutex used recursively can be 'taken' repeatedly by the owner. The mutex
 * doesn't become available again until the owner has called
 * xSemaphoreGiveRecursive_core1() for each successful 'take' request.  For example,
 * if a task_core1 successfully 'takes' the same mutex 5 times then the mutex will
 * not be available to any other task_core1 until it has also  'given' the mutex back
 * exactly five times.
 *
 * @param xMutex_core1 A handle to the mutex being released, or 'given'.  This is the
 * handle returned by xSemaphoreCreateMutex_core1();
 *
 * @return pdTRUE_core1 if the semaphore was given.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core1 xMutex_core1 = NULL;
 *
 * // A task_core1 that creates a mutex.
 * void vATask( void * pvParameters_core1 )
 * {
 *  // Create the mutex to guard a shared resource.
 *  xMutex_core1 = xSemaphoreCreateRecursiveMutex_core1();
 * }
 *
 * // A task_core1 that uses the mutex.
 * void vAnotherTask( void * pvParameters_core1 )
 * {
 *  // ... Do other things.
 *
 *  if( xMutex_core1 != NULL )
 *  {
 *      // See if we can obtain the mutex.  If the mutex is not available
 *      // wait 10 ticks to see if it becomes free.
 *      if( xSemaphoreTakeRecursive_core1( xMutex_core1, ( TickType_t_core1 ) 10 ) == pdTRUE_core1 )
 *      {
 *          // We were able to obtain the mutex and can now access the
 *          // shared resource.
 *
 *          // ...
 *          // For some reason due to the nature of the code further calls to
 *          // xSemaphoreTakeRecursive_core1() are made on the same mutex.  In real
 *          // code these would not be just sequential calls as this would make
 *          // no sense.  Instead the calls are likely to be buried inside
 *          // a more complex call structure.
 *          xSemaphoreTakeRecursive_core1( xMutex_core1, ( TickType_t_core1 ) 10 );
 *          xSemaphoreTakeRecursive_core1( xMutex_core1, ( TickType_t_core1 ) 10 );
 *
 *          // The mutex has now been 'taken' three times, so will not be
 *          // available to another task_core1 until it has also been given back
 *          // three times.  Again it is unlikely that real code would have
 *          // these calls sequentially, it would be more likely that the calls
 *          // to xSemaphoreGiveRecursive_core1() would be called as a call stack
 *          // unwound.  This is just for demonstrative purposes.
 *          xSemaphoreGiveRecursive_core1( xMutex_core1 );
 *          xSemaphoreGiveRecursive_core1( xMutex_core1 );
 *          xSemaphoreGiveRecursive_core1( xMutex_core1 );
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
 * \defgroup xSemaphoreGiveRecursive_core1 xSemaphoreGiveRecursive_core1
 * \ingroup Semaphores
 */
#if ( configUSE_RECURSIVE_MUTEXES_core1 == 1 )
    #define xSemaphoreGiveRecursive_core1( xMutex_core1 )    xQueueGiveMutexRecursive_core1( ( xMutex_core1 ) )
#endif

/**
 * semphr. h
 * @code{c}
 * xSemaphoreGiveFromISR_core1(
 *                        SemaphoreHandle_t_core1 xSemaphore_core1,
 *                        BaseType_t_core1 *pxHigherPriorityTaskWoken_core1
 *                    );
 * @endcode
 *
 * <i>Macro</i> to  release a semaphore.  The semaphore must have previously been
 * created with a call to xSemaphoreCreateBinary_core1() or xSemaphoreCreateCounting_core1().
 *
 * Mutex type semaphores (those created using a call to xSemaphoreCreateMutex_core1())
 * must not be used with this macro.
 *
 * This macro can be used from an ISR.
 *
 * @param xSemaphore_core1 A handle to the semaphore being released.  This is the
 * handle returned when the semaphore was created.
 *
 * @param pxHigherPriorityTaskWoken_core1 xSemaphoreGiveFromISR_core1() will set
 * *pxHigherPriorityTaskWoken_core1 to pdTRUE_core1 if giving the semaphore caused a task_core1
 * to unblock, and the unblocked task_core1 has a priority higher than the currently
 * running task_core1.  If xSemaphoreGiveFromISR_core1() sets this value to pdTRUE_core1 then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE_core1 if the semaphore was successfully given, otherwise errQUEUE_FULL_core1.
 *
 * Example usage:
 * @code{c}
 \#define LONG_TIME 0xffff
 \#define TICKS_TO_WAIT 10
 * SemaphoreHandle_t_core1 xSemaphore_core1 = NULL;
 *
 * // Repetitive task_core1.
 * void vATask( void * pvParameters_core1 )
 * {
 *  for( ;; )
 *  {
 *      // We want this task_core1 to run every 10 ticks of a timer.  The semaphore
 *      // was created before this task_core1 was started.
 *
 *      // Block waiting for the semaphore to become available.
 *      if( xSemaphoreTake_core1( xSemaphore_core1, LONG_TIME ) == pdTRUE_core1 )
 *      {
 *          // It is time to execute.
 *
 *          // ...
 *
 *          // We have finished our task_core1.  Return to the top of the loop where
 *          // we will block on the semaphore until it is time to execute
 *          // again.  Note when using the semaphore for synchronisation with an
 *          // ISR in this manner there is no need to 'give' the semaphore back.
 *      }
 *  }
 * }
 *
 * // Timer ISR
 * void vTimerISR( void * pvParameters_core1 )
 * {
 * static uint8_t ucLocalTickCount = 0;
 * static BaseType_t_core1 xHigherPriorityTaskWoken_core1;
 *
 *  // A timer tick has occurred.
 *
 *  // ... Do other time functions.
 *
 *  // Is it time for vATask () to run?
 *  xHigherPriorityTaskWoken_core1 = pdFALSE_core1;
 *  ucLocalTickCount++;
 *  if( ucLocalTickCount >= TICKS_TO_WAIT )
 *  {
 *      // Unblock the task_core1 by releasing the semaphore.
 *      xSemaphoreGiveFromISR_core1( xSemaphore_core1, &xHigherPriorityTaskWoken_core1 );
 *
 *      // Reset the count so we release the semaphore again in 10 ticks time.
 *      ucLocalTickCount = 0;
 *  }
 *
 *  if( xHigherPriorityTaskWoken_core1 != pdFALSE_core1 )
 *  {
 *      // We can force a context switch here.  Context switching from an
 *      // ISR uses port specific syntax.  Check the demo task_core1 for your port
 *      // to find the syntax required.
 *  }
 * }
 * @endcode
 * \defgroup xSemaphoreGiveFromISR_core1 xSemaphoreGiveFromISR_core1
 * \ingroup Semaphores
 */
#define xSemaphoreGiveFromISR_core1( xSemaphore_core1, pxHigherPriorityTaskWoken_core1 )    xQueueGiveFromISR_core1( ( QueueHandle_t_core1 ) ( xSemaphore_core1 ), ( pxHigherPriorityTaskWoken_core1 ) )

/**
 * semphr. h
 * @code{c}
 * xSemaphoreTakeFromISR_core1(
 *                        SemaphoreHandle_t_core1 xSemaphore_core1,
 *                        BaseType_t_core1 *pxHigherPriorityTaskWoken_core1
 *                    );
 * @endcode
 *
 * <i>Macro</i> to  take a semaphore from an ISR.  The semaphore must have
 * previously been created with a call to xSemaphoreCreateBinary_core1() or
 * xSemaphoreCreateCounting_core1().
 *
 * Mutex type semaphores (those created using a call to xSemaphoreCreateMutex_core1())
 * must not be used with this macro.
 *
 * This macro can be used from an ISR, however taking a semaphore from an ISR
 * is not a common operation.  It is likely to only be useful when taking a
 * counting semaphore when an interrupt is obtaining an object from a resource
 * pool (when the semaphore count indicates the number of resources available).
 *
 * @param xSemaphore_core1 A handle to the semaphore being taken.  This is the
 * handle returned when the semaphore was created.
 *
 * @param pxHigherPriorityTaskWoken_core1 xSemaphoreTakeFromISR_core1() will set
 * *pxHigherPriorityTaskWoken_core1 to pdTRUE_core1 if taking the semaphore caused a task_core1
 * to unblock, and the unblocked task_core1 has a priority higher than the currently
 * running task_core1.  If xSemaphoreTakeFromISR_core1() sets this value to pdTRUE_core1 then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE_core1 if the semaphore was successfully taken, otherwise
 * pdFALSE_core1
 */
#define xSemaphoreTakeFromISR_core1( xSemaphore_core1, pxHigherPriorityTaskWoken_core1 )    xQueueReceiveFromISR_core1( ( QueueHandle_t_core1 ) ( xSemaphore_core1 ), NULL, ( pxHigherPriorityTaskWoken_core1 ) )

/**
 * semphr. h
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphoreCreateMutex_core1( void );
 * @endcode
 *
 * Creates a new mutex type semaphore instance, and returns a handle by which
 * the new mutex can be referenced.
 *
 * Internally, within the FreeRTOS_core1 implementation, mutex semaphores use a block
 * of memory, in which the mutex structure is stored.  If a mutex is created
 * using xSemaphoreCreateMutex_core1() then the required memory is automatically
 * dynamically allocated inside the xSemaphoreCreateMutex_core1() function.  (see
 * https://www.FreeRTOS_core1.org/a00111.html).  If a mutex is created using
 * xSemaphoreCreateMutexStatic_core1() then the application writer must provided the
 * memory.  xSemaphoreCreateMutexStatic_core1() therefore allows a mutex to be created
 * without using any dynamic memory allocation.
 *
 * Mutexes created using this function can be accessed using the xSemaphoreTake_core1()
 * and xSemaphoreGive_core1() macros.  The xSemaphoreTakeRecursive_core1() and
 * xSemaphoreGiveRecursive_core1() macros must not be used.
 *
 * This type of semaphore uses a priority inheritance mechanism so a task_core1
 * 'taking' a semaphore MUST ALWAYS 'give' the semaphore back once the
 * semaphore it is no longer required.
 *
 * Mutex type semaphores cannot be used from within interrupt service routines.
 *
 * See xSemaphoreCreateBinary_core1() for an alternative implementation that can be
 * used for pure synchronisation (where one task_core1 or interrupt always 'gives' the
 * semaphore and another always 'takes' the semaphore) and from within interrupt
 * service routines.
 *
 * @return If the mutex was successfully created then a handle to the created
 * semaphore is returned.  If there was not enough heap to allocate the mutex
 * data structures then NULL is returned.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphore_core1;
 *
 * void vATask( void * pvParameters_core1 )
 * {
 *  // Semaphore cannot be used before a call to xSemaphoreCreateMutex_core1().
 *  // This is a macro so pass the variable in directly.
 *  xSemaphore_core1 = xSemaphoreCreateMutex_core1();
 *
 *  if( xSemaphore_core1 != NULL )
 *  {
 *      // The semaphore was created successfully.
 *      // The semaphore can now be used.
 *  }
 * }
 * @endcode
 * \defgroup xSemaphoreCreateMutex_core1 xSemaphoreCreateMutex_core1
 * \ingroup Semaphores
 */
#if ( ( configSUPPORT_DYNAMIC_ALLOCATION_core1 == 1 ) && ( configUSE_MUTEXES_core1 == 1 ) )
    #define xSemaphoreCreateMutex_core1()    xQueueCreateMutex_core1( queueQUEUE_TYPE_MUTEX_core1 )
#endif

/**
 * semphr. h
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphoreCreateMutexStatic_core1( StaticSemaphore_t_core1 *pxMutexBuffer_core1 );
 * @endcode
 *
 * Creates a new mutex type semaphore instance, and returns a handle by which
 * the new mutex can be referenced.
 *
 * Internally, within the FreeRTOS_core1 implementation, mutex semaphores use a block
 * of memory, in which the mutex structure is stored.  If a mutex is created
 * using xSemaphoreCreateMutex_core1() then the required memory is automatically
 * dynamically allocated inside the xSemaphoreCreateMutex_core1() function.  (see
 * https://www.FreeRTOS_core1.org/a00111.html).  If a mutex is created using
 * xSemaphoreCreateMutexStatic_core1() then the application writer must provided the
 * memory.  xSemaphoreCreateMutexStatic_core1() therefore allows a mutex to be created
 * without using any dynamic memory allocation.
 *
 * Mutexes created using this function can be accessed using the xSemaphoreTake_core1()
 * and xSemaphoreGive_core1() macros.  The xSemaphoreTakeRecursive_core1() and
 * xSemaphoreGiveRecursive_core1() macros must not be used.
 *
 * This type of semaphore uses a priority inheritance mechanism so a task_core1
 * 'taking' a semaphore MUST ALWAYS 'give' the semaphore back once the
 * semaphore it is no longer required.
 *
 * Mutex type semaphores cannot be used from within interrupt service routines.
 *
 * See xSemaphoreCreateBinary_core1() for an alternative implementation that can be
 * used for pure synchronisation (where one task_core1 or interrupt always 'gives' the
 * semaphore and another always 'takes' the semaphore) and from within interrupt
 * service routines.
 *
 * @param pxMutexBuffer_core1 Must point to a variable of type StaticSemaphore_t_core1,
 * which will be used to hold the mutex's data structure, removing the need for
 * the memory to be allocated dynamically.
 *
 * @return If the mutex was successfully created then a handle to the created
 * mutex is returned.  If pxMutexBuffer_core1 was NULL then NULL is returned.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphore_core1;
 * StaticSemaphore_t_core1 xMutexBuffer;
 *
 * void vATask( void * pvParameters_core1 )
 * {
 *  // A mutex cannot be used before it has been created.  xMutexBuffer is
 *  // into xSemaphoreCreateMutexStatic_core1() so no dynamic memory allocation is
 *  // attempted.
 *  xSemaphore_core1 = xSemaphoreCreateMutexStatic_core1( &xMutexBuffer );
 *
 *  // As no dynamic memory allocation was performed, xSemaphore_core1 cannot be NULL,
 *  // so there is no need to check it.
 * }
 * @endcode
 * \defgroup xSemaphoreCreateMutexStatic_core1 xSemaphoreCreateMutexStatic_core1
 * \ingroup Semaphores
 */
#if ( ( configSUPPORT_STATIC_ALLOCATION_core1 == 1 ) && ( configUSE_MUTEXES_core1 == 1 ) )
    #define xSemaphoreCreateMutexStatic_core1( pxMutexBuffer_core1 )    xQueueCreateMutexStatic_core1( queueQUEUE_TYPE_MUTEX_core1, ( pxMutexBuffer_core1 ) )
#endif


/**
 * semphr. h
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphoreCreateRecursiveMutex_core1( void );
 * @endcode
 *
 * Creates a new recursive mutex type semaphore instance, and returns a handle
 * by which the new recursive mutex can be referenced.
 *
 * Internally, within the FreeRTOS_core1 implementation, recursive mutexes use a block
 * of memory, in which the mutex structure is stored.  If a recursive mutex is
 * created using xSemaphoreCreateRecursiveMutex_core1() then the required memory is
 * automatically dynamically allocated inside the
 * xSemaphoreCreateRecursiveMutex_core1() function.  (see
 * https://www.FreeRTOS_core1.org/a00111.html).  If a recursive mutex is created using
 * xSemaphoreCreateRecursiveMutexStatic_core1() then the application writer must
 * provide the memory that will get used by the mutex.
 * xSemaphoreCreateRecursiveMutexStatic_core1() therefore allows a recursive mutex to
 * be created without using any dynamic memory allocation.
 *
 * Mutexes created using this macro can be accessed using the
 * xSemaphoreTakeRecursive_core1() and xSemaphoreGiveRecursive_core1() macros.  The
 * xSemaphoreTake_core1() and xSemaphoreGive_core1() macros must not be used.
 *
 * A mutex used recursively can be 'taken' repeatedly by the owner. The mutex
 * doesn't become available again until the owner has called
 * xSemaphoreGiveRecursive_core1() for each successful 'take' request.  For example,
 * if a task_core1 successfully 'takes' the same mutex 5 times then the mutex will
 * not be available to any other task_core1 until it has also  'given' the mutex back
 * exactly five times.
 *
 * This type of semaphore uses a priority inheritance mechanism so a task_core1
 * 'taking' a semaphore MUST ALWAYS 'give' the semaphore back once the
 * semaphore it is no longer required.
 *
 * Mutex type semaphores cannot be used from within interrupt service routines.
 *
 * See xSemaphoreCreateBinary_core1() for an alternative implementation that can be
 * used for pure synchronisation (where one task_core1 or interrupt always 'gives' the
 * semaphore and another always 'takes' the semaphore) and from within interrupt
 * service routines.
 *
 * @return xSemaphore_core1 Handle to the created mutex semaphore.  Should be of type
 * SemaphoreHandle_t_core1.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphore_core1;
 *
 * void vATask( void * pvParameters_core1 )
 * {
 *  // Semaphore cannot be used before a call to xSemaphoreCreateMutex_core1().
 *  // This is a macro so pass the variable in directly.
 *  xSemaphore_core1 = xSemaphoreCreateRecursiveMutex_core1();
 *
 *  if( xSemaphore_core1 != NULL )
 *  {
 *      // The semaphore was created successfully.
 *      // The semaphore can now be used.
 *  }
 * }
 * @endcode
 * \defgroup xSemaphoreCreateRecursiveMutex_core1 xSemaphoreCreateRecursiveMutex_core1
 * \ingroup Semaphores
 */
#if ( ( configSUPPORT_DYNAMIC_ALLOCATION_core1 == 1 ) && ( configUSE_RECURSIVE_MUTEXES_core1 == 1 ) )
    #define xSemaphoreCreateRecursiveMutex_core1()    xQueueCreateMutex_core1( queueQUEUE_TYPE_RECURSIVE_MUTEX_core1 )
#endif

/**
 * semphr. h
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphoreCreateRecursiveMutexStatic_core1( StaticSemaphore_t_core1 *pxMutexBuffer_core1 );
 * @endcode
 *
 * Creates a new recursive mutex type semaphore instance, and returns a handle
 * by which the new recursive mutex can be referenced.
 *
 * Internally, within the FreeRTOS_core1 implementation, recursive mutexes use a block
 * of memory, in which the mutex structure is stored.  If a recursive mutex is
 * created using xSemaphoreCreateRecursiveMutex_core1() then the required memory is
 * automatically dynamically allocated inside the
 * xSemaphoreCreateRecursiveMutex_core1() function.  (see
 * https://www.FreeRTOS_core1.org/a00111.html).  If a recursive mutex is created using
 * xSemaphoreCreateRecursiveMutexStatic_core1() then the application writer must
 * provide the memory that will get used by the mutex.
 * xSemaphoreCreateRecursiveMutexStatic_core1() therefore allows a recursive mutex to
 * be created without using any dynamic memory allocation.
 *
 * Mutexes created using this macro can be accessed using the
 * xSemaphoreTakeRecursive_core1() and xSemaphoreGiveRecursive_core1() macros.  The
 * xSemaphoreTake_core1() and xSemaphoreGive_core1() macros must not be used.
 *
 * A mutex used recursively can be 'taken' repeatedly by the owner. The mutex
 * doesn't become available again until the owner has called
 * xSemaphoreGiveRecursive_core1() for each successful 'take' request.  For example,
 * if a task_core1 successfully 'takes' the same mutex 5 times then the mutex will
 * not be available to any other task_core1 until it has also  'given' the mutex back
 * exactly five times.
 *
 * This type of semaphore uses a priority inheritance mechanism so a task_core1
 * 'taking' a semaphore MUST ALWAYS 'give' the semaphore back once the
 * semaphore it is no longer required.
 *
 * Mutex type semaphores cannot be used from within interrupt service routines.
 *
 * See xSemaphoreCreateBinary_core1() for an alternative implementation that can be
 * used for pure synchronisation (where one task_core1 or interrupt always 'gives' the
 * semaphore and another always 'takes' the semaphore) and from within interrupt
 * service routines.
 *
 * @param pxMutexBuffer_core1 Must point to a variable of type StaticSemaphore_t_core1,
 * which will then be used to hold the recursive mutex's data structure,
 * removing the need for the memory to be allocated dynamically.
 *
 * @return If the recursive mutex was successfully created then a handle to the
 * created recursive mutex is returned.  If pxMutexBuffer_core1 was NULL then NULL is
 * returned.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphore_core1;
 * StaticSemaphore_t_core1 xMutexBuffer;
 *
 * void vATask( void * pvParameters_core1 )
 * {
 *  // A recursive semaphore cannot be used before it is created.  Here a
 *  // recursive mutex is created using xSemaphoreCreateRecursiveMutexStatic_core1().
 *  // The address of xMutexBuffer is passed into the function, and will hold
 *  // the mutexes data structures - so no dynamic memory allocation will be
 *  // attempted.
 *  xSemaphore_core1 = xSemaphoreCreateRecursiveMutexStatic_core1( &xMutexBuffer );
 *
 *  // As no dynamic memory allocation was performed, xSemaphore_core1 cannot be NULL,
 *  // so there is no need to check it.
 * }
 * @endcode
 * \defgroup xSemaphoreCreateRecursiveMutexStatic_core1 xSemaphoreCreateRecursiveMutexStatic_core1
 * \ingroup Semaphores
 */
#if ( ( configSUPPORT_STATIC_ALLOCATION_core1 == 1 ) && ( configUSE_RECURSIVE_MUTEXES_core1 == 1 ) )
    #define xSemaphoreCreateRecursiveMutexStatic_core1( pxStaticSemaphore_core1 )    xQueueCreateMutexStatic_core1( queueQUEUE_TYPE_RECURSIVE_MUTEX_core1, ( pxStaticSemaphore_core1 ) )
#endif /* configSUPPORT_STATIC_ALLOCATION_core1 */

/**
 * semphr. h
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphoreCreateCounting_core1( UBaseType_t_core1 uxMaxCount_core1, UBaseType_t_core1 uxInitialCount_core1 );
 * @endcode
 *
 * Creates a new counting semaphore instance, and returns a handle by which the
 * new counting semaphore can be referenced.
 *
 * In many usage scenarios it is faster and more memory efficient to use a
 * direct to task_core1 notification in place of a counting semaphore!
 * https://www.FreeRTOS_core1.org/RTOS-task_core1-notifications.html
 *
 * Internally, within the FreeRTOS_core1 implementation, counting semaphores use a
 * block of memory, in which the counting semaphore structure is stored.  If a
 * counting semaphore is created using xSemaphoreCreateCounting_core1() then the
 * required memory is automatically dynamically allocated inside the
 * xSemaphoreCreateCounting_core1() function.  (see
 * https://www.FreeRTOS_core1.org/a00111.html).  If a counting semaphore is created
 * using xSemaphoreCreateCountingStatic_core1() then the application writer can
 * instead optionally provide the memory that will get used by the counting
 * semaphore.  xSemaphoreCreateCountingStatic_core1() therefore allows a counting
 * semaphore to be created without using any dynamic memory allocation.
 *
 * Counting semaphores are typically used for two things:
 *
 * 1) Counting events.
 *
 *    In this usage scenario an event handler will 'give' a semaphore each time
 *    an event occurs (incrementing the semaphore count value), and a handler
 *    task_core1 will 'take' a semaphore each time it processes an event
 *    (decrementing the semaphore count value).  The count value is therefore
 *    the difference between the number of events that have occurred and the
 *    number that have been processed.  In this case it is desirable for the
 *    initial count value to be zero.
 *
 * 2) Resource management.
 *
 *    In this usage scenario the count value indicates the number of resources
 *    available.  To obtain control of a resource a task_core1 must first obtain a
 *    semaphore - decrementing the semaphore count value.  When the count value
 *    reaches zero there are no free resources.  When a task_core1 finishes with the
 *    resource it 'gives' the semaphore back - incrementing the semaphore count
 *    value.  In this case it is desirable for the initial count value to be
 *    equal to the maximum count value, indicating that all resources are free.
 *
 * @param uxMaxCount_core1 The maximum count value that can be reached.  When the
 *        semaphore reaches this value it can no longer be 'given'.
 *
 * @param uxInitialCount_core1 The count value assigned to the semaphore when it is
 *        created.
 *
 * @return Handle to the created semaphore.  Null if the semaphore could not be
 *         created.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphore_core1;
 *
 * void vATask( void * pvParameters_core1 )
 * {
 * SemaphoreHandle_t_core1 xSemaphore_core1 = NULL;
 *
 *  // Semaphore cannot be used before a call to xSemaphoreCreateCounting_core1().
 *  // The max value to which the semaphore can count should be 10, and the
 *  // initial value assigned to the count should be 0.
 *  xSemaphore_core1 = xSemaphoreCreateCounting_core1( 10, 0 );
 *
 *  if( xSemaphore_core1 != NULL )
 *  {
 *      // The semaphore was created successfully.
 *      // The semaphore can now be used.
 *  }
 * }
 * @endcode
 * \defgroup xSemaphoreCreateCounting_core1 xSemaphoreCreateCounting_core1
 * \ingroup Semaphores
 */
#if ( configSUPPORT_DYNAMIC_ALLOCATION_core1 == 1 )
    #define xSemaphoreCreateCounting_core1( uxMaxCount_core1, uxInitialCount_core1 )    xQueueCreateCountingSemaphore_core1( ( uxMaxCount_core1 ), ( uxInitialCount_core1 ) )
#endif

/**
 * semphr. h
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphoreCreateCountingStatic_core1( UBaseType_t_core1 uxMaxCount_core1, UBaseType_t_core1 uxInitialCount_core1, StaticSemaphore_t_core1 *pxSemaphoreBuffer_core1 );
 * @endcode
 *
 * Creates a new counting semaphore instance, and returns a handle by which the
 * new counting semaphore can be referenced.
 *
 * In many usage scenarios it is faster and more memory efficient to use a
 * direct to task_core1 notification in place of a counting semaphore!
 * https://www.FreeRTOS_core1.org/RTOS-task_core1-notifications.html
 *
 * Internally, within the FreeRTOS_core1 implementation, counting semaphores use a
 * block of memory, in which the counting semaphore structure is stored.  If a
 * counting semaphore is created using xSemaphoreCreateCounting_core1() then the
 * required memory is automatically dynamically allocated inside the
 * xSemaphoreCreateCounting_core1() function.  (see
 * https://www.FreeRTOS_core1.org/a00111.html).  If a counting semaphore is created
 * using xSemaphoreCreateCountingStatic_core1() then the application writer must
 * provide the memory.  xSemaphoreCreateCountingStatic_core1() therefore allows a
 * counting semaphore to be created without using any dynamic memory allocation.
 *
 * Counting semaphores are typically used for two things:
 *
 * 1) Counting events.
 *
 *    In this usage scenario an event handler will 'give' a semaphore each time
 *    an event occurs (incrementing the semaphore count value), and a handler
 *    task_core1 will 'take' a semaphore each time it processes an event
 *    (decrementing the semaphore count value).  The count value is therefore
 *    the difference between the number of events that have occurred and the
 *    number that have been processed.  In this case it is desirable for the
 *    initial count value to be zero.
 *
 * 2) Resource management.
 *
 *    In this usage scenario the count value indicates the number of resources
 *    available.  To obtain control of a resource a task_core1 must first obtain a
 *    semaphore - decrementing the semaphore count value.  When the count value
 *    reaches zero there are no free resources.  When a task_core1 finishes with the
 *    resource it 'gives' the semaphore back - incrementing the semaphore count
 *    value.  In this case it is desirable for the initial count value to be
 *    equal to the maximum count value, indicating that all resources are free.
 *
 * @param uxMaxCount_core1 The maximum count value that can be reached.  When the
 *        semaphore reaches this value it can no longer be 'given'.
 *
 * @param uxInitialCount_core1 The count value assigned to the semaphore when it is
 *        created.
 *
 * @param pxSemaphoreBuffer_core1 Must point to a variable of type StaticSemaphore_t_core1,
 * which will then be used to hold the semaphore's data structure, removing the
 * need for the memory to be allocated dynamically.
 *
 * @return If the counting semaphore was successfully created then a handle to
 * the created counting semaphore is returned.  If pxSemaphoreBuffer_core1 was NULL
 * then NULL is returned.
 *
 * Example usage:
 * @code{c}
 * SemaphoreHandle_t_core1 xSemaphore_core1;
 * StaticSemaphore_t_core1 xSemaphoreBuffer;
 *
 * void vATask( void * pvParameters_core1 )
 * {
 * SemaphoreHandle_t_core1 xSemaphore_core1 = NULL;
 *
 *  // Counting semaphore cannot be used before they have been created.  Create
 *  // a counting semaphore using xSemaphoreCreateCountingStatic_core1().  The max
 *  // value to which the semaphore can count is 10, and the initial value
 *  // assigned to the count will be 0.  The address of xSemaphoreBuffer is
 *  // passed in and will be used to hold the semaphore structure, so no dynamic
 *  // memory allocation will be used.
 *  xSemaphore_core1 = xSemaphoreCreateCounting_core1( 10, 0, &xSemaphoreBuffer );
 *
 *  // No memory allocation was attempted so xSemaphore_core1 cannot be NULL, so there
 *  // is no need to check its value.
 * }
 * @endcode
 * \defgroup xSemaphoreCreateCountingStatic_core1 xSemaphoreCreateCountingStatic_core1
 * \ingroup Semaphores
 */
#if ( configSUPPORT_STATIC_ALLOCATION_core1 == 1 )
    #define xSemaphoreCreateCountingStatic_core1( uxMaxCount_core1, uxInitialCount_core1, pxSemaphoreBuffer_core1 )    xQueueCreateCountingSemaphoreStatic_core1( ( uxMaxCount_core1 ), ( uxInitialCount_core1 ), ( pxSemaphoreBuffer_core1 ) )
#endif /* configSUPPORT_STATIC_ALLOCATION_core1 */

/**
 * semphr. h
 * @code{c}
 * void vSemaphoreDelete_core1( SemaphoreHandle_t_core1 xSemaphore_core1 );
 * @endcode
 *
 * Delete a semaphore.  This function must be used with care.  For example,
 * do not delete a mutex type semaphore if the mutex is held by a task_core1.
 *
 * @param xSemaphore_core1 A handle to the semaphore to be deleted.
 *
 * \defgroup vSemaphoreDelete_core1 vSemaphoreDelete_core1
 * \ingroup Semaphores
 */
#define vSemaphoreDelete_core1( xSemaphore_core1 )    vQueueDelete_core1( ( QueueHandle_t_core1 ) ( xSemaphore_core1 ) )

/**
 * semphr.h
 * @code{c}
 * TaskHandle_t_core1 xSemaphoreGetMutexHolder_core1( SemaphoreHandle_t_core1 xMutex_core1 );
 * @endcode
 *
 * If xMutex_core1 is indeed a mutex type semaphore, return the current mutex holder.
 * If xMutex_core1 is not a mutex type semaphore, or the mutex is available (not held
 * by a task_core1), return NULL.
 *
 * Note: This is a good way of determining if the calling task_core1 is the mutex
 * holder, but not a good way of determining the identity of the mutex holder as
 * the holder may change between the function exiting and the returned value
 * being tested.
 */
#if ( ( configUSE_MUTEXES_core1 == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder_core1 == 1 ) )
    #define xSemaphoreGetMutexHolder_core1( xSemaphore_core1 )    xQueueGetMutexHolder_core1( ( xSemaphore_core1 ) )
#endif

/**
 * semphr.h
 * @code{c}
 * TaskHandle_t_core1 xSemaphoreGetMutexHolderFromISR_core1( SemaphoreHandle_t_core1 xMutex_core1 );
 * @endcode
 *
 * If xMutex_core1 is indeed a mutex type semaphore, return the current mutex holder.
 * If xMutex_core1 is not a mutex type semaphore, or the mutex is available (not held
 * by a task_core1), return NULL.
 *
 */
#if ( ( configUSE_MUTEXES_core1 == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder_core1 == 1 ) )
    #define xSemaphoreGetMutexHolderFromISR_core1( xSemaphore_core1 )    xQueueGetMutexHolderFromISR_core1( ( xSemaphore_core1 ) )
#endif

/**
 * semphr.h
 * @code{c}
 * UBaseType_t_core1 uxSemaphoreGetCount_core1( SemaphoreHandle_t_core1 xSemaphore_core1 );
 * @endcode
 *
 * If the semaphore is a counting semaphore then uxSemaphoreGetCount_core1() returns
 * its current count value.  If the semaphore is a binary semaphore then
 * uxSemaphoreGetCount_core1() returns 1 if the semaphore is available, and 0 if the
 * semaphore is not available.
 *
 */
#define uxSemaphoreGetCount_core1( xSemaphore_core1 )           uxQueueMessagesWaiting_core1( ( QueueHandle_t_core1 ) ( xSemaphore_core1 ) )

/**
 * semphr.h
 * @code{c}
 * UBaseType_t_core1 uxSemaphoreGetCountFromISR_core1( SemaphoreHandle_t_core1 xSemaphore_core1 );
 * @endcode
 *
 * If the semaphore is a counting semaphore then uxSemaphoreGetCountFromISR_core1() returns
 * its current count value.  If the semaphore is a binary semaphore then
 * uxSemaphoreGetCountFromISR_core1() returns 1 if the semaphore is available, and 0 if the
 * semaphore is not available.
 *
 */
#define uxSemaphoreGetCountFromISR_core1( xSemaphore_core1 )    uxQueueMessagesWaitingFromISR_core1( ( QueueHandle_t_core1 ) ( xSemaphore_core1 ) )

#endif /* SEMAPHORE_H */
