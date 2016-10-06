//
// Created by Yanke Guo on 2016/9/22.
//

#ifndef _SYLVA_PARSER_H_
#define _SYLVA_PARSER_H_

#include "sylva/define.h"
#include "sylva/string.h"
#include "token.h"

#include <yaml.h>

#include <stdio.h>
#include <stdlib.h>

__BEGIN_STD_C

typedef struct {
  char *file_name;
  FILE *file;
  char *line_buf;
  int line_buf_size;
} sl_parser;

typedef sl_parser *sl_parser_ref;

sl_parser_ref sl_parser_create(char *file_name);

void sl_parser_print_scan_result(sl_parser_ref parser, FILE *output);

void sl_parser_destroy(sl_parser_ref parser);

__END_STD_C

#endif // _SYLVA_PARSER_H_
