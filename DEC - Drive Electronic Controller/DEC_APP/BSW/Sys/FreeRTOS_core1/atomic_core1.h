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

/**
 * @file atomic.h
 * @brief FreeRTOS_core1 atomic operation support.
 *
 * This file implements atomic functions by disabling interrupts globally.
 * Implementations with architecture specific atomic instructions can be
 * provided under each compiler directory.
 */

#ifndef ATOMIC_CORE1_H
#define ATOMICCORE1_H

#ifndef INC_FREERTOSCORE1_H
    #error "include FreeRTOS_core1.h must appear in source files before include atomic.h"
#endif

/* Standard includes. */
#include <stdint.h>

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

/*
 * Port specific definitions -- entering/exiting critical section.
 * Refer template -- ./lib/FreeRTOS_core1/portable/Compiler/Arch/portmacro.h
 *
 * Every call to ATOMIC_EXIT_CRITICAL_core1() must be closely paired with
 * ATOMIC_ENTER_CRITICAL_core1().
 *
 */
#if defined( portSET_INTERRUPT_MASK_FROM_ISR_core1 )

/* Nested interrupt scheme is supported in this port. */
    #define ATOMIC_ENTER_CRITICAL_core1() \
    UBaseType_t_core1 uxCriticalSectionType_core1 = portSET_INTERRUPT_MASK_FROM_ISR_core1()

    #define ATOMIC_EXIT_CRITICAL_core1() \
    portCLEAR_INTERRUPT_MASK_FROM_ISR_core1( uxCriticalSectionType_core1 )

#else

/* Nested interrupt scheme is NOT supported in this port. */
    #define ATOMIC_ENTER_CRITICAL_core1()    portENTER_CRITICAL_core1()
    #define ATOMIC_EXIT_CRITICAL_core1()     portEXIT_CRITICAL_core1()

#endif /* portSET_INTERRUPT_MASK_FROM_ISR_core1() */

/*
 * Port specific definition -- "always inline".
 * Inline is compiler specific, and may not always get inlined depending on your
 * optimization level.  Also, inline is considered as performance optimization
 * for atomic.  Thus, if portFORCE_INLINE_core1 is not provided by portmacro.h,
 * instead of resulting error, simply define it away.
 */
#ifndef portFORCE_INLINE_core1
    #define portFORCE_INLINE_core1
#endif

#define ATOMIC_COMPARE_AND_SWAP_SUCCESS_core1    0x1U     /**< Compare and swap succeeded, swapped. */
#define ATOMIC_COMPARE_AND_SWAP_FAILURE_core1    0x0U     /**< Compare and swap failed, did not swap. */

/*----------------------------- Swap && CAS ------------------------------*/

/**
 * Atomic compare-and-swap
 *
 * @brief Performs an atomic compare-and-swap operation on the specified values.
 *
 * @param[in, out] pulDestination_core1  Pointer to memory location from where value is
 *                               to be loaded and checked.
 * @param[in] ulExchange_core1         If condition meets, write this value to memory.
 * @param[in] ulComparand_core1        Swap condition.
 *
 * @return Unsigned integer of value 1 or 0. 1 for swapped, 0 for not swapped.
 *
 * @note This function only swaps *pulDestination_core1 with ulExchange_core1, if previous
 *       *pulDestination_core1 value equals ulComparand_core1.
 */
static portFORCE_INLINE_core1 uint32_t Atomic_CompareAndSwap_u32_core1( uint32_t volatile * pulDestination_core1,
                                                            uint32_t ulExchange_core1,
                                                            uint32_t ulComparand_core1 )
{
    uint32_t ulReturnValue_core1;

    ATOMIC_ENTER_CRITICAL_core1();
    {
        if( *pulDestination_core1 == ulComparand_core1 )
        {
            *pulDestination_core1 = ulExchange_core1;
            ulReturnValue_core1 = ATOMIC_COMPARE_AND_SWAP_SUCCESS_core1;
        }
        else
        {
            ulReturnValue_core1 = ATOMIC_COMPARE_AND_SWAP_FAILURE_core1;
        }
    }
    ATOMIC_EXIT_CRITICAL_core1();

    return ulReturnValue_core1;
}
/*-----------------------------------------------------------*/

/**
 * Atomic swap (pointers)
 *
 * @brief Atomically sets the address pointed to by *ppvDestination_core1 to the value
 *        of *pvExchange_core1.
 *
 * @param[in, out] ppvDestination_core1  Pointer to memory location from where a pointer
 *                                 value is to be loaded and written back to.
 * @param[in] pvExchange_core1           Pointer value to be written to *ppvDestination_core1.
 *
 * @return The initial value of *ppvDestination_core1.
 */
static portFORCE_INLINE_core1 void * Atomic_SwapPointers_p32_core1( void * volatile * ppvDestination_core1,
                                                        void * pvExchange_core1 )
{
    void * pReturnValue_core1;

    ATOMIC_ENTER_CRITICAL_core1();
    {
        pReturnValue_core1 = *ppvDestination_core1;
        *ppvDestination_core1 = pvExchange_core1;
    }
    ATOMIC_EXIT_CRITICAL_core1();

    return pReturnValue_core1;
}
/*-----------------------------------------------------------*/

/**
 * Atomic compare-and-swap (pointers)
 *
 * @brief Performs an atomic compare-and-swap operation on the specified pointer
 *        values.
 *
 * @param[in, out] ppvDestination_core1  Pointer to memory location from where a pointer
 *                                 value is to be loaded and checked.
 * @param[in] pvExchange_core1           If condition meets, write this value to memory.
 * @param[in] pvComparand_core1          Swap condition.
 *
 * @return Unsigned integer of value 1 or 0. 1 for swapped, 0 for not swapped.
 *
 * @note This function only swaps *ppvDestination_core1 with pvExchange_core1, if previous
 *       *ppvDestination_core1 value equals pvComparand_core1.
 */
static portFORCE_INLINE_core1 uint32_t Atomic_CompareAndSwapPointers_p32_core1( void * volatile * ppvDestination_core1,
                                                                    void * pvExchange_core1,
                                                                    void * pvComparand_core1 )
{
    uint32_t ulReturnValue_core1 = ATOMIC_COMPARE_AND_SWAP_FAILURE_core1;

    ATOMIC_ENTER_CRITICAL_core1();
    {
        if( *ppvDestination_core1 == pvComparand_core1 )
        {
            *ppvDestination_core1 = pvExchange_core1;
            ulReturnValue_core1 = ATOMIC_COMPARE_AND_SWAP_SUCCESS_core1;
        }
    }
    ATOMIC_EXIT_CRITICAL_core1();

    return ulReturnValue_core1;
}


/*----------------------------- Arithmetic ------------------------------*/

/**
 * Atomic add
 *
 * @brief Atomically adds count to the value of the specified pointer points to.
 *
 * @param[in,out] pulAddend_core1  Pointer to memory location from where value is to be
 *                         loaded and written back to.
 * @param[in] ulCount_core1      Value to be added to *pulAddend_core1.
 *
 * @return previous *pulAddend_core1 value.
 */
static portFORCE_INLINE_core1 uint32_t Atomic_Add_u32_core1( uint32_t volatile * pulAddend_core1,
                                                 uint32_t ulCount_core1 )
{
    uint32_t ulCurrent_core1;

    ATOMIC_ENTER_CRITICAL_core1();
    {
        ulCurrent_core1 = *pulAddend_core1;
        *pulAddend_core1 += ulCount_core1;
    }
    ATOMIC_EXIT_CRITICAL_core1();

    return ulCurrent_core1;
}
/*-----------------------------------------------------------*/

/**
 * Atomic subtract
 *
 * @brief Atomically subtracts count from the value of the specified pointer
 *        pointers to.
 *
 * @param[in,out] pulAddend_core1  Pointer to memory location from where value is to be
 *                         loaded and written back to.
 * @param[in] ulCount_core1      Value to be subtract from *pulAddend_core1.
 *
 * @return previous *pulAddend_core1 value.
 */
static portFORCE_INLINE_core1 uint32_t Atomic_Subtract_u32_core1( uint32_t volatile * pulAddend_core1,
                                                      uint32_t ulCount_core1 )
{
    uint32_t ulCurrent_core1;

    ATOMIC_ENTER_CRITICAL_core1();
    {
        ulCurrent_core1 = *pulAddend_core1;
        *pulAddend_core1 -= ulCount_core1;
    }
    ATOMIC_EXIT_CRITICAL_core1();

    return ulCurrent_core1;
}
/*-----------------------------------------------------------*/

/**
 * Atomic increment
 *
 * @brief Atomically increments the value of the specified pointer points to.
 *
 * @param[in,out] pulAddend_core1  Pointer to memory location from where value is to be
 *                         loaded and written back to.
 *
 * @return *pulAddend_core1 value before increment.
 */
static portFORCE_INLINE_core1 uint32_t Atomic_Increment_u32_core1( uint32_t volatile * pulAddend_core1 )
{
    uint32_t ulCurrent_core1;

    ATOMIC_ENTER_CRITICAL_core1();
    {
        ulCurrent_core1 = *pulAddend_core1;
        *pulAddend_core1 += 1;
    }
    ATOMIC_EXIT_CRITICAL_core1();

    return ulCurrent_core1;
}
/*-----------------------------------------------------------*/

/**
 * Atomic decrement
 *
 * @brief Atomically decrements the value of the specified pointer points to
 *
 * @param[in,out] pulAddend_core1  Pointer to memory location from where value is to be
 *                         loaded and written back to.
 *
 * @return *pulAddend_core1 value before decrement.
 */
static portFORCE_INLINE_core1 uint32_t Atomic_Decrement_u32_core1( uint32_t volatile * pulAddend_core1 )
{
    uint32_t ulCurrent_core1;

    ATOMIC_ENTER_CRITICAL_core1();
    {
        ulCurrent_core1 = *pulAddend_core1;
        *pulAddend_core1 -= 1;
    }
    ATOMIC_EXIT_CRITICAL_core1();

    return ulCurrent_core1;
}

/*----------------------------- Bitwise Logical ------------------------------*/

/**
 * Atomic OR
 *
 * @brief Performs an atomic OR operation on the specified values.
 *
 * @param [in, out] pulDestination_core1  Pointer to memory location from where value is
 *                                to be loaded and written back to.
 * @param [in] ulValue_core1            Value to be ORed with *pulDestination_core1.
 *
 * @return The original value of *pulDestination_core1.
 */
static portFORCE_INLINE_core1 uint32_t Atomic_OR_u32_core1( uint32_t volatile * pulDestination_core1,
                                                uint32_t ulValue_core1 )
{
    uint32_t ulCurrent_core1;

    ATOMIC_ENTER_CRITICAL_core1();
    {
        ulCurrent_core1 = *pulDestination_core1;
        *pulDestination_core1 |= ulValue_core1;
    }
    ATOMIC_EXIT_CRITICAL_core1();

    return ulCurrent_core1;
}
/*-----------------------------------------------------------*/

/**
 * Atomic AND
 *
 * @brief Performs an atomic AND operation on the specified values.
 *
 * @param [in, out] pulDestination_core1  Pointer to memory location from where value is
 *                                to be loaded and written back to.
 * @param [in] ulValue_core1            Value to be ANDed with *pulDestination_core1.
 *
 * @return The original value of *pulDestination_core1.
 */
static portFORCE_INLINE_core1 uint32_t Atomic_AND_u32_core1( uint32_t volatile * pulDestination_core1,
                                                 uint32_t ulValue_core1 )
{
    uint32_t ulCurrent_core1;

    ATOMIC_ENTER_CRITICAL_core1();
    {
        ulCurrent_core1 = *pulDestination_core1;
        *pulDestination_core1 &= ulValue_core1;
    }
    ATOMIC_EXIT_CRITICAL_core1();

    return ulCurrent_core1;
}
/*-----------------------------------------------------------*/

/**
 * Atomic NAND
 *
 * @brief Performs an atomic NAND operation on the specified values.
 *
 * @param [in, out] pulDestination_core1  Pointer to memory location from where value is
 *                                to be loaded and written back to.
 * @param [in] ulValue_core1            Value to be NANDed with *pulDestination_core1.
 *
 * @return The original value of *pulDestination_core1.
 */
static portFORCE_INLINE_core1 uint32_t Atomic_NAND_u32_core1( uint32_t volatile * pulDestination_core1,
                                                  uint32_t ulValue_core1 )
{
    uint32_t ulCurrent_core1;

    ATOMIC_ENTER_CRITICAL_core1();
    {
        ulCurrent_core1 = *pulDestination_core1;
        *pulDestination_core1 = ~( ulCurrent_core1 & ulValue_core1 );
    }
    ATOMIC_EXIT_CRITICAL_core1();

    return ulCurrent_core1;
}
/*-----------------------------------------------------------*/

/**
 * Atomic XOR
 *
 * @brief Performs an atomic XOR operation on the specified values.
 *
 * @param [in, out] pulDestination_core1  Pointer to memory location from where value is
 *                                to be loaded and written back to.
 * @param [in] ulValue_core1            Value to be XORed with *pulDestination_core1.
 *
 * @return The original value of *pulDestination_core1.
 */
static portFORCE_INLINE_core1 uint32_t Atomic_XOR_u32_core1( uint32_t volatile * pulDestination_core1,
                                                 uint32_t ulValue_core1 )
{
    uint32_t ulCurrent_core1;

    ATOMIC_ENTER_CRITICAL_core1();
    {
        ulCurrent_core1 = *pulDestination_core1;
        *pulDestination_core1 ^= ulValue_core1;
    }
    ATOMIC_EXIT_CRITICAL_core1();

    return ulCurrent_core1;
}

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ATOMIC_H */
