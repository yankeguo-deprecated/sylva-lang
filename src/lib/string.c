//
//  sylva-string.c
//  sylva
//
//  Created by Yanke Guo on 2016/9/21.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#define __SYLVA_SOURCE__
#include "sylva/string.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sylva/string.h>

sl_string_ref sl_string_create_il(char *string, sl_index index, sl_index length) {
  assert(index >= 0);
  assert(index + length <= strlen(string));
  sl_string_ref ref = malloc(sizeof(sl_string));
  ref->length = length;
  ref->string = malloc(sizeof(char) * (ref->length + 1));
  memcpy(ref->string, &string[index], sizeof(char) * length);
  ref->string[length] = 0;
  return ref;
}

sl_string_ref sl_string_create_l(char *string, sl_index length) {
  return sl_string_create_il(string, 0, length);
}

sl_string_ref sl_string_create(char *string) {
  return sl_string_create_l(string, strlen(string));
}

sl_index sl_string_seek_no_blank(sl_string_ref string, sl_index start) {
  for (sl_index i = start; i < string->length; i++) {
    if (!isblank(string->string[i])) {
      return i;
    }
  }
  return sl_index_not_found;
}

sl_index sl_string_seek_blank(sl_string_ref string, sl_index start) {
  for (sl_index i = start; i < string->length; i++) {
    if (isblank(string->string[i])) {
      return i;
    }
  }
  return sl_index_not_found;
}

sl_index sl_string_seek_new_line(sl_string_ref string, sl_index start) {
  for (sl_index i = start; i < string->length; i++) {
    if (string->string[i] == '\n' || string->string[i] == '\r') {
      return i;
    }
  }
  return sl_index_not_found;
}

sl_index sl_string_seek_id(sl_string_ref string, sl_index start) {
  for (sl_index i = start; i < string->length; i++) {
    if (!isalnum(string->string[i]) && string->string[i] != '_') {
      return i;
    }
  }
  return sl_index_not_found;
}

void sl_string_destroy(sl_string_ref string) {
  if (string == NULL)
    return;
  free(string->string);
  free(string);
}
