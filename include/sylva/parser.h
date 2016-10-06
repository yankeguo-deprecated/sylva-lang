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

#include <yaml.h>

__BEGIN_STD_C

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
 * print lexer scan result to a stream
 *
 * @param parser
 * @param output
 */
void sl_parser_print_scan_result(sl_parser_ref parser, FILE *output);

/**
 * destroy a parser and close file
 *
 * @param parser
 */
void sl_parser_destroy(sl_parser_ref parser);

__END_STD_C

#endif // _SYLVA_PARSER_H_
