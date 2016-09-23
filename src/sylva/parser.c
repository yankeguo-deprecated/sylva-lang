//
// Created by Yanke Guo on 2016/9/22.
//

#define __SYLVA_SOURCE__
#include "sylva/parser.h"

#include <stdlib.h>

SParserRef SParserCreate(char *src) {
  SParserRef parser = malloc(sizeof(SParser));
  parser->source = SStringCreate(src);
  return parser;
}

void SParserDestroy(SParserRef parser) {
  SStringDestroy(parser->source);
  free(parser);
}
