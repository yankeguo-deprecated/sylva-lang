//
//  sylva-lexer.h
//  sylva
//
//  Created by Yanke Guo on 2016/9/20.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#ifndef _SYLVA_LEXER_H_
#define _SYLVA_LEXER_H_

#include "sylva/define.h"
#include "sylva/token.h"

__CPP_DECL_START

typedef enum {
  sl_lexer_error_ok,
  sl_lexer_error_invalid_sequence,
  sl_lexer_error_invalid_id,
  sl_lexer_error_found_new_line, // new line (\r or \n) should be handled by sl_parser not sl_lexer
} sl_lexer_error;

SYLVA_EXTERN char *const sl_lexer_error_get_name(sl_lexer_error error);

typedef struct {
  sl_string_ref source;
  sl_index index;
} sl_lexer;

typedef sl_lexer *sl_lexer_ref;

/**
 Create a SLexer from source

 @param source the source code will be parsed
 */
SYLVA_EXTERN sl_lexer_ref sl_lexer_create(sl_string_ref source);

/**
 Get the next token from source

 @param lexer the lexer in use
 @return token next token
 */
SYLVA_EXTERN sl_token_ref sl_lexer_next_token(sl_lexer_ref lexer, sl_lexer_error *err, sl_index *errIndex);

/**
 Reset the SLexer to initial state
 */
SYLVA_EXTERN void sl_lexer_reset(sl_lexer_ref lexer);

/**
 Destroy the SLexer

 @param lexer the lexer to destroy
 */
SYLVA_EXTERN void sl_lexer_destroy(sl_lexer_ref lexer);

__CPP_DECL_END

#endif // _SYLVA_LEXER_H_
