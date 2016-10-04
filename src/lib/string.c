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

SStringRef SStringCreateIL(char *string, sl_index index, sl_index length) {
  assert(index >= 0);
  assert(index + length <= strlen(string));
  SStringRef ref = malloc(sizeof(SString));
  ref->length = length;
  ref->string = malloc(sizeof(char) * (ref->length + 1));
  memcpy(ref->string, &string[index], sizeof(char) * length);
  ref->string[length] = 0;
  return ref;
}

SStringRef SStringCreateL(char *string, sl_index length) {
  return SStringCreateIL(string, 0, length);
}

SStringRef SStringCreate(char *string) {
  return SStringCreateL(string, strlen(string));
}

sl_index SStringSeekNoBlank(SStringRef string, sl_index start) {
  for (sl_index i = start; i < string->length; i++) {
    if (!isblank(string->string[i])) {
      return i;
    }
  }
  return sl_index_not_found;
}

sl_index SStringSeekBlank(SStringRef string, sl_index start) {
  for (sl_index i = start; i < string->length; i++) {
    if (isblank(string->string[i])) {
      return i;
    }
  }
  return sl_index_not_found;
}

sl_index SStringSeekNewLine(SStringRef string, sl_index start) {
  for (sl_index i = start; i < string->length; i++) {
    if (string->string[i] == '\n' || string->string[i] == '\r') {
      return i;
    }
  }
  return sl_index_not_found;
}

sl_index SStringSeekNoAlphaNumberUnderscore(SStringRef string, sl_index start) {
  for (sl_index i = start; i < string->length; i++) {
    if (!isalnum(string->string[i]) && string->string[i] != '_') {
      return i;
    }
  }
  return sl_index_not_found;
}

void SStringDestroy(SStringRef string) {
  if (string == NULL)
    return;
  free(string->string);
  free(string);
}
