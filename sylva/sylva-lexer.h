//
//  sylva-lexer.h
//  sylva
//
//  Created by Yanke Guo on 2016/9/20.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#ifndef sylva_lexer_h
#define sylva_lexer_h

#include <stdio.h>

typedef enum {
  sylva_lexer_error_ok = 0,
  sylva_lexer_error_common,
	sylva_lexer_error_no_capacity,
} sylva_lexer_error;

typedef enum sylva_token_type {
  
  ////////////////// Import ////////////////////
  
  /*
   syntax `import folder/file`
   */
  sylva_token_import,
  
  /*
   syntax `internal import folder/file`
   */
  sylva_token_internal,
  
  //////////////// Common ///////////////
  
  /**
   any identifier, such as class name, method name, variable name, etc
   */
  sylva_token_id,
  
  /**
   syntax `var what`
   */
  sylva_token_var,
  
  /**
   syntax `class SomeClass : AnotherClass`
   */
  sylva_token_colon,
  
  /**
   syntax `end`
   */
  sylva_token_end,
  
  /**
   line break for each line
   */
  sylva_token_line_break,
  
  /**
   semicolon act as an inline line break
   */
  sylva_token_semicolon,
  
  /**
   the operator `/`
   */
  sylva_token_slash,
  
  ////////////// Class / Module ///////////////
  
  /*
   syntax `class SomeClass`
   */
  sylva_token_class,
  
  /*
   syntax `module SomeModule`
   */
  sylva_token_module,
  
  /*
   syntax `require func what()`
   */
  sylva_token_require,
  
  /*
   syntax `include SomeModule`
   */
  sylva_token_include,
  
  /*
   syntax `static func what()`
   */
  sylva_token_static,
  
  /*
   syntax `func what()`
   */
  sylva_token_func,
  
  /////////////// Instant Variable ////////
  
  /**
   syntax `super`
   */
  sylva_token_super,
  
  /**
   syntax `self`
   */
  sylva_token_self,
  
  /**
   syntax `nil`
   */
  sylva_token_nil,
  
  /**
   syntax `true`
   */
  sylva_token_true,
  
  /**
   syntax `false`
   */
  sylva_token_false,
  
  /**
   syntax `10890223`
   */
  sylva_token_integer,
  
  /**
   syntax `10.223`
   */
  sylva_token_float,
  
  /**
   syntax `"string"`
   */
  sylva_token_string,
  
  //////////////// Operator //////////////
  
  /**
   syntax `+`
   */
  sylva_token_plus,
  
  /**
   syntax `-`
   */
  sylva_token_minus,
  
  /**
   syntax `*`
   */
  sylva_token_times,
  
  /**
   syntax `/`
   */
  sylva_token_div = sylva_token_slash,
  
  /**
   syntax `%`
   */
  sylva_token_mod,
  
  /**
   syntax `=`
   */
  sylva_token_eq,
  
  /**
   syntax `<`
   */
  sylva_token_lt,
  
  /**
   syntax `>`
   */
  sylva_token_gt,
  
  /**
   syntax `>=`
   */
  sylva_token_gteq,
  
  /**
   syntax `<=`
   */
  sylva_token_lteq,
  
  /**
   syntax `!`
   */
  sylva_token_not,
  
  /**
   syntax `!=`
   */
  sylva_token_neq,
  
  /**
   syntax `->`
   */
  sylva_token_arrow,
  
  /**
   syntax `|`
   */
  sylva_token_bit_or,
  
  /**
   syntax `&`
   */
  sylva_token_bit_and,
  
  /**
   syntax `||`
   */
  sylva_token_or,
  
  /**
   syntax `&&`
   */
  sylva_token_and,
  
  /**
   syntax `<<`
   */
  sylva_token_bit_shift,
  
  /**
   syntax `>>`
   */
  sylva_token_bit_unshift,
  
  /**
   syntax `?`
   */
  sylva_token_question,
  
  /**
   syntax `()`
   */
  sylva_token_paren_l,
  sylva_token_paren_r,
  
  /**
   syntax `[]`
   */
  sylva_token_bracket_l,
  sylva_token_bracket_r,
  
  /**
   syntax `{}`
   */
  sylva_token_brace_l,
  sylva_token_brace_r,
  
  //////////////// Flow  /////////////////
  
  /**
   syntax `if`
   */
  sylva_token_if,
  
  /**
   syntax `else`
   */
  sylva_token_else,
  
  /**
   syntax `unless`
   */
  sylva_token_unless,
  
  /**
   syntax `while`
   */
  sylva_token_while,
  
  /**
   syntax `break`
   */
  sylva_token_break,
  
  /**
   syntax `for`
   */
  sylva_token_for,
  
  /**
   syntax `in`
   */
  sylva_token_in,
  
  /////////////// Expression /////////////
  
  /**
   syntax `instance.method`
   */
  sylva_token_dot,
  
  /**
   syntax `return ?`
   */
  sylva_token_return,
  
} sylva_token_type;

typedef union sylva_token_value {
  sylva_lex_integer integer_value;
  sylva_lex_float float_value;
  sylva_lex_string string_value;
} sylva_token_value;

typedef struct sylva_token {
  sylva_token_value value;
  sylva_token_type  type;
} sylva_token;

typedef struct sylva_lexer {
	int tokens_capacity;
	int tokens_count;
	sylva_token *tokens;
} sylva_lexer;

typedef sylva_lexer *sylva_lexer_ref;

sylva_lexer_ref sylva_lexer_create(int tokens_len);

/**
 Scan source string and append tokens
 
 @param lexer the lexer in use
 @param source source code
 @param source_len source code length
 @param err_pos if error occured, passing out where error occured
 
 @return error code
 */
sylva_lexer_error sylva_lexer_scan(sylva_lexer_ref lexer, char *source, int source_len, int *err_pos);

/**
 Clear stored tokens
 
 @param lexer the lexer in use
 */
void sylva_lexer_clear(sylva_lexer_ref lexer);

/**
 Deinit the lexer, cannot be used any more
 
 @param lexer the lexer in use
 */
void sylva_lexer_destroy(sylva_lexer_ref lexer);

#endif /* sylva_lexer_h */
