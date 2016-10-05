//
//  cli.c
//
//  sylva-lang command-line interface
//
//  Created by Yanke Guo on 2016/9/22.
//

#include <sylva/define.h>
#include <sylva/parser.h>

typedef enum {
  SCLIExitCommon = 1,
  SCLIExitBadCommand = 2,
  SCLIExitBadArguments = 3,
} SCLIExitCode;

static void SCLIPrintUsageAndExit(char *binName, SCLIExitCode code) {
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

static void SCLIScanFile(char *file_name) {
  sl_parser_ref parser = sl_parser_create(file_name);
  if (parser == NULL)
    return;
  sl_parser_print_scan_result(parser, stdout);
  sl_parser_destroy(parser);
}

int main(int argc, char **argv) {
  char *binName = argv[0];

  if (argc >= 2) {
    if (strcmp(argv[1], "compile") == 0) {
      printf("compile not supported !");
    } else if (strcmp(argv[1], "scan") == 0) {
      if (argc != 3) {
        SCLIPrintUsageAndExit(binName, SCLIExitBadArguments);
      }
      SCLIScanFile(argv[2]);
    } else {
      SCLIPrintUsageAndExit(binName, SCLIExitBadCommand);
    }
  } else {
    SCLIPrintUsageAndExit(binName, SCLIExitBadCommand);
  }

  return 0;
}
