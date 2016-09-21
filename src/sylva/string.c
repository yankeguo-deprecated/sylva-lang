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

SStringRef SStringCreateIL(char *string, SIndex index, SIndex length) {
  assert(index >= 0);
  assert(index + length <= strlen(string));
  SStringRef ref = malloc(sizeof(SString));
  ref->length = length;
  ref->string = malloc(sizeof(char) * (ref->length + 1));
  memcpy(ref->string, &string[index], sizeof(char) * length);
  ref->string[length] = 0;
  return ref;
}

SStringRef SStringCreateL(char *string, SIndex length) {
  return SStringCreateIL(string, 0, length);
}

SStringRef SStringCreate(char *string) {
  return SStringCreateL(string, strlen(string));
}

SIndex SStringSeekNoBlank(SStringRef string, SIndex start) {
  for (SIndex i = start; i < string->length; i++) {
    if (!isblank(string->string[i])) {
      return i;
    }
  }
  return SIndexNotFound;
}

void SStringDestroy(SStringRef string) {
  if (string == NULL)
    return;
  free(string->string);
  free(string);
}
