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

#ifndef EVENT_GROUPS_CORE2_H
#define EVENT_GROUPS_CORE2_H

#ifndef INC_FREERTOS_CORE2_H
    #error "include FreeRTOS_core2.h" must appear in source files before "include event_groups.h"
#endif

/* FreeRTOS_core2 includes. */
#include "timers_core2.h"

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

/**
 * An event group is a collection of bits to which an application can assign a
 * meaning.  For example, an application may create an event group to convey
 * the status of various CAN bus related events in which bit 0 might mean "A CAN
 * message has been received and is ready for processing", bit 1 might mean "The
 * application has queued a message that is ready for sending onto the CAN
 * network", and bit 2 might mean "It is time to send a SYNC message onto the
 * CAN network" etc.  A task_core2 can then test the bit values to see which events
 * are active, and optionally enter the Blocked state to wait for a specified
 * bit or a group of specified bits to be active.  To continue the CAN bus
 * example, a CAN controlling task_core2 can enter the Blocked state (and therefore
 * not consume any processing time) until either bit 0, bit 1 or bit 2 are
 * active, at which time the bit that was actually active would inform the task_core2
 * which action it had to take (process a received message, send a message, or
 * send a SYNC).
 *
 * The event groups implementation contains intelligence to avoid race
 * conditions that would otherwise occur were an application to use a simple
 * variable for the same purpose.  This is particularly important with respect
 * to when a bit within an event group is to be cleared, and when bits have to
 * be set and then tested atomically - as is the case where event groups are
 * used to create a synchronisation point between multiple tasks (a
 * 'rendezvous').
 */



/**
 * event_groups.h
 *
 * Type by which event groups are referenced.  For example, a call to
 * xEventGroupCreate_core2() returns an EventGroupHandle_t_core2 variable that can then
 * be used as a parameter to other event group functions.
 *
 * \defgroup EventGroupHandle_t_core2 EventGroupHandle_t_core2
 * \ingroup EventGroup
 */
struct EventGroupDef_t_core2;
typedef struct EventGroupDef_t_core2   * EventGroupHandle_t_core2;

/*
 * The type that holds event bits always matches TickType_t_core2 - therefore the
 * number of bits it holds is set by configUSE_16_BIT_TICKS_core2 (16 bits if set to 1,
 * 32 bits if set to 0.
 *
 * \defgroup EventBits_t_core2 EventBits_t_core2
 * \ingroup EventGroup
 */
typedef TickType_t_core2               EventBits_t_core2;

/**
 * event_groups.h
 * @code{c}
 * EventGroupHandle_t_core2 xEventGroupCreate_core2( void );
 * @endcode
 *
 * Create a new event group.
 *
 * Internally, within the FreeRTOS_core2 implementation, event groups use a [small]
 * block of memory, in which the event group's structure is stored.  If an event
 * groups is created using xEventGroupCreate_core2() then the required memory is
 * automatically dynamically allocated inside the xEventGroupCreate_core2() function.
 * (see https://www.FreeRTOS_core2.org/a00111.html).  If an event group is created
 * using xEventGroupCreateStatic_core2() then the application writer must instead
 * provide the memory that will get used by the event group.
 * xEventGroupCreateStatic_core2() therefore allows an event group to be created
 * without using any dynamic memory allocation.
 *
 * Although event groups are not related to ticks, for internal implementation
 * reasons the number of bits available for use in an event group is dependent
 * on the configUSE_16_BIT_TICKS_core2 setting in FreeRTOSConfig.h.  If
 * configUSE_16_BIT_TICKS_core2 is 1 then each event group contains 8 usable bits (bit
 * 0 to bit 7).  If configUSE_16_BIT_TICKS_core2 is set to 0 then each event group has
 * 24 usable bits (bit 0 to bit 23).  The EventBits_t_core2 type is used to store
 * event bits within an event group.
 *
 * @return If the event group was created then a handle to the event group is
 * returned.  If there was insufficient FreeRTOS_core2 heap available to create the
 * event group then NULL is returned.  See https://www.FreeRTOS_core2.org/a00111.html
 *
 * Example usage:
 * @code{c}
 *  // Declare a variable to hold the created event group.
 *  EventGroupHandle_t_core2 xCreatedEventGroup;
 *
 *  // Attempt to create the event group.
 *  xCreatedEventGroup = xEventGroupCreate_core2();
 *
 *  // Was the event group created successfully?
 *  if( xCreatedEventGroup == NULL )
 *  {
 *      // The event group was not created because there was insufficient
 *      // FreeRTOS_core2 heap available.
 *  }
 *  else
 *  {
 *      // The event group was created.
 *  }
 * @endcode
 * \defgroup xEventGroupCreate_core2 xEventGroupCreate_core2
 * \ingroup EventGroup
 */
#if ( configSUPPORT_DYNAMIC_ALLOCATION_core2 == 1 )
    EventGroupHandle_t_core2 xEventGroupCreate_core2( void ) PRIVILEGED_FUNCTION_core2;
#endif

/**
 * event_groups.h
 * @code{c}
 * EventGroupHandle_t_core2 xEventGroupCreateStatic_core2( EventGroupHandle_t_core2 * pxEventGroupBuffer_core2 );
 * @endcode
 *
 * Create a new event group.
 *
 * Internally, within the FreeRTOS_core2 implementation, event groups use a [small]
 * block of memory, in which the event group's structure is stored.  If an event
 * groups is created using xEventGroupCreate_core2() then the required memory is
 * automatically dynamically allocated inside the xEventGroupCreate_core2() function.
 * (see https://www.FreeRTOS_core2.org/a00111.html).  If an event group is created
 * using xEventGroupCreateStatic_core2() then the application writer must instead
 * provide the memory that will get used by the event group.
 * xEventGroupCreateStatic_core2() therefore allows an event group to be created
 * without using any dynamic memory allocation.
 *
 * Although event groups are not related to ticks, for internal implementation
 * reasons the number of bits available for use in an event group is dependent
 * on the configUSE_16_BIT_TICKS_core2 setting in FreeRTOSConfig.h.  If
 * configUSE_16_BIT_TICKS_core2 is 1 then each event group contains 8 usable bits (bit
 * 0 to bit 7).  If configUSE_16_BIT_TICKS_core2 is set to 0 then each event group has
 * 24 usable bits (bit 0 to bit 23).  The EventBits_t_core2 type is used to store
 * event bits within an event group.
 *
 * @param pxEventGroupBuffer_core2 pxEventGroupBuffer_core2 must point to a variable of type
 * StaticEventGroup_t_core2, which will be then be used to hold the event group's data
 * structures, removing the need for the memory to be allocated dynamically.
 *
 * @return If the event group was created then a handle to the event group is
 * returned.  If pxEventGroupBuffer_core2 was NULL then NULL is returned.
 *
 * Example usage:
 * @code{c}
 *  // StaticEventGroup_t_core2 is a publicly accessible structure that has the same
 *  // size and alignment requirements as the real event group structure.  It is
 *  // provided as a mechanism for applications to know the size of the event
 *  // group (which is dependent on the architecture and configuration file
 *  // settings) without breaking the strict data hiding policy by exposing the
 *  // real event group internals.  This StaticEventGroup_t_core2 variable is passed
 *  // into the xSemaphoreCreateEventGroupStatic() function and is used to store
 *  // the event group's data structures
 *  StaticEventGroup_t_core2 xEventGroupBuffer;
 *
 *  // Create the event group without dynamically allocating any memory.
 *  xEventGroup_core2 = xEventGroupCreateStatic_core2( &xEventGroupBuffer );
 * @endcode
 */
#if ( configSUPPORT_STATIC_ALLOCATION_core2 == 1 )
    EventGroupHandle_t_core2 xEventGroupCreateStatic_core2( StaticEventGroup_t_core2 * pxEventGroupBuffer_core2 ) PRIVILEGED_FUNCTION_core2;
#endif

/**
 * event_groups.h
 * @code{c}
 *  EventBits_t_core2 xEventGroupWaitBits_core2(    EventGroupHandle_t_core2 xEventGroup_core2,
 *                                      const EventBits_t_core2 uxBitsToWaitFor_core2,
 *                                      const BaseType_t_core2 xClearOnExit_core2,
 *                                      const BaseType_t_core2 xWaitForAllBits_core2,
 *                                      const TickType_t_core2 xTicksToWait_core2 );
 * @endcode
 *
 * [Potentially] block to wait for one or more bits to be set within a
 * previously created event group.
 *
 * This function cannot be called from an interrupt.
 *
 * @param xEventGroup_core2 The event group in which the bits are being tested.  The
 * event group must have previously been created using a call to
 * xEventGroupCreate_core2().
 *
 * @param uxBitsToWaitFor_core2 A bitwise value that indicates the bit or bits to test
 * inside the event group.  For example, to wait for bit 0 and/or bit 2 set
 * uxBitsToWaitFor_core2 to 0x05.  To wait for bits 0 and/or bit 1 and/or bit 2 set
 * uxBitsToWaitFor_core2 to 0x07.  Etc.
 *
 * @param xClearOnExit_core2 If xClearOnExit_core2 is set to pdTRUE_core2 then any bits within
 * uxBitsToWaitFor_core2 that are set within the event group will be cleared before
 * xEventGroupWaitBits_core2() returns if the wait condition was met (if the function
 * returns for a reason other than a timeout).  If xClearOnExit_core2 is set to
 * pdFALSE_core2 then the bits set in the event group are not altered when the call to
 * xEventGroupWaitBits_core2() returns.
 *
 * @param xWaitForAllBits_core2 If xWaitForAllBits_core2 is set to pdTRUE_core2 then
 * xEventGroupWaitBits_core2() will return when either all the bits in uxBitsToWaitFor_core2
 * are set or the specified block time expires.  If xWaitForAllBits_core2 is set to
 * pdFALSE_core2 then xEventGroupWaitBits_core2() will return when any one of the bits set
 * in uxBitsToWaitFor_core2 is set or the specified block time expires.  The block
 * time is specified by the xTicksToWait_core2 parameter.
 *
 * @param xTicksToWait_core2 The maximum amount of time (specified in 'ticks') to wait
 * for one/all (depending on the xWaitForAllBits_core2 value) of the bits specified by
 * uxBitsToWaitFor_core2 to become set. A value of portMAX_DELAY_core2 can be used to block
 * indefinitely (provided INCLUDE_vTaskSuspend_core2 is set to 1 in FreeRTOSConfig.h).
 *
 * @return The value of the event group at the time either the bits being waited
 * for became set, or the block time expired.  Test the return value to know
 * which bits were set.  If xEventGroupWaitBits_core2() returned because its timeout
 * expired then not all the bits being waited for will be set.  If
 * xEventGroupWaitBits_core2() returned because the bits it was waiting for were set
 * then the returned value is the event group value before any bits were
 * automatically cleared in the case that xClearOnExit_core2 parameter was set to
 * pdTRUE_core2.
 *
 * Example usage:
 * @code{c}
 * #define BIT_0 ( 1 << 0 )
 * #define BIT_4 ( 1 << 4 )
 *
 * void aFunction( EventGroupHandle_t_core2 xEventGroup_core2 )
 * {
 * EventBits_t_core2 uxBits;
 * const TickType_t_core2 xTicksToWait_core2 = 100 / portTICK_PERIOD_MS_core2;
 *
 *      // Wait a maximum of 100ms for either bit 0 or bit 4 to be set within
 *      // the event group.  Clear the bits before exiting.
 *      uxBits = xEventGroupWaitBits_core2(
 *                  xEventGroup_core2,    // The event group being tested.
 *                  BIT_0 | BIT_4,  // The bits within the event group to wait for.
 *                  pdTRUE_core2,         // BIT_0 and BIT_4 should be cleared before returning.
 *                  pdFALSE_core2,        // Don't wait for both bits, either bit will do.
 *                  xTicksToWait_core2 ); // Wait a maximum of 100ms for either bit to be set.
 *
 *      if( ( uxBits & ( BIT_0 | BIT_4 ) ) == ( BIT_0 | BIT_4 ) )
 *      {
 *          // xEventGroupWaitBits_core2() returned because both bits were set.
 *      }
 *      else if( ( uxBits & BIT_0 ) != 0 )
 *      {
 *          // xEventGroupWaitBits_core2() returned because just BIT_0 was set.
 *      }
 *      else if( ( uxBits & BIT_4 ) != 0 )
 *      {
 *          // xEventGroupWaitBits_core2() returned because just BIT_4 was set.
 *      }
 *      else
 *      {
 *          // xEventGroupWaitBits_core2() returned because xTicksToWait_core2 ticks passed
 *          // without either BIT_0 or BIT_4 becoming set.
 *      }
 * }
 * @endcode
 * \defgroup xEventGroupWaitBits_core2 xEventGroupWaitBits_core2
 * \ingroup EventGroup
 */
EventBits_t_core2 xEventGroupWaitBits_core2( EventGroupHandle_t_core2 xEventGroup_core2,
                                 const EventBits_t_core2 uxBitsToWaitFor_core2,
                                 const BaseType_t_core2 xClearOnExit_core2,
                                 const BaseType_t_core2 xWaitForAllBits_core2,
                                 TickType_t_core2 xTicksToWait_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * event_groups.h
 * @code{c}
 *  EventBits_t_core2 xEventGroupClearBits_core2( EventGroupHandle_t_core2 xEventGroup_core2, const EventBits_t_core2 uxBitsToClear_core2 );
 * @endcode
 *
 * Clear bits within an event group.  This function cannot be called from an
 * interrupt.
 *
 * @param xEventGroup_core2 The event group in which the bits are to be cleared.
 *
 * @param uxBitsToClear_core2 A bitwise value that indicates the bit or bits to clear
 * in the event group.  For example, to clear bit 3 only, set uxBitsToClear_core2 to
 * 0x08.  To clear bit 3 and bit 0 set uxBitsToClear_core2 to 0x09.
 *
 * @return The value of the event group before the specified bits were cleared.
 *
 * Example usage:
 * @code{c}
 * #define BIT_0 ( 1 << 0 )
 * #define BIT_4 ( 1 << 4 )
 *
 * void aFunction( EventGroupHandle_t_core2 xEventGroup_core2 )
 * {
 * EventBits_t_core2 uxBits;
 *
 *      // Clear bit 0 and bit 4 in xEventGroup_core2.
 *      uxBits = xEventGroupClearBits_core2(
 *                              xEventGroup_core2,    // The event group being updated.
 *                              BIT_0 | BIT_4 );// The bits being cleared.
 *
 *      if( ( uxBits & ( BIT_0 | BIT_4 ) ) == ( BIT_0 | BIT_4 ) )
 *      {
 *          // Both bit 0 and bit 4 were set before xEventGroupClearBits_core2() was
 *          // called.  Both will now be clear (not set).
 *      }
 *      else if( ( uxBits & BIT_0 ) != 0 )
 *      {
 *          // Bit 0 was set before xEventGroupClearBits_core2() was called.  It will
 *          // now be clear.
 *      }
 *      else if( ( uxBits & BIT_4 ) != 0 )
 *      {
 *          // Bit 4 was set before xEventGroupClearBits_core2() was called.  It will
 *          // now be clear.
 *      }
 *      else
 *      {
 *          // Neither bit 0 nor bit 4 were set in the first place.
 *      }
 * }
 * @endcode
 * \defgroup xEventGroupClearBits_core2 xEventGroupClearBits_core2
 * \ingroup EventGroup
 */
EventBits_t_core2 xEventGroupClearBits_core2( EventGroupHandle_t_core2 xEventGroup_core2,
                                  const EventBits_t_core2 uxBitsToClear_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * event_groups.h
 * @code{c}
 *  BaseType_t_core2 xEventGroupClearBitsFromISR_core2( EventGroupHandle_t_core2 xEventGroup_core2, const EventBits_t_core2 uxBitsToSet_core2 );
 * @endcode
 *
 * A version of xEventGroupClearBits_core2() that can be called from an interrupt.
 *
 * Setting bits in an event group is not a deterministic operation because there
 * are an unknown number of tasks that may be waiting for the bit or bits being
 * set.  FreeRTOS_core2 does not allow nondeterministic operations to be performed
 * while interrupts are disabled, so protects event groups that are accessed
 * from tasks by suspending the scheduler rather than disabling interrupts.  As
 * a result event groups cannot be accessed directly from an interrupt service
 * routine.  Therefore xEventGroupClearBitsFromISR_core2() sends a message to the
 * timer task_core2 to have the clear operation performed in the context of the timer
 * task_core2.
 *
 * @note If this function returns pdPASS_core2 then the timer task_core2 is ready to run
 * and a portYIELD_FROM_ISR_core2(pdTRUE_core2) should be executed to perform the needed
 * clear on the event group.  This behavior is different from
 * xEventGroupSetBitsFromISR_core2 because the parameter xHigherPriorityTaskWoken_core2 is
 * not present.
 *
 * @param xEventGroup_core2 The event group in which the bits are to be cleared.
 *
 * @param uxBitsToClear_core2 A bitwise value that indicates the bit or bits to clear.
 * For example, to clear bit 3 only, set uxBitsToClear_core2 to 0x08.  To clear bit 3
 * and bit 0 set uxBitsToClear_core2 to 0x09.
 *
 * @return If the request to execute the function was posted successfully then
 * pdPASS_core2 is returned, otherwise pdFALSE_core2 is returned.  pdFALSE_core2 will be returned
 * if the timer service queue was full.
 *
 * Example usage:
 * @code{c}
 * #define BIT_0 ( 1 << 0 )
 * #define BIT_4 ( 1 << 4 )
 *
 * // An event group which it is assumed has already been created by a call to
 * // xEventGroupCreate_core2().
 * EventGroupHandle_t_core2 xEventGroup_core2;
 *
 * void anInterruptHandler( void )
 * {
 *      // Clear bit 0 and bit 4 in xEventGroup_core2.
 *      xResult = xEventGroupClearBitsFromISR_core2(
 *                          xEventGroup_core2,     // The event group being updated.
 *                          BIT_0 | BIT_4 ); // The bits being set.
 *
 *      if( xResult == pdPASS_core2 )
 *      {
 *          // The message was posted successfully.
 *          portYIELD_FROM_ISR_core2(pdTRUE_core2);
 *      }
 * }
 * @endcode
 * \defgroup xEventGroupClearBitsFromISR_core2 xEventGroupClearBitsFromISR_core2
 * \ingroup EventGroup
 */
#if ( configUSE_TRACE_FACILITY_core2 == 1 )
    BaseType_t_core2 xEventGroupClearBitsFromISR_core2( EventGroupHandle_t_core2 xEventGroup_core2,
                                            const EventBits_t_core2 uxBitsToClear_core2 ) PRIVILEGED_FUNCTION_core2;
#else
    #define xEventGroupClearBitsFromISR_core2( xEventGroup_core2, uxBitsToClear_core2 ) \
    xTimerPendFunctionCallFromISR_core2( vEventGroupClearBitsCallback_core2, ( void * ) ( xEventGroup_core2 ), ( uint32_t ) ( uxBitsToClear_core2 ), NULL )
#endif

/**
 * event_groups.h
 * @code{c}
 *  EventBits_t_core2 xEventGroupSetBits_core2( EventGroupHandle_t_core2 xEventGroup_core2, const EventBits_t_core2 uxBitsToSet_core2 );
 * @endcode
 *
 * Set bits within an event group.
 * This function cannot be called from an interrupt.  xEventGroupSetBitsFromISR_core2()
 * is a version that can be called from an interrupt.
 *
 * Setting bits in an event group will automatically unblock tasks that are
 * blocked waiting for the bits.
 *
 * @param xEventGroup_core2 The event group in which the bits are to be set.
 *
 * @param uxBitsToSet_core2 A bitwise value that indicates the bit or bits to set.
 * For example, to set bit 3 only, set uxBitsToSet_core2 to 0x08.  To set bit 3
 * and bit 0 set uxBitsToSet_core2 to 0x09.
 *
 * @return The value of the event group at the time the call to
 * xEventGroupSetBits_core2() returns.  There are two reasons why the returned value
 * might have the bits specified by the uxBitsToSet_core2 parameter cleared.  First,
 * if setting a bit results in a task_core2 that was waiting for the bit leaving the
 * blocked state then it is possible the bit will be cleared automatically
 * (see the xClearBitOnExit parameter of xEventGroupWaitBits_core2()).  Second, any
 * unblocked (or otherwise Ready state) task_core2 that has a priority above that of
 * the task_core2 that called xEventGroupSetBits_core2() will execute and may change the
 * event group value before the call to xEventGroupSetBits_core2() returns.
 *
 * Example usage:
 * @code{c}
 * #define BIT_0 ( 1 << 0 )
 * #define BIT_4 ( 1 << 4 )
 *
 * void aFunction( EventGroupHandle_t_core2 xEventGroup_core2 )
 * {
 * EventBits_t_core2 uxBits;
 *
 *      // Set bit 0 and bit 4 in xEventGroup_core2.
 *      uxBits = xEventGroupSetBits_core2(
 *                          xEventGroup_core2,    // The event group being updated.
 *                          BIT_0 | BIT_4 );// The bits being set.
 *
 *      if( ( uxBits & ( BIT_0 | BIT_4 ) ) == ( BIT_0 | BIT_4 ) )
 *      {
 *          // Both bit 0 and bit 4 remained set when the function returned.
 *      }
 *      else if( ( uxBits & BIT_0 ) != 0 )
 *      {
 *          // Bit 0 remained set when the function returned, but bit 4 was
 *          // cleared.  It might be that bit 4 was cleared automatically as a
 *          // task_core2 that was waiting for bit 4 was removed from the Blocked
 *          // state.
 *      }
 *      else if( ( uxBits & BIT_4 ) != 0 )
 *      {
 *          // Bit 4 remained set when the function returned, but bit 0 was
 *          // cleared.  It might be that bit 0 was cleared automatically as a
 *          // task_core2 that was waiting for bit 0 was removed from the Blocked
 *          // state.
 *      }
 *      else
 *      {
 *          // Neither bit 0 nor bit 4 remained set.  It might be that a task_core2
 *          // was waiting for both of the bits to be set, and the bits were
 *          // cleared as the task_core2 left the Blocked state.
 *      }
 * }
 * @endcode
 * \defgroup xEventGroupSetBits_core2 xEventGroupSetBits_core2
 * \ingroup EventGroup
 */
EventBits_t_core2 xEventGroupSetBits_core2( EventGroupHandle_t_core2 xEventGroup_core2,
                                const EventBits_t_core2 uxBitsToSet_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * event_groups.h
 * @code{c}
 *  BaseType_t_core2 xEventGroupSetBitsFromISR_core2( EventGroupHandle_t_core2 xEventGroup_core2, const EventBits_t_core2 uxBitsToSet_core2, BaseType_t_core2 *pxHigherPriorityTaskWoken_core2 );
 * @endcode
 *
 * A version of xEventGroupSetBits_core2() that can be called from an interrupt.
 *
 * Setting bits in an event group is not a deterministic operation because there
 * are an unknown number of tasks that may be waiting for the bit or bits being
 * set.  FreeRTOS_core2 does not allow nondeterministic operations to be performed in
 * interrupts or from critical sections.  Therefore xEventGroupSetBitsFromISR_core2()
 * sends a message to the timer task_core2 to have the set operation performed in the
 * context of the timer task_core2 - where a scheduler lock is used in place of a
 * critical section.
 *
 * @param xEventGroup_core2 The event group in which the bits are to be set.
 *
 * @param uxBitsToSet_core2 A bitwise value that indicates the bit or bits to set.
 * For example, to set bit 3 only, set uxBitsToSet_core2 to 0x08.  To set bit 3
 * and bit 0 set uxBitsToSet_core2 to 0x09.
 *
 * @param pxHigherPriorityTaskWoken_core2 As mentioned above, calling this function
 * will result in a message being sent to the timer daemon task_core2.  If the
 * priority of the timer daemon task_core2 is higher than the priority of the
 * currently running task_core2 (the task_core2 the interrupt interrupted) then
 * *pxHigherPriorityTaskWoken_core2 will be set to pdTRUE_core2 by
 * xEventGroupSetBitsFromISR_core2(), indicating that a context switch should be
 * requested before the interrupt exits.  For that reason
 * *pxHigherPriorityTaskWoken_core2 must be initialised to pdFALSE_core2.  See the
 * example code below.
 *
 * @return If the request to execute the function was posted successfully then
 * pdPASS_core2 is returned, otherwise pdFALSE_core2 is returned.  pdFALSE_core2 will be returned
 * if the timer service queue was full.
 *
 * Example usage:
 * @code{c}
 * #define BIT_0 ( 1 << 0 )
 * #define BIT_4 ( 1 << 4 )
 *
 * // An event group which it is assumed has already been created by a call to
 * // xEventGroupCreate_core2().
 * EventGroupHandle_t_core2 xEventGroup_core2;
 *
 * void anInterruptHandler( void )
 * {
 * BaseType_t_core2 xHigherPriorityTaskWoken_core2, xResult;
 *
 *      // xHigherPriorityTaskWoken_core2 must be initialised to pdFALSE_core2.
 *      xHigherPriorityTaskWoken_core2 = pdFALSE_core2;
 *
 *      // Set bit 0 and bit 4 in xEventGroup_core2.
 *      xResult = xEventGroupSetBitsFromISR_core2(
 *                          xEventGroup_core2,    // The event group being updated.
 *                          BIT_0 | BIT_4   // The bits being set.
 *                          &xHigherPriorityTaskWoken_core2 );
 *
 *      // Was the message posted successfully?
 *      if( xResult == pdPASS_core2 )
 *      {
 *          // If xHigherPriorityTaskWoken_core2 is now set to pdTRUE_core2 then a context
 *          // switch should be requested.  The macro used is port specific and
 *          // will be either portYIELD_FROM_ISR_core2() or portEND_SWITCHING_ISR() -
 *          // refer to the documentation page for the port being used.
 *          portYIELD_FROM_ISR_core2( xHigherPriorityTaskWoken_core2 );
 *      }
 * }
 * @endcode
 * \defgroup xEventGroupSetBitsFromISR_core2 xEventGroupSetBitsFromISR_core2
 * \ingroup EventGroup
 */
#if ( configUSE_TRACE_FACILITY_core2 == 1 )
    BaseType_t_core2 xEventGroupSetBitsFromISR_core2( EventGroupHandle_t_core2 xEventGroup_core2,
                                          const EventBits_t_core2 uxBitsToSet_core2,
                                          BaseType_t_core2 * pxHigherPriorityTaskWoken_core2 ) PRIVILEGED_FUNCTION_core2;
#else
    #define xEventGroupSetBitsFromISR_core2( xEventGroup_core2, uxBitsToSet_core2, pxHigherPriorityTaskWoken_core2 ) \
    xTimerPendFunctionCallFromISR_core2( vEventGroupSetBitsCallback_core2, ( void * ) ( xEventGroup_core2 ), ( uint32_t ) ( uxBitsToSet_core2 ), ( pxHigherPriorityTaskWoken_core2 ) )
#endif

/**
 * event_groups.h
 * @code{c}
 *  EventBits_t_core2 xEventGroupSync_core2(    EventGroupHandle_t_core2 xEventGroup_core2,
 *                                  const EventBits_t_core2 uxBitsToSet_core2,
 *                                  const EventBits_t_core2 uxBitsToWaitFor_core2,
 *                                  TickType_t_core2 xTicksToWait_core2 );
 * @endcode
 *
 * Atomically set bits within an event group, then wait for a combination of
 * bits to be set within the same event group.  This functionality is typically
 * used to synchronise multiple tasks, where each task_core2 has to wait for the other
 * tasks to reach a synchronisation point before proceeding.
 *
 * This function cannot be used from an interrupt.
 *
 * The function will return before its block time expires if the bits specified
 * by the uxBitsToWait parameter are set, or become set within that time.  In
 * this case all the bits specified by uxBitsToWait will be automatically
 * cleared before the function returns.
 *
 * @param xEventGroup_core2 The event group in which the bits are being tested.  The
 * event group must have previously been created using a call to
 * xEventGroupCreate_core2().
 *
 * @param uxBitsToSet_core2 The bits to set in the event group before determining
 * if, and possibly waiting for, all the bits specified by the uxBitsToWait
 * parameter are set.
 *
 * @param uxBitsToWaitFor_core2 A bitwise value that indicates the bit or bits to test
 * inside the event group.  For example, to wait for bit 0 and bit 2 set
 * uxBitsToWaitFor_core2 to 0x05.  To wait for bits 0 and bit 1 and bit 2 set
 * uxBitsToWaitFor_core2 to 0x07.  Etc.
 *
 * @param xTicksToWait_core2 The maximum amount of time (specified in 'ticks') to wait
 * for all of the bits specified by uxBitsToWaitFor_core2 to become set.
 *
 * @return The value of the event group at the time either the bits being waited
 * for became set, or the block time expired.  Test the return value to know
 * which bits were set.  If xEventGroupSync_core2() returned because its timeout
 * expired then not all the bits being waited for will be set.  If
 * xEventGroupSync_core2() returned because all the bits it was waiting for were
 * set then the returned value is the event group value before any bits were
 * automatically cleared.
 *
 * Example usage:
 * @code{c}
 * // Bits used by the three tasks.
 * #define TASK_0_BIT     ( 1 << 0 )
 * #define TASK_1_BIT     ( 1 << 1 )
 * #define TASK_2_BIT     ( 1 << 2 )
 *
 * #define ALL_SYNC_BITS ( TASK_0_BIT | TASK_1_BIT | TASK_2_BIT )
 *
 * // Use an event group to synchronise three tasks.  It is assumed this event
 * // group has already been created elsewhere.
 * EventGroupHandle_t_core2 xEventBits;
 *
 * void vTask0( void *pvParameters_core2 )
 * {
 * EventBits_t_core2 uxReturn_core2;
 * TickType_t_core2 xTicksToWait_core2 = 100 / portTICK_PERIOD_MS_core2;
 *
 *   for( ;; )
 *   {
 *      // Perform task_core2 functionality here.
 *
 *      // Set bit 0 in the event flag to note this task_core2 has reached the
 *      // sync point.  The other two tasks will set the other two bits defined
 *      // by ALL_SYNC_BITS.  All three tasks have reached the synchronisation
 *      // point when all the ALL_SYNC_BITS are set.  Wait a maximum of 100ms
 *      // for this to happen.
 *      uxReturn_core2 = xEventGroupSync_core2( xEventBits, TASK_0_BIT, ALL_SYNC_BITS, xTicksToWait_core2 );
 *
 *      if( ( uxReturn_core2 & ALL_SYNC_BITS ) == ALL_SYNC_BITS )
 *      {
 *          // All three tasks reached the synchronisation point before the call
 *          // to xEventGroupSync_core2() timed out.
 *      }
 *  }
 * }
 *
 * void vTask1( void *pvParameters_core2 )
 * {
 *   for( ;; )
 *   {
 *      // Perform task_core2 functionality here.
 *
 *      // Set bit 1 in the event flag to note this task_core2 has reached the
 *      // synchronisation point.  The other two tasks will set the other two
 *      // bits defined by ALL_SYNC_BITS.  All three tasks have reached the
 *      // synchronisation point when all the ALL_SYNC_BITS are set.  Wait
 *      // indefinitely for this to happen.
 *      xEventGroupSync_core2( xEventBits, TASK_1_BIT, ALL_SYNC_BITS, portMAX_DELAY_core2 );
 *
 *      // xEventGroupSync_core2() was called with an indefinite block time, so
 *      // this task_core2 will only reach here if the synchronisation was made by all
 *      // three tasks, so there is no need to test the return value.
 *   }
 * }
 *
 * void vTask2( void *pvParameters_core2 )
 * {
 *   for( ;; )
 *   {
 *      // Perform task_core2 functionality here.
 *
 *      // Set bit 2 in the event flag to note this task_core2 has reached the
 *      // synchronisation point.  The other two tasks will set the other two
 *      // bits defined by ALL_SYNC_BITS.  All three tasks have reached the
 *      // synchronisation point when all the ALL_SYNC_BITS are set.  Wait
 *      // indefinitely for this to happen.
 *      xEventGroupSync_core2( xEventBits, TASK_2_BIT, ALL_SYNC_BITS, portMAX_DELAY_core2 );
 *
 *      // xEventGroupSync_core2() was called with an indefinite block time, so
 *      // this task_core2 will only reach here if the synchronisation was made by all
 *      // three tasks, so there is no need to test the return value.
 *  }
 * }
 *
 * @endcode
 * \defgroup xEventGroupSync_core2 xEventGroupSync_core2
 * \ingroup EventGroup
 */
EventBits_t_core2 xEventGroupSync_core2( EventGroupHandle_t_core2 xEventGroup_core2,
                             const EventBits_t_core2 uxBitsToSet_core2,
                             const EventBits_t_core2 uxBitsToWaitFor_core2,
                             TickType_t_core2 xTicksToWait_core2 ) PRIVILEGED_FUNCTION_core2;


/**
 * event_groups.h
 * @code{c}
 *  EventBits_t_core2 xEventGroupGetBits_core2( EventGroupHandle_t_core2 xEventGroup_core2 );
 * @endcode
 *
 * Returns the current value of the bits in an event group.  This function
 * cannot be used from an interrupt.
 *
 * @param xEventGroup_core2 The event group being queried.
 *
 * @return The event group bits at the time xEventGroupGetBits_core2() was called.
 *
 * \defgroup xEventGroupGetBits_core2 xEventGroupGetBits_core2
 * \ingroup EventGroup
 */
#define xEventGroupGetBits_core2( xEventGroup_core2 )    xEventGroupClearBits_core2( ( xEventGroup_core2 ), 0 )

/**
 * event_groups.h
 * @code{c}
 *  EventBits_t_core2 xEventGroupGetBitsFromISR_core2( EventGroupHandle_t_core2 xEventGroup_core2 );
 * @endcode
 *
 * A version of xEventGroupGetBits_core2() that can be called from an ISR.
 *
 * @param xEventGroup_core2 The event group being queried.
 *
 * @return The event group bits at the time xEventGroupGetBitsFromISR_core2() was called.
 *
 * \defgroup xEventGroupGetBitsFromISR_core2 xEventGroupGetBitsFromISR_core2
 * \ingroup EventGroup
 */
EventBits_t_core2 xEventGroupGetBitsFromISR_core2( EventGroupHandle_t_core2 xEventGroup_core2 ) PRIVILEGED_FUNCTION_core2;

/**
 * event_groups.h
 * @code{c}
 *  void xEventGroupDelete( EventGroupHandle_t_core2 xEventGroup_core2 );
 * @endcode
 *
 * Delete an event group that was previously created by a call to
 * xEventGroupCreate_core2().  Tasks that are blocked on the event group will be
 * unblocked and obtain 0 as the event group's value.
 *
 * @param xEventGroup_core2 The event group being deleted.
 */
void vEventGroupDelete_core2( EventGroupHandle_t_core2 xEventGroup_core2 ) PRIVILEGED_FUNCTION_core2;

/* For internal use only. */
void vEventGroupSetBitsCallback_core2( void * pvEventGroup_core2,
                                 const uint32_t ulBitsToSet_core2 ) PRIVILEGED_FUNCTION_core2;
void vEventGroupClearBitsCallback_core2( void * pvEventGroup_core2,
                                   const uint32_t ulBitsToClear_core2 ) PRIVILEGED_FUNCTION_core2;


#if ( configUSE_TRACE_FACILITY_core2 == 1 )
    UBaseType_t_core2 uxEventGroupGetNumber_core2( void * xEventGroup_core2 ) PRIVILEGED_FUNCTION_core2;
    void vEventGroupSetNumber_core2( void * xEventGroup_core2,
                               UBaseType_t_core2 uxEventGroupNumber_core2 ) PRIVILEGED_FUNCTION_core2;
#endif

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* EVENT_GROUPS_H */
