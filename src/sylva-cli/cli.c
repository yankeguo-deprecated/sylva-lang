//
//  cli.c
//
//  sylva-lang command-line interface
//
//  Created by Yanke Guo on 2016/9/22.
//

#include <stdio.h>

#include <sylva/token.h>
#include <sylva/lexer.h>

int main(int argc, char **argv) {
  SLexerRef lexer = SLexerCreate(SStringCreate("someValue->someVariable.call(a + b << c)"));
  for (;;) {
    SLexerError err = SLexerErrorOK;
    SIndex errIndex = 0;
    STokenRef token = SLexerGetNextToken(lexer, &err, &errIndex);
    int end = token == NULL || token->type == STokenEOF;
    STokenPrint(token);
    STokenDestroy(token);
    if (end) {
      break;
    }
  }
  SLexerDestroy(lexer);
  return 0;
}
