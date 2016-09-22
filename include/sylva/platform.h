//
//  sylva-platform.h
//  sylva
//
//  Created by Yanke Guo on 2016/9/20.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#ifndef _SYLVA_PLATFORM_H_
#define _SYLVA_PLATFORM_H_

#include <limits.h>

/////////////    OS    ///////////////////////////

#ifdef __APPLE__
  #include "TargetConditionals.h"

  #if !TARGET_OS_OSX
    #error sylva is currently available on macOS only
  #endif
#endif

////////////    CPP    //////////////////////////

#ifdef __cplusplus
  #define __CPP_DECL_START  extern "C" {
  #define __CPP_DECL_END }
#else
  #define __CPP_DECL_START
  #define __CPP_DECL_END
#endif

////////////   EXPORT      //////////////////////

#ifdef __SYLVA_SOURCE__
  #define SYLVA_EXPORT
#else
  #define SYLVA_EXPORT extern
#endif

//////////// Numeric Types //////////////////////

typedef long SInteger;
typedef double SFloat;
typedef unsigned long SIndex;
#define SIndexMax ULONG_MAX
#define SIndexNotFound SIndexMax

#endif // _SYLVA_PLATFORM_H_
