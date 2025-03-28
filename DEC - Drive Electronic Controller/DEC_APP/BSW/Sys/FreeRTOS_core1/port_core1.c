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
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "FreeRTOS_core1.h"
#include "task_core1.h"

/* Prgoram status word macros */
#define portINITIAL_SYSTEM_PSW_core1 \
        ( 0x000008FFUL ) /* Supervisor Mode, MPU Register Set 0 and Call Depth Counting disabled. */

/* Context save area macros */
#define portCSA_FCX_MASK_core1          ( 0x000FFFFFUL )
#define portINITIAL_LOWER_PCXI_core1    ( 0x00300000UL ) /* Set UL to upper and PIE to 1 */
#define portINITIAL_UPPER_PCXI_core1    ( 0x00200000UL ) /* Set UL to lower and PIE to 1 */
#define portNUM_WORDS_IN_CSA_core1      ( 16 )

//#define configTICK_STM_DEBUG_core1 1
//#define configCPU_STM_DEBUG 1

extern volatile unsigned long * pxCurrentTCB_core1;

/* Tick and context switch config */
#define portTICK_COUNT_core1    ( configSTM_CLOCK_HZ_core1 / configTICK_RATE_HZ_core1 )

/* Register defines */
static volatile uint32_t *const pxStm_core1 = configSTM_core1;
static volatile uint32_t *const pxStmSrc_core1 = configSTM_SRC_core1;
static volatile uint32_t *const pxContextSrc_core1 = configCONTEXT_SRC_core1;
#define portSTM_TIM0_core1                 0x10
#define portSTM_CMP0_core1                 0x30
#define portSTM_COMCON_core1               0x38
#define portSTM_ICR_core1                  0x3C
#define portSTM_ISCR_core1                 0x40
#define portSTM_OCS_core1                  0xE8

#define portSTM_CMCON_MSTART0_OFF_core1    8
#define portSTM_CMCON_MSIZE0_OFF_core1     0
#define portSTM_ICR_CMP0EN_OFF_core1       0
#define portSTM_ICR_CMP0OS_OFF_core1       2
#define portSTM_ISCR_CMP0IRR_OFF_core1     0

static inline void vPortStartFirstTask_core1( void );
static inline void vPortInitContextSrc_core1( void );
static inline void vPortInitTickTimer_core1( void );

static inline void __attribute__( ( always_inline ) ) vPortLoadContext_core1( unsigned char ucCallDepth_core1 );
static inline void __attribute__( ( always_inline ) ) vPortSaveContext_core1( unsigned char ucCallDepth_core1 );

static inline uint32_t * __attribute__( ( always_inline ) ) pxPortCsaToAddress_core1( uint32_t xCsa_core1 );

static UBaseType_t_core1 uxCriticalNesting_core1 = 0xaaaaaaaa;

/* FreeRTOS_core1 required functions */
BaseType_t_core1 xPortStartScheduler_core1( void )
{
    vPortInitTickTimer_core1();
    vPortInitContextSrc_core1();
    vPortStartFirstTask_core1();

    return 0;
}

void vPortEndScheduler_core1()
{
    pxStmSrc_core1[ 0 ] &= ~( 1 << portSRC_SRCR_SRE_OFF_core1 );
    pxContextSrc_core1[ 0 ] &= ~( 1 << portSRC_SRCR_SRE_OFF_core1 );
}

StackType_t_core1 *pxPortInitialiseStack_core1( StackType_t_core1 * pxTopOfStack_core1,
        TaskFunction_t_core1 pxCode_core1,
        void * pvParameters_core1 )
{
    uint32_t xLowerCsa = 0, xUpperCsa = 0;
    uint32_t * pxUpperCSA = NULL;
    uint32_t * pxLowerCSA = NULL;

    /* Have to disable interrupts here because the CSAs are going to be
     * manipulated. */
    __disable();
    {
        /* DSync to ensure that buffering is not a problem. */
        __dsync();

        /* Consume two free CSAs. */
        xLowerCsa = __mfcr( portCPU_FCX_core1 );
        pxLowerCSA = pxPortCsaToAddress_core1( xLowerCsa );

        if( pxLowerCSA != NULL )
        {
            /* The Lower Links to the Upper. */
            xUpperCsa = pxLowerCSA[ 0 ];
            pxUpperCSA = pxPortCsaToAddress_core1( pxLowerCSA[ 0 ] );
        }

        /* Check that we have successfully reserved two CSAs. */
        if( ( pxLowerCSA != NULL ) && ( pxUpperCSA != NULL ) )
        {
            /* Remove the two consumed CSAs from the free CSA list. */
            __mtcr( portCPU_FCX_core1, pxUpperCSA[ 0 ] );
        }
        else
        {
            /* Simply trigger a context list depletion trap. */
            __asm( "\tsvlcx" );
        }
    }
    __enable();

    /* Upper Context. */
    memset( pxUpperCSA, 0, portNUM_WORDS_IN_CSA_core1 * sizeof( uint32_t ) );
    pxUpperCSA[ 2 ] = ( uint32_t ) pxTopOfStack_core1; /* A10;    Stack Return aka Stack Pointer */
    pxUpperCSA[ 1 ] = portINITIAL_SYSTEM_PSW_core1;    /* PSW    */
    pxUpperCSA[ 0 ] = portINITIAL_UPPER_PCXI_core1;

    /* Lower Context. */
    memset( pxLowerCSA, 0, portNUM_WORDS_IN_CSA_core1 * sizeof( uint32_t ) );
    pxLowerCSA[ 8 ] = ( uint32_t ) pvParameters_core1;          /* A4;    Address Type Parameter Register    */
    pxLowerCSA[ 1 ] = ( uint32_t ) pxCode_core1;                /* A11;    Return Address aka RA */
    pxLowerCSA[ 0 ] = portINITIAL_LOWER_PCXI_core1 | xUpperCsa; /* PCXI pointing to the Upper context. */

    /* Initialize the uxCriticalNesting_core1. */
    pxTopOfStack_core1--;
    *pxTopOfStack_core1 = 0;
    /* Save the link to the CSA to the top of stack. */
    pxTopOfStack_core1--;
    *pxTopOfStack_core1 = xLowerCsa;

    return pxTopOfStack_core1;
}

void __interrupt( configCONTEXT_INTERRUPT_PRIORITY_core1 ) __vector_table( configCPU_NR_core1 )
        vPortSystemContextHandler_core1()
        {
    /* Disable interrupts to protect section*/
    __disable();

    /* Do a save, switch, execute */
    vPortSaveContext_core1( 0 );
    vTaskSwitchContext_core1();
    vPortLoadContext_core1( 0 );

    __enable();
        }

void __interrupt( configTIMER_INTERRUPT_PRIORITY_core1 ) __vector_table( configCPU_NR_core1 )
        vPortSystemTickHandler_core1()
        {
    unsigned long ulSavedInterruptMask;
    BaseType_t_core1 xYieldRequired_core1;

    /* Increment compare value by tick count */
    pxStm_core1[ portSTM_CMP0_core1 >> 2 ] = pxStm_core1[ portSTM_CMP0_core1 >> 2 ] + portTICK_COUNT_core1;
    pxStm_core1[ portSTM_ISCR_core1 >> 2 ] |= ( 1 << portSTM_ISCR_CMP0IRR_OFF_core1 );

    /* Check for possible tick drop.
     * If the time is beyond the compare value, the next tick will need a complete
     * wrap around. The tick count isn't accruate any more. Increase the tick count
     * or adapt to execute xTaskIncrementTick_core1 multiple times depending on the
     * counts missed.   */
    //configASSERT_core1( ( pxStm_core1[ portSTM_CMP0_core1 >> 2 ] - pxStm_core1[ portSTM_TIM0_core1 >> 2 ] ) <= portTICK_COUNT_core1 );

    /* Kernel API calls require Critical Sections. */
    ulSavedInterruptMask = portSET_INTERRUPT_MASK_FROM_ISR_core1();
    {
        /* Increment the Tick. */
        xYieldRequired_core1 = xTaskIncrementTick_core1();
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR_core1( ulSavedInterruptMask );

    portYIELD_FROM_ISR_core1( xYieldRequired_core1 );
        }

void __attribute__( ( noinline ) ) vPortSyscallYield_core1( void );

int
#if configPROVIDE_SYSCALL_TRAP_core1 != 0
__trap( configTIMER_INTERRUPT_PRIORITY_core1 ) __vector_table( configCPU_NR_core1 )
#endif
vPortSyscallHandler_core1( unsigned char id )
{
    switch( id )
    {
        case 0:
            vPortSyscallYield_core1();
            break;

        default:
            break;
    }

    return 0;
}

void vPortInitTickTimer_core1()
{
    pxStm_core1[ portSTM_COMCON_core1 >> 2 ] =
            ( 0 << portSTM_CMCON_MSTART0_OFF_core1 ) | ( 31 << portSTM_CMCON_MSIZE0_OFF_core1 );
    pxStm_core1[ portSTM_ICR_core1 >> 2 ] &= ~( 1 << portSTM_ICR_CMP0OS_OFF_core1 );
    pxStmSrc_core1[ 0 ] = ( ( configCPU_NR_core1 > 0 ?
            configCPU_NR_core1 + 1 : configCPU_NR_core1 ) << portSRC_SRCR_TOS_OFF_core1 ) |
            ( ( configTIMER_INTERRUPT_PRIORITY_core1 ) << portSRC_SRCR_SRPN_OFF_core1 );
    pxStmSrc_core1[ 0 ] |= ( 1 << portSRC_SRCR_SRE_OFF_core1 );
    pxStm_core1[ portSTM_CMP0_core1 >> 2 ] = pxStm_core1[ portSTM_TIM0_core1 >> 2 ];
    pxStm_core1[ portSTM_ISCR_core1 >> 2 ] |= ( 1 << portSTM_ISCR_CMP0IRR_OFF_core1 );
    pxStm_core1[ portSTM_ICR_core1 >> 2 ] |= ( 1 << portSTM_ICR_CMP0EN_OFF_core1 );
    pxStm_core1[ portSTM_CMP0_core1 >> 2 ] = pxStm_core1[ portSTM_TIM0_core1 >> 2 ] + portTICK_COUNT_core1;
    //pxStm_core1[ portSTM_OCS_core1 >> 2 ] = 0x12000000;
}

void vPortInitContextSrc_core1()
{
    pxContextSrc_core1[ 0 ] =
            ( ( configCPU_NR_core1 > 0 ?
                    configCPU_NR_core1 + 1 : configCPU_NR_core1 ) << portSRC_SRCR_TOS_OFF_core1 ) |
                    ( ( configCONTEXT_INTERRUPT_PRIORITY_core1 ) << portSRC_SRCR_SRPN_OFF_core1 );
    pxContextSrc_core1[ 0 ] |= ( 1 << portSRC_SRCR_SRE_OFF_core1 );
}

void vPortStartFirstTask_core1()
{
    /* Disable interrupts  */
    __disable();

    vPortLoadContext_core1( 0 );

    /* Reset the call stack counting, to avoid trap on rfe */
    unsigned long ulPsw = __mfcr( portCPU_PSW_core1 );

    ulPsw &= ~( portCPU_PSW_CSC_MSK_core1 );
    __mtcr( portCPU_PSW_core1, ulPsw );

    /* Load the lower context and upper context through rfe to enable irqs */
    __asm( "\trslcx" );
    __asm( "\trfe" );
    __nop();
    __nop();
    __nop();
}

void vPortLoadContext_core1( unsigned char ucCallDepth_core1 )
{
    uint32_t ** ppxTopOfStack;
    uint32_t uxLowerCSA;

    /* Dsync is required to complete any memory requests */
    __dsync();

    /* Load the new CSA id from the stack and update the stack pointer */
    ppxTopOfStack = ( uint32_t ** ) pxCurrentTCB_core1;
    uxLowerCSA = **ppxTopOfStack;
    ( *ppxTopOfStack )++;
    uxCriticalNesting_core1 = **ppxTopOfStack;
    ( *ppxTopOfStack )++;

    /* Store the lower context directly if inside the syscall or interrupt,
     * else replace the lower context in the call stack. */
    if( !ucCallDepth_core1 )
    {
        /* Update the link register */
        __mtcr( portCPU_PCXI_core1, uxLowerCSA );
    }
    else
    {
        /* Update previous lower context */
        uint32_t * pxCSA = pxPortCsaToAddress_core1( __mfcr( portCPU_PCXI_core1 ) );
        int i;

        for(i = 0; i < ucCallDepth_core1 - 1; i++)
        {
            pxCSA = pxPortCsaToAddress_core1( pxCSA[ 0 ] );
        }

        pxCSA[ 0 ] = uxLowerCSA;
    }
}

void vPortSaveContext_core1( unsigned char ucCallDepth_core1 )
{
    uint32_t ** ppxTopOfStack;
    uint32_t * pxLowerCSA, * pxUpperCSA;
    uint32_t uxLowerCSA;

    /* Dsync is required for save CSA access */
    __dsync();

    /* Get the current context information. */
    uxLowerCSA = __mfcr( portCPU_PCXI_core1 );

    /* If this function is used inside a function from the syscall or interrupt,
     * load the correct context from the call stack */
    if( ucCallDepth_core1 )
    {
        uint32_t * pxCSA = pxPortCsaToAddress_core1( uxLowerCSA );
        int i;

        for(i = 0; i < ucCallDepth_core1 - 1; i++)
        {
            pxCSA = pxPortCsaToAddress_core1( pxCSA[ 0 ] );
        }

        uxLowerCSA = pxCSA[ 0 ];
    }

    pxLowerCSA = pxPortCsaToAddress_core1( uxLowerCSA );
    pxUpperCSA = pxPortCsaToAddress_core1( pxLowerCSA[ 0 ] );

    /* Load the stack pointer */
    ppxTopOfStack = ( uint32_t ** ) pxCurrentTCB_core1;
    /* Update the stack info in the TCB */
    *ppxTopOfStack = ( uint32_t * ) pxUpperCSA[ 2 ];
    /* Place ucNestedContext */
    ( *ppxTopOfStack )--;
    **ppxTopOfStack = uxCriticalNesting_core1;
    /* Place the lower CSA id on the stack */
    ( *ppxTopOfStack )--;
    **ppxTopOfStack = uxLowerCSA;
}

void vPortSyscallYield_core1()
{
    /* Do a save, switch, execute */
    vPortSaveContext_core1( configSYSCALL_CALL_DEPTH_core1 );
    vTaskSwitchContext_core1();
    vPortLoadContext_core1( configSYSCALL_CALL_DEPTH_core1 );
}

uint32_t * pxPortCsaToAddress_core1( uint32_t xCsa_core1 )
{
    uint32_t pxCsa;

    pxCsa = ( __extru( xCsa_core1, 16, 4 ) << 28 );
    pxCsa = __insert( pxCsa, xCsa_core1, 6, 16 );
    return ( uint32_t * ) pxCsa;
}

void vPortEnterCritical_core1( void )
{
    portDISABLE_INTERRUPTS_core1();
    uxCriticalNesting_core1++;

    /* This is not the interrupt safe version of the enter critical function so
     * assert() if it is being called from an interrupt context.  Only API
     * functions that end in "FromISR" can be used in an interrupt.  Only assert if
     * the critical nesting count is 1 to protect against recursive calls if the
     * assert function also uses a critical section. */
    if( uxCriticalNesting_core1 == 1 )
    {
        portASSERT_IF_IN_ISR_core1();
    }
}

void vPortExitCritical_core1( void )
{
    configASSERT_core1( uxCriticalNesting_core1 );
    uxCriticalNesting_core1--;

    if( uxCriticalNesting_core1 == 0 )
    {
        portENABLE_INTERRUPTS_core1();
    }
}

/*
 * When a task_core1 is deleted, it is yielded permanently until the IDLE_core1 task_core1
 * has an opportunity to reclaim the memory that that task_core1 was using.
 * Typically, the memory used by a task_core1 is the TCB and Stack but in the
 * TriCore this includes the CSAs that were consumed as part of the Call
 * Stack. These CSAs can only be returned to the Globally Free Pool when
 * they are not part of the current Call Stack, hence, delaying the
 * reclamation until the IDLE_core1 task_core1 is freeing the task_core1's other resources.
 * This function uses the head of the linked list of CSAs (from when the
 * task_core1 yielded for the last time) and finds the tail (the very bottom of
 * the call stack) and inserts this list at the head of the Free list,
 * attaching the existing Free List to the tail of the reclaimed call stack.
 *
 * NOTE: In highly loaded systems the release of used CSAs might be delayed,
 * since it is executed es part of the calling tasks, if the deleted task_core1 is
 * different from the calling tasks, or as part of the idle task_core1, if the deleted
 * tasks is the same as the calling task_core1.
 */
void vPortReclaimCSA_core1( unsigned long ** pxTCB_core1 )
{
    uint32_t ulHeadCSA, ulFreeCSA;
    uint32_t * pulNextCSA;

    /* The lower context (PCXI value) to return to the task_core1 is stored as the
     * current element on the stack. Mask off everything in the PCXI register
     * other than the address. */
    ulHeadCSA = ( **pxTCB_core1 ) & portCSA_FCX_MASK_core1;

    /* Iterate over the CSAs that were consumed as part of the task_core1. */
    for(pulNextCSA = pxPortCsaToAddress_core1( ulHeadCSA );
            ( pulNextCSA[ 0 ] & portCSA_FCX_MASK_core1 ) != 0;
            pulNextCSA = pxPortCsaToAddress_core1( pulNextCSA[ 0 ] ) )
    {
        /* Mask off everything in the PCXI value other than the address. */
        pulNextCSA[ 0 ] &= portCSA_FCX_MASK_core1;
    }

    __disable();
    {
        /* Look up the current free CSA head. */
        __dsync();
        ulFreeCSA = __mfcr( portCPU_FCX_core1 );

        /* Join the current free onto the tail of what is being reclaimed. */
        pulNextCSA[ 0 ] = ulFreeCSA;

        /* Move the head of the reclaimed into the Free. */
        __mtcr( portCPU_FCX_core1, ulHeadCSA );
    }
    __enable();
}

void __attribute__( ( noreturn ) ) vPortLoopForever_core1( void )
        {
    while( 1 )
    {
    }
        }
