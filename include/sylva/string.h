//
//  sylva-string.h
//  sylva
//
//  Created by Yanke Guo on 2016/9/21.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#ifndef _SYLVA_STRING_H_
#define _SYLVA_STRING_H_

#include "sylva/define.h"

__CPP_DECL_START

typedef struct {
  char *string;
  sl_index length;
} SString;

typedef SString *SStringRef;

SYLVA_EXTERN SStringRef SStringCreate(char *string);

SYLVA_EXTERN SStringRef SStringCreateIL(char *string, sl_index index, sl_index length);

SYLVA_EXTERN SStringRef SStringCreateL(char *string, sl_index length);

SYLVA_EXTERN sl_index SStringSeekNoBlank(SStringRef string, sl_index start);

SYLVA_EXTERN sl_index SStringSeekBlank(SStringRef string, sl_index start);

SYLVA_EXTERN sl_index SStringSeekNewLine(SStringRef string, sl_index start);

SYLVA_EXTERN sl_index SStringSeekNoAlphaNumberUnderscore(SStringRef string, sl_index start);

SYLVA_EXTERN void SStringDestroy(SStringRef string);

__CPP_DECL_END

#endif // _SYLVA_STRING_H_
