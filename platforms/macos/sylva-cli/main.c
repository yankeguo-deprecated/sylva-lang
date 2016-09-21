//
//  main.c
//  sylva-cli
//
//  Created by Yanke Guo on 2016/9/20.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#include <stdio.h>
#include "sylva.h"
#include "sylva-lexer.h"

int main(int argc, const char * argv[]) {
  SStringRef source = SStringCreate("class SomeClass");
  SLexerRef lexer   = SLexerCreate(source);
  for (;;) {
    STokenRef token = SLexerGetNextToken(lexer);
    STokenPrint(token);
    if (token->type == STokenEOF) {
      break;
    }
  }
  return 0;
}
