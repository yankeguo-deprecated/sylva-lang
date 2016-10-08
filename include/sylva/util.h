//
//  util.h
//  sylva
//
//  Created by Ryan Guo<ryan@islandzero.net> on 2016/9/21.
//

#ifndef _SYLVA_UTIL_H_
#define _SYLVA_UTIL_H_

#include "sylva/define.h"

__BEGIN_STD_C

/**
 * sl_string preserves a copy of char* and its length, provide methods for char searching
 *
 * this struct is designed for sylva compiler only, String in sylva is implemented by sl_class SYLVA_C_String
 */
typedef struct {
  /**
   * internal char*
   */
  char *string;
  /**
   * length of this string
   */
  sl_index length;
} sl_string;

/**
 * pointer type of sl_string
 */
typedef sl_string *sl_string_ref;

/**
 * create a sl_string by copying char*
 *
 * @param string
 * @return allocated sl_string_ref
 */
sl_string_ref sl_string_create(char *string);

/**
 * create a sl_string by copying char with speicifed index and length
 * @param string char* to copy
 * @param index start position for copy
 * @param length length of chars for copy
 * @return allocated sl_string_ref
 */
sl_string_ref sl_string_create_il(char *string, sl_index index, sl_index length);

/**
 * @see sl_string_create_il
 *
 * @param string
 * @param length
 * @return
 */
sl_string_ref sl_string_create_l(char *string, sl_index length);

/**
 * find the first not blank char from the speicifed position
 * @param string sl_string_ref
 * @param start position from where searching started
 * @return index, sl_index_not_found if not found
 */
sl_index sl_string_seek_no_blank(sl_string_ref string, sl_index start);

/**
 * find the blank char
 * @see sl_string_seek_no_blank
 * @param string
 * @param start
 * @return
 */
sl_index sl_string_seek_blank(sl_string_ref string, sl_index start);

/**
 * find the '\n' or '\r\n' sequence
 * @see sl_string_seek_no_blank
 * @param string
 * @param start
 * @return
 */
sl_index sl_string_seek_new_line(sl_string_ref string, sl_index start);

/**
 * find the 1-0, a-z, A-Z and _ sequence
 * @see sl_string_seek_no_blank
 * @param string
 * @param start
 * @return
 */
sl_index sl_string_seek_id(sl_string_ref string, sl_index start);

char sl_string_get_last(sl_string_ref string);

/**
 * append string
 * @param string
 * @param src
 */
void sl_string_append(sl_string_ref string, char *src);

/**
 * destroy a previously created sl_string_ref
 * @param string
 */
void sl_string_destroy(sl_string_ref string);

/**
 * sl_array provides auto-expand array facility for pointers
 *
 * sl_array is used for sylva compiler only, Array in sylva-lang is provided by sl_class SYLVA_C_Array
 */

typedef void(*sl_array_value_deallocator)(void *value);

typedef struct sl_array_t {
  void **values;
  sl_index capacity;
  sl_index count;
  sl_array_value_deallocator value_deallocator;
} sl_array;

typedef sl_array *sl_array_ref;

sl_array_ref sl_array_create(sl_index capacity);

void sl_array_append(sl_array_ref array, void *value);

void sl_array_destroy(sl_array_ref array);

__END_STD_C

#endif // _SYLVA_UTIL_H_
