/*
 * Copyright (c) 2023 Infineon Technologies AG. All rights reserved.
 *
 *
 *                               IMPORTANT NOTICE
 *
 *
 * Use of this file is subject to the terms of use agreed between (i) you or
 * the company in which ordinary course of business you are acting and (ii)
 * Infineon Technologies AG or its licensees. If and as long as no such
 * terms of use are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or
 * organization obtaining a copy of the software and accompanying
 * documentation covered by this license (the "Software") to use, reproduce,
 * display, distribute, execute, and transmit the Software, and to prepare
 * derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer, must
 * be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are
 * solely in the form of machine-executable object code generated by a source
 * language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * SPDX-License-Identifier: BSL-1.0
 */

#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
    extern "C"
#endif

#include "FreeRTOSConfig.h"

#define portCPU_PSW              0xFE04
#define portCPU_PSW_IS_OFF       ( 9 )
#define portCPU_PSW_CSC_MSK      ( 0x7F )
#define portCPU_ICR              0xFE2C
#define portCPU_ICR_CCPN_OFF     ( 0 )
#define portCPU_ICR_CCPN_MSK     ( 0x000000FFUL )
#define portCPU_FCX              0xFE38
#define portCPU_PCXI             0xFE00
#define portCPU_CORE_ID          0xFE1C

/* Register defintions */
#define portSRC_SRCR_SRPN_OFF    0
#define portSRC_SRCR_SRE_OFF     10
#define portSRC_SRCR_TOS_OFF     11
#define portSRC_SRCR_SRR_OFF     24
#define portSRC_SRCR_SETR_OFF    26

/* Type definitions. */
#define portCHAR                 char
#define portSHORT                short
#define portLONG                 long
#define portFLOAT                float
#define portDOUBLE               double
#define portSTACK_TYPE           uint32_t
#define portBASE_TYPE            long
#define portPOINTER_SIZE_TYPE    uintptr_t

typedef portSTACK_TYPE       StackType_t;
typedef long                 BaseType_t;
typedef unsigned long        UBaseType_t;

#if ( configUSE_16_BIT_TICKS == 1 )
    typedef unsigned short   TickType_t;
    #define portMAX_DELAY    ( TickType_t ) 0xffff
#else
    typedef unsigned int     TickType_t;
    #define portMAX_DELAY    ( TickType_t ) 0xffffffffUL
#endif

/* FreeRTOS parameters */
#define portTICK_TYPE_IS_ATOMIC        1
#define portSTACK_GROWTH               ( -1 )
#define portTICK_PERIOD_MS             ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT             8
#define portCRITICAL_NESTING_IN_TCB    0

/* Attributes */
#define portDONT_DISCARD               __attribute__( ( used ) )
#define portNORETURN                   __attribute__( ( noreturn ) )

/* Instructions */
#define portNOP()               __nop()
#define portMEMORY_BARRIER()    __dsync()

/* Critical section management */
extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );
#define portENTER_CRITICAL()                                     vPortEnterCritical()
#define portEXIT_CRITICAL()                                      vPortExitCritical()
#define portENABLE_INTERRUPTS()                                  vPortSetCCPN( 0 );
#define portDISABLE_INTERRUPTS()                                 vPortSetCCPN( configMAX_API_CALL_INTERRUPT_PRIORITY )
#define portASSERT_IF_IN_ISR()                                   configASSERT( ( __mfcr( portCPU_PSW ) & ( 1 << portCPU_PSW_IS_OFF ) ) == 0 )
#define portCLEAR_INTERRUPT_MASK_FROM_ISR( ulSavedMaskValue )    vPortSetICR( ulSavedMaskValue )
#define portSET_INTERRUPT_MASK_FROM_ISR()                        xPortSetCCPN( configMAX_API_CALL_INTERRUPT_PRIORITY )


#ifndef configYIELD_SYSCALL_ID
    #define configYIELD_SYSCALL_ID    0
#endif

#define portYIELD()    __syscall( configYIELD_SYSCALL_ID )
#define portYIELD_FROM_ISR( xHigherPriorityTaskWoken )                                                                                \
    {                                                                                                                                 \
        const uint32_t xTrigger = ( ( *configCONTEXT_SRC >> portSRC_SRCR_SRR_OFF ) & 0x1 ) != 1 && ( xHigherPriorityTaskWoken != 0 ); \
        *configCONTEXT_SRC |= ( xTrigger << portSRC_SRCR_SETR_OFF );                                                                  \
                                                                                                                                      \
        /* Wait until write request completes to trigger IRQ */                                                                       \
        __dsync();                                                                                                                    \
        __isync();                                                                                                                    \
    }


#ifndef configUSE_PORT_OPTIMISED_TASK_SELECTION
    #define configUSE_PORT_OPTIMISED_TASK_SELECTION    1
#endif

#if ( configUSE_PORT_OPTIMISED_TASK_SELECTION == 1 )

/* Check the configuration. */
    #if ( configMAX_PRIORITIES > 32 )
        #error configUSE_PORT_OPTIMISED_TASK_SELECTION can only be set to 1 when configMAX_PRIORITIES is less than or equal to 32.  It is very rare that a system requires more than 10 to 15 difference priorities as tasks that share a priority will time slice.
    #endif

/* Store/clear the ready priorities in a bit map. */
    #define portRECORD_READY_PRIORITY( uxPriority, uxReadyPriorities )      ( uxReadyPriorities ) |= ( 1UL << ( uxPriority ) )
    #define portRESET_READY_PRIORITY( uxPriority, uxReadyPriorities )       ( uxReadyPriorities ) &= ~( 1UL << ( uxPriority ) )

    #define portGET_HIGHEST_PRIORITY( uxTopPriority, uxReadyPriorities )    uxTopPriority = ( 31UL - ( ( uint32_t ) __clz( ( uxReadyPriorities ) ) ) )

#endif /* configUSE_PORT_OPTIMISED_TASK_SELECTION */

/* Function prototypes */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters )    void vFunction( void * pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters )          void vFunction( void * pvParameters )

/* TCB handling */
extern void vPortReclaimCSA( unsigned long ** pxTCB );
#define portCLEAN_UP_TCB( pxTCB )    vPortReclaimCSA( ( unsigned long ** ) ( pxTCB ) )


/* ICR & CCPN modifying functions to enable and disable interrupts.
 * Only interrupts with a priority lower than
 */
static void __attribute__( ( used, always_inline ) ) vPortSetCCPN( unsigned char ucCCPN )
{
    __disable();
    __mtcr( portCPU_ICR, ( __mfcr( portCPU_ICR ) & ~portCPU_ICR_CCPN_MSK ) | ( ucCCPN & portCPU_ICR_CCPN_MSK ) );
    __enable();
}

static void __attribute__( ( used, always_inline ) ) vPortSetICR( portBASE_TYPE ulICR )
{
    __disable();
    __mtcr( portCPU_ICR, ( unsigned int ) ulICR );
    __enable();
}

static portBASE_TYPE __attribute__( ( used, always_inline ) ) xPortSetCCPN( unsigned char ucCCPN )
{
    uint32_t xICR;

    __disable();
    xICR = __mfcr( portCPU_ICR );
    __mtcr( portCPU_ICR, ( xICR & ~portCPU_ICR_CCPN_MSK ) | ( ucCCPN & portCPU_ICR_CCPN_MSK ) );
    __enable();
    return ( portBASE_TYPE ) xICR;
}

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */
