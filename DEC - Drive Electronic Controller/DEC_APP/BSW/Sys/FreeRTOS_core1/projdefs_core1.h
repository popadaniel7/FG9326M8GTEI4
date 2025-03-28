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

#ifndef PROJDEFS_CORE1_H
#define PROJDEFS_CORE1_H

/*
 * Defines the prototype to which task_core1 functions must conform.  Defined in this
 * file to ensure the type is known before portable.h is included.
 */
typedef void (* TaskFunction_t_core1)( void * );

/* Converts a time in milliseconds to a time in ticks.  This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application. */
#ifndef pdMS_TO_TICKS_core1
    #define pdMS_TO_TICKS_core1( xTimeInMs_core1 )    ( ( TickType_t_core1 ) ( ( ( TickType_t_core1 ) ( xTimeInMs_core1 ) * ( TickType_t_core1 ) configTICK_RATE_HZ_core1 ) / ( TickType_t_core1 ) 1000U ) )
#endif

#define pdFALSE_core1                                  ( ( BaseType_t_core1 ) 0 )
#define pdTRUE_core1                                   ( ( BaseType_t_core1 ) 1 )

#define pdPASS_core1                                   ( pdTRUE_core1 )
#define pdFAIL_core1                                   ( pdFALSE_core1 )
#define errQUEUE_EMPTY_core1                           ( ( BaseType_t_core1 ) 0 )
#define errQUEUE_FULL_core1                            ( ( BaseType_t_core1 ) 0 )

/* FreeRTOS_core1 error definitions. */
#define errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY_core1    ( -1 )
#define errQUEUE_BLOCKED_core1                         ( -4 )
#define errQUEUE_YIELD_core1                           ( -5 )

/* Macros used for basic data corruption checks. */
#ifndef configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES_core1
    #define configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES_core1    0
#endif

#if ( configUSE_16_BIT_TICKS_core1 == 1 )
    #define pdINTEGRITY_CHECK_VALUE_core1    0x5a5a
#else
    #define pdINTEGRITY_CHECK_VALUE_core1    0x5a5a5a5aUL
#endif

/* The following errno values are used by FreeRTOS_core1+ components, not FreeRTOS_core1
 * itself. */
#define pdFREERTOS_ERRNO_NONE_core1             0   /* No errors */
#define pdFREERTOS_ERRNO_ENOENT_core1           2   /* No such file or directory */
#define pdFREERTOS_ERRNO_EINTR_core1            4   /* Interrupted system call */
#define pdFREERTOS_ERRNO_EIO_core1              5   /* I/O error */
#define pdFREERTOS_ERRNO_ENXIO_core1            6   /* No such device or address */
#define pdFREERTOS_ERRNO_EBADF_core1            9   /* Bad file number */
#define pdFREERTOS_ERRNO_EAGAIN_core1           11  /* No more processes */
#define pdFREERTOS_ERRNO_EWOULDBLOCK_core1      11  /* Operation would block */
#define pdFREERTOS_ERRNO_ENOMEM_core1           12  /* Not enough memory */
#define pdFREERTOS_ERRNO_EACCES_core1           13  /* Permission denied */
#define pdFREERTOS_ERRNO_EFAULT_core1           14  /* Bad address */
#define pdFREERTOS_ERRNO_EBUSY_core1            16  /* Mount device busy */
#define pdFREERTOS_ERRNO_EEXIST_core1           17  /* File exists */
#define pdFREERTOS_ERRNO_EXDEV_core1            18  /* Cross-device link */
#define pdFREERTOS_ERRNO_ENODEV_core1           19  /* No such device */
#define pdFREERTOS_ERRNO_ENOTDIR_core1           20  /* Not a directory */
#define pdFREERTOS_ERRNO_EISDIR_core1            21  /* Is a directory */
#define pdFREERTOS_ERRNO_EINVAL_core1            22  /* Invalid argument */
#define pdFREERTOS_ERRNO_ENOSPC_core1            28  /* No space left on device */
#define pdFREERTOS_ERRNO_ESPIPE_core1            29  /* Illegal seek */
#define pdFREERTOS_ERRNO_EROFS_core1             30  /* Read only file system */
#define pdFREERTOS_ERRNO_EUNATCH_core1           42  /* Protocol driver not attached */
#define pdFREERTOS_ERRNO_EBADE_core1             50  /* Invalid exchange */
#define pdFREERTOS_ERRNO_EFTYPE_core1            79  /* Inappropriate file type or format */
#define pdFREERTOS_ERRNO_ENMFILE_core1           89  /* No more files */
#define pdFREERTOS_ERRNO_ENOTEMPTY_core1         90  /* Directory not empty */
#define pdFREERTOS_ERRNO_ENAMETOOLONG_core1      91  /* File or path name too long */
#define pdFREERTOS_ERRNO_EOPNOTSUPP_core1        95  /* Operation not supported on transport endpoint */
#define pdFREERTOS_ERRNO_ENOBUFS_core1           105 /* No buffer space available */
#define pdFREERTOS_ERRNO_ENOPROTOOPT_core1       109 /* Protocol not available */
#define pdFREERTOS_ERRNO_EADDRINUSE_core1        112 /* Address already in use */
#define pdFREERTOS_ERRNO_ETIMEDOUT_core1         116 /* Connection timed out */
#define pdFREERTOS_ERRNO_EINPROGRESS_core1       119 /* Connection already in progress */
#define pdFREERTOS_ERRNO_EALREADY_core1          120 /* Socket already connected */
#define pdFREERTOS_ERRNO_EADDRNOTAVAIL_core1     125 /* Address not available */
#define pdFREERTOS_ERRNO_EISCONN_core1           127 /* Socket is already connected */
#define pdFREERTOS_ERRNO_ENOTCONN_core1          128 /* Socket is not connected */
#define pdFREERTOS_ERRNO_ENOMEDIUM_core1         135 /* No medium inserted */
#define pdFREERTOS_ERRNO_EILSEQ_core1            138 /* An invalid UTF-16 sequence was encountered. */
#define pdFREERTOS_ERRNO_ECANCELED_core1         140 /* Operation canceled. */

/* The following endian values are used by FreeRTOS_core1+ components, not FreeRTOS_core1
 * itself. */
#define pdFREERTOS_LITTLE_ENDIAN_core1           0
#define pdFREERTOS_BIG_ENDIAN_core1              1

/* Re-defining endian values for generic naming. */
#define pdLITTLE_ENDIAN_core1                    pdFREERTOS_LITTLE_ENDIAN_core1 
#define pdBIG_ENDIAN_core1                       pdFREERTOS_BIG_ENDIAN_core1 


#endif /* PROJDEFS_H */
