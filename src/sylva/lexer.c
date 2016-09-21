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

SLexerRef SLexerCreate(SStringRef source) {
  SLexerRef lexer = malloc(sizeof(SLexer));
  lexer->source = source;
  lexer->index = 0;
  return lexer;
};

STokenRef SLexerGetNextToken(SLexerRef lexer, SLexerError *err, SStringIndex *errIndex) {
  //  Assign local variables
  SStringIndex idx  = lexer->index;
  char*        src  = lexer->source->string;
  SStringIndex max  = lexer->source->length;
  
  //  Check EOF
  if (idx >= max) return STokenCreate(STokenEOF);
  
  //  Result
  STokenRef token;
  
  //  The Grand Loop
  for (;;) {
    //  Skipping Leading Blanks
    if (isblank(src[idx])) {
      goto end;
    } else {
    }
    
  end:
    //  try move idx to next
    if (idx + 1 >= max) {
      token = STokenCreate(STokenEOF);
      break;
    } else {
      idx ++;
    }
  }
  
  //  Update index and return
  lexer->index = idx;
  
  return token;
}

void SLexerReset(SLexerRef lexer) {
  lexer->index = 0;
}

void SLexerDestroy(SLexerRef lexer) {
  free(lexer);
}
