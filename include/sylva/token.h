//
//  sylva-token.h
//  sylva
//
//  Created by Yanke Guo on 2016/9/21.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#ifndef _SYLVA_TOKEN_H_
#define _SYLVA_TOKEN_H_

#include "sylva/define.h"
#include "sylva/string.h"

#include <stdio.h>

__CPP_DECL_START

typedef enum {

  sl_token_eof = -1,

  //////////////// Common ///////////////

  /**
   sharp started comment
   */
      sl_token_comment,

  /**
   * '`' started line will be considered as inline C code
   */
      sl_token_inline_c,

  /**
   Identifier, such as class name, method name, variable name, etc
   */
      sl_token_id,

  /**
   keyword `var`
   */
      sl_token_var,

  /**
   * keyword `weak`
   */
      sl_token_weak,

  /**
   mark `:`
   */
      sl_token_colon,

  /**
   mark `,`
   */
      sl_token_comma,

  /**
   keyword `end`
   */
      sl_token_end,

  /**
   line break
   */
      sl_token_new_line,

  /**
   semicolon act as an inline line break
   */
      sl_token_semicolon,

  ////////////// Class / Module ///////////////

  /**
   keyword `class` for class declaration
   */
      sl_token_class,

  /**
   keyword `module` for module declaration
   */
      sl_token_module,

  /**
   keyword `native` for native C binding
   */
      sl_token_native,

  /**
   keyword `require` for module integration method check
   */
      sl_token_require,

  /**
   keyword `include` for module integration
   */
      sl_token_include,

  /**
   keyword `static` for static variables / methods
   */
      sl_token_static,

  /**
   keyword `func` for method declaration
   */
      sl_token_function,

  /////////////// Instant Values ///////////

  /**
   keyword `super` for super class
   */
      sl_token_super,

  /**
   keyword `self`
   */
      sl_token_self,

  /**
   keyword `nil`
   */
      sl_token_nil,

  /**
   keyword `true` for boolean
   */
      sl_token_true,

  /**
   keyword `false` for boolean
   */
      sl_token_false,

  /**
   instant integer like `10890223`
   */
      sl_token_integer,

  /**
   instant float like `10.223`
   */
      sl_token_float,

  /**
   instant string like `"string"`
   */
      sl_token_string,

  //////////////// Operator //////////////

  /**
   operator `+`
   */
      sl_token_plus,

  /**
   operator `-`
   */
      sl_token_minus,

  /**
   operator `*`
   */
      sl_token_multiply,

  /**
   operator `/`
   */
      sl_token_divide,

  /**
   operator `%`
   */
      sl_token_mod,

  /**
   operator `=`
   */
      sl_token_eq,

  /**
   operator `==`
   */
      sl_token_eq_to,

  /**
   * operator `===`
   */
      sl_token_identical_to,

  /**
   opertaor `<`
   */
      sl_token_lt,

  /**
   operator `>`
   */
      sl_token_gt,

  /**
   * operator `<>
   */
      sl_token_compare,

  /**
   operator `>=`
   */
      sl_token_gt_eq,

  /**
   operator `<=`
   */
      sl_token_lt_eq,

  /**
   operator `!`
   */
      sl_token_not,

  /**
   operator `!=`
   */
      sl_token_not_eq,

  /**
   operator `->`
   */
      sl_token_arrow,

  /**
   bit operator `|`
   */
      sl_token_bit_or,

  /**
   bit operator `&`
   */
      sl_token_bit_and,

  /**
   * bit operator `~`
   */
      sl_token_tilde,

  /**
   * bit operator `^`
   */
      sl_token_bit_xor,

  /**
   operator `||`
   */
      sl_token_or,

  /**
   operator `&&`
   */
      sl_token_and,

  /**
   bit operator `<<`
   */
      sl_token_bit_lshift,

  /**
   bit operator `>>`
   */
      sl_token_bit_rshift,

  /**
   keyword `?`
   */
      sl_token_question,

  /**
   mark `()`
   */
      sl_token_paren_l, sl_token_paren_r,

  /**
   mark `[]`
   */
      sl_token_bracket_l, sl_token_bracket_r,

  /**
   mark `{}`
   */
      sl_token_brace_l, sl_token_brace_r,

  //////////////// Flow  /////////////////

  /**
   keyword `if`
   */
      sl_token_if,

  /**
   keyword `else`
   */
      sl_token_else,

  /**
   keyword `unless`
   */
      sl_token_unless,

  /**
   keyword `while`
   */
      sl_token_while,

  /**
   keyword `break`
   */
      sl_token_break,

  /**
   keyword `for`
   */
      sl_token_for,

  /**
   keyword `in`
   */
      sl_token_in,

  /////////////// Expression /////////////

  /**
   syntax `instance.method`
   */
      sl_token_dot,

  /**
   keyword `return`
   */
      sl_token_return,

} sl_token_type;

typedef enum {
  sl_sema_none,
  sl_sema_integer,
  sl_sema_float,
  sl_sema_string
} sl_sema_type;

/**
 Get SSemaType from STokenType

 @param tokenType STokenType
 @return SSemaType
 */
SYLVA_EXTERN sl_sema_type sl_sema_type_from_token_type(sl_token_type token_type);

/**
 * Get the name of a STokenType
 *
 * @param tokenType STokenType
 * @return name
 */
SYLVA_EXTERN char *const sl_token_get_name(sl_token_type token_type);

typedef struct {
  union {
    sl_integer integerValue;
    sl_float floatValue;
    sl_string_ref stringValue;
  };
  sl_sema_type semaType;
  sl_token_type type;
} sl_token;

typedef sl_token *sl_token_ref;

SYLVA_EXTERN void sl_token_print(FILE *stream, sl_token_ref token);

/**
 Create a STokenRef with None sema
 */
SYLVA_EXTERN sl_token_ref sl_token_create(sl_token_type type);

/**
 Create a STokenRef with Integer sema
 */
SYLVA_EXTERN sl_token_ref sl_token_create_integer(sl_token_type type, sl_integer integer);

/**
 Create a STokenRef with Float sema
 */
SYLVA_EXTERN sl_token_ref sl_token_create_float(sl_token_type type, sl_float f);

/**
 Create a STokenRef with SString sema
 */
SYLVA_EXTERN sl_token_ref sl_token_create_string(sl_token_type type, char *string);

SYLVA_EXTERN sl_token_ref sl_token_create_string_l(sl_token_type type, char *string, sl_index length);

SYLVA_EXTERN sl_token_ref sl_token_create_string_il(sl_token_type type, char *string, sl_index start, sl_index length);

/**
 Destroy a STokenRef, free SString if existed
 */
SYLVA_EXTERN void sl_token_destroy(sl_token_ref token);

__CPP_DECL_END

#endif // _SYLVA_TOKEN_H_
