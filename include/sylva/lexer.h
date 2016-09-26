//
//  sylva-lexer.h
//  sylva
//
//  Created by Yanke Guo on 2016/9/20.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#ifndef _SYLVA_LEXER_H_
#define _SYLVA_LEXER_H_

#include "sylva/platform.h"
#include "sylva/token.h"

__CPP_DECL_START

typedef enum {
  SLexerErrorOK,
  SLexerErrorInvalidSequence,
  SLexerErrorInvalidId,
  SLexerErrorFoundNewLine, // new line (\r or \n) should be handled by SParser not SLexer
} SLexerError;

SYLVA_EXTERN char *const SLexerErrorGetName(SLexerError error);

typedef struct {
  SStringRef source;
  SIndex index;
} SLexer;

typedef SLexer *SLexerRef;

/**
 Create a SLexer from source
 
 @param source the source code will be parsed
 */
SYLVA_EXTERN SLexerRef SLexerCreate(SStringRef source);

/**
 Get the next token from source
 
 @param lexer the lexer in use
 @return token next token
 */
SYLVA_EXTERN STokenRef SLexerGetNextToken(SLexerRef lexer, SLexerError *err, SIndex *errIndex);

/**
 Reset the SLexer to initial state
 */
SYLVA_EXTERN void SLexerReset(SLexerRef lexer);

/**
 Destroy the SLexer
 
 @param lexer the lexer to destroy
 */
SYLVA_EXTERN void SLexerDestroy(SLexerRef lexer);

__CPP_DECL_END

#endif // _SYLVA_LEXER_H_
