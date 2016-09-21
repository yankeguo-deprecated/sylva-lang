//
//  sylva-lexer.c
//  sylva
//
//  Created by Yanke Guo on 2016/9/20.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#define __SYLVA_SOURCE__

#include "sylva/string.h"
#include "sylva/lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <sylva/lexer.h>
#include <sylva/string.h>

SLexerRef SLexerCreate(SStringRef source) {
  SLexerRef lexer = malloc(sizeof(SLexer));
  lexer->source = source;
  lexer->index = 0;
  return lexer;
};

STokenRef SLexerGetNextToken(SLexerRef lexer, SLexerError *err, SIndex *errIndex) {
  SIndex idx = lexer->index;

  //  Check EOF
  if (idx >= lexer->source->length) {
    return STokenCreate(STokenEOF);
  }

  //  Seek first non blank
  idx = SStringSeekNoBlank(lexer->source, lexer->index);
  if (idx == SIndexNotFound) {
    if (err) {
      *err = SLexerErrorOK;
    }
    lexer->index = lexer->source->length;
    return STokenCreate(STokenEOF);
  }

  //  Get first letter
  char first = lexer->source->string[idx];

  //  Check comment
  if (first == '#') {
    if (err) {
      *err = SLexerErrorOK;
    }
    lexer->index = lexer->source->length;
    return STokenCreateStringIL(STokenComment, lexer->source->string, idx + 1, lexer->source->length - idx - 1);
  }

  return STokenCreate(STokenEOF);
}

void SLexerReset(SLexerRef lexer) {
  lexer->index = 0;
}

void SLexerDestroy(SLexerRef lexer) {
  free(lexer);
}
