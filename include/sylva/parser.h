//
//  parser.h
//  sylva
//
//  Created by Ryan Guo<ryan@islandzero.net> on 2016/9/22.
//

#ifndef _SYLVA_PARSER_H_
#define _SYLVA_PARSER_H_

#include <sylva/define.h>
#include <sylva/util.h>
#include <sylva/token.h>
#include <sylva/schema.h>
#include <sylva/project.h>

_SL_BEGIN_STD_C

/**
 * sl_parser_context stores currently status of parsing
 */
typedef struct {
  /**
   * schema of current parsing class
   */
  sl_class_schema_ref class_schema;
  /**
   * schema of current parsing module
   */
  sl_module_schema_ref module_schema;
  /**
   * schema of current function
   */
  sl_func_schema_ref func_schema;
  /**
   * tokens parsed, may be cleared periodically
   */
  sl_array_ref tokens;
  /**
   * level of scope, i.e. how many 'end's needed to escape
   *
   * scope_level == 1, i.e. class or module definition
   * scope_level >=2, function definition
   */
  int scope_level;
  /**
   * target project
   */
  sl_project_ref project;
  /**
   * current parser
   */
  char *file_name;
} sl_scan_context;

/**
 * pointer of scan context
 */
typedef sl_scan_context *sl_scan_context_ref;

/**
 * create a scan context
 * @return
 */
sl_scan_context_ref sl_scan_context_create();

/**
 * destroy a scan context
 * @param context
 */
void sl_scan_context_destroy(sl_scan_context_ref context);

/**
 * consume a token
 * @param context
 * @param token
 */
void sl_scan_context_start_consuming(sl_scan_context_ref context);

void sl_scan_context_consume_token(sl_scan_context_ref context, sl_token_ref income_token);

void sl_scan_context_end_consuming(sl_scan_context_ref context);
/**
 * sl_parser is sylva-lang lexer in file scope, it parse a single file and fill the global table
 */
typedef struct {
  /**
   * file name
   */
  char *file_name;
  /**
   * file handler
   */
  FILE *file;
  /**
   * buffer for each line
   */
  char *line_buf;
  /**
   * size of line buffer
   */
  int line_buf_size;
} sl_parser;

/**
 * pointer
 */
typedef sl_parser *sl_parser_ref;

/**
 * create a sl_parser from file_name, will returns NULL if file not found
 * @param file_name
 * @return sl_parser_ref
 */
sl_parser_ref sl_parser_create(char *file_name);

/**
 * token iterator function definition
 */
typedef void(*sl_parser_iterator)(void *context, sl_token_ref token, sl_parser_ref parser);

/**
 * iterate over tokens of this parser, with a optional context
 *
 * @param parser
 * @param iterator
 * @param context
 */
void sl_parser_iterate_over_tokens(sl_parser_ref parser, sl_parser_iterator iterator, void *context);

/**
 * print lexer scan result to a stream
 *
 * @param parser
 * @param output
 */
void sl_parser_print_scan_result(sl_parser_ref parser, FILE *output);

/**
 * scan the source files, create schemas and write to project
 *
 * @param parser
 * @param project
 */
void sl_parser_scan_to_project(sl_parser_ref parser, sl_project_ref project);

/**
 * destroy a parser and close file
 *
 * @param parser
 */
void sl_parser_destroy(sl_parser_ref parser);

_SL_END_STD_C

#endif // _SYLVA_PARSER_H_
