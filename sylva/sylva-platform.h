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
#endif

////////////   EXPORT      //////////////////////

#ifdef __SYLVA_SOURCE__
  #define SYLVA_EXPORT
#else
  #define SYLVA_EXPORT extern
#endif

//////////// Numeric Types //////////////////////

#define SInteger long
#define SFloat double

#define SLexInteger SInteger
#define SLexFloat   SFloat
#define SLexString  SStringRef

#endif /* sylva_platform_h */
