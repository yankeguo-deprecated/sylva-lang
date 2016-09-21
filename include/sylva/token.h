//
//  sylva-token.h
//  sylva
//
//  Created by Yanke Guo on 2016/9/21.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#ifndef _SYLVA_TOKEN_H_
#define _SYLVA_TOKEN_H_

#include "sylva/string.h"

__CPP_DECL_START

typedef enum {

  STokenEOF = -1,

  //////////////// Common ///////////////

  /*
   sharp started comment
   */
      STokenComment,

  /**
   Identifier, such as class name, method name, variable name, etc
   */
      STokenId,

  /**
   keyword `var`
   */
      STokenVar,

  /**
   mark `:`
   */
      STokenColon,

  /**
   keyword `end`
   */
      STokenEnd,

  /**
   line break
   */
      STokenNewLine,

  /**
   semicolon act as an inline line break
   */
      STokenSemicolon,

  ////////////// Class / Module ///////////////

  /**
   keyword `class` for class declaration
   */
      STokenClass,

  /**
   keyword `module` for module declaration
   */
      STokenModule,

  /**
   keyword `native` for native C binding
   */
      STokenNative,

  /**
   keyword `require` for module integration method check
   */
      STokenRequire,

  /*
   keyword `include` for module integration
   */
      STokenInclude,

  /*
   keyword `static` for static variables / methods
   */
      STokenStatic,

  /*
   keyword `func` for method declaration
   */
      STokenFunc,

  /////////////// Instant Values ///////////

  /**
   keyword `super` for super class
   */
      STokenSuper,

  /**
   keyword `self`
   */
      STokenSelf,

  /**
   keyword `nil`
   */
      STokenNil,

  /**
   keyword `true` for boolean
   */
      STokenTrue,

  /**
   keyword `false` for boolean
   */
      STokenFalse,

  /**
   instant integer like `10890223`
   */
      STokenInteger,

  /**
   instant float like `10.223`
   */
      STokenFloat,

  /**
   instant string like `"string"`
   */
      STokenString,

  //////////////// Operator //////////////

  /**
   operator `+`
   */
      STokenPlus,

  /**
   operator `-`
   */
      STokenMinus,

  /**
   operator `*`
   */
      STokenMultiply,

  /**
   operator `/`
   */
      STokenDivide,

  /**
   operator `%`
   */
      STokenMod,

  /**
   operator `=`
   */
      STokenEq,

  /**
   operator `==`
   */
      STokenEqTo,

  /**
   opertaor `<`
   */
      STokenLt,

  /**
   operator `>`
   */
      STokenGt,

  /**
   operator `>=`
   */
      STokenGtEq,

  /**
   operator `<=`
   */
      STokenLtEq,

  /**
   operator `!`
   */
      STokenNot,

  /**
   operator `!=`
   */
      STokenNotEq,

  /**
   operator `->`
   */
      STokenArrow,

  /**
   bit operator `|`
   */
      STokenBitOr,

  /**
   bit operator `&`
   */
      STokenBitAnd,

  /**
   operator `||`
   */
      STokenOr,

  /**
   operator `&&`
   */
      STokenAnd,

  /**
   bit operator `<<`
   */
      STokenBitShift,

  /**
   bit operator `>>`
   */
      STokenBitUnshift,

  /**
   keyword `?`
   */
      STokenQuestion,

  /**
   mark `()`
   */
      STokenParenL,
  STokenParenR,

  /**
   mark `[]`
   */
      STokenBracketL,
  STokenBracketR,

  /**
   mark `{}`
   */
      STokenBraceL,
  STokenBraceR,

  //////////////// Flow  /////////////////

  /**
   keyword `if`
   */
      STokenIf,

  /**
   keyword `else`
   */
      STokenElse,

  /**
   keyword `unless`
   */
      STokenUnless,

  /**
   keyword `while`
   */
      STokenWhile,

  /**
   keyword `break`
   */
      STokenBreak,

  /**
   keyword `for`
   */
      STokenFor,

  /**
   keyword `in`
   */
      STokenIn,

  /////////////// Expression /////////////

  /**
   syntax `instance.method`
   */
      STokenDot,

  /**
   keyword `return`
   */
      STokenReturn,

} STokenType;

typedef union {
  SLexInteger integerValue;
  SLexFloat floatValue;
  SLexString stringValue;
} SSemaValue;

typedef enum {
  SSemaNone,
  SSemaInteger,
  SSemaFloat,
  SSemaString
} SSemaType;

/**
 Get SSemaType from STokenType
 
 @param tokenType STokenType
 @return SSemaType
 */
SYLVA_EXPORT SSemaType STokenTypeGetSemaType(STokenType tokenType);

SYLVA_EXPORT char *const STokenTypeGetName(STokenType tokenType);

typedef struct {
  SSemaValue value;
  SSemaType semaType;
  STokenType type;
} SToken;

typedef SToken *STokenRef;

SYLVA_EXPORT void STokenPrint(STokenRef token);

/**
 Create a STokenRef with None sema
 */
SYLVA_EXPORT STokenRef STokenCreate(STokenType type);

/**
 Create a STokenRef with Integer sema
 */
SYLVA_EXPORT STokenRef STokenCreateInteger(STokenType type, SLexInteger integer);

/**
 Create a STokenRef with Float sema
 */
SYLVA_EXPORT STokenRef STokenCreateFloat(STokenType type, SLexFloat f);

/**
 Create a STokenRef with SString sema
 */
SYLVA_EXPORT STokenRef STokenCreateString(STokenType type, char *string);

SYLVA_EXPORT STokenRef STokenCreateStringWithLength(STokenType type, char *string, int length);

/**
 Destroy a STokenRef, free SString if existed
 */
SYLVA_EXPORT void STokenDestroy(STokenRef token);

__CPP_DECL_END

#endif // _SYLVA_TOKEN_H_
