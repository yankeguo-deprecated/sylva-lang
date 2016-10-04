//
// Created by Yanke Guo on 2016/9/22.
//

#ifndef _SYLVA_PARSER_H_
#define _SYLVA_PARSER_H_

#include "sylva/define.h"
#include "sylva/string.h"

__CPP_DECL_START

typedef struct {
  sl_string *source;
} sl_parser;

typedef sl_parser *sl_parser_ref;

SYLVA_EXTERN sl_parser_ref sl_parser_create(char *src);

SYLVA_EXTERN void sl_parser_destroy(sl_parser_ref parser);

__CPP_DECL_END

#endif // _SYLVA_PARSER_H_
