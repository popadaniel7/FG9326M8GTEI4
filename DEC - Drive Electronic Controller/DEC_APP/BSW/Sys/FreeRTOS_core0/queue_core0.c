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

#include <stdlib.h>
#include <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE_core0 prevents task_core0.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task_core0.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE_core0

#include "FreeRTOS_core0.h"
#include "task_core0.h"
#include "queue_core0.h"

#if ( configUSE_CO_ROUTINES_core0 == 1 )
    #include "croutine_core0.h"
#endif

/* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE_core0 to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE_core0 /*lint !e961 !e750 !e9021. */


/* Constants used with the cRxLock_core0 and cTxLock_core0 structure members. */
#define queueUNLOCKED_core0             ( ( int8_t ) -1 )
#define queueLOCKED_UNMODIFIED_core0    ( ( int8_t ) 0 )
#define queueINT8_MAX_core0             ( ( int8_t ) 127 )

/* When the Queue_t_core0 structure is used to represent a base queue its pcHead_core0 and
 * pcTail_core0 members are used as pointers into the queue storage area.  When the
 * Queue_t_core0 structure is used to represent a mutex pcHead_core0 and pcTail_core0 pointers are
 * not necessary, and the pcHead_core0 pointer is set to NULL to indicate that the
 * structure instead holds a pointer to the mutex holder (if any).  Map alternative
 * names to the pcHead_core0 and structure member to ensure the readability of the code
 * is maintained.  The QueuePointers_t_core0 and SemaphoreData_t_core0 types are used to form
 * a union as their usage is mutually exclusive dependent on what the queue is
 * being used for. */
#define uxQueueType_core0               pcHead_core0
#define queueQUEUE_IS_MUTEX_core0       NULL

typedef struct QueuePointers_core0
{
    int8_t * pcTail_core0;     /*< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
    int8_t * pcReadFrom_core0; /*< Points to the last place that a queued item was read from when the structure is used as a queue. */
} QueuePointers_t_core0;

typedef struct SemaphoreData_core0
{
    TaskHandle_t_core0 xMutexHolder_core0;        /*< The handle of the task_core0 that holds the mutex. */
    UBaseType_t_core0 uxRecursiveCallCount_core0; /*< Maintains a count of the number of times a recursive mutex has been recursively 'taken' when the structure is used as a mutex. */
} SemaphoreData_t_core0;

/* Semaphores do not actually store or copy data, so have an item size of
 * zero. */
#define queueSEMAPHORE_QUEUE_ITEM_LENGTH_core0    ( ( UBaseType_t_core0 ) 0 )
#define queueMUTEX_GIVE_BLOCK_TIME_core0          ( ( TickType_t_core0 ) 0U )

#if ( configUSE_PREEMPTION_core0 == 0 )

/* If the cooperative scheduler is being used then a yield should not be
 * performed just because a higher priority task_core0 has been woken. */
    #define queueYIELD_IF_USING_PREEMPTION_core0()
#else
    #define queueYIELD_IF_USING_PREEMPTION_core0()    portYIELD_WITHIN_API_core0()
#endif

/*
 * Definition of the queue used by the scheduler.
 * Items are queued by copy, not reference.  See the following link for the
 * rationale: https://www.FreeRTOS_core0.org/Embedded-RTOS-Queues.html
 */
typedef struct QueueDefinition_core0 /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    int8_t * pcHead_core0;           /*< Points to the beginning of the queue storage area. */
    int8_t * pcWriteTo_core0;        /*< Points to the free next place in the storage area. */

    union
    {
        QueuePointers_t_core0 xQueue_core0;     /*< Data required exclusively when this structure is used as a queue. */
        SemaphoreData_t_core0 xSemaphore_core0; /*< Data required exclusively when this structure is used as a semaphore. */
    } u;

    List_t_core0 xTasksWaitingToSend_core0;             /*< List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
    List_t_core0 xTasksWaitingToReceive_core0;          /*< List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

    volatile UBaseType_t_core0 uxMessagesWaiting_core0; /*< The number of items currently in the queue. */
    UBaseType_t_core0 uxLength_core0;                   /*< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    UBaseType_t_core0 uxItemSize_core0;                 /*< The size of each items that the queue will hold. */

    volatile int8_t cRxLock_core0;                /*< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED_core0 when the queue is not locked. */
    volatile int8_t cTxLock_core0;                /*< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED_core0 when the queue is not locked. */

    #if ( ( configSUPPORT_STATIC_ALLOCATION_core0 == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION_core0 == 1 ) )
        uint8_t ucStaticallyAllocated_core0; /*< Set to pdTRUE_core0 if the memory used by the queue was statically allocated to ensure no attempt is made to free the memory. */
    #endif

    #if ( configUSE_QUEUE_SETS_core0 == 1 )
        struct QueueDefinition_core0 * pxQueueSetContainer_core0;
    #endif

    #if ( configUSE_TRACE_FACILITY_core0 == 1 )
        UBaseType_t_core0 uxQueueNumber_core0;
        uint8_t ucQueueType_core0;
    #endif
} xQUEUE_core0;

/* The old xQUEUE_core0 name is maintained above then typedefed to the new Queue_t_core0
 * name below to enable the use of older kernel aware debuggers. */
typedef xQUEUE_core0 Queue_t_core0;

/*-----------------------------------------------------------*/

/*
 * The queue registry is just a means for kernel aware debuggers to locate
 * queue structures.  It has no other purpose so is an optional component.
 */
#if ( configQUEUE_REGISTRY_SIZE_core0 > 0 )

/* The type stored within the queue registry array.  This allows a name
 * to be assigned to each queue making kernel aware debugging a little
 * more user friendly. */
    typedef struct QUEUE_REGISTRY_ITEM_core0
    {
        const char * pcQueueName_core0; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        QueueHandle_t_core0 xHandle_core0;
    } xQueueRegistryItem_core0;

/* The old xQueueRegistryItem_core0 name is maintained above then typedefed to the
 * new xQueueRegistryItem_core0 name below to enable the use of older kernel aware
 * debuggers. */
    typedef xQueueRegistryItem_core0 QueueRegistryItem_t_core0;

/* The queue registry is simply an array of QueueRegistryItem_t_core0 structures.
 * The pcQueueName_core0 member of a structure being NULL is indicative of the
 * array position being vacant. */
    PRIVILEGED_DATA_core0 QueueRegistryItem_t_core0 xQueueRegistry[ configQUEUE_REGISTRY_SIZE_core0 ];

#endif /* configQUEUE_REGISTRY_SIZE_core0 */

/*
 * Unlocks a queue locked by a call to prvLockQueue_core0.  Locking a queue does not
 * prevent an ISR from adding or removing items to the queue, but does prevent
 * an ISR from removing tasks from the queue event lists.  If an ISR finds a
 * queue is locked it will instead increment the appropriate queue lock count
 * to indicate that a task_core0 may require unblocking.  When the queue in unlocked
 * these lock counts are inspected, and the appropriate action taken.
 */
static void prvUnlockQueue_core0( Queue_t_core0 * const pxQueue_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * Uses a critical section to determine if there is any data in a queue.
 *
 * @return pdTRUE_core0 if the queue contains no items, otherwise pdFALSE_core0.
 */
static BaseType_t_core0 prvIsQueueEmpty_core0( const Queue_t_core0 * pxQueue_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * Uses a critical section to determine if there is any space in a queue.
 *
 * @return pdTRUE_core0 if there is no space, otherwise pdFALSE_core0;
 */
static BaseType_t_core0 prvIsQueueFull_core0( const Queue_t_core0 * pxQueue_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * Copies an item into the queue, either at the front of the queue or the
 * back of the queue.
 */
static BaseType_t_core0 prvCopyDataToQueue_core0( Queue_t_core0 * const pxQueue_core0,
                                      const void * pvItemToQueue_core0,
                                      const BaseType_t_core0 xPosition ) PRIVILEGED_FUNCTION_core0;

/*
 * Copies an item out of a queue.
 */
static void prvCopyDataFromQueue_core0( Queue_t_core0 * const pxQueue_core0,
                                  void * const pvBuffer_core0 ) PRIVILEGED_FUNCTION_core0;

#if ( configUSE_QUEUE_SETS_core0 == 1 )

/*
 * Checks to see if a queue is a member of a queue set, and if so, notifies
 * the queue set that the queue contains data.
 */
    static BaseType_t_core0 prvNotifyQueueSetContainer_core0( const Queue_t_core0 * const pxQueue_core0 ) PRIVILEGED_FUNCTION_core0;
#endif

/*
 * Called after a Queue_t_core0 structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
static void prvInitialiseNewQueue_core0( const UBaseType_t_core0 uxQueueLength_core0,
                                   const UBaseType_t_core0 uxItemSize_core0,
                                   uint8_t * pucQueueStorage_core0,
                                   const uint8_t ucQueueType_core0,
                                   Queue_t_core0 * pxNewQueue_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * Mutexes are a special type of queue.  When a mutex is created, first the
 * queue is created, then prvInitialiseMutex_core0() is called to configure the queue
 * as a mutex.
 */
#if ( configUSE_MUTEXES_core0 == 1 )
    static void prvInitialiseMutex_core0( Queue_t_core0 * pxNewQueue_core0 ) PRIVILEGED_FUNCTION_core0;
#endif

#if ( configUSE_MUTEXES_core0 == 1 )

/*
 * If a task_core0 waiting for a mutex causes the mutex holder to inherit a
 * priority, but the waiting task_core0 times out, then the holder should
 * disinherit the priority - but only down to the highest priority of any
 * other tasks that are waiting for the same mutex.  This function returns
 * that priority.
 */
    static UBaseType_t_core0 prvGetDisinheritPriorityAfterTimeout_core0( const Queue_t_core0 * const pxQueue_core0 ) PRIVILEGED_FUNCTION_core0;
#endif
/*-----------------------------------------------------------*/

/*
 * Macro to mark a queue as locked.  Locking a queue prevents an ISR from
 * accessing the queue event lists.
 */
#define prvLockQueue_core0( pxQueue_core0 )                            \
    taskENTER_CRITICAL_core0();                                  \
    {                                                      \
        if( ( pxQueue_core0 )->cRxLock_core0 == queueUNLOCKED_core0 )        \
        {                                                  \
            ( pxQueue_core0 )->cRxLock_core0 = queueLOCKED_UNMODIFIED_core0; \
        }                                                  \
        if( ( pxQueue_core0 )->cTxLock_core0 == queueUNLOCKED_core0 )        \
        {                                                  \
            ( pxQueue_core0 )->cTxLock_core0 = queueLOCKED_UNMODIFIED_core0; \
        }                                                  \
    }                                                      \
    taskEXIT_CRITICAL_core0()

/*
 * Macro to increment cTxLock_core0 member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
#define prvIncrementQueueTxLock_core0( pxQueue_core0, cTxLock_core0 )                           \
    {                                                                         \
        const UBaseType_t_core0 uxNumberOfTasks_core0 = uxTaskGetNumberOfTasks_core0();         \
        if( ( UBaseType_t_core0 ) ( cTxLock_core0 ) < uxNumberOfTasks_core0 )                   \
        {                                                                     \
            configASSERT_core0( ( cTxLock_core0 ) != queueINT8_MAX_core0 );                     \
            ( pxQueue_core0 )->cTxLock_core0 = ( int8_t ) ( ( cTxLock_core0 ) + ( int8_t ) 1 ); \
        }                                                                     \
    }

/*
 * Macro to increment cRxLock_core0 member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
#define prvIncrementQueueRxLock_core0( pxQueue_core0, cRxLock_core0 )                           \
    {                                                                         \
        const UBaseType_t_core0 uxNumberOfTasks_core0 = uxTaskGetNumberOfTasks_core0();         \
        if( ( UBaseType_t_core0 ) ( cRxLock_core0 ) < uxNumberOfTasks_core0 )                   \
        {                                                                     \
            configASSERT_core0( ( cRxLock_core0 ) != queueINT8_MAX_core0 );                     \
            ( pxQueue_core0 )->cRxLock_core0 = ( int8_t ) ( ( cRxLock_core0 ) + ( int8_t ) 1 ); \
        }                                                                     \
    }
/*-----------------------------------------------------------*/

BaseType_t_core0 xQueueGenericReset_core0( QueueHandle_t_core0 xQueue_core0,
                               BaseType_t_core0 xNewQueue_core0 )
{
    BaseType_t_core0 xReturn_core0 = pdPASS_core0;
    Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

    configASSERT_core0( pxQueue_core0 );

    if( ( pxQueue_core0 != NULL ) &&
        ( pxQueue_core0->uxLength_core0 >= 1U ) &&
        /* Check for multiplication overflow. */
        ( ( SIZE_MAX / pxQueue_core0->uxLength_core0 ) >= pxQueue_core0->uxItemSize_core0 ) )
    {
        taskENTER_CRITICAL_core0();
        {
            pxQueue_core0->u.xQueue_core0.pcTail_core0 = pxQueue_core0->pcHead_core0 + ( pxQueue_core0->uxLength_core0 * pxQueue_core0->uxItemSize_core0 ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
            pxQueue_core0->uxMessagesWaiting_core0 = ( UBaseType_t_core0 ) 0U;
            pxQueue_core0->pcWriteTo_core0 = pxQueue_core0->pcHead_core0;
            pxQueue_core0->u.xQueue_core0.pcReadFrom_core0 = pxQueue_core0->pcHead_core0 + ( ( pxQueue_core0->uxLength_core0 - 1U ) * pxQueue_core0->uxItemSize_core0 ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
            pxQueue_core0->cRxLock_core0 = queueUNLOCKED_core0;
            pxQueue_core0->cTxLock_core0 = queueUNLOCKED_core0;

            if( xNewQueue_core0 == pdFALSE_core0 )
            {
                /* If there are tasks blocked waiting to read from the queue, then
                 * the tasks will remain blocked as after this function exits the queue
                 * will still be empty.  If there are tasks blocked waiting to write to
                 * the queue, then one should be unblocked as after this function exits
                 * it will be possible to write to it. */
                if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToSend_core0 ) ) == pdFALSE_core0 )
                {
                    if( xTaskRemoveFromEventList_core0( &( pxQueue_core0->xTasksWaitingToSend_core0 ) ) != pdFALSE_core0 )
                    {
                        queueYIELD_IF_USING_PREEMPTION_core0();
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }
            }
            else
            {
                /* Ensure the event queues start in the correct state. */
                vListInitialise_core0( &( pxQueue_core0->xTasksWaitingToSend_core0 ) );
                vListInitialise_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) );
            }
        }
        taskEXIT_CRITICAL_core0();
    }
    else
    {
        xReturn_core0 = pdFAIL_core0;
    }

    configASSERT_core0( xReturn_core0 != pdFAIL_core0 );

    /* A value is returned for calling semantic consistency with previous
     * versions. */
    return xReturn_core0;
}
/*-----------------------------------------------------------*/

#if ( configSUPPORT_STATIC_ALLOCATION_core0 == 1 )

    QueueHandle_t_core0 xQueueGenericCreateStatic_core0( const UBaseType_t_core0 uxQueueLength_core0,
                                             const UBaseType_t_core0 uxItemSize_core0,
                                             uint8_t * pucQueueStorage_core0,
                                             StaticQueue_t_core0 * pxStaticQueue_core0,
                                             const uint8_t ucQueueType_core0 )
    {
        Queue_t_core0 * pxNewQueue_core0 = NULL;

        /* The StaticQueue_t_core0 structure and the queue storage area must be
         * supplied. */
        configASSERT_core0( pxStaticQueue_core0 );

        if( ( uxQueueLength_core0 > ( UBaseType_t_core0 ) 0 ) &&
            ( pxStaticQueue_core0 != NULL ) &&

            /* A queue storage area should be provided if the item size is not 0, and
             * should not be provided if the item size is 0. */
            ( !( ( pucQueueStorage_core0 != NULL ) && ( uxItemSize_core0 == 0 ) ) ) &&
            ( !( ( pucQueueStorage_core0 == NULL ) && ( uxItemSize_core0 != 0 ) ) ) )
        {
            #if ( configASSERT_DEFINED_core0 == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticQueue_t_core0 or StaticSemaphore_t_core0 equals the size of
                 * the real queue and semaphore structures. */
                volatile size_t xSize_core0 = sizeof( StaticQueue_t_core0 );

                /* This assertion cannot be branch covered in unit tests */
                configASSERT_core0( xSize_core0 == sizeof( Queue_t_core0 ) ); /* LCOV_EXCL_BR_LINE */
                ( void ) xSize_core0;                             /* Keeps lint quiet when configASSERT_core0() is not defined. */
            }
            #endif /* configASSERT_DEFINED_core0 */

            /* The address of a statically allocated queue was passed in, use it.
             * The address of a statically allocated storage area was also passed in
             * but is already set. */
            pxNewQueue_core0 = ( Queue_t_core0 * ) pxStaticQueue_core0; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */

            #if ( configSUPPORT_DYNAMIC_ALLOCATION_core0 == 1 )
            {
                /* Queues can be allocated wither statically or dynamically, so
                 * note this queue was allocated statically in case the queue is
                 * later deleted. */
                pxNewQueue_core0->ucStaticallyAllocated_core0 = pdTRUE_core0;
            }
            #endif /* configSUPPORT_DYNAMIC_ALLOCATION_core0 */

            prvInitialiseNewQueue_core0( uxQueueLength_core0, uxItemSize_core0, pucQueueStorage_core0, ucQueueType_core0, pxNewQueue_core0 );
        }
        else
        {
            configASSERT_core0( pxNewQueue_core0 );
            mtCOVERAGE_TEST_MARKER_core0();
        }

        return pxNewQueue_core0;
    }

#endif /* configSUPPORT_STATIC_ALLOCATION_core0 */
/*-----------------------------------------------------------*/

#if ( configSUPPORT_DYNAMIC_ALLOCATION_core0 == 1 )

    QueueHandle_t_core0 xQueueGenericCreate_core0( const UBaseType_t_core0 uxQueueLength_core0,
                                       const UBaseType_t_core0 uxItemSize_core0,
                                       const uint8_t ucQueueType_core0 )
    {
        Queue_t_core0 * pxNewQueue_core0 = NULL;
        size_t xQueueSizeInBytes_core0;
        uint8_t * pucQueueStorage_core0;

        if( ( uxQueueLength_core0 > ( UBaseType_t_core0 ) 0 ) &&
            /* Check for multiplication overflow. */
            ( ( SIZE_MAX / uxQueueLength_core0 ) >= uxItemSize_core0 ) &&
            /* Check for addition overflow. */
            ( ( SIZE_MAX - sizeof( Queue_t_core0 ) ) >= ( uxQueueLength_core0 * uxItemSize_core0 ) ) )
        {
            /* Allocate enough space to hold the maximum number of items that
             * can be in the queue at any time.  It is valid for uxItemSize_core0 to be
             * zero in the case the queue is used as a semaphore. */
            xQueueSizeInBytes_core0 = ( size_t ) ( uxQueueLength_core0 * uxItemSize_core0 ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

            /* Allocate the queue and storage area.  Justification for MISRA
             * deviation as follows:  pvPortMalloc_core0() always ensures returned memory
             * blocks are aligned per the requirements of the MCU stack.  In this case
             * pvPortMalloc_core0() must return a pointer that is guaranteed to meet the
             * alignment requirements of the Queue_t_core0 structure - which in this case
             * is an int8_t *.  Therefore, whenever the stack alignment requirements
             * are greater than or equal to the pointer to char requirements the cast
             * is safe.  In other cases alignment requirements are not strict (one or
             * two bytes). */
            pxNewQueue_core0 = ( Queue_t_core0 * ) pvPortMalloc_core0( sizeof( Queue_t_core0 ) + xQueueSizeInBytes_core0 ); /*lint !e9087 !e9079 see comment above. */

            if( pxNewQueue_core0 != NULL )
            {
                /* Jump past the queue structure to find the location of the queue
                 * storage area. */
                pucQueueStorage_core0 = ( uint8_t * ) pxNewQueue_core0;
                pucQueueStorage_core0 += sizeof( Queue_t_core0 ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */

                #if ( configSUPPORT_STATIC_ALLOCATION_core0 == 1 )
                {
                    /* Queues can be created either statically or dynamically, so
                     * note this task_core0 was created dynamically in case it is later
                     * deleted. */
                    pxNewQueue_core0->ucStaticallyAllocated_core0 = pdFALSE_core0;
                }
                #endif /* configSUPPORT_STATIC_ALLOCATION_core0 */

                prvInitialiseNewQueue_core0( uxQueueLength_core0, uxItemSize_core0, pucQueueStorage_core0, ucQueueType_core0, pxNewQueue_core0 );
            }
            else
            {
                traceQUEUE_CREATE_FAILED_core0( ucQueueType_core0 );
                mtCOVERAGE_TEST_MARKER_core0();
            }
        }
        else
        {
            configASSERT_core0( pxNewQueue_core0 );
            mtCOVERAGE_TEST_MARKER_core0();
        }

        return pxNewQueue_core0;
    }

#endif /* configSUPPORT_STATIC_ALLOCATION_core0 */
/*-----------------------------------------------------------*/

static void prvInitialiseNewQueue_core0( const UBaseType_t_core0 uxQueueLength_core0,
                                   const UBaseType_t_core0 uxItemSize_core0,
                                   uint8_t * pucQueueStorage_core0,
                                   const uint8_t ucQueueType_core0,
                                   Queue_t_core0 * pxNewQueue_core0 )
{
    /* Remove compiler warnings about unused parameters should
     * configUSE_TRACE_FACILITY_core0 not be set to 1. */
    ( void ) ucQueueType_core0;

    if( uxItemSize_core0 == ( UBaseType_t_core0 ) 0 )
    {
        /* No RAM was allocated for the queue storage area, but PC head cannot
         * be set to NULL because NULL is used as a key to say the queue is used as
         * a mutex.  Therefore just set pcHead_core0 to point to the queue as a benign
         * value that is known to be within the memory map. */
        pxNewQueue_core0->pcHead_core0 = ( int8_t * ) pxNewQueue_core0;
    }
    else
    {
        /* Set the head to the start of the queue storage area. */
        pxNewQueue_core0->pcHead_core0 = ( int8_t * ) pucQueueStorage_core0;
    }

    /* Initialise the queue members as described where the queue type is
     * defined. */
    pxNewQueue_core0->uxLength_core0 = uxQueueLength_core0;
    pxNewQueue_core0->uxItemSize_core0 = uxItemSize_core0;
    ( void ) xQueueGenericReset_core0( pxNewQueue_core0, pdTRUE_core0 );

    #if ( configUSE_TRACE_FACILITY_core0 == 1 )
    {
        pxNewQueue_core0->ucQueueType_core0 = ucQueueType_core0;
    }
    #endif /* configUSE_TRACE_FACILITY_core0 */

    #if ( configUSE_QUEUE_SETS_core0 == 1 )
    {
        pxNewQueue_core0->pxQueueSetContainer_core0 = NULL;
    }
    #endif /* configUSE_QUEUE_SETS_core0 */

    traceQUEUE_CREATE_core0( pxNewQueue_core0 );
}
/*-----------------------------------------------------------*/

#if ( configUSE_MUTEXES_core0 == 1 )

    static void prvInitialiseMutex_core0( Queue_t_core0 * pxNewQueue_core0 )
    {
        if( pxNewQueue_core0 != NULL )
        {
            /* The queue create function will set all the queue structure members
            * correctly for a generic queue, but this function is creating a
            * mutex.  Overwrite those members that need to be set differently -
            * in particular the information required for priority inheritance. */
            pxNewQueue_core0->u.xSemaphore_core0.xMutexHolder_core0 = NULL;
            pxNewQueue_core0->uxQueueType_core0 = queueQUEUE_IS_MUTEX_core0;

            /* In case this is a recursive mutex. */
            pxNewQueue_core0->u.xSemaphore_core0.uxRecursiveCallCount_core0 = 0;

            traceCREATE_MUTEX_core0( pxNewQueue_core0 );

            /* Start with the semaphore in the expected state. */
            ( void ) xQueueGenericSend_core0( pxNewQueue_core0, NULL, ( TickType_t_core0 ) 0U, queueSEND_TO_BACK_core0 );
        }
        else
        {
            traceCREATE_MUTEX_FAILED_core0();
        }
    }

#endif /* configUSE_MUTEXES_core0 */
/*-----------------------------------------------------------*/

#if ( ( configUSE_MUTEXES_core0 == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION_core0 == 1 ) )

    QueueHandle_t_core0 xQueueCreateMutex_core0( const uint8_t ucQueueType_core0 )
    {
        QueueHandle_t_core0 xNewQueue_core0;
        const UBaseType_t_core0 uxMutexLength = ( UBaseType_t_core0 ) 1, uxMutexSize_core0 = ( UBaseType_t_core0 ) 0;

        xNewQueue_core0 = xQueueGenericCreate_core0( uxMutexLength, uxMutexSize_core0, ucQueueType_core0 );
        prvInitialiseMutex_core0( ( Queue_t_core0 * ) xNewQueue_core0 );

        return xNewQueue_core0;
    }

#endif /* configUSE_MUTEXES_core0 */
/*-----------------------------------------------------------*/

#if ( ( configUSE_MUTEXES_core0 == 1 ) && ( configSUPPORT_STATIC_ALLOCATION_core0 == 1 ) )

    QueueHandle_t_core0 xQueueCreateMutexStatic_core0( const uint8_t ucQueueType_core0,
                                           StaticQueue_t_core0 * pxStaticQueue_core0 )
    {
        QueueHandle_t_core0 xNewQueue_core0;
        const UBaseType_t_core0 uxMutexLength = ( UBaseType_t_core0 ) 1, uxMutexSize_core0 = ( UBaseType_t_core0 ) 0;

        /* Prevent compiler warnings about unused parameters if
         * configUSE_TRACE_FACILITY_core0 does not equal 1. */
        ( void ) ucQueueType_core0;

        xNewQueue_core0 = xQueueGenericCreateStatic_core0( uxMutexLength, uxMutexSize_core0, NULL, pxStaticQueue_core0, ucQueueType_core0 );
        prvInitialiseMutex_core0( ( Queue_t_core0 * ) xNewQueue_core0 );

        return xNewQueue_core0;
    }

#endif /* configUSE_MUTEXES_core0 */
/*-----------------------------------------------------------*/

#if ( ( configUSE_MUTEXES_core0 == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder_core0 == 1 ) )

    TaskHandle_t_core0 xQueueGetMutexHolder_core0( QueueHandle_t_core0 xSemaphore_core0 )
    {
        TaskHandle_t_core0 pxReturn_core0;
        Queue_t_core0 * const pxSemaphore = ( Queue_t_core0 * ) xSemaphore_core0;

        configASSERT_core0( xSemaphore_core0 );

        /* This function is called by xSemaphoreGetMutexHolder_core0(), and should not
         * be called directly.  Note:  This is a good way of determining if the
         * calling task_core0 is the mutex holder, but not a good way of determining the
         * identity of the mutex holder, as the holder may change between the
         * following critical section exiting and the function returning. */
        taskENTER_CRITICAL_core0();
        {
            if( pxSemaphore->uxQueueType_core0 == queueQUEUE_IS_MUTEX_core0 )
            {
                pxReturn_core0 = pxSemaphore->u.xSemaphore_core0.xMutexHolder_core0;
            }
            else
            {
                pxReturn_core0 = NULL;
            }
        }
        taskEXIT_CRITICAL_core0();

        return pxReturn_core0;
    } /*lint !e818 xSemaphore_core0 cannot be a pointer to const because it is a typedef. */

#endif /* if ( ( configUSE_MUTEXES_core0 == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder_core0 == 1 ) ) */
/*-----------------------------------------------------------*/

#if ( ( configUSE_MUTEXES_core0 == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder_core0 == 1 ) )

    TaskHandle_t_core0 xQueueGetMutexHolderFromISR_core0( QueueHandle_t_core0 xSemaphore_core0 )
    {
        TaskHandle_t_core0 pxReturn_core0;

        configASSERT_core0( xSemaphore_core0 );

        /* Mutexes cannot be used in interrupt service routines, so the mutex
         * holder should not change in an ISR, and therefore a critical section is
         * not required here. */
        if( ( ( Queue_t_core0 * ) xSemaphore_core0 )->uxQueueType_core0 == queueQUEUE_IS_MUTEX_core0 )
        {
            pxReturn_core0 = ( ( Queue_t_core0 * ) xSemaphore_core0 )->u.xSemaphore_core0.xMutexHolder_core0;
        }
        else
        {
            pxReturn_core0 = NULL;
        }

        return pxReturn_core0;
    } /*lint !e818 xSemaphore_core0 cannot be a pointer to const because it is a typedef. */

#endif /* if ( ( configUSE_MUTEXES_core0 == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder_core0 == 1 ) ) */
/*-----------------------------------------------------------*/

#if ( configUSE_RECURSIVE_MUTEXES_core0 == 1 )

    BaseType_t_core0 xQueueGiveMutexRecursive_core0( QueueHandle_t_core0 xMutex_core0 )
    {
        BaseType_t_core0 xReturn_core0;
        Queue_t_core0 * const pxMutex_core0 = ( Queue_t_core0 * ) xMutex_core0;

        configASSERT_core0( pxMutex_core0 );

        /* If this is the task_core0 that holds the mutex then xMutexHolder_core0 will not
         * change outside of this task_core0.  If this task_core0 does not hold the mutex then
         * pxMutexHolder_core0 can never coincidentally equal the tasks handle, and as
         * this is the only condition we are interested in it does not matter if
         * pxMutexHolder_core0 is accessed simultaneously by another task_core0.  Therefore no
         * mutual exclusion is required to test the pxMutexHolder_core0 variable. */
        if( pxMutex_core0->u.xSemaphore_core0.xMutexHolder_core0 == xTaskGetCurrentTaskHandle_core0() )
        {
            traceGIVE_MUTEX_RECURSIVE_core0( pxMutex_core0 );

            /* uxRecursiveCallCount_core0 cannot be zero if xMutexHolder_core0 is equal to
             * the task_core0 handle, therefore no underflow check is required.  Also,
             * uxRecursiveCallCount_core0 is only modified by the mutex holder, and as
             * there can only be one, no mutual exclusion is required to modify the
             * uxRecursiveCallCount_core0 member. */
            ( pxMutex_core0->u.xSemaphore_core0.uxRecursiveCallCount_core0 )--;

            /* Has the recursive call count unwound to 0? */
            if( pxMutex_core0->u.xSemaphore_core0.uxRecursiveCallCount_core0 == ( UBaseType_t_core0 ) 0 )
            {
                /* Return the mutex.  This will automatically unblock any other
                 * task_core0 that might be waiting to access the mutex. */
                ( void ) xQueueGenericSend_core0( pxMutex_core0, NULL, queueMUTEX_GIVE_BLOCK_TIME_core0, queueSEND_TO_BACK_core0 );
            }
            else
            {
                mtCOVERAGE_TEST_MARKER_core0();
            }

            xReturn_core0 = pdPASS_core0;
        }
        else
        {
            /* The mutex cannot be given because the calling task_core0 is not the
             * holder. */
            xReturn_core0 = pdFAIL_core0;

            traceGIVE_MUTEX_RECURSIVE_FAILED_core0( pxMutex_core0 );
        }

        return xReturn_core0;
    }

#endif /* configUSE_RECURSIVE_MUTEXES_core0 */
/*-----------------------------------------------------------*/

#if ( configUSE_RECURSIVE_MUTEXES_core0 == 1 )

    BaseType_t_core0 xQueueTakeMutexRecursive_core0( QueueHandle_t_core0 xMutex_core0,
                                         TickType_t_core0 xTicksToWait_core0 )
    {
        BaseType_t_core0 xReturn_core0;
        Queue_t_core0 * const pxMutex_core0 = ( Queue_t_core0 * ) xMutex_core0;

        configASSERT_core0( pxMutex_core0 );

        /* Comments regarding mutual exclusion as per those within
         * xQueueGiveMutexRecursive_core0(). */

        traceTAKE_MUTEX_RECURSIVE_core0( pxMutex_core0 );

        if( pxMutex_core0->u.xSemaphore_core0.xMutexHolder_core0 == xTaskGetCurrentTaskHandle_core0() )
        {
            ( pxMutex_core0->u.xSemaphore_core0.uxRecursiveCallCount_core0 )++;
            xReturn_core0 = pdPASS_core0;
        }
        else
        {
            xReturn_core0 = xQueueSemaphoreTake_core0( pxMutex_core0, xTicksToWait_core0 );

            /* pdPASS_core0 will only be returned if the mutex was successfully
             * obtained.  The calling task_core0 may have entered the Blocked state
             * before reaching here. */
            if( xReturn_core0 != pdFAIL_core0 )
            {
                ( pxMutex_core0->u.xSemaphore_core0.uxRecursiveCallCount_core0 )++;
            }
            else
            {
                traceTAKE_MUTEX_RECURSIVE_FAILED_core0( pxMutex_core0 );
            }
        }

        return xReturn_core0;
    }

#endif /* configUSE_RECURSIVE_MUTEXES_core0 */
/*-----------------------------------------------------------*/

#if ( ( configUSE_COUNTING_SEMAPHORES_core0 == 1 ) && ( configSUPPORT_STATIC_ALLOCATION_core0 == 1 ) )

    QueueHandle_t_core0 xQueueCreateCountingSemaphoreStatic_core0( const UBaseType_t_core0 uxMaxCount_core0,
                                                       const UBaseType_t_core0 uxInitialCount_core0,
                                                       StaticQueue_t_core0 * pxStaticQueue_core0 )
    {
        QueueHandle_t_core0 xHandle_core0 = NULL;

        if( ( uxMaxCount_core0 != 0 ) &&
            ( uxInitialCount_core0 <= uxMaxCount_core0 ) )
        {
            xHandle_core0 = xQueueGenericCreateStatic_core0( uxMaxCount_core0, queueSEMAPHORE_QUEUE_ITEM_LENGTH_core0, NULL, pxStaticQueue_core0, queueQUEUE_TYPE_COUNTING_SEMAPHORE );

            if( xHandle_core0 != NULL )
            {
                ( ( Queue_t_core0 * ) xHandle_core0 )->uxMessagesWaiting_core0 = uxInitialCount_core0;

                traceCREATE_COUNTING_SEMAPHORE_core0();
            }
            else
            {
                traceCREATE_COUNTING_SEMAPHORE_FAILED_core0();
            }
        }
        else
        {
            configASSERT_core0( xHandle_core0 );
            mtCOVERAGE_TEST_MARKER_core0();
        }

        return xHandle_core0;
    }

#endif /* ( ( configUSE_COUNTING_SEMAPHORES_core0 == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION_core0 == 1 ) ) */
/*-----------------------------------------------------------*/

#if ( ( configUSE_COUNTING_SEMAPHORES_core0 == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION_core0 == 1 ) )

    QueueHandle_t_core0 xQueueCreateCountingSemaphore_core0( const UBaseType_t_core0 uxMaxCount_core0,
                                                 const UBaseType_t_core0 uxInitialCount_core0 )
    {
        QueueHandle_t_core0 xHandle_core0 = NULL;

        if( ( uxMaxCount_core0 != 0 ) &&
            ( uxInitialCount_core0 <= uxMaxCount_core0 ) )
        {
            xHandle_core0 = xQueueGenericCreate_core0( uxMaxCount_core0, queueSEMAPHORE_QUEUE_ITEM_LENGTH_core0, queueQUEUE_TYPE_COUNTING_SEMAPHORE_core0 );

            if( xHandle_core0 != NULL )
            {
                ( ( Queue_t_core0 * ) xHandle_core0 )->uxMessagesWaiting_core0 = uxInitialCount_core0;

                traceCREATE_COUNTING_SEMAPHORE_core0();
            }
            else
            {
                traceCREATE_COUNTING_SEMAPHORE_FAILED_core0();
            }
        }
        else
        {
            configASSERT_core0( xHandle_core0 );
            mtCOVERAGE_TEST_MARKER_core0();
        }

        return xHandle_core0;
    }

#endif /* ( ( configUSE_COUNTING_SEMAPHORES_core0 == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION_core0 == 1 ) ) */
/*-----------------------------------------------------------*/

BaseType_t_core0 xQueueGenericSend_core0( QueueHandle_t_core0 xQueue_core0,
                              const void * const pvItemToQueue_core0,
                              TickType_t_core0 xTicksToWait_core0,
                              const BaseType_t_core0 xCopyPosition_core0 )
{
    BaseType_t_core0 xEntryTimeSet = pdFALSE_core0, xYieldRequired_core0;
    TimeOut_t_core0 xTimeOut;
    Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

    configASSERT_core0( pxQueue_core0 );
    configASSERT_core0( !( ( pvItemToQueue_core0 == NULL ) && ( pxQueue_core0->uxItemSize_core0 != ( UBaseType_t_core0 ) 0U ) ) );
    configASSERT_core0( !( ( xCopyPosition_core0 == queueOVERWRITE_core0 ) && ( pxQueue_core0->uxLength_core0 != 1 ) ) );
    #if ( ( INCLUDE_xTaskGetSchedulerState_core0 == 1 ) || ( configUSE_TIMERS_core0 == 1 ) )
    {
        configASSERT_core0( !( ( xTaskGetSchedulerState_core0() == taskSCHEDULER_SUSPENDED_core0 ) && ( xTicksToWait_core0 != 0 ) ) );
    }
    #endif

    /*lint -save -e904 This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    for( ; ; )
    {
        taskENTER_CRITICAL_core0();
        {
            /* Is there room on the queue now?  The running task_core0 must be the
             * highest priority task_core0 wanting to access the queue.  If the head item
             * in the queue is to be overwritten then it does not matter if the
             * queue is full. */
            if( ( pxQueue_core0->uxMessagesWaiting_core0 < pxQueue_core0->uxLength_core0 ) || ( xCopyPosition_core0 == queueOVERWRITE_core0 ) )
            {
                traceQUEUE_SEND_core0( pxQueue_core0 );

                #if ( configUSE_QUEUE_SETS_core0 == 1 )
                {
                    const UBaseType_t_core0 uxPreviousMessagesWaiting = pxQueue_core0->uxMessagesWaiting_core0;

                    xYieldRequired_core0 = prvCopyDataToQueue_core0( pxQueue_core0, pvItemToQueue_core0, xCopyPosition_core0 );

                    if( pxQueue_core0->pxQueueSetContainer_core0 != NULL )
                    {
                        if( ( xCopyPosition_core0 == queueOVERWRITE_core0 ) && ( uxPreviousMessagesWaiting != ( UBaseType_t_core0 ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER_core0();
                        }
                        else if( prvNotifyQueueSetContainer_core0( pxQueue_core0 ) != pdFALSE_core0 )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task_core0 to
                             * unblock. A context switch is required. */
                            queueYIELD_IF_USING_PREEMPTION_core0();
                        }
                        else
                        {
                            mtCOVERAGE_TEST_MARKER_core0();
                        }
                    }
                    else
                    {
                        /* If there was a task_core0 waiting for data to arrive on the
                         * queue then unblock it now. */
                        if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) == pdFALSE_core0 )
                        {
                            if( xTaskRemoveFromEventList_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) != pdFALSE_core0 )
                            {
                                /* The unblocked task_core0 has a priority higher than
                                 * our own so yield immediately.  Yes it is ok to
                                 * do this from within the critical section - the
                                 * kernel takes care of that. */
                                queueYIELD_IF_USING_PREEMPTION_core0();
                            }
                            else
                            {
                                mtCOVERAGE_TEST_MARKER_core0();
                            }
                        }
                        else if( xYieldRequired_core0 != pdFALSE_core0 )
                        {
                            /* This path is a special case that will only get
                             * executed if the task_core0 was holding multiple mutexes
                             * and the mutexes were given back in an order that is
                             * different to that in which they were taken. */
                            queueYIELD_IF_USING_PREEMPTION_core0();
                        }
                        else
                        {
                            mtCOVERAGE_TEST_MARKER_core0();
                        }
                    }
                }
                #else /* configUSE_QUEUE_SETS_core0 */
                {
                    xYieldRequired_core0 = prvCopyDataToQueue_core0( pxQueue_core0, pvItemToQueue_core0, xCopyPosition_core0 );

                    /* If there was a task_core0 waiting for data to arrive on the
                     * queue then unblock it now. */
                    if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) == pdFALSE_core0 )
                    {
                        if( xTaskRemoveFromEventList_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) != pdFALSE_core0 )
                        {
                            /* The unblocked task_core0 has a priority higher than
                             * our own so yield immediately.  Yes it is ok to do
                             * this from within the critical section - the kernel
                             * takes care of that. */
                            queueYIELD_IF_USING_PREEMPTION_core0();
                        }
                        else
                        {
                            mtCOVERAGE_TEST_MARKER_core0();
                        }
                    }
                    else if( xYieldRequired_core0 != pdFALSE_core0 )
                    {
                        /* This path is a special case that will only get
                         * executed if the task_core0 was holding multiple mutexes and
                         * the mutexes were given back in an order that is
                         * different to that in which they were taken. */
                        queueYIELD_IF_USING_PREEMPTION_core0();
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                #endif /* configUSE_QUEUE_SETS_core0 */

                taskEXIT_CRITICAL_core0();
                return pdPASS_core0;
            }
            else
            {
                if( xTicksToWait_core0 == ( TickType_t_core0 ) 0 )
                {
                    /* The queue was full and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL_core0();

                    /* Return to the original privilege level before exiting
                     * the function. */
                    traceQUEUE_SEND_FAILED_core0( pxQueue_core0 );
                    return errQUEUE_FULL_core0;
                }
                else if( xEntryTimeSet == pdFALSE_core0 )
                {
                    /* The queue was full and a block time was specified so
                     * configure the timeout structure. */
                    vTaskInternalSetTimeOutState_core0( &xTimeOut );
                    xEntryTimeSet = pdTRUE_core0;
                }
                else
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER_core0();
                }
            }
        }
        taskEXIT_CRITICAL_core0();

        /* Interrupts and other tasks can send to and receive from the queue
         * now the critical section has been exited. */

        vTaskSuspendAll_core0();
        prvLockQueue_core0( pxQueue_core0 );

        /* Update the timeout state to see if it has expired yet. */
        if( xTaskCheckForTimeOut_core0( &xTimeOut, &xTicksToWait_core0 ) == pdFALSE_core0 )
        {
            if( prvIsQueueFull_core0( pxQueue_core0 ) != pdFALSE_core0 )
            {
                traceBLOCKING_ON_QUEUE_SEND_core0( pxQueue_core0 );
                vTaskPlaceOnEventList_core0( &( pxQueue_core0->xTasksWaitingToSend_core0 ), xTicksToWait_core0 );

                /* Unlocking the queue means queue events can effect the
                 * event list. It is possible that interrupts occurring now
                 * remove this task_core0 from the event list again - but as the
                 * scheduler is suspended the task_core0 will go onto the pending
                 * ready list instead of the actual ready list. */
                prvUnlockQueue_core0( pxQueue_core0 );

                /* Resuming the scheduler will move tasks from the pending
                 * ready list into the ready list - so it is feasible that this
                 * task_core0 is already in the ready list before it yields - in which
                 * case the yield will not cause a context switch unless there
                 * is also a higher priority task_core0 in the pending ready list. */
                if( xTaskResumeAll_core0() == pdFALSE_core0 )
                {
                    portYIELD_WITHIN_API_core0();
                }
            }
            else
            {
                /* Try again. */
                prvUnlockQueue_core0( pxQueue_core0 );
                ( void ) xTaskResumeAll_core0();
            }
        }
        else
        {
            /* The timeout has expired. */
            prvUnlockQueue_core0( pxQueue_core0 );
            ( void ) xTaskResumeAll_core0();

            traceQUEUE_SEND_FAILED_core0( pxQueue_core0 );
            return errQUEUE_FULL_core0;
        }
    } /*lint -restore */

    return 0U;
}
/*-----------------------------------------------------------*/

BaseType_t_core0 xQueueGenericSendFromISR_core0( QueueHandle_t_core0 xQueue_core0,
                                     const void * const pvItemToQueue_core0,
                                     BaseType_t_core0 * const pxHigherPriorityTaskWoken_core0,
                                     const BaseType_t_core0 xCopyPosition_core0 )
{
    BaseType_t_core0 xReturn_core0;
    UBaseType_t_core0 uxSavedInterruptStatus_core0;
    Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

    configASSERT_core0( pxQueue_core0 );
    configASSERT_core0( !( ( pvItemToQueue_core0 == NULL ) && ( pxQueue_core0->uxItemSize_core0 != ( UBaseType_t_core0 ) 0U ) ) );
    configASSERT_core0( !( ( xCopyPosition_core0 == queueOVERWRITE_core0 ) && ( pxQueue_core0->uxLength_core0 != 1 ) ) );

    /* RTOS ports that support interrupt nesting have the concept of a maximum
     * system call (or maximum API call) interrupt priority.  Interrupts that are
     * above the maximum system call priority are kept permanently enabled, even
     * when the RTOS kernel is in a critical section, but cannot make any calls to
     * FreeRTOS_core0 API functions.  If configASSERT_core0() is defined in FreeRTOSConfig.h
     * then portASSERT_IF_INTERRUPT_PRIORITY_INVALID_core0() will result in an assertion
     * failure if a FreeRTOS_core0 API function is called from an interrupt that has been
     * assigned a priority above the configured maximum system call priority.
     * Only FreeRTOS_core0 functions that end in FromISR can be called from interrupts
     * that have been assigned a priority at or (logically) below the maximum
     * system call interrupt priority.  FreeRTOS_core0 maintains a separate interrupt
     * safe API to ensure interrupt entry is as fast and as simple as possible.
     * More information (albeit Cortex-M specific) is provided on the following
     * link: https://www.FreeRTOS_core0.org/RTOS-Cortex-M3-M4.html */
    portASSERT_IF_INTERRUPT_PRIORITY_INVALID_core0();

    /* Similar to xQueueGenericSend_core0, except without blocking if there is no room
     * in the queue.  Also don't directly wake a task_core0 that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task_core0 with a higher priority than us been woken by this
     * post). */
    uxSavedInterruptStatus_core0 = portSET_INTERRUPT_MASK_FROM_ISR_core0();
    {
        if( ( pxQueue_core0->uxMessagesWaiting_core0 < pxQueue_core0->uxLength_core0 ) || ( xCopyPosition_core0 == queueOVERWRITE_core0 ) )
        {
            const int8_t cTxLock_core0 = pxQueue_core0->cTxLock_core0;
            const UBaseType_t_core0 uxPreviousMessagesWaiting = pxQueue_core0->uxMessagesWaiting_core0;

            traceQUEUE_SEND_FROM_ISR_core0( pxQueue_core0 );

            /* Semaphores use xQueueGiveFromISR_core0(), so pxQueue_core0 will not be a
             *  semaphore or mutex.  That means prvCopyDataToQueue_core0() cannot result
             *  in a task_core0 disinheriting a priority and prvCopyDataToQueue_core0() can be
             *  called here even though the disinherit function does not check if
             *  the scheduler is suspended before accessing the ready lists. */
            ( void ) prvCopyDataToQueue_core0( pxQueue_core0, pvItemToQueue_core0, xCopyPosition_core0 );

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            if( cTxLock_core0 == queueUNLOCKED_core0 )
            {
                #if ( configUSE_QUEUE_SETS_core0 == 1 )
                {
                    if( pxQueue_core0->pxQueueSetContainer_core0 != NULL )
                    {
                        if( ( xCopyPosition_core0 == queueOVERWRITE_core0 ) && ( uxPreviousMessagesWaiting != ( UBaseType_t_core0 ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER_core0();
                        }
                        else if( prvNotifyQueueSetContainer_core0( pxQueue_core0 ) != pdFALSE_core0 )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task_core0 to
                             * unblock.  A context switch is required. */
                            if( pxHigherPriorityTaskWoken_core0 != NULL )
                            {
                                *pxHigherPriorityTaskWoken_core0 = pdTRUE_core0;
                            }
                            else
                            {
                                mtCOVERAGE_TEST_MARKER_core0();
                            }
                        }
                        else
                        {
                            mtCOVERAGE_TEST_MARKER_core0();
                        }
                    }
                    else
                    {
                        if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) == pdFALSE_core0 )
                        {
                            if( xTaskRemoveFromEventList_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) != pdFALSE_core0 )
                            {
                                /* The task_core0 waiting has a higher priority so
                                 *  record that a context switch is required. */
                                if( pxHigherPriorityTaskWoken_core0 != NULL )
                                {
                                    *pxHigherPriorityTaskWoken_core0 = pdTRUE_core0;
                                }
                                else
                                {
                                    mtCOVERAGE_TEST_MARKER_core0();
                                }
                            }
                            else
                            {
                                mtCOVERAGE_TEST_MARKER_core0();
                            }
                        }
                        else
                        {
                            mtCOVERAGE_TEST_MARKER_core0();
                        }
                    }
                }
                #else /* configUSE_QUEUE_SETS_core0 */
                {
                    if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) == pdFALSE_core0 )
                    {
                        if( xTaskRemoveFromEventList_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) != pdFALSE_core0 )
                        {
                            /* The task_core0 waiting has a higher priority so record that a
                             * context switch is required. */
                            if( pxHigherPriorityTaskWoken_core0 != NULL )
                            {
                                *pxHigherPriorityTaskWoken_core0 = pdTRUE_core0;
                            }
                            else
                            {
                                mtCOVERAGE_TEST_MARKER_core0();
                            }
                        }
                        else
                        {
                            mtCOVERAGE_TEST_MARKER_core0();
                        }
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }

                    /* Not used in this path. */
                    ( void ) uxPreviousMessagesWaiting;
                }
                #endif /* configUSE_QUEUE_SETS_core0 */
            }
            else
            {
                /* Increment the lock count so the task_core0 that unlocks the queue
                 * knows that data was posted while it was locked. */
                prvIncrementQueueTxLock_core0( pxQueue_core0, cTxLock_core0 );
            }

            xReturn_core0 = pdPASS_core0;
        }
        else
        {
            traceQUEUE_SEND_FROM_ISR_FAILED_core0( pxQueue_core0 );
            xReturn_core0 = errQUEUE_FULL_core0;
        }
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR_core0( uxSavedInterruptStatus_core0 );

    return xReturn_core0;
}
/*-----------------------------------------------------------*/

BaseType_t_core0 xQueueGiveFromISR_core0( QueueHandle_t_core0 xQueue_core0,
                              BaseType_t_core0 * const pxHigherPriorityTaskWoken_core0 )
{
    BaseType_t_core0 xReturn_core0;
    UBaseType_t_core0 uxSavedInterruptStatus_core0;
    Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

    /* Similar to xQueueGenericSendFromISR_core0() but used with semaphores where the
     * item size is 0.  Don't directly wake a task_core0 that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task_core0 with a higher priority than us been woken by this
     * post). */

    configASSERT_core0( pxQueue_core0 );

    /* xQueueGenericSendFromISR_core0() should be used instead of xQueueGiveFromISR_core0()
     * if the item size is not 0. */
    configASSERT_core0( pxQueue_core0->uxItemSize_core0 == 0 );

    /* Normally a mutex would not be given from an interrupt, especially if
     * there is a mutex holder, as priority inheritance makes no sense for an
     * interrupts, only tasks. */
    configASSERT_core0( !( ( pxQueue_core0->uxQueueType_core0 == queueQUEUE_IS_MUTEX_core0 ) && ( pxQueue_core0->u.xSemaphore_core0.xMutexHolder_core0 != NULL ) ) );

    /* RTOS ports that support interrupt nesting have the concept of a maximum
     * system call (or maximum API call) interrupt priority.  Interrupts that are
     * above the maximum system call priority are kept permanently enabled, even
     * when the RTOS kernel is in a critical section, but cannot make any calls to
     * FreeRTOS_core0 API functions.  If configASSERT_core0() is defined in FreeRTOSConfig.h
     * then portASSERT_IF_INTERRUPT_PRIORITY_INVALID_core0() will result in an assertion
     * failure if a FreeRTOS_core0 API function is called from an interrupt that has been
     * assigned a priority above the configured maximum system call priority.
     * Only FreeRTOS_core0 functions that end in FromISR can be called from interrupts
     * that have been assigned a priority at or (logically) below the maximum
     * system call interrupt priority.  FreeRTOS_core0 maintains a separate interrupt
     * safe API to ensure interrupt entry is as fast and as simple as possible.
     * More information (albeit Cortex-M specific) is provided on the following
     * link: https://www.FreeRTOS_core0.org/RTOS-Cortex-M3-M4.html */
    portASSERT_IF_INTERRUPT_PRIORITY_INVALID_core0();

    uxSavedInterruptStatus_core0 = portSET_INTERRUPT_MASK_FROM_ISR_core0();
    {
        const UBaseType_t_core0 uxMessagesWaiting_core0 = pxQueue_core0->uxMessagesWaiting_core0;

        /* When the queue is used to implement a semaphore no data is ever
         * moved through the queue but it is still valid to see if the queue 'has
         * space'. */
        if( uxMessagesWaiting_core0 < pxQueue_core0->uxLength_core0 )
        {
            const int8_t cTxLock_core0 = pxQueue_core0->cTxLock_core0;

            traceQUEUE_SEND_FROM_ISR_core0( pxQueue_core0 );

            /* A task_core0 can only have an inherited priority if it is a mutex
             * holder - and if there is a mutex holder then the mutex cannot be
             * given from an ISR.  As this is the ISR version of the function it
             * can be assumed there is no mutex holder and no need to determine if
             * priority disinheritance is needed.  Simply increase the count of
             * messages (semaphores) available. */
            pxQueue_core0->uxMessagesWaiting_core0 = uxMessagesWaiting_core0 + ( UBaseType_t_core0 ) 1;

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            if( cTxLock_core0 == queueUNLOCKED_core0 )
            {
                #if ( configUSE_QUEUE_SETS_core0 == 1 )
                {
                    if( pxQueue_core0->pxQueueSetContainer_core0 != NULL )
                    {
                        if( prvNotifyQueueSetContainer_core0( pxQueue_core0 ) != pdFALSE_core0 )
                        {
                            /* The semaphore is a member of a queue set, and
                             * posting to the queue set caused a higher priority
                             * task_core0 to unblock.  A context switch is required. */
                            if( pxHigherPriorityTaskWoken_core0 != NULL )
                            {
                                *pxHigherPriorityTaskWoken_core0 = pdTRUE_core0;
                            }
                            else
                            {
                                mtCOVERAGE_TEST_MARKER_core0();
                            }
                        }
                        else
                        {
                            mtCOVERAGE_TEST_MARKER_core0();
                        }
                    }
                    else
                    {
                        if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) == pdFALSE_core0 )
                        {
                            if( xTaskRemoveFromEventList_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) != pdFALSE_core0 )
                            {
                                /* The task_core0 waiting has a higher priority so
                                 *  record that a context switch is required. */
                                if( pxHigherPriorityTaskWoken_core0 != NULL )
                                {
                                    *pxHigherPriorityTaskWoken_core0 = pdTRUE_core0;
                                }
                                else
                                {
                                    mtCOVERAGE_TEST_MARKER_core0();
                                }
                            }
                            else
                            {
                                mtCOVERAGE_TEST_MARKER_core0();
                            }
                        }
                        else
                        {
                            mtCOVERAGE_TEST_MARKER_core0();
                        }
                    }
                }
                #else /* configUSE_QUEUE_SETS_core0 */
                {
                    if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) == pdFALSE_core0 )
                    {
                        if( xTaskRemoveFromEventList_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) != pdFALSE_core0 )
                        {
                            /* The task_core0 waiting has a higher priority so record that a
                             * context switch is required. */
                            if( pxHigherPriorityTaskWoken_core0 != NULL )
                            {
                                *pxHigherPriorityTaskWoken_core0 = pdTRUE_core0;
                            }
                            else
                            {
                                mtCOVERAGE_TEST_MARKER_core0();
                            }
                        }
                        else
                        {
                            mtCOVERAGE_TEST_MARKER_core0();
                        }
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                #endif /* configUSE_QUEUE_SETS_core0 */
            }
            else
            {
                /* Increment the lock count so the task_core0 that unlocks the queue
                 * knows that data was posted while it was locked. */
                prvIncrementQueueTxLock_core0( pxQueue_core0, cTxLock_core0 );
            }

            xReturn_core0 = pdPASS_core0;
        }
        else
        {
            traceQUEUE_SEND_FROM_ISR_FAILED_core0( pxQueue_core0 );
            xReturn_core0 = errQUEUE_FULL_core0;
        }
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR_core0( uxSavedInterruptStatus_core0 );

    return xReturn_core0;
}
/*-----------------------------------------------------------*/

BaseType_t_core0 xQueueReceive_core0( QueueHandle_t_core0 xQueue_core0,
                          void * const pvBuffer_core0,
                          TickType_t_core0 xTicksToWait_core0 )
{
    BaseType_t_core0 xEntryTimeSet = pdFALSE_core0;
    TimeOut_t_core0 xTimeOut;
    Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

    /* Check the pointer is not NULL. */
    configASSERT_core0( ( pxQueue_core0 ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer). */
    configASSERT_core0( !( ( ( pvBuffer_core0 ) == NULL ) && ( ( pxQueue_core0 )->uxItemSize_core0 != ( UBaseType_t_core0 ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #if ( ( INCLUDE_xTaskGetSchedulerState_core0 == 1 ) || ( configUSE_TIMERS_core0 == 1 ) )
    {
        configASSERT_core0( !( ( xTaskGetSchedulerState_core0() == taskSCHEDULER_SUSPENDED_core0 ) && ( xTicksToWait_core0 != 0 ) ) );
    }
    #endif

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    for( ; ; )
    {
        taskENTER_CRITICAL_core0();
        {
            const UBaseType_t_core0 uxMessagesWaiting_core0 = pxQueue_core0->uxMessagesWaiting_core0;

            /* Is there data in the queue now?  To be running the calling task_core0
             * must be the highest priority task_core0 wanting to access the queue. */
            if( uxMessagesWaiting_core0 > ( UBaseType_t_core0 ) 0 )
            {
                /* Data available, remove one item. */
                prvCopyDataFromQueue_core0( pxQueue_core0, pvBuffer_core0 );
                traceQUEUE_RECEIVE_core0( pxQueue_core0 );
                pxQueue_core0->uxMessagesWaiting_core0 = uxMessagesWaiting_core0 - ( UBaseType_t_core0 ) 1;

                /* There is now space in the queue, were any tasks waiting to
                 * post to the queue?  If so, unblock the highest priority waiting
                 * task_core0. */
                if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToSend_core0 ) ) == pdFALSE_core0 )
                {
                    if( xTaskRemoveFromEventList_core0( &( pxQueue_core0->xTasksWaitingToSend_core0 ) ) != pdFALSE_core0 )
                    {
                        queueYIELD_IF_USING_PREEMPTION_core0();
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }

                taskEXIT_CRITICAL_core0();
                return pdPASS_core0;
            }
            else
            {
                if( xTicksToWait_core0 == ( TickType_t_core0 ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL_core0();
                    traceQUEUE_RECEIVE_FAILED_core0( pxQueue_core0 );
                    return errQUEUE_EMPTY_core0;
                }
                else if( xEntryTimeSet == pdFALSE_core0 )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure. */
                    vTaskInternalSetTimeOutState_core0( &xTimeOut );
                    xEntryTimeSet = pdTRUE_core0;
                }
                else
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER_core0();
                }
            }
        }
        taskEXIT_CRITICAL_core0();

        /* Interrupts and other tasks can send to and receive from the queue
         * now the critical section has been exited. */

        vTaskSuspendAll_core0();
        prvLockQueue_core0( pxQueue_core0 );

        /* Update the timeout state to see if it has expired yet. */
        if( xTaskCheckForTimeOut_core0( &xTimeOut, &xTicksToWait_core0 ) == pdFALSE_core0 )
        {
            /* The timeout has not expired.  If the queue is still empty place
             * the task_core0 on the list of tasks waiting to receive from the queue. */
            if( prvIsQueueEmpty_core0( pxQueue_core0 ) != pdFALSE_core0 )
            {
                traceBLOCKING_ON_QUEUE_RECEIVE_core0( pxQueue_core0 );
                vTaskPlaceOnEventList_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ), xTicksToWait_core0 );
                prvUnlockQueue_core0( pxQueue_core0 );

                if( xTaskResumeAll_core0() == pdFALSE_core0 )
                {
                    portYIELD_WITHIN_API_core0();
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }
            }
            else
            {
                /* The queue contains data again.  Loop back to try and read the
                 * data. */
                prvUnlockQueue_core0( pxQueue_core0 );
                ( void ) xTaskResumeAll_core0();
            }
        }
        else
        {
            /* Timed out.  If there is no data in the queue exit, otherwise loop
             * back and attempt to read the data. */
            prvUnlockQueue_core0( pxQueue_core0 );
            ( void ) xTaskResumeAll_core0();

            if( prvIsQueueEmpty_core0( pxQueue_core0 ) != pdFALSE_core0 )
            {
                traceQUEUE_RECEIVE_FAILED_core0( pxQueue_core0 );
                return errQUEUE_EMPTY_core0;
            }
            else
            {
                mtCOVERAGE_TEST_MARKER_core0();
            }
        }
    } /*lint -restore */

    return 0U;
}
/*-----------------------------------------------------------*/

BaseType_t_core0 xQueueSemaphoreTake_core0( QueueHandle_t_core0 xQueue_core0,
                                TickType_t_core0 xTicksToWait_core0 )
{
    BaseType_t_core0 xEntryTimeSet = pdFALSE_core0;
    TimeOut_t_core0 xTimeOut;
    Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

    #if ( configUSE_MUTEXES_core0 == 1 )
        BaseType_t_core0 xInheritanceOccurred = pdFALSE_core0;
    #endif

    /* Check the queue pointer is not NULL. */
    configASSERT_core0( ( pxQueue_core0 ) );

    /* Check this really is a semaphore, in which case the item size will be
     * 0. */
    configASSERT_core0( pxQueue_core0->uxItemSize_core0 == 0 );

    /* Cannot block if the scheduler is suspended. */
    #if ( ( INCLUDE_xTaskGetSchedulerState_core0 == 1 ) || ( configUSE_TIMERS_core0 == 1 ) )
    {
        configASSERT_core0( !( ( xTaskGetSchedulerState_core0() == taskSCHEDULER_SUSPENDED_core0 ) && ( xTicksToWait_core0 != 0 ) ) );
    }
    #endif

    /*lint -save -e904 This function relaxes the coding standard somewhat to allow return
     * statements within the function itself.  This is done in the interest
     * of execution time efficiency. */
    for( ; ; )
    {
        taskENTER_CRITICAL_core0();
        {
            /* Semaphores are queues with an item size of 0, and where the
             * number of messages in the queue is the semaphore's count value. */
            const UBaseType_t_core0 uxSemaphoreCount = pxQueue_core0->uxMessagesWaiting_core0;

            /* Is there data in the queue now?  To be running the calling task_core0
             * must be the highest priority task_core0 wanting to access the queue. */
            if( uxSemaphoreCount > ( UBaseType_t_core0 ) 0 )
            {
                traceQUEUE_RECEIVE_core0( pxQueue_core0 );

                /* Semaphores are queues with a data size of zero and where the
                 * messages waiting is the semaphore's count.  Reduce the count. */
                pxQueue_core0->uxMessagesWaiting_core0 = uxSemaphoreCount - ( UBaseType_t_core0 ) 1;

                #if ( configUSE_MUTEXES_core0 == 1 )
                {
                    if( pxQueue_core0->uxQueueType_core0 == queueQUEUE_IS_MUTEX_core0 )
                    {
                        /* Record the information required to implement
                         * priority inheritance should it become necessary. */
                        pxQueue_core0->u.xSemaphore_core0.xMutexHolder_core0 = pvTaskIncrementMutexHeldCount_core0();
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                #endif /* configUSE_MUTEXES_core0 */

                /* Check to see if other tasks are blocked waiting to give the
                 * semaphore, and if so, unblock the highest priority such task_core0. */
                if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToSend_core0 ) ) == pdFALSE_core0 )
                {
                    if( xTaskRemoveFromEventList_core0( &( pxQueue_core0->xTasksWaitingToSend_core0 ) ) != pdFALSE_core0 )
                    {
                        queueYIELD_IF_USING_PREEMPTION_core0();
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }

                taskEXIT_CRITICAL_core0();
                return pdPASS_core0;
            }
            else
            {
                if( xTicksToWait_core0 == ( TickType_t_core0 ) 0 )
                {
                    /* The semaphore count was 0 and no block time is specified
                     * (or the block time has expired) so exit now. */
                    taskEXIT_CRITICAL_core0();
                    traceQUEUE_RECEIVE_FAILED_core0( pxQueue_core0 );
                    return errQUEUE_EMPTY_core0;
                }
                else if( xEntryTimeSet == pdFALSE_core0 )
                {
                    /* The semaphore count was 0 and a block time was specified
                     * so configure the timeout structure ready to block. */
                    vTaskInternalSetTimeOutState_core0( &xTimeOut );
                    xEntryTimeSet = pdTRUE_core0;
                }
                else
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER_core0();
                }
            }
        }
        taskEXIT_CRITICAL_core0();

        /* Interrupts and other tasks can give to and take from the semaphore
         * now the critical section has been exited. */

        vTaskSuspendAll_core0();
        prvLockQueue_core0( pxQueue_core0 );

        /* Update the timeout state to see if it has expired yet. */
        if( xTaskCheckForTimeOut_core0( &xTimeOut, &xTicksToWait_core0 ) == pdFALSE_core0 )
        {
            /* A block time is specified and not expired.  If the semaphore
             * count is 0 then enter the Blocked state to wait for a semaphore to
             * become available.  As semaphores are implemented with queues the
             * queue being empty is equivalent to the semaphore count being 0. */
            if( prvIsQueueEmpty_core0( pxQueue_core0 ) != pdFALSE_core0 )
            {
                traceBLOCKING_ON_QUEUE_RECEIVE_core0( pxQueue_core0 );

                #if ( configUSE_MUTEXES_core0 == 1 )
                {
                    if( pxQueue_core0->uxQueueType_core0 == queueQUEUE_IS_MUTEX_core0 )
                    {
                        taskENTER_CRITICAL_core0();
                        {
                            xInheritanceOccurred = xTaskPriorityInherit_core0( pxQueue_core0->u.xSemaphore_core0.xMutexHolder_core0 );
                        }
                        taskEXIT_CRITICAL_core0();
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                #endif /* if ( configUSE_MUTEXES_core0 == 1 ) */

                vTaskPlaceOnEventList_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ), xTicksToWait_core0 );
                prvUnlockQueue_core0( pxQueue_core0 );

                if( xTaskResumeAll_core0() == pdFALSE_core0 )
                {
                    portYIELD_WITHIN_API_core0();
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }
            }
            else
            {
                /* There was no timeout and the semaphore count was not 0, so
                 * attempt to take the semaphore again. */
                prvUnlockQueue_core0( pxQueue_core0 );
                ( void ) xTaskResumeAll_core0();
            }
        }
        else
        {
            /* Timed out. */
            prvUnlockQueue_core0( pxQueue_core0 );
            ( void ) xTaskResumeAll_core0();

            /* If the semaphore count is 0 exit now as the timeout has
             * expired.  Otherwise return to attempt to take the semaphore that is
             * known to be available.  As semaphores are implemented by queues the
             * queue being empty is equivalent to the semaphore count being 0. */
            if( prvIsQueueEmpty_core0( pxQueue_core0 ) != pdFALSE_core0 )
            {
                #if ( configUSE_MUTEXES_core0 == 1 )
                {
                    /* xInheritanceOccurred could only have be set if
                     * pxQueue_core0->uxQueueType_core0 == queueQUEUE_IS_MUTEX_core0 so no need to
                     * test the mutex type again to check it is actually a mutex. */
                    if( xInheritanceOccurred != pdFALSE_core0 )
                    {
                        taskENTER_CRITICAL_core0();
                        {
                            UBaseType_t_core0 uxHighestWaitingPriority;

                            /* This task_core0 blocking on the mutex caused another
                             * task_core0 to inherit this task_core0's priority.  Now this task_core0
                             * has timed out the priority should be disinherited
                             * again, but only as low as the next highest priority
                             * task_core0 that is waiting for the same mutex. */
                            uxHighestWaitingPriority = prvGetDisinheritPriorityAfterTimeout_core0( pxQueue_core0 );
                            vTaskPriorityDisinheritAfterTimeout_core0( pxQueue_core0->u.xSemaphore_core0.xMutexHolder_core0, uxHighestWaitingPriority );
                        }
                        taskEXIT_CRITICAL_core0();
                    }
                }
                #endif /* configUSE_MUTEXES_core0 */

                traceQUEUE_RECEIVE_FAILED_core0( pxQueue_core0 );
                return errQUEUE_EMPTY_core0;
            }
            else
            {
                mtCOVERAGE_TEST_MARKER_core0();
            }
        }
    } /*lint -restore */

    return 0U;
}
/*-----------------------------------------------------------*/

BaseType_t_core0 xQueuePeek_core0( QueueHandle_t_core0 xQueue_core0,
                       void * const pvBuffer_core0,
                       TickType_t_core0 xTicksToWait_core0 )
{
    BaseType_t_core0 xEntryTimeSet = pdFALSE_core0;
    TimeOut_t_core0 xTimeOut;
    int8_t * pcOriginalReadPosition;
    Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

    /* Check the pointer is not NULL. */
    configASSERT_core0( ( pxQueue_core0 ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer. */
    configASSERT_core0( !( ( ( pvBuffer_core0 ) == NULL ) && ( ( pxQueue_core0 )->uxItemSize_core0 != ( UBaseType_t_core0 ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #if ( ( INCLUDE_xTaskGetSchedulerState_core0 == 1 ) || ( configUSE_TIMERS_core0 == 1 ) )
    {
        configASSERT_core0( !( ( xTaskGetSchedulerState_core0() == taskSCHEDULER_SUSPENDED_core0 ) && ( xTicksToWait_core0 != 0 ) ) );
    }
    #endif

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    for( ; ; )
    {
        taskENTER_CRITICAL_core0();
        {
            const UBaseType_t_core0 uxMessagesWaiting_core0 = pxQueue_core0->uxMessagesWaiting_core0;

            /* Is there data in the queue now?  To be running the calling task_core0
             * must be the highest priority task_core0 wanting to access the queue. */
            if( uxMessagesWaiting_core0 > ( UBaseType_t_core0 ) 0 )
            {
                /* Remember the read position so it can be reset after the data
                 * is read from the queue as this function is only peeking the
                 * data, not removing it. */
                pcOriginalReadPosition = pxQueue_core0->u.xQueue_core0.pcReadFrom_core0;

                prvCopyDataFromQueue_core0( pxQueue_core0, pvBuffer_core0 );
                traceQUEUE_PEEK_core0( pxQueue_core0 );

                /* The data is not being removed, so reset the read pointer. */
                pxQueue_core0->u.xQueue_core0.pcReadFrom_core0 = pcOriginalReadPosition;

                /* The data is being left in the queue, so see if there are
                 * any other tasks waiting for the data. */
                if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) == pdFALSE_core0 )
                {
                    if( xTaskRemoveFromEventList_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) != pdFALSE_core0 )
                    {
                        /* The task_core0 waiting has a higher priority than this task_core0. */
                        queueYIELD_IF_USING_PREEMPTION_core0();
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }

                taskEXIT_CRITICAL_core0();
                return pdPASS_core0;
            }
            else
            {
                if( xTicksToWait_core0 == ( TickType_t_core0 ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL_core0();
                    traceQUEUE_PEEK_FAILED_core0( pxQueue_core0 );
                    return errQUEUE_EMPTY_core0;
                }
                else if( xEntryTimeSet == pdFALSE_core0 )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure ready to enter the blocked
                     * state. */
                    vTaskInternalSetTimeOutState_core0( &xTimeOut );
                    xEntryTimeSet = pdTRUE_core0;
                }
                else
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER_core0();
                }
            }
        }
        taskEXIT_CRITICAL_core0();

        /* Interrupts and other tasks can send to and receive from the queue
         * now that the critical section has been exited. */

        vTaskSuspendAll_core0();
        prvLockQueue_core0( pxQueue_core0 );

        /* Update the timeout state to see if it has expired yet. */
        if( xTaskCheckForTimeOut_core0( &xTimeOut, &xTicksToWait_core0 ) == pdFALSE_core0 )
        {
            /* Timeout has not expired yet, check to see if there is data in the
            * queue now, and if not enter the Blocked state to wait for data. */
            if( prvIsQueueEmpty_core0( pxQueue_core0 ) != pdFALSE_core0 )
            {
                traceBLOCKING_ON_QUEUE_PEEK_core0( pxQueue_core0 );
                vTaskPlaceOnEventList_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ), xTicksToWait_core0 );
                prvUnlockQueue_core0( pxQueue_core0 );

                if( xTaskResumeAll_core0() == pdFALSE_core0 )
                {
                    portYIELD_WITHIN_API_core0();
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }
            }
            else
            {
                /* There is data in the queue now, so don't enter the blocked
                 * state, instead return to try and obtain the data. */
                prvUnlockQueue_core0( pxQueue_core0 );
                ( void ) xTaskResumeAll_core0();
            }
        }
        else
        {
            /* The timeout has expired.  If there is still no data in the queue
             * exit, otherwise go back and try to read the data again. */
            prvUnlockQueue_core0( pxQueue_core0 );
            ( void ) xTaskResumeAll_core0();

            if( prvIsQueueEmpty_core0( pxQueue_core0 ) != pdFALSE_core0 )
            {
                traceQUEUE_PEEK_FAILED_core0( pxQueue_core0 );
                return errQUEUE_EMPTY_core0;
            }
            else
            {
                mtCOVERAGE_TEST_MARKER_core0();
            }
        }
    } /*lint -restore */

    return 0U;
}
/*-----------------------------------------------------------*/

BaseType_t_core0 xQueueReceiveFromISR_core0( QueueHandle_t_core0 xQueue_core0,
                                 void * const pvBuffer_core0,
                                 BaseType_t_core0 * const pxHigherPriorityTaskWoken_core0 )
{
    BaseType_t_core0 xReturn_core0;
    UBaseType_t_core0 uxSavedInterruptStatus_core0;
    Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

    configASSERT_core0( pxQueue_core0 );
    configASSERT_core0( !( ( pvBuffer_core0 == NULL ) && ( pxQueue_core0->uxItemSize_core0 != ( UBaseType_t_core0 ) 0U ) ) );

    /* RTOS ports that support interrupt nesting have the concept of a maximum
     * system call (or maximum API call) interrupt priority.  Interrupts that are
     * above the maximum system call priority are kept permanently enabled, even
     * when the RTOS kernel is in a critical section, but cannot make any calls to
     * FreeRTOS_core0 API functions.  If configASSERT_core0() is defined in FreeRTOSConfig.h
     * then portASSERT_IF_INTERRUPT_PRIORITY_INVALID_core0() will result in an assertion
     * failure if a FreeRTOS_core0 API function is called from an interrupt that has been
     * assigned a priority above the configured maximum system call priority.
     * Only FreeRTOS_core0 functions that end in FromISR can be called from interrupts
     * that have been assigned a priority at or (logically) below the maximum
     * system call interrupt priority.  FreeRTOS_core0 maintains a separate interrupt
     * safe API to ensure interrupt entry is as fast and as simple as possible.
     * More information (albeit Cortex-M specific) is provided on the following
     * link: https://www.FreeRTOS_core0.org/RTOS-Cortex-M3-M4.html */
    portASSERT_IF_INTERRUPT_PRIORITY_INVALID_core0();

    uxSavedInterruptStatus_core0 = portSET_INTERRUPT_MASK_FROM_ISR_core0();
    {
        const UBaseType_t_core0 uxMessagesWaiting_core0 = pxQueue_core0->uxMessagesWaiting_core0;

        /* Cannot block in an ISR, so check there is data available. */
        if( uxMessagesWaiting_core0 > ( UBaseType_t_core0 ) 0 )
        {
            const int8_t cRxLock_core0 = pxQueue_core0->cRxLock_core0;

            traceQUEUE_RECEIVE_FROM_ISR_core0( pxQueue_core0 );

            prvCopyDataFromQueue_core0( pxQueue_core0, pvBuffer_core0 );
            pxQueue_core0->uxMessagesWaiting_core0 = uxMessagesWaiting_core0 - ( UBaseType_t_core0 ) 1;

            /* If the queue is locked the event list will not be modified.
             * Instead update the lock count so the task_core0 that unlocks the queue
             * will know that an ISR has removed data while the queue was
             * locked. */
            if( cRxLock_core0 == queueUNLOCKED_core0 )
            {
                if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToSend_core0 ) ) == pdFALSE_core0 )
                {
                    if( xTaskRemoveFromEventList_core0( &( pxQueue_core0->xTasksWaitingToSend_core0 ) ) != pdFALSE_core0 )
                    {
                        /* The task_core0 waiting has a higher priority than us so
                         * force a context switch. */
                        if( pxHigherPriorityTaskWoken_core0 != NULL )
                        {
                            *pxHigherPriorityTaskWoken_core0 = pdTRUE_core0;
                        }
                        else
                        {
                            mtCOVERAGE_TEST_MARKER_core0();
                        }
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }
            }
            else
            {
                /* Increment the lock count so the task_core0 that unlocks the queue
                 * knows that data was removed while it was locked. */
                prvIncrementQueueRxLock_core0( pxQueue_core0, cRxLock_core0 );
            }

            xReturn_core0 = pdPASS_core0;
        }
        else
        {
            xReturn_core0 = pdFAIL_core0;
            traceQUEUE_RECEIVE_FROM_ISR_FAILED_core0( pxQueue_core0 );
        }
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR_core0( uxSavedInterruptStatus_core0 );

    return xReturn_core0;
}
/*-----------------------------------------------------------*/

BaseType_t_core0 xQueuePeekFromISR_core0( QueueHandle_t_core0 xQueue_core0,
                              void * const pvBuffer_core0 )
{
    BaseType_t_core0 xReturn_core0;
    UBaseType_t_core0 uxSavedInterruptStatus_core0;
    int8_t * pcOriginalReadPosition;
    Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

    configASSERT_core0( pxQueue_core0 );
    configASSERT_core0( !( ( pvBuffer_core0 == NULL ) && ( pxQueue_core0->uxItemSize_core0 != ( UBaseType_t_core0 ) 0U ) ) );
    configASSERT_core0( pxQueue_core0->uxItemSize_core0 != 0 ); /* Can't peek a semaphore. */

    /* RTOS ports that support interrupt nesting have the concept of a maximum
     * system call (or maximum API call) interrupt priority.  Interrupts that are
     * above the maximum system call priority are kept permanently enabled, even
     * when the RTOS kernel is in a critical section, but cannot make any calls to
     * FreeRTOS_core0 API functions.  If configASSERT_core0() is defined in FreeRTOSConfig.h
     * then portASSERT_IF_INTERRUPT_PRIORITY_INVALID_core0() will result in an assertion
     * failure if a FreeRTOS_core0 API function is called from an interrupt that has been
     * assigned a priority above the configured maximum system call priority.
     * Only FreeRTOS_core0 functions that end in FromISR can be called from interrupts
     * that have been assigned a priority at or (logically) below the maximum
     * system call interrupt priority.  FreeRTOS_core0 maintains a separate interrupt
     * safe API to ensure interrupt entry is as fast and as simple as possible.
     * More information (albeit Cortex-M specific) is provided on the following
     * link: https://www.FreeRTOS_core0.org/RTOS-Cortex-M3-M4.html */
    portASSERT_IF_INTERRUPT_PRIORITY_INVALID_core0();

    uxSavedInterruptStatus_core0 = portSET_INTERRUPT_MASK_FROM_ISR_core0();
    {
        /* Cannot block in an ISR, so check there is data available. */
        if( pxQueue_core0->uxMessagesWaiting_core0 > ( UBaseType_t_core0 ) 0 )
        {
            traceQUEUE_PEEK_FROM_ISR_core0( pxQueue_core0 );

            /* Remember the read position so it can be reset as nothing is
             * actually being removed from the queue. */
            pcOriginalReadPosition = pxQueue_core0->u.xQueue_core0.pcReadFrom_core0;
            prvCopyDataFromQueue_core0( pxQueue_core0, pvBuffer_core0 );
            pxQueue_core0->u.xQueue_core0.pcReadFrom_core0 = pcOriginalReadPosition;

            xReturn_core0 = pdPASS_core0;
        }
        else
        {
            xReturn_core0 = pdFAIL_core0;
            traceQUEUE_PEEK_FROM_ISR_FAILED_core0( pxQueue_core0 );
        }
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR_core0( uxSavedInterruptStatus_core0 );

    return xReturn_core0;
}
/*-----------------------------------------------------------*/

UBaseType_t_core0 uxQueueMessagesWaiting_core0( const QueueHandle_t_core0 xQueue_core0 )
{
    UBaseType_t_core0 uxReturn_core0;

    configASSERT_core0( xQueue_core0 );

    taskENTER_CRITICAL_core0();
    {
        uxReturn_core0 = ( ( Queue_t_core0 * ) xQueue_core0 )->uxMessagesWaiting_core0;
    }
    taskEXIT_CRITICAL_core0();

    return uxReturn_core0;
} /*lint !e818 Pointer cannot be declared const as xQueue_core0 is a typedef not pointer. */
/*-----------------------------------------------------------*/

UBaseType_t_core0 uxQueueSpacesAvailable_core0( const QueueHandle_t_core0 xQueue_core0 )
{
    UBaseType_t_core0 uxReturn_core0;
    Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

    configASSERT_core0( pxQueue_core0 );

    taskENTER_CRITICAL_core0();
    {
        uxReturn_core0 = pxQueue_core0->uxLength_core0 - pxQueue_core0->uxMessagesWaiting_core0;
    }
    taskEXIT_CRITICAL_core0();

    return uxReturn_core0;
} /*lint !e818 Pointer cannot be declared const as xQueue_core0 is a typedef not pointer. */
/*-----------------------------------------------------------*/

UBaseType_t_core0 uxQueueMessagesWaitingFromISR_core0( const QueueHandle_t_core0 xQueue_core0 )
{
    UBaseType_t_core0 uxReturn_core0;
    Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

    configASSERT_core0( pxQueue_core0 );
    uxReturn_core0 = pxQueue_core0->uxMessagesWaiting_core0;

    return uxReturn_core0;
} /*lint !e818 Pointer cannot be declared const as xQueue_core0 is a typedef not pointer. */
/*-----------------------------------------------------------*/

void vQueueDelete_core0( QueueHandle_t_core0 xQueue_core0 )
{
    Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

    configASSERT_core0( pxQueue_core0 );
    traceQUEUE_DELETE_core0( pxQueue_core0 );

    #if ( configQUEUE_REGISTRY_SIZE_core0 > 0 )
    {
        vQueueUnregisterQueue_core0( pxQueue_core0 );
    }
    #endif

    #if ( ( configSUPPORT_DYNAMIC_ALLOCATION_core0 == 1 ) && ( configSUPPORT_STATIC_ALLOCATION_core0 == 0 ) )
    {
        /* The queue can only have been allocated dynamically - free it
         * again. */
        vPortFree_core0( pxQueue_core0 );
    }
    #elif ( ( configSUPPORT_DYNAMIC_ALLOCATION_core0 == 1 ) && ( configSUPPORT_STATIC_ALLOCATION_core0 == 1 ) )
    {
        /* The queue could have been allocated statically or dynamically, so
         * check before attempting to free the memory. */
        if( pxQueue_core0->ucStaticallyAllocated_core0 == ( uint8_t ) pdFALSE_core0 )
        {
            vPortFree_core0( pxQueue_core0 );
        }
        else
        {
            mtCOVERAGE_TEST_MARKER_core0();
        }
    }
    #else /* if ( ( configSUPPORT_DYNAMIC_ALLOCATION_core0 == 1 ) && ( configSUPPORT_STATIC_ALLOCATION_core0 == 0 ) ) */
    {
        /* The queue must have been statically allocated, so is not going to be
         * deleted.  Avoid compiler warnings about the unused parameter. */
        ( void ) pxQueue_core0;
    }
    #endif /* configSUPPORT_DYNAMIC_ALLOCATION_core0 */
}
/*-----------------------------------------------------------*/

#if ( configUSE_TRACE_FACILITY_core0 == 1 )

    UBaseType_t_core0 uxQueueGetQueueNumber_core0( QueueHandle_t_core0 xQueue_core0 )
    {
        return ( ( Queue_t_core0 * ) xQueue_core0 )->uxQueueNumber_core0;
    }

#endif /* configUSE_TRACE_FACILITY_core0 */
/*-----------------------------------------------------------*/

#if ( configUSE_TRACE_FACILITY_core0 == 1 )

    void vQueueSetQueueNumber_core0( QueueHandle_t_core0 xQueue_core0,
                               UBaseType_t_core0 uxQueueNumber_core0 )
    {
        ( ( Queue_t_core0 * ) xQueue_core0 )->uxQueueNumber_core0 = uxQueueNumber_core0;
    }

#endif /* configUSE_TRACE_FACILITY_core0 */
/*-----------------------------------------------------------*/

#if ( configUSE_TRACE_FACILITY_core0 == 1 )

    uint8_t ucQueueGetQueueType_core0( QueueHandle_t_core0 xQueue_core0 )
    {
        return ( ( Queue_t_core0 * ) xQueue_core0 )->ucQueueType_core0;
    }

#endif /* configUSE_TRACE_FACILITY_core0 */
/*-----------------------------------------------------------*/

#if ( configUSE_MUTEXES_core0 == 1 )

    static UBaseType_t_core0 prvGetDisinheritPriorityAfterTimeout_core0( const Queue_t_core0 * const pxQueue_core0 )
    {
        UBaseType_t_core0 uxHighestPriorityOfWaitingTasks;

        /* If a task_core0 waiting for a mutex causes the mutex holder to inherit a
         * priority, but the waiting task_core0 times out, then the holder should
         * disinherit the priority - but only down to the highest priority of any
         * other tasks that are waiting for the same mutex.  For this purpose,
         * return the priority of the highest priority task_core0 that is waiting for the
         * mutex. */
        if( listCURRENT_LIST_LENGTH_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) > 0U )
        {
            uxHighestPriorityOfWaitingTasks = ( UBaseType_t_core0 ) configMAX_PRIORITIES_core0 - ( UBaseType_t_core0 ) listGET_ITEM_VALUE_OF_HEAD_ENTRY_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) );
        }
        else
        {
            uxHighestPriorityOfWaitingTasks = tskIDLE_PRIORITY_core0;
        }

        return uxHighestPriorityOfWaitingTasks;
    }

#endif /* configUSE_MUTEXES_core0 */
/*-----------------------------------------------------------*/

static BaseType_t_core0 prvCopyDataToQueue_core0( Queue_t_core0 * const pxQueue_core0,
                                      const void * pvItemToQueue_core0,
                                      const BaseType_t_core0 xPosition )
{
    BaseType_t_core0 xReturn_core0 = pdFALSE_core0;
    UBaseType_t_core0 uxMessagesWaiting_core0;

    /* This function is called from a critical section. */

    uxMessagesWaiting_core0 = pxQueue_core0->uxMessagesWaiting_core0;

    if( pxQueue_core0->uxItemSize_core0 == ( UBaseType_t_core0 ) 0 )
    {
        #if ( configUSE_MUTEXES_core0 == 1 )
        {
            if( pxQueue_core0->uxQueueType_core0 == queueQUEUE_IS_MUTEX_core0 )
            {
                /* The mutex is no longer being held. */
                xReturn_core0 = xTaskPriorityDisinherit_core0( pxQueue_core0->u.xSemaphore_core0.xMutexHolder_core0 );
                pxQueue_core0->u.xSemaphore_core0.xMutexHolder_core0 = NULL;
            }
            else
            {
                mtCOVERAGE_TEST_MARKER_core0();
            }
        }
        #endif /* configUSE_MUTEXES_core0 */
    }
    else if( xPosition == queueSEND_TO_BACK_core0 )
    {
        ( void ) memcpy( ( void * ) pxQueue_core0->pcWriteTo_core0, pvItemToQueue_core0, ( size_t ) pxQueue_core0->uxItemSize_core0 ); /*lint !e961 !e418 !e9087 MISRA exception as the casts are only redundant for some ports, plus previous logic ensures a null pointer can only be passed to memcpy() if the copy size is 0.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes. */
        pxQueue_core0->pcWriteTo_core0 += pxQueue_core0->uxItemSize_core0;                                                       /*lint !e9016 Pointer arithmetic on char types ok, especially in this use case where it is the clearest way of conveying intent. */

        if( pxQueue_core0->pcWriteTo_core0 >= pxQueue_core0->u.xQueue_core0.pcTail_core0 )                                             /*lint !e946 MISRA exception justified as comparison of pointers is the cleanest solution. */
        {
            pxQueue_core0->pcWriteTo_core0 = pxQueue_core0->pcHead_core0;
        }
        else
        {
            mtCOVERAGE_TEST_MARKER_core0();
        }
    }
    else
    {
        ( void ) memcpy( ( void * ) pxQueue_core0->u.xQueue_core0.pcReadFrom_core0, pvItemToQueue_core0, ( size_t ) pxQueue_core0->uxItemSize_core0 ); /*lint !e961 !e9087 !e418 MISRA exception as the casts are only redundant for some ports.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes.  Assert checks null pointer only used when length is 0. */
        pxQueue_core0->u.xQueue_core0.pcReadFrom_core0 -= pxQueue_core0->uxItemSize_core0;

        if( pxQueue_core0->u.xQueue_core0.pcReadFrom_core0 < pxQueue_core0->pcHead_core0 ) /*lint !e946 MISRA exception justified as comparison of pointers is the cleanest solution. */
        {
            pxQueue_core0->u.xQueue_core0.pcReadFrom_core0 = ( pxQueue_core0->u.xQueue_core0.pcTail_core0 - pxQueue_core0->uxItemSize_core0 );
        }
        else
        {
            mtCOVERAGE_TEST_MARKER_core0();
        }

        if( xPosition == queueOVERWRITE_core0 )
        {
            if( uxMessagesWaiting_core0 > ( UBaseType_t_core0 ) 0 )
            {
                /* An item is not being added but overwritten, so subtract
                 * one from the recorded number of items in the queue so when
                 * one is added again below the number of recorded items remains
                 * correct. */
                --uxMessagesWaiting_core0;
            }
            else
            {
                mtCOVERAGE_TEST_MARKER_core0();
            }
        }
        else
        {
            mtCOVERAGE_TEST_MARKER_core0();
        }
    }

    pxQueue_core0->uxMessagesWaiting_core0 = uxMessagesWaiting_core0 + ( UBaseType_t_core0 ) 1;

    return xReturn_core0;
}
/*-----------------------------------------------------------*/

static void prvCopyDataFromQueue_core0( Queue_t_core0 * const pxQueue_core0,
                                  void * const pvBuffer_core0 )
{
    if( pxQueue_core0->uxItemSize_core0 != ( UBaseType_t_core0 ) 0 )
    {
        pxQueue_core0->u.xQueue_core0.pcReadFrom_core0 += pxQueue_core0->uxItemSize_core0;           /*lint !e9016 Pointer arithmetic on char types ok, especially in this use case where it is the clearest way of conveying intent. */

        if( pxQueue_core0->u.xQueue_core0.pcReadFrom_core0 >= pxQueue_core0->u.xQueue_core0.pcTail_core0 ) /*lint !e946 MISRA exception justified as use of the relational operator is the cleanest solutions. */
        {
            pxQueue_core0->u.xQueue_core0.pcReadFrom_core0 = pxQueue_core0->pcHead_core0;
        }
        else
        {
            mtCOVERAGE_TEST_MARKER_core0();
        }

        ( void ) memcpy( ( void * ) pvBuffer_core0, ( void * ) pxQueue_core0->u.xQueue_core0.pcReadFrom_core0, ( size_t ) pxQueue_core0->uxItemSize_core0 ); /*lint !e961 !e418 !e9087 MISRA exception as the casts are only redundant for some ports.  Also previous logic ensures a null pointer can only be passed to memcpy() when the count is 0.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes. */
    }
}
/*-----------------------------------------------------------*/

static void prvUnlockQueue_core0( Queue_t_core0 * const pxQueue_core0 )
{
    /* THIS FUNCTION MUST BE CALLED WITH THE SCHEDULER SUSPENDED. */

    /* The lock counts contains the number of extra data items placed or
     * removed from the queue while the queue was locked.  When a queue is
     * locked items can be added or removed, but the event lists cannot be
     * updated. */
    taskENTER_CRITICAL_core0();
    {
        int8_t cTxLock_core0 = pxQueue_core0->cTxLock_core0;

        /* See if data was added to the queue while it was locked. */
        while( cTxLock_core0 > queueLOCKED_UNMODIFIED_core0 )
        {
            /* Data was posted while the queue was locked.  Are any tasks
             * blocked waiting for data to become available? */
            #if ( configUSE_QUEUE_SETS_core0 == 1 )
            {
                if( pxQueue_core0->pxQueueSetContainer_core0 != NULL )
                {
                    if( prvNotifyQueueSetContainer_core0( pxQueue_core0 ) != pdFALSE_core0 )
                    {
                        /* The queue is a member of a queue set, and posting to
                         * the queue set caused a higher priority task_core0 to unblock.
                         * A context switch is required. */
                        vTaskMissedYield_core0();
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                else
                {
                    /* Tasks that are removed from the event list will get
                     * added to the pending ready list as the scheduler is still
                     * suspended. */
                    if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) == pdFALSE_core0 )
                    {
                        if( xTaskRemoveFromEventList_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) != pdFALSE_core0 )
                        {
                            /* The task_core0 waiting has a higher priority so record that a
                             * context switch is required. */
                            vTaskMissedYield_core0();
                        }
                        else
                        {
                            mtCOVERAGE_TEST_MARKER_core0();
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            #else /* configUSE_QUEUE_SETS_core0 */
            {
                /* Tasks that are removed from the event list will get added to
                 * the pending ready list as the scheduler is still suspended. */
                if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) == pdFALSE_core0 )
                {
                    if( xTaskRemoveFromEventList_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) != pdFALSE_core0 )
                    {
                        /* The task_core0 waiting has a higher priority so record that
                         * a context switch is required. */
                        vTaskMissedYield_core0();
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                else
                {
                    break;
                }
            }
            #endif /* configUSE_QUEUE_SETS_core0 */

            --cTxLock_core0;
        }

        pxQueue_core0->cTxLock_core0 = queueUNLOCKED_core0;
    }
    taskEXIT_CRITICAL_core0();

    /* Do the same for the Rx lock. */
    taskENTER_CRITICAL_core0();
    {
        int8_t cRxLock_core0 = pxQueue_core0->cRxLock_core0;

        while( cRxLock_core0 > queueLOCKED_UNMODIFIED_core0 )
        {
            if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToSend_core0 ) ) == pdFALSE_core0 )
            {
                if( xTaskRemoveFromEventList_core0( &( pxQueue_core0->xTasksWaitingToSend_core0 ) ) != pdFALSE_core0 )
                {
                    vTaskMissedYield_core0();
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }

                --cRxLock_core0;
            }
            else
            {
                break;
            }
        }

        pxQueue_core0->cRxLock_core0 = queueUNLOCKED_core0;
    }
    taskEXIT_CRITICAL_core0();
}
/*-----------------------------------------------------------*/

static BaseType_t_core0 prvIsQueueEmpty_core0( const Queue_t_core0 * pxQueue_core0 )
{
    BaseType_t_core0 xReturn_core0;

    taskENTER_CRITICAL_core0();
    {
        if( pxQueue_core0->uxMessagesWaiting_core0 == ( UBaseType_t_core0 ) 0 )
        {
            xReturn_core0 = pdTRUE_core0;
        }
        else
        {
            xReturn_core0 = pdFALSE_core0;
        }
    }
    taskEXIT_CRITICAL_core0();

    return xReturn_core0;
}
/*-----------------------------------------------------------*/

BaseType_t_core0 xQueueIsQueueEmptyFromISR_core0( const QueueHandle_t_core0 xQueue_core0 )
{
    BaseType_t_core0 xReturn_core0;
    Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

    configASSERT_core0( pxQueue_core0 );

    if( pxQueue_core0->uxMessagesWaiting_core0 == ( UBaseType_t_core0 ) 0 )
    {
        xReturn_core0 = pdTRUE_core0;
    }
    else
    {
        xReturn_core0 = pdFALSE_core0;
    }

    return xReturn_core0;
} /*lint !e818 xQueue_core0 could not be pointer to const because it is a typedef. */
/*-----------------------------------------------------------*/

static BaseType_t_core0 prvIsQueueFull_core0( const Queue_t_core0 * pxQueue_core0 )
{
    BaseType_t_core0 xReturn_core0;

    taskENTER_CRITICAL_core0();
    {
        if( pxQueue_core0->uxMessagesWaiting_core0 == pxQueue_core0->uxLength_core0 )
        {
            xReturn_core0 = pdTRUE_core0;
        }
        else
        {
            xReturn_core0 = pdFALSE_core0;
        }
    }
    taskEXIT_CRITICAL_core0();

    return xReturn_core0;
}
/*-----------------------------------------------------------*/

BaseType_t_core0 xQueueIsQueueFullFromISR_core0( const QueueHandle_t_core0 xQueue_core0 )
{
    BaseType_t_core0 xReturn_core0;
    Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

    configASSERT_core0( pxQueue_core0 );

    if( pxQueue_core0->uxMessagesWaiting_core0 == pxQueue_core0->uxLength_core0 )
    {
        xReturn_core0 = pdTRUE_core0;
    }
    else
    {
        xReturn_core0 = pdFALSE_core0;
    }

    return xReturn_core0;
} /*lint !e818 xQueue_core0 could not be pointer to const because it is a typedef. */
/*-----------------------------------------------------------*/

#if ( configUSE_CO_ROUTINES_core0 == 1 )

    BaseType_t_core0 xQueueCRSend_core0( QueueHandle_t_core0 xQueue_core0,
                             const void * pvItemToQueue_core0,
                             TickType_t_core0 xTicksToWait_core0 )
    {
        BaseType_t_core0 xReturn_core0;
        Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

        /* If the queue is already full we may have to block.  A critical section
         * is required to prevent an interrupt removing something from the queue
         * between the check to see if the queue is full and blocking on the queue. */
        portDISABLE_INTERRUPTS_core0();
        {
            if( prvIsQueueFull_core0( pxQueue_core0 ) != pdFALSE_core0 )
            {
                /* The queue is full - do we want to block or just leave without
                 * posting? */
                if( xTicksToWait_core0 > ( TickType_t_core0 ) 0 )
                {
                    /* As this is called from a coroutine we cannot block directly, but
                     * return indicating that we need to block. */
                    vCoRoutineAddToDelayedList_core0( xTicksToWait_core0, &( pxQueue_core0->xTasksWaitingToSend_core0 ) );
                    portENABLE_INTERRUPTS_core0();
                    return errQUEUE_BLOCKED_core0;
                }
                else
                {
                    portENABLE_INTERRUPTS_core0();
                    return errQUEUE_FULL_core0;
                }
            }
        }
        portENABLE_INTERRUPTS_core0();

        portDISABLE_INTERRUPTS_core0();
        {
            if( pxQueue_core0->uxMessagesWaiting_core0 < pxQueue_core0->uxLength_core0 )
            {
                /* There is room in the queue, copy the data into the queue. */
                prvCopyDataToQueue_core0( pxQueue_core0, pvItemToQueue_core0, queueSEND_TO_BACK_core0 );
                xReturn_core0 = pdPASS_core0;

                /* Were any co-routines waiting for data to become available? */
                if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) == pdFALSE_core0 )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    if( xCoRoutineRemoveFromEventList( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) != pdFALSE_core0 )
                    {
                        /* The co-routine waiting has a higher priority so record
                         * that a yield might be appropriate. */
                        xReturn_core0 = errQUEUE_YIELD_core0;
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }
            }
            else
            {
                xReturn_core0 = errQUEUE_FULL_core0;
            }
        }
        portENABLE_INTERRUPTS_core0();

        return xReturn_core0;
    }

#endif /* configUSE_CO_ROUTINES_core0 */
/*-----------------------------------------------------------*/

#if ( configUSE_CO_ROUTINES_core0 == 1 )

    BaseType_t_core0 xQueueCRReceive_core0( QueueHandle_t_core0 xQueue_core0,
                                void * pvBuffer_core0,
                                TickType_t_core0 xTicksToWait_core0 )
    {
        BaseType_t_core0 xReturn_core0;
        Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

        /* If the queue is already empty we may have to block.  A critical section
         * is required to prevent an interrupt adding something to the queue
         * between the check to see if the queue is empty and blocking on the queue. */
        portDISABLE_INTERRUPTS_core0();
        {
            if( pxQueue_core0->uxMessagesWaiting_core0 == ( UBaseType_t_core0 ) 0 )
            {
                /* There are no messages in the queue, do we want to block or just
                 * leave with nothing? */
                if( xTicksToWait_core0 > ( TickType_t_core0 ) 0 )
                {
                    /* As this is a co-routine we cannot block directly, but return
                     * indicating that we need to block. */
                    vCoRoutineAddToDelayedList_core0( xTicksToWait_core0, &( pxQueue_core0->xTasksWaitingToReceive_core0 ) );
                    portENABLE_INTERRUPTS_core0();
                    return errQUEUE_BLOCKED_core0;
                }
                else
                {
                    portENABLE_INTERRUPTS_core0();
                    return errQUEUE_FULL_core0;
                }
            }
            else
            {
                mtCOVERAGE_TEST_MARKER_core0();
            }
        }
        portENABLE_INTERRUPTS_core0();

        portDISABLE_INTERRUPTS_core0();
        {
            if( pxQueue_core0->uxMessagesWaiting_core0 > ( UBaseType_t_core0 ) 0 )
            {
                /* Data is available from the queue. */
                pxQueue_core0->u.xQueue_core0.pcReadFrom_core0 += pxQueue_core0->uxItemSize_core0;

                if( pxQueue_core0->u.xQueue_core0.pcReadFrom_core0 >= pxQueue_core0->u.xQueue_core0.pcTail_core0 )
                {
                    pxQueue_core0->u.xQueue_core0.pcReadFrom_core0 = pxQueue_core0->pcHead_core0;
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }

                --( pxQueue_core0->uxMessagesWaiting_core0 );
                ( void ) memcpy( ( void * ) pvBuffer_core0, ( void * ) pxQueue_core0->u.xQueue_core0.pcReadFrom_core0, ( unsigned ) pxQueue_core0->uxItemSize_core0 );

                xReturn_core0 = pdPASS_core0;

                /* Were any co-routines waiting for space to become available? */
                if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToSend_core0 ) ) == pdFALSE_core0 )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    if( xCoRoutineRemoveFromEventList( &( pxQueue_core0->xTasksWaitingToSend_core0 ) ) != pdFALSE_core0 )
                    {
                        xReturn_core0 = errQUEUE_YIELD_core0;
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }
            }
            else
            {
                xReturn_core0 = pdFAIL_core0;
            }
        }
        portENABLE_INTERRUPTS_core0();

        return xReturn_core0;
    }

#endif /* configUSE_CO_ROUTINES_core0 */
/*-----------------------------------------------------------*/

#if ( configUSE_CO_ROUTINES_core0 == 1 )

    BaseType_t_core0 xQueueCRSendFromISR_core0( QueueHandle_t_core0 xQueue_core0,
                                    const void * pvItemToQueue_core0,
                                    BaseType_t_core0 xCoRoutinePreviouslyWoken_core0 )
    {
        Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

        /* Cannot block within an ISR so if there is no space on the queue then
         * exit without doing anything. */
        if( pxQueue_core0->uxMessagesWaiting_core0 < pxQueue_core0->uxLength_core0 )
        {
            prvCopyDataToQueue_core0( pxQueue_core0, pvItemToQueue_core0, queueSEND_TO_BACK_core0 );

            /* We only want to wake one co-routine per ISR, so check that a
             * co-routine has not already been woken. */
            if( xCoRoutinePreviouslyWoken_core0 == pdFALSE_core0 )
            {
                if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) == pdFALSE_core0 )
                {
                    if( xCoRoutineRemoveFromEventList( &( pxQueue_core0->xTasksWaitingToReceive_core0 ) ) != pdFALSE_core0 )
                    {
                        return pdTRUE_core0;
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }
            }
            else
            {
                mtCOVERAGE_TEST_MARKER_core0();
            }
        }
        else
        {
            mtCOVERAGE_TEST_MARKER_core0();
        }

        return xCoRoutinePreviouslyWoken_core0;
    }

#endif /* configUSE_CO_ROUTINES_core0 */
/*-----------------------------------------------------------*/

#if ( configUSE_CO_ROUTINES_core0 == 1 )

    BaseType_t_core0 xQueueCRReceiveFromISR_core0( QueueHandle_t_core0 xQueue_core0,
                                       void * pvBuffer_core0,
                                       BaseType_t_core0 * pxCoRoutineWoken_core0 )
    {
        BaseType_t_core0 xReturn_core0;
        Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

        /* We cannot block from an ISR, so check there is data available. If
         * not then just leave without doing anything. */
        if( pxQueue_core0->uxMessagesWaiting_core0 > ( UBaseType_t_core0 ) 0 )
        {
            /* Copy the data from the queue. */
            pxQueue_core0->u.xQueue_core0.pcReadFrom_core0 += pxQueue_core0->uxItemSize_core0;

            if( pxQueue_core0->u.xQueue_core0.pcReadFrom_core0 >= pxQueue_core0->u.xQueue_core0.pcTail_core0 )
            {
                pxQueue_core0->u.xQueue_core0.pcReadFrom_core0 = pxQueue_core0->pcHead_core0;
            }
            else
            {
                mtCOVERAGE_TEST_MARKER_core0();
            }

            --( pxQueue_core0->uxMessagesWaiting_core0 );
            ( void ) memcpy( ( void * ) pvBuffer_core0, ( void * ) pxQueue_core0->u.xQueue_core0.pcReadFrom_core0, ( unsigned ) pxQueue_core0->uxItemSize_core0 );

            if( ( *pxCoRoutineWoken_core0 ) == pdFALSE_core0 )
            {
                if( listLIST_IS_EMPTY_core0( &( pxQueue_core0->xTasksWaitingToSend_core0 ) ) == pdFALSE_core0 )
                {
                    if( xCoRoutineRemoveFromEventList( &( pxQueue_core0->xTasksWaitingToSend_core0 ) ) != pdFALSE_core0 )
                    {
                        *pxCoRoutineWoken_core0 = pdTRUE_core0;
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }
            }
            else
            {
                mtCOVERAGE_TEST_MARKER_core0();
            }

            xReturn_core0 = pdPASS_core0;
        }
        else
        {
            xReturn_core0 = pdFAIL_core0;
        }

        return xReturn_core0;
    }

#endif /* configUSE_CO_ROUTINES_core0 */
/*-----------------------------------------------------------*/

#if ( configQUEUE_REGISTRY_SIZE_core0 > 0 )

    void vQueueAddToRegistry_core0( QueueHandle_t_core0 xQueue_core0,
                              const char * pcQueueName_core0 ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        UBaseType_t_core0 ux;
        QueueRegistryItem_t_core0 * pxEntryToWrite = NULL;

        configASSERT_core0( xQueue_core0 );

        if( pcQueueName_core0 != NULL )
        {
            /* See if there is an empty space in the registry.  A NULL name denotes
             * a free slot. */
            for( ux = ( UBaseType_t_core0 ) 0U; ux < ( UBaseType_t_core0 ) configQUEUE_REGISTRY_SIZE_core0; ux++ )
            {
                /* Replace an existing entry if the queue is already in the registry. */
                if( xQueue_core0 == xQueueRegistry[ ux ].xHandle_core0 )
                {
                    pxEntryToWrite = &( xQueueRegistry[ ux ] );
                    break;
                }
                /* Otherwise, store in the next empty location */
                else if( ( pxEntryToWrite == NULL ) && ( xQueueRegistry[ ux ].pcQueueName_core0 == NULL ) )
                {
                    pxEntryToWrite = &( xQueueRegistry[ ux ] );
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }
            }
        }

        if( pxEntryToWrite != NULL )
        {
            /* Store the information on this queue. */
            pxEntryToWrite->pcQueueName_core0 = pcQueueName_core0;
            pxEntryToWrite->xHandle_core0 = xQueue_core0;

            traceQUEUE_REGISTRY_ADD_core0( xQueue_core0, pcQueueName_core0 );
        }
    }

#endif /* configQUEUE_REGISTRY_SIZE_core0 */
/*-----------------------------------------------------------*/

#if ( configQUEUE_REGISTRY_SIZE_core0 > 0 )

    const char * pcQueueGetName_core0( QueueHandle_t_core0 xQueue_core0 ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        UBaseType_t_core0 ux;
        const char * pcReturn = NULL; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

        configASSERT_core0( xQueue_core0 );

        /* Note there is nothing here to protect against another task_core0 adding or
         * removing entries from the registry while it is being searched. */

        for( ux = ( UBaseType_t_core0 ) 0U; ux < ( UBaseType_t_core0 ) configQUEUE_REGISTRY_SIZE_core0; ux++ )
        {
            if( xQueueRegistry[ ux ].xHandle_core0 == xQueue_core0 )
            {
                pcReturn = xQueueRegistry[ ux ].pcQueueName_core0;
                break;
            }
            else
            {
                mtCOVERAGE_TEST_MARKER_core0();
            }
        }

        return pcReturn;
    } /*lint !e818 xQueue_core0 cannot be a pointer to const because it is a typedef. */

#endif /* configQUEUE_REGISTRY_SIZE_core0 */
/*-----------------------------------------------------------*/

#if ( configQUEUE_REGISTRY_SIZE_core0 > 0 )

    void vQueueUnregisterQueue_core0( QueueHandle_t_core0 xQueue_core0 )
    {
        UBaseType_t_core0 ux;

        configASSERT_core0( xQueue_core0 );

        /* See if the handle of the queue being unregistered in actually in the
         * registry. */
        for( ux = ( UBaseType_t_core0 ) 0U; ux < ( UBaseType_t_core0 ) configQUEUE_REGISTRY_SIZE_core0; ux++ )
        {
            if( xQueueRegistry[ ux ].xHandle_core0 == xQueue_core0 )
            {
                /* Set the name to NULL to show that this slot if free again. */
                xQueueRegistry[ ux ].pcQueueName_core0 = NULL;

                /* Set the handle to NULL to ensure the same queue handle cannot
                 * appear in the registry twice if it is added, removed, then
                 * added again. */
                xQueueRegistry[ ux ].xHandle_core0 = ( QueueHandle_t_core0 ) 0;
                break;
            }
            else
            {
                mtCOVERAGE_TEST_MARKER_core0();
            }
        }
    } /*lint !e818 xQueue_core0 could not be pointer to const because it is a typedef. */

#endif /* configQUEUE_REGISTRY_SIZE_core0 */
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS_core0 == 1 )

    void vQueueWaitForMessageRestricted_core0( QueueHandle_t_core0 xQueue_core0,
                                         TickType_t_core0 xTicksToWait_core0,
                                         const BaseType_t_core0 xWaitIndefinitely_core0 )
    {
        Queue_t_core0 * const pxQueue_core0 = xQueue_core0;

        /* This function should not be called by application code hence the
         * 'Restricted' in its name.  It is not part of the public API.  It is
         * designed for use by kernel code, and has special calling requirements.
         * It can result in vListInsert_core0() being called on a list that can only
         * possibly ever have one item in it, so the list will be fast, but even
         * so it should be called with the scheduler locked and not from a critical
         * section. */

        /* Only do anything if there are no messages in the queue.  This function
         *  will not actually cause the task_core0 to block, just place it on a blocked
         *  list.  It will not block until the scheduler is unlocked - at which
         *  time a yield will be performed.  If an item is added to the queue while
         *  the queue is locked, and the calling task_core0 blocks on the queue, then the
         *  calling task_core0 will be immediately unblocked when the queue is unlocked. */
        prvLockQueue_core0( pxQueue_core0 );

        if( pxQueue_core0->uxMessagesWaiting_core0 == ( UBaseType_t_core0 ) 0U )
        {
            /* There is nothing in the queue, block for the specified period. */
            vTaskPlaceOnEventListRestricted_core0( &( pxQueue_core0->xTasksWaitingToReceive_core0 ), xTicksToWait_core0, xWaitIndefinitely_core0 );
        }
        else
        {
            mtCOVERAGE_TEST_MARKER_core0();
        }

        prvUnlockQueue_core0( pxQueue_core0 );
    }

#endif /* configUSE_TIMERS_core0 */
/*-----------------------------------------------------------*/

#if ( ( configUSE_QUEUE_SETS_core0 == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION_core0 == 1 ) )

    QueueSetHandle_t_core0 xQueueCreateSet_core0( const UBaseType_t_core0 uxEventQueueLength_core0 )
    {
        QueueSetHandle_t_core0 pxQueue_core0;

        pxQueue_core0 = xQueueGenericCreate_core0( uxEventQueueLength_core0, ( UBaseType_t_core0 ) sizeof( Queue_t_core0 * ), queueQUEUE_TYPE_SET );

        return pxQueue_core0;
    }

#endif /* configUSE_QUEUE_SETS_core0 */
/*-----------------------------------------------------------*/

#if ( configUSE_QUEUE_SETS_core0 == 1 )

    BaseType_t_core0 xQueueAddToSet_core0( QueueSetMemberHandle_t_core0 xQueueOrSemaphore_core0,
                               QueueSetHandle_t_core0 xQueueSet_core0 )
    {
        BaseType_t_core0 xReturn_core0;

        taskENTER_CRITICAL_core0();
        {
            if( ( ( Queue_t_core0 * ) xQueueOrSemaphore_core0 )->pxQueueSetContainer_core0 != NULL )
            {
                /* Cannot add a queue/semaphore to more than one queue set. */
                xReturn_core0 = pdFAIL_core0;
            }
            else if( ( ( Queue_t_core0 * ) xQueueOrSemaphore_core0 )->uxMessagesWaiting_core0 != ( UBaseType_t_core0 ) 0 )
            {
                /* Cannot add a queue/semaphore to a queue set if there are already
                 * items in the queue/semaphore. */
                xReturn_core0 = pdFAIL_core0;
            }
            else
            {
                ( ( Queue_t_core0 * ) xQueueOrSemaphore_core0 )->pxQueueSetContainer_core0 = xQueueSet_core0;
                xReturn_core0 = pdPASS_core0;
            }
        }
        taskEXIT_CRITICAL_core0();

        return xReturn_core0;
    }

#endif /* configUSE_QUEUE_SETS_core0 */
/*-----------------------------------------------------------*/

#if ( configUSE_QUEUE_SETS_core0 == 1 )

    BaseType_t_core0 xQueueRemoveFromSet_core0( QueueSetMemberHandle_t_core0 xQueueOrSemaphore_core0,
                                    QueueSetHandle_t_core0 xQueueSet_core0 )
    {
        BaseType_t_core0 xReturn_core0;
        Queue_t_core0 * const pxQueueOrSemaphore = ( Queue_t_core0 * ) xQueueOrSemaphore_core0;

        if( pxQueueOrSemaphore->pxQueueSetContainer_core0 != xQueueSet_core0 )
        {
            /* The queue was not a member of the set. */
            xReturn_core0 = pdFAIL_core0;
        }
        else if( pxQueueOrSemaphore->uxMessagesWaiting_core0 != ( UBaseType_t_core0 ) 0 )
        {
            /* It is dangerous to remove a queue from a set when the queue is
             * not empty because the queue set will still hold pending events for
             * the queue. */
            xReturn_core0 = pdFAIL_core0;
        }
        else
        {
            taskENTER_CRITICAL_core0();
            {
                /* The queue is no longer contained in the set. */
                pxQueueOrSemaphore->pxQueueSetContainer_core0 = NULL;
            }
            taskEXIT_CRITICAL_core0();
            xReturn_core0 = pdPASS_core0;
        }

        return xReturn_core0;
    } /*lint !e818 xQueueSet_core0 could not be declared as pointing to const as it is a typedef. */

#endif /* configUSE_QUEUE_SETS_core0 */
/*-----------------------------------------------------------*/

#if ( configUSE_QUEUE_SETS_core0 == 1 )

    QueueSetMemberHandle_t_core0 xQueueSelectFromSet_core0( QueueSetHandle_t_core0 xQueueSet_core0,
                                                TickType_t_core0 const xTicksToWait_core0 )
    {
        QueueSetMemberHandle_t_core0 xReturn_core0 = NULL;

        ( void ) xQueueReceive_core0( ( QueueHandle_t_core0 ) xQueueSet_core0, &xReturn_core0, xTicksToWait_core0 ); /*lint !e961 Casting from one typedef to another is not redundant. */
        return xReturn_core0;
    }

#endif /* configUSE_QUEUE_SETS_core0 */
/*-----------------------------------------------------------*/

#if ( configUSE_QUEUE_SETS_core0 == 1 )

    QueueSetMemberHandle_t_core0 xQueueSelectFromSetFromISR_core0( QueueSetHandle_t_core0 xQueueSet_core0 )
    {
        QueueSetMemberHandle_t_core0 xReturn_core0 = NULL;

        ( void ) xQueueReceiveFromISR_core0( ( QueueHandle_t_core0 ) xQueueSet_core0, &xReturn_core0, NULL ); /*lint !e961 Casting from one typedef to another is not redundant. */
        return xReturn_core0;
    }

#endif /* configUSE_QUEUE_SETS_core0 */
/*-----------------------------------------------------------*/

#if ( configUSE_QUEUE_SETS_core0 == 1 )

    static BaseType_t_core0 prvNotifyQueueSetContainer_core0( const Queue_t_core0 * const pxQueue_core0 )
    {
        Queue_t_core0 * pxQueueSetContainer_core0 = pxQueue_core0->pxQueueSetContainer_core0;
        BaseType_t_core0 xReturn_core0 = pdFALSE_core0;

        /* This function must be called form a critical section. */

        /* The following line is not reachable in unit tests because every call
         * to prvNotifyQueueSetContainer_core0 is preceded by a check that
         * pxQueueSetContainer_core0 != NULL */
        configASSERT_core0( pxQueueSetContainer_core0 ); /* LCOV_EXCL_BR_LINE */
        configASSERT_core0( pxQueueSetContainer_core0->uxMessagesWaiting_core0 < pxQueueSetContainer_core0->uxLength_core0 );

        if( pxQueueSetContainer_core0->uxMessagesWaiting_core0 < pxQueueSetContainer_core0->uxLength_core0 )
        {
            const int8_t cTxLock_core0 = pxQueueSetContainer_core0->cTxLock_core0;

            traceQUEUE_SET_SEND_core0( pxQueueSetContainer_core0 );

            /* The data copied is the handle of the queue that contains data. */
            xReturn_core0 = prvCopyDataToQueue_core0( pxQueueSetContainer_core0, &pxQueue_core0, queueSEND_TO_BACK_core0 );

            if( cTxLock_core0 == queueUNLOCKED_core0 )
            {
                if( listLIST_IS_EMPTY_core0( &( pxQueueSetContainer_core0->xTasksWaitingToReceive_core0 ) ) == pdFALSE_core0 )
                {
                    if( xTaskRemoveFromEventList_core0( &( pxQueueSetContainer_core0->xTasksWaitingToReceive_core0 ) ) != pdFALSE_core0 )
                    {
                        /* The task_core0 waiting has a higher priority. */
                        xReturn_core0 = pdTRUE_core0;
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER_core0();
                    }
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER_core0();
                }
            }
            else
            {
                prvIncrementQueueTxLock_core0( pxQueueSetContainer_core0, cTxLock_core0 );
            }
        }
        else
        {
            mtCOVERAGE_TEST_MARKER_core0();
        }

        return xReturn_core0;
    }

#endif /* configUSE_QUEUE_SETS_core0 */
