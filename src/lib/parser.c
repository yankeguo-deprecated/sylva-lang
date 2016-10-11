//
// Created by Yanke Guo on 2016/9/22.
//

#define __SYLVA_SOURCE__
#include "sylva/parser.h"

#include <sylva/sylva.h>
#include <sylva/token.h>
#include <sylva/util.h>

sl_scan_context_ref sl_scan_context_create() {
  sl_scan_context_ref context = malloc(sizeof(sl_scan_context));
  context->class_schema = NULL;
  context->module_schema = NULL;
  context->func_schema = NULL;
  context->tokens = sl_array_create(10);
  context->tokens->value_deallocator = (sl_array_value_deallocator) &sl_token_destroy;
  return context;
}

void sl_scan_context_destroy(sl_scan_context_ref context) {
  sl_array_destroy(context->tokens);
  free(context);
}

void sl_scan_context_start_consuming(_sl_unused sl_scan_context_ref context) {
}

void sl_scan_context_consume_token(_sl_unused sl_scan_context_ref context, _sl_unused sl_token_ref income_token) {
  //  ignore comment and inline_c, because in scanning stage, they are not important
  if (income_token->type == sl_token_comment || income_token->type == sl_token_inline_c) {
    return;
  }

  //  if got new_line, parse the tokens
  if (income_token->type == sl_token_new_line) {
    if (context->tokens->count > 0) {
      //  determine current scope
      sl_scope current_scope;
      if (context->module_schema != NULL) {
        current_scope = sl_scope_module;
      } else if (context->class_schema != NULL) {
        current_scope = sl_scope_class;
      } else {
        current_scope = sl_scope_global;
      }

      //  handle special case and validate
      for (sl_index i = 0; i < context->tokens->count; i++) {
        sl_token_ref token = context->tokens->values[i];

        //  special cases
        {
          //  'class' as ID
          if (token->type == sl_token_class && i != 0) {
            sl_token_destroy(token);
            token = sl_token_create_string(sl_token_id, "class");
            context->tokens->values[i] = token;
          }

          //  'module' as ID
          if (token->type == sl_token_module && i != 0) {
            sl_token_destroy(token);
            token = sl_token_create_string(sl_token_id, "module");
            context->tokens->values[i] = token;
          }
        }

        //  check token scope limitation
        sl_check(sl_scope_from_token_type(token->type) & current_scope,
                 "unexpected token %s in %s at [%ld:%ld]\n",
                 sl_token_get_name(token->type),
                 context->file_name,
                 token->mark.line,
                 token->mark.column);
      }

      //  extract variables
      sl_array_ref tokens = context->tokens;
      sl_token_ref token0 = tokens->values[0];

      //  check class definition
      {
        if (token0->type == sl_token_class) {
          sl_token_ref token1;
          sl_check((tokens->count == 2 || tokens->count == 4) && (token1 = tokens->values[1])->type == sl_token_id,
                   "invalid class definition in %s at %ld:%ld",
                   context->file_name,
                   token0->mark.line,
                   token0->mark.column);
          //  init class_schema
          context->class_schema = sl_class_schema_create();
          context->class_schema->name = sl_string_create(token1->value.as_string->string);
          if (tokens->count == 4) {
            sl_token_ref token2 = tokens->values[2];
            sl_token_ref token3 = tokens->values[3];
            sl_check(token2->type == sl_token_colon && token3->type == sl_token_id,
                     "invalid class definition with superclass in %s at %ld:%ld",
                     context->file_name,
                     token2->mark.line,
                     token2->mark.column);
            //  set superclass
            context->class_schema->super_class_name = sl_string_create(token3->value.as_string->string);
          }
        }
      }

      //  check module definition
      {
        if (token0->type == sl_token_module) {
          sl_token_ref token1;
          sl_check(tokens->count == 2 && (token1 = tokens->values[1])->type == sl_token_id,
                   "invalid module definition in %s at %ld:%ld",
                   context->file_name,
                   token0->mark.line,
                   token0->mark.column);
          //  init class_schema
          context->module_schema = sl_module_schema_create();
          context->module_schema->name = sl_string_create(token1->value.as_string->string);
        }
      }

      //  check 'include'
      {
        if (token0->type == sl_token_include) {
          sl_token_ref token1;
          sl_check(tokens->count == 2 && (token1 = tokens->values[1])->type == sl_token_id,
                   "invalid include statement in %s at %ld:%ld",
                   context->file_name,
                   token1->mark.line,
                   token1->mark.column);
          if (context->class_schema != NULL) {
            sl_array_append(context->class_schema->included_module_names,
                            sl_string_create(token1->value.as_string->string));
          } else if (context->module_schema != NULL) {
            sl_array_append(context->class_schema->included_module_names,
                            sl_string_create(token1->value.as_string->string));
          }
        }
      }

      //  check variable/function declaration
      {
        sl_boolean found_var = sl_false;
        sl_index var_idx = sl_index_not_found;

        sl_boolean found_func = sl_false;
        sl_index func_idx = sl_index_not_found;

        sl_boolean static_flag = sl_false;
        sl_boolean require_flag = sl_false;
        sl_boolean weak_flag = sl_false;
        sl_boolean native_flag = sl_false;

        for (sl_index i = 0; i < tokens->count; i++) {
          sl_token_ref token = tokens->values[i];

          if (token->type == sl_token_var) {
            found_var = sl_true;
            var_idx = i;
          }

          if (token->type == sl_token_function) {
            found_func = sl_true;
            func_idx = i;
          }

          if (token->type == sl_token_static) {
            static_flag = sl_true;
          }

          if (token->type == sl_token_native) {
            native_flag = sl_true;
          }

          if (token->type == sl_token_weak) {
            weak_flag = sl_true;
          }

          if (token->type == sl_token_require) {
            require_flag = sl_true;
          }
        }

        //  member variable declaration
        if (found_var) {
          sl_check((found_func || native_flag) == false && tokens->count == var_idx + 2,
                   "invalid member variable declaration in %s at line %ld",
                   context->file_name,
                   token0->mark.line);
          sl_token_ref var_id = tokens->values[var_idx + 1];
          sl_check(var_id->type == sl_token_id, "invalid member variable declaration in %s at line %ld",
                   context->file_name,
                   token0->mark.line);
          sl_member_schema_ref schema = sl_member_schema_create();
          schema->name = sl_string_create(var_id->value.as_string->string);
          schema->is_required = require_flag;
          schema->is_weak = weak_flag;
          schema->is_static = static_flag;
          if (context->module_schema != NULL) {
            sl_array_append(context->module_schema->members, schema);
          }
          if (context->class_schema != NULL) {
            sl_array_append(context->class_schema->members, schema);
          }
        }

        // function declaration
        if (found_func) {
          sl_check((found_var || weak_flag) == false && tokens->count > func_idx,
                   "invalid function declaration in %s at line %ld",
                   context->file_name,
                   token0->mark.line);
          sl_token_ref func_id = tokens->values[func_idx + 1];
          sl_string_ref func_name = sl_string_create(func_id->value.as_string->string);
        }
      }

      //  clear tokens
      sl_array_clear(tokens);
    }
  } else {
    sl_array_append(context->tokens, income_token);
  }
}

void sl_scan_context_end_consuming(_sl_unused sl_scan_context_ref context) {
}

sl_parser_ref sl_parser_create(char *file_name) {
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    fprintf(stderr, "Can not open file %s", file_name);
    return NULL;
  }
  sl_parser_ref parser = malloc(sizeof(sl_parser));
  parser->file_name = file_name;
  parser->file = file;
  parser->line_buf_size = 500;
  parser->line_buf = malloc(sizeof(char) * parser->line_buf_size);
  return parser;
}

void sl_parser_iterate_over_tokens(sl_parser_ref parser, sl_parser_iterator iterator, void *context) {
  sl_index line_no = 1;

  while (fgets(parser->line_buf, parser->line_buf_size, parser->file) != NULL) {
    // iterate each line
    bool lexer_failed = false;
    sl_lexer_ref lexer = sl_lexer_create(sl_string_create(parser->line_buf));

    for (;;) {
      //  iterate each token
      sl_token_ref token = sl_lexer_next_token(lexer);
      token->mark.line = line_no;

      if (token->type == sl_token_invalid) {
        //  mark lexer failed
        fprintf(stderr,
                "ERROR: invalid token found in %s at %ld:%ld\n",
                parser->file_name,
                line_no,
                token->mark.column);
        lexer_failed = true;
        break;
      } else if (token->type == sl_token_eof) {
        break;
      } else {
        iterator(context, token, parser);
      }
    }

    //  destroy line lexer
    sl_lexer_destroy(lexer);

    //  break if lexer failed
    if (lexer_failed) {
      break;
    }

    //  increase line_no
    line_no++;
  }

  //  append final EOF
  sl_token_ref eof = sl_token_create(sl_token_eof);
  eof->mark.line = line_no;
  eof->mark.column = 0;
  iterator(context, eof, parser);

  //  exit if file read failed
  if (ferror(parser->file)) {
    perror("fgets()");
    fprintf(stderr, "can not read file %s at line %ld\n", parser->file_name, line_no);
    exit(EXIT_FAILURE);
  }
}

void __sl_parser_printer_iterator(FILE *stream, sl_token_ref token, _sl_unused sl_parser_ref parser) {
  sl_token_print(stream, token);
  fprintf(stream, "\n");
  sl_token_destroy(token);
}

void sl_parser_print_scan_result(sl_parser_ref parser, FILE *output) {
  sl_parser_iterate_over_tokens(parser, (sl_parser_iterator) &__sl_parser_printer_iterator, output);
}

void __sl_parser_scan_to_project_iterator(sl_scan_context_ref context,
                                          sl_token_ref token,
                                          _sl_unused sl_parser_ref parser) {
  sl_scan_context_consume_token(context, token);
}

void sl_parser_scan_to_project(_sl_unused sl_parser_ref parser, _sl_unused sl_project_ref project) {
  sl_scan_context_ref context = sl_scan_context_create();
  context->project = project;
  context->file_name = parser->file_name;
  sl_scan_context_start_consuming(context);
  sl_parser_iterate_over_tokens(parser, (sl_parser_iterator) &__sl_parser_scan_to_project_iterator, context);
  sl_scan_context_end_consuming(context);
  sl_scan_context_destroy(context);
}

void sl_parser_destroy(sl_parser_ref parser) {
  fclose(parser->file);
  free(parser->line_buf);
  parser->line_buf_size = 0;
  free(parser);
}
