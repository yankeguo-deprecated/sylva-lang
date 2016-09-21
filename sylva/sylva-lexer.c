//
//  sylva-lexer.c
//  sylva
//
//  Created by Yanke Guo on 2016/9/20.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#define __SYLVA_SOURCE__
#include "sylva-lexer.h"

#include <stdlib.h>

SLexerRef SLexerCreate(SStringRef source) {
  SLexerRef lexer = malloc(sizeof(SLexer));
  lexer->source = source;
  lexer->position = 0;
  return lexer;
};

STokenRef SLexerGetNextToken(SLexerRef lexer) {
  if (lexer->position >= lexer->source->length) {
    return STokenCreate(STokenEOF);
  }
  return STokenCreate(STokenEOF);
}

void SLexerReset(SLexerRef lexer) {
  lexer->position = 0;
}

void SLexerDestroy(SLexerRef lexer) {
  free(lexer);
}
