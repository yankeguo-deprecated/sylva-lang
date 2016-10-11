//
//  lexer.h
//  sylva
//
//  Created by Ryan Guo<ryan@islandzero.net> on 2016/9/20.
//

#ifndef _SYLVA_LEXER_H_
#define _SYLVA_LEXER_H_

#include "sylva/define.h"
#include "sylva/token.h"

_SL_BEGIN_STD_C

/**
 * structure for lexer
 *
 * lexer works in line-level, it tokenize a line of sylva-lang source code
 */
typedef struct {
  /**
   * source code in sl_string
   */
  sl_string_ref source;
  /**
   * current lexer position
   */
  sl_index index;
} sl_lexer;

typedef sl_lexer *sl_lexer_ref;

/**
 create a sl_lexer from source

 @param source the source code will be parsed
 */
sl_lexer_ref sl_lexer_create(sl_string_ref source);

/**
 get the next token from source

 @param lexer the lexer in use
 @return token next token
 */
sl_token_ref sl_lexer_next_token(sl_lexer_ref lexer);

/**
 reset the sl_lexer to initial state
 */
void sl_lexer_reset(sl_lexer_ref lexer);

/**
 destroy the sl_lexer

 @param lexer the lexer to destroy
 */
void sl_lexer_destroy(sl_lexer_ref lexer);

_SL_END_STD_C

#endif // _SYLVA_LEXER_H_
