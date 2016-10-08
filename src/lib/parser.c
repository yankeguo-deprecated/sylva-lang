//
// Created by Yanke Guo on 2016/9/22.
//

#define __SYLVA_SOURCE__
#include "sylva/parser.h"

#include <sylva/sylva.h>

sl_scan_context_ref sl_parser_context_create() {
  sl_scan_context_ref context = malloc(sizeof(sl_scan_context));
  return context;
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
  int line_no = 1;

  while (fgets(parser->line_buf, parser->line_buf_size, parser->file) != NULL) {
    // iterate each line
    bool lexer_failed = false;
    sl_lexer_ref lexer = sl_lexer_create(sl_string_create(parser->line_buf));

    for (;;) {
      //  iterate each token
      sl_lexer_error err = sl_lexer_error_ok;
      sl_index errIndex = 0;
      sl_token_ref token = sl_lexer_next_token(lexer, &err, &errIndex);

      bool lexer_finished = (token == NULL) || (token->type == sl_token_eof);

      if (err != sl_lexer_error_ok) {
        //  mark lexer failed
        fprintf(stderr, "ERROR: %s at %d:%ld\n", sl_lexer_error_get_name(err), line_no, errIndex);
        lexer_failed = true;
      }

      if (lexer_finished) {
        if (token != NULL)
          sl_token_destroy(token);
        break;
      } else {
        iterator(context, token, parser);
        sl_token_destroy(token);
      }
    }

    sl_lexer_destroy(lexer);

    //  break if lexer failed
    if (lexer_failed)
      break;
    //  increase line_no
    line_no++;
  }

  if (ferror(parser->file)) {
    //  exit if file read failed
    perror("fgets()");
    fprintf(stderr, "can not read file %s at line %d\n", parser->file_name, line_no);
    exit(EXIT_FAILURE);
  }
}

void __sl_parser_printer_iterator(FILE *stream, sl_token_ref token, __unused sl_parser_ref parser) {
  sl_token_print(stream, token);
  fprintf(stream, "\n");
}

void sl_parser_print_scan_result(sl_parser_ref parser, FILE *output) {
  sl_parser_iterate_over_tokens(parser, (sl_parser_iterator) &__sl_parser_printer_iterator, output);
}

void sl_parser_scan_to_project(__unused sl_parser_ref parser, __unused sl_project_ref project) {
}

void sl_parser_destroy(sl_parser_ref parser) {
  fclose(parser->file);
  free(parser->line_buf);
  parser->line_buf_size = 0;
  free(parser);
}
