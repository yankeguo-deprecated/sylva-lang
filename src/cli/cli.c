//
//  cli.c
//
//  sylva-lang command-line interface
//
//  Created by Yanke Guo on 2016/9/22.
//

#include <sylva/sylva.h>

typedef enum {
  SCLIExitCommon = 1,
  SCLIExitBadCommand = 2,
  SCLIExitBadArguments = 3,
} SCLIExitCode;

static void print_usage_and_exit(char *binName, SCLIExitCode code) {
  if (code == SCLIExitBadCommand) {
    fprintf(stderr, "Bad command\n");
  }
  if (code == SCLIExitBadArguments) {
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
        print_usage_and_exit(binName, SCLIExitBadArguments);
      }
      compile_command(argv[2]);
    } else if (strcmp(argv[1], "scan") == 0) {
      if (argc != 3) {
        print_usage_and_exit(binName, SCLIExitBadArguments);
      }
      scan_command(argv[2]);
    } else {
      print_usage_and_exit(binName, SCLIExitBadCommand);
    }
  } else {
    print_usage_and_exit(binName, SCLIExitBadCommand);
  }

  return 0;
}
