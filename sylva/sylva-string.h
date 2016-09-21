//
//  sylva-string.h
//  sylva
//
//  Created by Yanke Guo on 2016/9/21.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#ifndef sylva_string_h
#define sylva_string_h

#include "sylva-platform.h"

typedef struct {
  char *string;
  unsigned long length;
} SString;

typedef SString* SStringRef;

SYLVA_EXPORT SStringRef SStringCreate(char *string);

SYLVA_EXPORT SStringRef SStringCreateWithLength(char *string, int length);

SYLVA_EXPORT void SStringDestroy(SStringRef string);

#endif /* sylva_string_h */
