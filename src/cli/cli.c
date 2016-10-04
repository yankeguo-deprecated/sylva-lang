//
//  cli.c
//
//  sylva-lang command-line interface
//
//  Created by Yanke Guo on 2016/9/22.
//

#include <sylva/define.h>
#include <sylva/token.h>
#include <sylva/lexer.h>

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

static void SCLIScanFile(char *fileName) {
  FILE *file = fopen(fileName, "r");
  if (file == NULL) {
    fprintf(stderr, "Can not open file %s", fileName);
    return;
  }
  char lineBuf[500];
  int lineNo = 1;
  while (fgets(lineBuf, sizeof(lineBuf), file) != NULL) {
    sl_lexer_ref lexer = sl_lexer_create(sl_string_create(lineBuf));
    int lexerFailed = 0;
    for (;;) {
      sl_lexer_error err = sl_lexer_error_ok;
      sl_index errIndex = 0;
      sl_token_ref token = sl_lexer_next_token(lexer, &err, &errIndex);
      int lexerFinished = (token == NULL) || (token->type == sl_token_eof);
      if (token == NULL) {
        fprintf(stderr, "ERROR: %s at %d:%ld\n", sl_lexer_error_get_name(err), lineNo, errIndex);
        lexerFailed = 1;
      }
      if (lexerFinished) {
        break;
      } else {
        sl_token_print(stdout, token);
        fprintf(stdout, "\n");
        sl_token_destroy(token);
      }
    }
    sl_lexer_destroy(lexer);
    if (lexerFailed) {
      break;
    }
    lineNo++;
  }

  if (feof(file)) {
  } else if (ferror(file)) {
    perror("fgets()");
    fprintf(stderr, "can not read file %s at line %d\n", fileName, lineNo);
    exit(EXIT_FAILURE);
  }

  fclose(file);
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
