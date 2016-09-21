//
//  sylva-lexer.h
//  sylva
//
//  Created by Yanke Guo on 2016/9/20.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#ifndef sylva_lexer_h
#define sylva_lexer_h

#include "sylva-platform.h"
#include "sylva-token.h"

typedef enum {
  SLexerErrorOK,
  SLexerErrorCommon
} SLexerError;

typedef unsigned long SLexerPosition;

typedef struct {
  SStringRef source;
  SLexerPosition position;
} SLexer;

typedef SLexer* SLexerRef;

/**
 Create a SLexer from source
 
 @param source the source code will be parsed
 */
SYLVA_EXPORT SLexerRef SLexerCreate(SStringRef source);

/**
 Get the next token from source
 
 @param lexer the lexer in use
 @return token next token
 */
SYLVA_EXPORT STokenRef SLexerGetNextToken(SLexerRef lexer);

/**
 Reset the SLexer to initial state
 */
SYLVA_EXPORT void SLexerReset(SLexerRef lexer);

/**
 Destroy the SLexer
 
 @param lexer the lexer to destroy
 */
SYLVA_EXPORT void SLexerDestroy(SLexerRef lexer);

#endif
