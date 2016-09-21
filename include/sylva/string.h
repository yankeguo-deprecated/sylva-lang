//
//  sylva-string.h
//  sylva
//
//  Created by Yanke Guo on 2016/9/21.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#ifndef _SYLVA_STRING_H_
#define _SYLVA_STRING_H_

#include "sylva/platform.h"

__CPP_DECL_START

typedef unsigned long SStringIndex;

typedef struct {
  char *string;
  SStringIndex length;
} SString;

typedef SString* SStringRef;

SYLVA_EXPORT SStringRef SStringCreate(char *string);

SYLVA_EXPORT SStringRef SStringCreateWithLength(char *string, int length);

SYLVA_EXPORT void SStringDestroy(SStringRef string);

__CPP_DECL_END

#endif // _SYLVA_STRING_H_
