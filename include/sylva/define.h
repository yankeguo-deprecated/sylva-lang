//
//  define.h
//  sylva
//
//  Created by Yanke Guo on 2016/9/20.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#ifndef _SYLVA_DEFINE_H_
#define _SYLVA_DEFINE_H_

#include <limits.h>
#include <assert.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/cdefs.h>

/***********************************************************************************************************************
 * OS
 **********************************************************************************************************************/

#ifdef __APPLE__
#include "TargetConditionals.h"

#if !TARGET_OS_OSX
#error sylva is currently available on macOS only
#endif
#endif

/***********************************************************************************************************************
 * CPP
 **********************************************************************************************************************/

#ifdef __cplusplus
#ifndef _BEGIN_STD_C
#define _BEGIN_STD_C extern "C" {
#endif
#ifndef _END_STD_C
#define _END_STD_C }
#endif
#else
#ifndef _BEGIN_STD_C
#define _BEGIN_STD_C
#endif
#ifndef _END_STD_C
#define _END_STD_C
#endif
#endif

/***********************************************************************************************************************
 * Primitive Types
 **********************************************************************************************************************/

_BEGIN_STD_C

/**
 * boolean type in sylva runtime, alias to C `bool`
 */
typedef bool sl_boolean;

/**
 * true value for boolean type
 */
#define sl_true true
#define sl_yes  sl_true

/**
 * false value for boolean type
 */
#define sl_false false
#define sl_no    sl_false

/**
 * index type, i.e. unsigned integer type, in sylva runtime, alias to C `unsigned long`
 *
 * this is also the type for function id and member id
 */
typedef unsigned long sl_index;

/**
 * max possible sl_index value
 */
#define sl_index_max ULONG_MAX

/**
 * index type returned when nothing is found, alias to max value for unsigned long
 */
#define sl_index_not_found sl_index_max

/**
 * integer type in sylva runtime, alias to C `long`
 */
typedef long sl_integer;

/**
 * float type in sylva runtime, alias to C `double`
 */
typedef double sl_float;

/**
 * comparison result
 */
typedef enum {
  /**
   * left < right
   */
      sl_ascending = -1,
  /**
   * left == right
   */
      sl_same = 0,
  /**
   * left > right
   */
      sl_descending = 1,
} sl_comparison_result;

/**
 * compare two values and returns sl_compare_result
 */
#define sl_compare(A, B) ((A) == (B) ? sl_same : ((A) > (B) ? sl_descending : sl_ascending))

/**
 * char* is used as symbole in sylva, used for identify a func or a member variable
 *
 * symbol should not be created dynamically
 */
typedef char *const sl_symbol;

/**
 * check if two symbols are same
 */
#define sl_symbol_equals(A, B) (strcmp((A), (B)) == 0)

_END_STD_C

#endif // _SYLVA_DEFINE_H_
