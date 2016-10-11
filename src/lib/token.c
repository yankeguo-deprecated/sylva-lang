//
//  sylva-token.c
//  sylva
//
//  Created by Yanke Guo on 2016/9/21.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#define __SYLVA_SOURCE__
#include "sylva/token.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <sylva/token.h>

int sl_scope_level_change_from_token_type(sl_token_type token_type) {
  switch (token_type) {
  case sl_token_module:
  case sl_token_class:
  case sl_token_if:
  case sl_token_for:
  case sl_token_unless:
  case sl_token_while: {
    return 1;
  }
  case sl_token_end: {
    return -1;
  }
  default: {
    return 0;
  }
  }
}

sl_scope sl_scope_from_token_type(sl_token_type token_type) {
  switch (token_type) {
  case sl_token_invalid: {
    return sl_scope_none;
  }
  case sl_token_eof:
  case sl_token_class:
  case sl_token_module: {
    return sl_scope_global;
  }
  case sl_token_inline_c: {
    return sl_scope_global | sl_scope_statement;
  }
  case sl_token_semicolon:
  case sl_token_new_line:
  case sl_token_end:
  case sl_token_colon:
  case sl_token_id:
  case sl_token_comment: {
    return sl_scope_all;
  }
  case sl_token_comma:
  case sl_token_paren_l:
  case sl_token_paren_r:
  case sl_token_var: {
    return sl_scope_no_global;
  }
  case sl_token_function:
  case sl_token_static:
  case sl_token_include:
  case sl_token_native:
  case sl_token_weak: {
    return sl_scope_module | sl_scope_class;
  }
  case sl_token_require: {
    return sl_scope_module;
  }
    // operators can be used as function name
  case sl_token_plus:
  case sl_token_minus:
  case sl_token_multiply:
  case sl_token_divide:
  case sl_token_mod:
  case sl_token_eq_to:
  case sl_token_lt:
  case sl_token_lt_eq:
  case sl_token_gt:
  case sl_token_gt_eq:
  case sl_token_compare:
  case sl_token_not:
  case sl_token_not_eq:
  case sl_token_bit_or:
  case sl_token_bit_and:
  case sl_token_tilde:
  case sl_token_bit_xor:
  case sl_token_bit_lshift:
  case sl_token_bit_rshift: {
    return sl_scope_module | sl_scope_class | sl_scope_statement;
  }
  default: {
    return sl_scope_statement;
  }
  }
}

sl_sema_type sl_sema_type_from_token_type(sl_token_type token_type) {
  if (token_type == sl_token_comment ||
      token_type == sl_token_inline_c ||
      token_type == sl_token_id ||
      token_type == sl_token_string) {
    return sl_sema_string;
  }
  if (token_type == sl_token_integer) {
    return sl_sema_integer;
  }
  if (token_type == sl_token_float) {
    return sl_sema_float;
  }
  return sl_sema_none;
}

char *sl_token_get_name(sl_token_type token_type) {
  switch (token_type) {
  case sl_token_invalid:return "INVALID";
  case sl_token_comment:return "COMMENT";
  case sl_token_inline_c:return "INLINE_C";
  case sl_token_eof:return "EOF";
  case sl_token_id:return "ID";
  case sl_token_var:return "VAR";
  case sl_token_weak:return "WEAK";
  case sl_token_colon:return ":";
  case sl_token_comma:return ",";
  case sl_token_end:return "END";
  case sl_token_new_line:return "\\n";
  case sl_token_semicolon:return ";";
  case sl_token_class:return "CLASS";
  case sl_token_module:return "MODULE";
  case sl_token_native:return "NATIVE";
  case sl_token_require:return "REQUIRE";
  case sl_token_include:return "INCLUDE";
  case sl_token_static:return "STATIC";
  case sl_token_function:return "FUNC";
  case sl_token_super:return "SUPER";
  case sl_token_self:return "SELF";
  case sl_token_nil:return "NIL";
  case sl_token_true:return "TRUE";
  case sl_token_false:return "FALSE";
  case sl_token_integer:return "INTEGER";
  case sl_token_float:return "FLOAT";
  case sl_token_string:return "STRING";
  case sl_token_plus:return "+";
  case sl_token_minus:return "-";
  case sl_token_multiply:return "*";
  case sl_token_divide:return "/";
  case sl_token_mod:return "%";
  case sl_token_eq:return "=";
  case sl_token_eq_to:return "==";
  case sl_token_identical_to:return "===";
  case sl_token_lt:return "'<'";
  case sl_token_gt:return "'>'";
  case sl_token_compare:return "'<>'";
  case sl_token_gt_eq:return "'>='";
  case sl_token_lt_eq:return "'<='";
  case sl_token_not:return "!";
  case sl_token_not_eq:return "!=";
  case sl_token_arrow:return "'->'";
  case sl_token_bit_or:return "|";
  case sl_token_tilde:return "~";
  case sl_token_bit_xor:return "^";
  case sl_token_bit_and:return "&";
  case sl_token_or:return "||";
  case sl_token_and:return "&&";
  case sl_token_bit_lshift:return "'<<'";
  case sl_token_bit_rshift:return "'>>'";
  case sl_token_question:return "?";
  case sl_token_paren_l:return "(";
  case sl_token_paren_r:return ")";
  case sl_token_bracket_l:return "[";
  case sl_token_bracket_r:return "]";
  case sl_token_brace_l:return "{";
  case sl_token_brace_r:return "}";
  case sl_token_if:return "IF";
  case sl_token_else:return "ELSE";
  case sl_token_unless:return "UNLESS";
  case sl_token_while:return "WHILE";
  case sl_token_break:return "BREAK";
  case sl_token_for:return "FOR";
  case sl_token_in:return "IN";
  case sl_token_dot:return ".";
  case sl_token_return:return "RETURN";
  default:return "???";
  }
}

void sl_token_print(FILE *stream, sl_token_ref token) {
  switch (token->sema_type) {
  case sl_sema_none: {
    fprintf(stream, "[%ld:%ld]<%s>", token->mark.line, token->mark.column, sl_token_get_name(token->type));
  }
    break;
  case sl_sema_integer: {
    fprintf(stream,
            "[%ld:%ld]<%s,%ld>",
            token->mark.line,
            token->mark.column,
            sl_token_get_name(token->type),
            token->value.as_integer);
  }
    break;
  case sl_sema_float: {
    fprintf(stream,
            "[%ld:%ld]<%s,%lf>",
            token->mark.line,
            token->mark.column,
            sl_token_get_name(token->type),
            token->value.as_float);
  }
    break;
  case sl_sema_string: {
    fprintf(stream,
            "[%ld:%ld]<%s,%s>",
            token->mark.line,
            token->mark.column,
            sl_token_get_name(token->type),
            token->value.as_string->string);
  }
    break;
  default:break;
  }
}

sl_token_ref sl_token_create(sl_token_type type) {
  assert(sl_sema_type_from_token_type(type) == sl_sema_none);
  sl_token_ref token = malloc(sizeof(sl_token));
  token->type = type;
  token->sema_type = sl_sema_none;
  token->value.as_integer = 0;
  return token;
}

sl_token_ref sl_token_create_integer(sl_token_type type, sl_integer integer) {
  assert(sl_sema_type_from_token_type(type) == sl_sema_integer);
  sl_token_ref token = malloc(sizeof(sl_token));
  token->type = type;
  token->sema_type = sl_sema_integer;
  token->value.as_integer = integer;
  return token;
}

sl_token_ref sl_token_create_float(sl_token_type type, sl_float f) {
  assert(sl_sema_type_from_token_type(type) == sl_sema_float);
  sl_token_ref token = malloc(sizeof(sl_token));
  token->type = type;
  token->sema_type = sl_sema_float;
  token->value.as_float = f;
  return token;
}

sl_token_ref sl_token_create_string_il(sl_token_type type, char *string, sl_index start, sl_index length) {
  assert(sl_sema_type_from_token_type(type) == sl_sema_string);
  sl_token_ref token = malloc(sizeof(sl_token));
  token->type = type;
  token->sema_type = sl_sema_string;
  token->value.as_string = sl_string_create_il(string, start, length);
  return token;
}

sl_token_ref sl_token_create_string_l(sl_token_type type, char *string, sl_index length) {
  return sl_token_create_string_il(type, string, 0, length);
}

sl_token_ref sl_token_create_string(sl_token_type type, char *string) {
  return sl_token_create_string_l(type, string, strlen(string));
}

void sl_token_destroy(sl_token_ref token) {
  if (token->sema_type == sl_sema_string) {
    sl_string_destroy(token->value.as_string);
  }
  free(token);
}
