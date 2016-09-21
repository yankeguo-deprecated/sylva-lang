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

typedef struct {
  char *string;
  SIndex length;
} SString;

typedef SString *SStringRef;

SYLVA_EXPORT SStringRef SStringCreate(char *string);

SYLVA_EXPORT SStringRef SStringCreateIL(char *string, SIndex index, SIndex length);

SYLVA_EXPORT SStringRef SStringCreateL(char *string, SIndex length);

SYLVA_EXPORT SIndex SStringSeekNoBlank(SStringRef string, SIndex start);

SYLVA_EXPORT void SStringDestroy(SStringRef string);

__CPP_DECL_END

#endif // _SYLVA_STRING_H_
