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
  sylva_lexer lexer;
  sylva_lexer_init(&lexer);
  
  // insert code here...
  printf("Hello, World!%p\n", &lexer);
  return 0;
}
