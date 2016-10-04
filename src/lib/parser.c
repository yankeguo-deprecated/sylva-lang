//
// Created by Yanke Guo on 2016/9/22.
//

#define __SYLVA_SOURCE__
#include "sylva/parser.h"

#include <stdlib.h>

sl_parser_ref sl_parser_create(char *src) {
  sl_parser_ref parser = malloc(sizeof(sl_parser));
  parser->source = sl_string_create(src);
  return parser;
}

void sl_parser_destroy(sl_parser_ref parser) {
  sl_string_destroy(parser->source);
  free(parser);
}
