//
//  define.h
//  sylva
//
//  Created by Ryan Guo<ryan@islandzero.net> on 2016/9/20.
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
 * Common Macros
 **********************************************************************************************************************/

#ifdef __cplusplus
#define _SL_BEGIN_STD_C extern "C" {
#define _SL_END_STD_C }
#else
#define _SL_BEGIN_STD_C
#define _SL_END_STD_C
#endif

#define _sl_unused __attribute__((unused))

/***********************************************************************************************************************
 * Constants
 **********************************************************************************************************************/

#define MANIFEST_FILE "manifest.yaml"
#define BUILD_DIRECTORY "build"

/***********************************************************************************************************************
 * Primitive Types
 **********************************************************************************************************************/

_SL_BEGIN_STD_C

/**
 * boolean type in sylva runtime, alias to C `bool`
 */
typedef bool sl_boolean;

/**
 * char type
 */
typedef unsigned char sl_char;

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

/***********************************************************************************************************************
 * Assert & Error
 **********************************************************************************************************************/

/**
 * sl_assert is used for debug only
 */
#ifdef DEBUG
#define sl_assert(CLAUSE, ...) if (!(CLAUSE)) { fprintf(stderr, __VA_ARGS__); abort(); }
#else
#define sl_assert(CLAUSE, ...)
#endif

/**
 * sl_error is used for raising errors and exit quickly
 */
#define sl_error(CLAUSE, ...) if (!(CLAUSE)) { fprintf(stderr, __VA_ARGS__); abort(); }

_SL_END_STD_C

#endif // _SYLVA_DEFINE_H_
