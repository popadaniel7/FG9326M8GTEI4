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

/*-----------------------------------------------------------
* Portable layer API.  Each function must be defined for each port.
*----------------------------------------------------------*/

#ifndef PORTABLE_CORE0_H
#define PORTABLE_CORE0_H

/* Each FreeRTOS_core0 port has a unique portmacro.h header file.  Originally a
 * pre-processor definition was used to ensure the pre-processor found the correct
 * portmacro.h file for the port being used.  That scheme was deprecated in favour
 * of setting the compiler's include path such that it found the correct
 * portmacro.h file - removing the need for the constant and allowing the
 * portmacro.h file to be located anywhere in relation to the port being used.
 * Purely for reasons of backward compatibility the old method is still valid, but
 * to make it clear that new projects should not use it, support for the port
 * specific constants has been moved into the deprecated_definitions.h header
 * file. */
#include "deprecated_definitions_core0.h"

/* If portENTER_CRITICAL_core0 is not defined then including deprecated_definitions.h
 * did not result in a portmacro.h header file being included - and it should be
 * included here.  In this case the path to the correct portmacro.h header file
 * must be set in the compiler's include path. */
#ifndef portENTER_CRITICAL_core0
    #include "portmacro_core0.h"
#endif

#if portBYTE_ALIGNMENT_core0 == 32
    #define portBYTE_ALIGNMENT_MASK_core0    ( 0x001f )
#elif portBYTE_ALIGNMENT_core0 == 16
    #define portBYTE_ALIGNMENT_MASK_core0    ( 0x000f )
#elif portBYTE_ALIGNMENT_core0 == 8
    #define portBYTE_ALIGNMENT_MASK_core0    ( 0x0007 )
#elif portBYTE_ALIGNMENT_core0 == 4
    #define portBYTE_ALIGNMENT_MASK_core0    ( 0x0003 )
#elif portBYTE_ALIGNMENT_core0 == 2
    #define portBYTE_ALIGNMENT_MASK_core0    ( 0x0001 )
#elif portBYTE_ALIGNMENT_core0 == 1
    #define portBYTE_ALIGNMENT_MASK_core0    ( 0x0000 )
#else /* if portBYTE_ALIGNMENT_core0 == 32 */
    #error "Invalid portBYTE_ALIGNMENT_core0 definition"
#endif /* if portBYTE_ALIGNMENT_core0 == 32 */

#ifndef portUSING_MPU_WRAPPERS_core0
    #define portUSING_MPU_WRAPPERS_core0    0
#endif

#ifndef portNUM_CONFIGURABLE_REGIONS_core0
    #define portNUM_CONFIGURABLE_REGIONS_core0    1
#endif

#ifndef portHAS_STACK_OVERFLOW_CHECKING_core0
    #define portHAS_STACK_OVERFLOW_CHECKING_core0    0
#endif

#ifndef portARCH_NAME_core0
    #define portARCH_NAME_core0    NULL
#endif

#ifndef configSTACK_ALLOCATION_FROM_SEPARATE_HEAP_core0
    /* Defaults to 0 for backward compatibility. */
    #define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP_core0    0
#endif

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

#include "mpu_wrappers_core0.h"

/*
 * Setup the stack of a new task_core0 so it is ready to be placed under the
 * scheduler control.  The registers have to be placed on the stack in
 * the order that the port expects to find them.
 *
 */
#if ( portUSING_MPU_WRAPPERS_core0 == 1 )
    #if ( portHAS_STACK_OVERFLOW_CHECKING_core0 == 1 )
        StackType_t_core0 * pxPortInitialiseStack_core0( StackType_t_core0 * pxTopOfStack_core0,
                                             StackType_t_core0 * pxEndOfStack_core0,
                                             TaskFunction_t_core0 pxCode_core0,
                                             void * pvParameters_core0,
                                             BaseType_t_core0 xRunPrivileged_core0 ) PRIVILEGED_FUNCTION_core0;
    #else
        StackType_t_core0 * pxPortInitialiseStack_core0( StackType_t_core0 * pxTopOfStack_core0,
                                             TaskFunction_t_core0 pxCode_core0,
                                             void * pvParameters_core0,
                                             BaseType_t_core0 xRunPrivileged_core0 ) PRIVILEGED_FUNCTION_core0;
    #endif
#else /* if ( portUSING_MPU_WRAPPERS_core0 == 1 ) */
    #if ( portHAS_STACK_OVERFLOW_CHECKING_core0 == 1 )
        StackType_t_core0 * pxPortInitialiseStack_core0( StackType_t_core0 * pxTopOfStack_core0,
                                             StackType_t_core0 * pxEndOfStack_core0,
                                             TaskFunction_t_core0 pxCode_core0,
                                             void * pvParameters_core0 ) PRIVILEGED_FUNCTION_core0;
    #else
        StackType_t_core0 * pxPortInitialiseStack_core0( StackType_t_core0 * pxTopOfStack_core0,
                                             TaskFunction_t_core0 pxCode_core0,
                                             void * pvParameters_core0 ) PRIVILEGED_FUNCTION_core0;
    #endif
#endif /* if ( portUSING_MPU_WRAPPERS_core0 == 1 ) */

/* Used by heap_5.c to define the start address and size of each memory region
 * that together comprise the total FreeRTOS_core0 heap space. */
typedef struct HeapRegion_core0
{
    uint8_t * pucStartAddress_core0;
    size_t xSizeInBytes_core0;
} HeapRegion_t_core0;

/* Used to pass information about the heap out of vPortGetHeapStats_core0(). */
typedef struct xHeapStats_core0
{
    size_t xAvailableHeapSpaceInBytes_core0;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
    size_t xSizeOfLargestFreeBlockInBytes_core0;  /* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats_core0() is called. */
    size_t xSizeOfSmallestFreeBlockInBytes_core0; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats_core0() is called. */
    size_t xNumberOfFreeBlocks_core0;             /* The number of free memory blocks within the heap at the time vPortGetHeapStats_core0() is called. */
    size_t xMinimumEverFreeBytesRemaining_core0;  /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
    size_t xNumberOfSuccessfulAllocations_core0;  /* The number of calls to pvPortMalloc_core0() that have returned a valid memory block. */
    size_t xNumberOfSuccessfulFrees_core0;        /* The number of calls to vPortFree_core0() that has successfully freed a block of memory. */
} HeapStats_t_core0;

/*
 * Used to define multiple heap regions for use by heap_5.c.  This function
 * must be called before any calls to pvPortMalloc_core0() - not creating a task_core0,
 * queue, semaphore, mutex, software timer, event group, etc. will result in
 * pvPortMalloc_core0 being called.
 *
 * pxHeapRegions_core0 passes in an array of HeapRegion_t_core0 structures - each of which
 * defines a region of memory that can be used as the heap.  The array is
 * terminated by a HeapRegions_t structure that has a size of 0.  The region
 * with the lowest start address must appear first in the array.
 */
void vPortDefineHeapRegions_core0( const HeapRegion_t_core0 * const pxHeapRegions_core0 ) PRIVILEGED_FUNCTION_core0;

/*
 * Returns a HeapStats_t_core0 structure filled with information about the current
 * heap state.
 */
void vPortGetHeapStats_core0( HeapStats_t_core0 * pxHeapStats_core0 );

/*
 * Map to the memory management routines required for the port.
 */
void * pvPortMalloc_core0( size_t xSize_core0 ) PRIVILEGED_FUNCTION_core0;
void * pvPortCalloc_core0( size_t xNum_core0,
                     size_t xSize_core0 ) PRIVILEGED_FUNCTION_core0;
void vPortFree_core0( void * pv_core0 ) PRIVILEGED_FUNCTION_core0;
void vPortInitialiseBlocks_core0( void ) PRIVILEGED_FUNCTION_core0;
size_t xPortGetFreeHeapSize_core0( void ) PRIVILEGED_FUNCTION_core0;
size_t xPortGetMinimumEverFreeHeapSize_core0( void ) PRIVILEGED_FUNCTION_core0;

#if ( configSTACK_ALLOCATION_FROM_SEPARATE_HEAP_core0 == 1 )
    void * pvPortMallocStack_core0( size_t xSize_core0 ) PRIVILEGED_FUNCTION_core0;
    void vPortFreeStack_core0( void * pv_core0 ) PRIVILEGED_FUNCTION_core0;
#else
    #define pvPortMallocStack_core0    pvPortMalloc_core0
    #define vPortFreeStack_core0       vPortFree_core0
#endif

#if ( configUSE_MALLOC_FAILED_HOOK_core0 == 1 )

/**
 * task_core0.h
 * @code{c}
 * void vApplicationMallocFailedHook_core0( void )
 * @endcode
 *
 * This hook function is called when allocation failed.
 */
    void vApplicationMallocFailedHook_core0( void ); /*lint !e526 Symbol not defined as it is an application callback. */
#endif

/*
 * Setup the hardware ready for the scheduler to take control.  This generally
 * sets up a tick interrupt and sets timers_core0 for the correct tick frequency.
 */
BaseType_t_core0 xPortStartScheduler_core0( void ) PRIVILEGED_FUNCTION_core0;

/*
 * Undo any hardware/ISR setup that was performed by xPortStartScheduler_core0() so
 * the hardware is left in its original condition after the scheduler stops
 * executing.
 */
void vPortEndScheduler_core0( void ) PRIVILEGED_FUNCTION_core0;

/*
 * The structures and methods of manipulating the MPU are contained within the
 * port layer.
 *
 * Fills the xMPUSettings_core0 structure with the memory region information
 * contained in xRegions_core0.
 */
#if ( portUSING_MPU_WRAPPERS_core0 == 1 )
    struct xMEMORY_REGION_core0;
    void vPortStoreTaskMPUSettings_core0( xMPU_SETTINGS_core0 * xMPUSettings_core0,
                                    const struct xMEMORY_REGION_core0 * const xRegions_core0,
                                    StackType_t_core0 * pxBottomOfStack_core0,
                                    uint32_t ulStackDepth_core0 ) PRIVILEGED_FUNCTION_core0;
#endif

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* PORTABLE_H */
