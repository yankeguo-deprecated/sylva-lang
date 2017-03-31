//
//  cli.c
//
//  sylva-lang command-line interface
//
//  Created by Yanke Guo on 2016/9/22.
//

#include <sylva/sylva.h>

typedef enum {
  cli_exit_common = 1,
  cli_exit_bad_command = 2,
  cli_exit_bad_arguments = 3,
} cli_exit_code;

static void print_usage_and_exit(char *binName, cli_exit_code code) {
  if (code == cli_exit_bad_command) {
    fprintf(stderr, "Bad command\n");
  }
  if (code == cli_exit_bad_arguments) {
    fprintf(stderr, "Bad arguments\n");
  }
  fprintf(stderr,
          "Usage:\n"
              "%s compile [DIRECTORY] - compile a whole source directory\n"
              "%s scan    [FILE]      - print lexer result of a single source file\n",
          binName,
          binName);
  exit(code);
}

static void scan_command(char *file_name) {
  sl_parser_ref parser = sl_parser_create(file_name);
  if (parser == NULL)
    return;
  sl_parser_print_scan_result(parser, stdout);
  sl_parser_destroy(parser);
}

static void compile_command(char *directory) {
  sl_project_ref project = sl_project_create(directory);
  sl_project_load_manifest(project);
  sl_project_ensure_build_directory(project);
  sl_project_scan_source_files(project);
  sl_project_destroy(project);
}

int main(int argc, char **argv) {
  char *binName = argv[0];

  if (argc >= 2) {
    if (strcmp(argv[1], "compile") == 0) {
      if (argc != 3) {
        print_usage_and_exit(binName, cli_exit_bad_arguments);
      }
      compile_command(argv[2]);
    } else if (strcmp(argv[1], "scan") == 0) {
      if (argc != 3) {
        print_usage_and_exit(binName, cli_exit_bad_arguments);
      }
      scan_command(argv[2]);
    } else {
      print_usage_and_exit(binName, cli_exit_bad_command);
    }
  } else {
    print_usage_and_exit(binName, cli_exit_bad_command);
  }

  return 0;
}
