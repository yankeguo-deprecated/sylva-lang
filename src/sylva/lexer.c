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
#include <string.h>
#include <sylva/string.h>
#include <sylva/lexer.h>

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

  //  Sharp started (comment)
  if (first == '#') {
    if (err) {
      *err = SLexerErrorOK;
    }
    lexer->index = lexer->source->length;
    return STokenCreateStringIL(STokenComment, lexer->source->string, idx + 1, lexer->source->length - idx - 1);
  }

  //  Alphabet started
  if (isalpha(first) || first == '_') {
    SIndex endIdx = SStringSeekNoAlphaNumberUnderscore(lexer->source, idx);
    if (endIdx == SIndexNotFound) {
      endIdx = lexer->source->length;
    }
    SStringRef sema = SStringCreateIL(lexer->source->string, idx, endIdx - idx);
    //  Check keywords
    STokenRef token = NULL;
    if (strcmp(sema->string, "class") == 0) {
      token = STokenCreate(STokenClass);
    } else if (strcmp(sema->string, "var") == 0) {
      token = STokenCreate(STokenVar);
    } else if (strcmp(sema->string, "end") == 0) {
      token = STokenCreate(STokenEnd);
    } else if (strcmp(sema->string, "module") == 0) {
      token = STokenCreate(STokenModule);
    } else if (strcmp(sema->string, "native") == 0) {
      token = STokenCreate(STokenNative);
    } else if (strcmp(sema->string, "require") == 0) {
      token = STokenCreate(STokenRequire);
    } else if (strcmp(sema->string, "include") == 0) {
      token = STokenCreate(STokenInclude);
    } else if (strcmp(sema->string, "static") == 0) {
      token = STokenCreate(STokenStatic);
    } else if (strcmp(sema->string, "func") == 0) {
      token = STokenCreate(STokenFunc);
    } else if (strcmp(sema->string, "super") == 0) {
      token = STokenCreate(STokenSuper);
    } else if (strcmp(sema->string, "self") == 0) {
      token = STokenCreate(STokenSelf);
    } else if (strcmp(sema->string, "nil") == 0) {
      token = STokenCreate(STokenNil);
    } else if (strcmp(sema->string, "true") == 0) {
      token = STokenCreate(STokenTrue);
    } else if (strcmp(sema->string, "false") == 0) {
      token = STokenCreate(STokenFalse);
    } else if (strcmp(sema->string, "if") == 0) {
      token = STokenCreate(STokenIf);
    } else if (strcmp(sema->string, "else") == 0) {
      token = STokenCreate(STokenElse);
    } else if (strcmp(sema->string, "unless") == 0) {
      token = STokenCreate(STokenUnless);
    } else if (strcmp(sema->string, "while") == 0) {
      token = STokenCreate(STokenWhile);
    } else if (strcmp(sema->string, "break") == 0) {
      token = STokenCreate(STokenBreak);
    } else if (strcmp(sema->string, "for") == 0) {
      token = STokenCreate(STokenFor);
    } else if (strcmp(sema->string, "In") == 0) {
      token = STokenCreate(STokenIn);
    } else if (strcmp(sema->string, "return") == 0) {
      token = STokenCreate(STokenReturn);
    } else {
      //  No keyword found, make it ID
      token = STokenCreateString(STokenId, sema->string);
    }
    SStringDestroy(sema);
    if (err) {
      *err = SLexerErrorOK;
    }
    lexer->index = endIdx;
    return token;
  }

  return STokenCreate(STokenEOF);
}

void SLexerReset(SLexerRef lexer) {
  lexer->index = 0;
}

void SLexerDestroy(SLexerRef lexer) {
  free(lexer);
}
