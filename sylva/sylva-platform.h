//
//  sylva-platform.h
//  sylva
//
//  Created by Yanke Guo on 2016/9/20.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#ifndef sylva_platform_h
#define sylva_platform_h

#ifdef __APPLE__
  #include "TargetConditionals.h"
  #if TARGET_OS_OSX
  #else
    #error sylva is currently available on macOS only
  #endif
#else
  #error sylva is currently available on macOS only
#endif

//////////// Numeric Types //////////////////////

#define sylva_lex_integer   long

#define sylva_lex_float double

#define sylva_lex_string char*

#endif /* sylva_platform_h */
