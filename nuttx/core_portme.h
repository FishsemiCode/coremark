/****************************************************************************
 * apps/external/tools/coremark/nuttx/core_portme.h
 *
 *   Copyright (C) 2018 Pinecone Inc. All rights reserved.
 *   Author: Pinecone <pinecone@pinecone.net>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef CORE_PORTME_H
#define CORE_PORTME_H

#include <nuttx/config.h>

/************************/
/* Data types and settings */
/************************/
/* Configuration: HAS_FLOAT
	Define to 1 if the platform supports floating point.
*/
#ifndef HAS_FLOAT
#ifdef CONFIG_LIBC_FLOATINGPOINT
#define HAS_FLOAT 1
#else
#define HAS_FLOAT 0
#endif
#endif
/* Configuration: HAS_TIME_H
	Define to 1 if platform has the time.h header file,
	and implementation of functions thereof.
*/
#ifndef HAS_TIME_H
#define HAS_TIME_H 1
#endif
/* Configuration: USE_CLOCK
	Define to 1 if platform has the time.h header file,
	and implementation of functions thereof.
*/
#ifndef USE_CLOCK
#define USE_CLOCK 0
#endif
/* Configuration: HAS_STDIO
	Define to 1 if the platform has stdio.h.
*/
#ifndef HAS_STDIO
#define HAS_STDIO 1
#endif
/* Configuration: HAS_PRINTF
	Define to 1 if the platform has stdio.h and implements the printf function.
*/
#ifndef HAS_PRINTF
#define HAS_PRINTF 1
#endif

/* Configuration: CORE_TICKS
	Define type of return from the timing functions.
 */
#if defined(HAS_TIME_H)
#include <time.h>
typedef clock_t CORE_TICKS;
#else
#error "Please define type of CORE_TICKS and implement start_time, end_time get_time and time_in_secs functions!"
#endif

/* Definitions: COMPILER_VERSION, COMPILER_FLAGS, MEM_LOCATION
	Initialize these strings per platform
*/
#ifndef COMPILER_VERSION
 #ifdef __GNUC__
 #define COMPILER_VERSION "GCC"__VERSION__
 #else
 #define COMPILER_VERSION "Please put compiler version here (e.g. gcc 4.1)"
 #endif
#endif
#ifndef COMPILER_FLAGS
 #define COMPILER_FLAGS "" /* "Please put compiler flags here (e.g. -o3)" */
#endif
#ifndef MEM_LOCATION
 #if CONFIG_TOOLS_COREMARK_MEM_METHOD == MEM_MALLOC
  #define MEM_LOCATION "MALLOC"
 #elif CONFIG_TOOLS_COREMARK_MEM_METHOD == MEM_STATIC
  #define MEM_LOCATION "STATIC"
 #elif CONFIG_TOOLS_COREMARK_MEM_METHOD == MEM_STACK
  #define MEM_LOCATION "STACK"
 #endif
#endif

/* Data Types:
	To avoid compiler issues, define the data types that need ot be used for 8b, 16b and 32b in <core_portme.h>.

	*Imprtant*:
	ee_ptr_int needs to be the data type used to hold pointers, otherwise coremark may fail!!!
*/
#include <stdint.h>
typedef int16_t ee_s16;
typedef uint16_t ee_u16;
typedef int32_t ee_s32;
typedef float ee_f32;
typedef uint8_t ee_u8;
typedef uint32_t ee_u32;
typedef intptr_t ee_ptr_int;
typedef size_t ee_size_t;
/* align_mem:
	This macro is used to align an offset to point to a 32b value. It is used in the Matrix algorithm to initialize the input memory blocks.
*/
#define align_mem(x) (void *)(sizeof(int32_t) + (((ee_ptr_int)(x) - 1) & ~(sizeof(int32_t) - 1)))

/* Configuration: SEED_METHOD
	Defines method to get seed values that cannot be computed at compile time.

	Valid values:
	SEED_ARG - from command line.
	SEED_FUNC - from a system function.
	SEED_VOLATILE - from volatile variables.
*/
#ifndef SEED_METHOD
#define SEED_METHOD CONFIG_TOOLS_COREMARK_SEED_METHOD
#endif

/* Configuration: MEM_METHOD
	Defines method to get a block of memry.

	Valid values:
	MEM_MALLOC - for platforms that implement malloc and have malloc.h.
	MEM_STATIC - to use a static memory array.
	MEM_STACK - to allocate the data block on the stack (NYI).
*/
#ifndef MEM_METHOD
#define MEM_METHOD CONFIG_TOOLS_COREMARK_MEM_METHOD
#endif

/* Configuration: MULTITHREAD
	Define for parallel execution

	Valid values:
	1 - only one context (default).
	N>1 - will execute N copies in parallel.

	Note:
	If this flag is defined to more then 1, an implementation for launching parallel contexts must be defined.

	Two sample implementations are provided. Use <USE_PTHREAD> or <USE_FORK> to enable them.

	It is valid to have a different implementation of <core_start_parallel> and <core_end_parallel> in <core_portme.c>,
	to fit a particular architecture.
*/
#ifndef MULTITHREAD
#define MULTITHREAD CONFIG_TOOLS_COREMARK_MULTITHREAD
#endif

/* Configuration: USE_PTHREAD
	Sample implementation for launching parallel contexts
	This implementation uses pthread_thread_create and pthread_join.

	Valid values:
	0 - Do not use pthreads API.
	1 - Use pthreads API

	Note:
	This flag only matters if MULTITHREAD has been defined to a value greater then 1.
*/
#ifndef USE_PTHREAD
#define USE_PTHREAD 0
#endif

/* Configuration: MAIN_HAS_NOARGC
	Needed if platform does not support getting arguments to main.

	Valid values:
	0 - argc/argv to main is supported
	1 - argc/argv to main is not supported
*/
#ifndef MAIN_HAS_NOARGC
#define MAIN_HAS_NOARGC 0
#endif

/* Configuration: MAIN_HAS_NORETURN
	Needed if platform does not support returning a value from main.

	Valid values:
	0 - main returns an int, and return value will be 0.
	1 - platform does not support returning a value from main
*/
#ifndef MAIN_HAS_NORETURN
#define MAIN_HAS_NORETURN 0
#endif

/* Variable: default_num_contexts
	Number of contexts to spawn in multicore context.
	Override this global value to change number of contexts used.

	Note:
	This value may not be set higher then the <MULTITHREAD> define.

	To experiment, you can set the <MULTITHREAD> define to the highest value expected, and use argc/argv in the <portable_init> to set this value from the command line.
*/
extern ee_u32 default_num_contexts;

#if (MULTITHREAD>1)
#if USE_PTHREAD
	#include <pthread.h>
	#define PARALLEL_METHOD "PThreads"
	#define USE_FORK 0
	#define USE_SOCKET 0
#endif /* Method for multithreading */
#endif /* MULTITHREAD > 1 */

typedef struct CORE_PORTABLE_S {
#if (MULTITHREAD>1)
	#if USE_PTHREAD
	pthread_t thread;
	#endif /* Method for multithreading */
#endif /* MULTITHREAD>1 */
	ee_u8	portable_id;
} core_portable;

/* target specific init/fini */
void portable_init(core_portable *p, int *argc, char *argv[]);
void portable_fini(core_portable *p);

/* Configuration: RUN_MODE
	CoreMark running mode.

	Valid values:
	VALIDATION_RUN - validation verification
	PERFORMANCE_RUN : performance verification
	PROFILE_RUN : prefile verification

*/
#define VALIDATION_RUN 0
#define PERFORMANCE_RUN 1
#define PROFILE_RUN 2

#ifndef RUN_MODE
#define RUN_MODE CONFIG_TOOLS_COREMARK_RUN_METHOD
#endif

/* Configuration: ITERATIONS
	Define for determine the number of iterations
*/
#ifndef ITERATIONS
#define ITERATIONS CONFIG_TOOLS_COREMARK_ITERATIONS
#endif

/* Configuration: CORE_DEBUG
	Defines debug mode.
*/
#ifndef CORE_DEBUG
#define CORE_DEBUG CONFIG_TOOLS_COREMARK_DEBUG
#endif

#define COMPILER_REQUIRES_SORT_RETURN 0

#endif /* CORE_PORTME_H */
