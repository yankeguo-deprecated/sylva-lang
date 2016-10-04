//
//  cli.c
//
//  sylva-lang command-line interface
//
//  Created by Yanke Guo on 2016/9/22.
//

#include <sylva/define.h>
#include <sylva/runtime.h>
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
    SLexerRef lexer = SLexerCreate(SStringCreate(lineBuf));
    int lexerFailed = 0;
    for (;;) {
      SLexerError err = SLexerErrorOK;
      sl_index errIndex = 0;
      STokenRef token = SLexerGetNextToken(lexer, &err, &errIndex);
      int lexerFinished = (token == NULL) || (token->type == STokenEOF);
      if (token == NULL) {
        fprintf(stderr, "ERROR: %s at %d:%ld\n", SLexerErrorGetName(err), lineNo, errIndex);
        lexerFailed = 1;
      }
      if (lexerFinished) {
        break;
      } else {
        STokenPrint(stdout, token);
        fprintf(stdout, "\n");
        STokenDestroy(token);
      }
    }
    SLexerDestroy(lexer);
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
