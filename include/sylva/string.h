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
} sl_string;

typedef sl_string *sl_string_ref;

SYLVA_EXTERN sl_string_ref sl_string_create(char *string);

SYLVA_EXTERN sl_string_ref sl_string_create_il(char *string, sl_index index, sl_index length);

SYLVA_EXTERN sl_string_ref sl_string_create_l(char *string, sl_index length);

SYLVA_EXTERN sl_index sl_string_seek_no_blank(sl_string_ref string, sl_index start);

SYLVA_EXTERN sl_index sl_string_seek_blank(sl_string_ref string, sl_index start);

SYLVA_EXTERN sl_index sl_string_seek_new_line(sl_string_ref string, sl_index start);

SYLVA_EXTERN sl_index sl_string_seek_id(sl_string_ref string, sl_index start);

SYLVA_EXTERN void sl_string_destroy(sl_string_ref string);

__CPP_DECL_END

#endif // _SYLVA_STRING_H_
