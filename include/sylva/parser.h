//
// Created by Yanke Guo on 2016/9/22.
//

#ifndef _SYLVA_PARSER_H_
#define _SYLVA_PARSER_H_

#include "sylva/platform.h"
#include "sylva/string.h"

__CPP_DECL_START

typedef struct {
  SString *source;
} SParser;

typedef SParser *SParserRef;

SYLVA_EXTERN SParserRef SParserCreate(char *src);

SYLVA_EXTERN void SParserDestroy(SParserRef parser);

__CPP_DECL_END

#endif // _SYLVA_PARSER_H_
