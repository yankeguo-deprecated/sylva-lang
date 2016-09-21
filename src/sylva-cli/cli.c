//
//  cli.c
//
//  sylva-lang command-line interface
//
//  Created by Yanke Guo on 2016/9/22.
//

#include "sylva/lexer.h"

#include <stdio.h>

int main(int argc, char **argv) {
  SLexerRef lexer = SLexerCreate(SStringCreate("class SomeClass"));
  printf("Hello %s ! %p", "world", lexer);
  SLexerDestroy(lexer);
  return 0;
}
