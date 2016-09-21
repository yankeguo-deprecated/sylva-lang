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

SYLVA_EXPORT SStringRef SStringCreateWithLength(char *string, int length) {
  SStringRef ref = malloc(sizeof(SString));
  ref->length = length;
  ref->string = malloc(sizeof(char) * (ref->length + 1));
  memcpy(ref->string, string, sizeof(char) * length);
  ref->string[length] = 0;
  return ref;
}

SStringRef SStringCreate(char *string) {
  SStringRef ref = malloc(sizeof(SString));
  ref->length = strlen(string);
  ref->string = malloc(sizeof(char) * (ref->length + 1));
  strcpy(ref->string, string);
  return ref;
}

void SStringDestroy(SStringRef string) {
  if (string == NULL) return;
  free(string->string);
  free(string);
}
