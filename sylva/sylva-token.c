//
//  sylva-token.c
//  sylva
//
//  Created by Yanke Guo on 2016/9/21.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#define __SYLVA_SOURCE__
#include "sylva-token.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

SSemanType STokenTypeGetSemanType(STokenType tokenType) {
  if (tokenType == STokenComment ||
      tokenType == STokenId ||
      tokenType == STokenString) {
    return SSemanString;
  }
  if (tokenType == STokenInteger) {
    return SSemanInteger;
  }
  if (tokenType == STokenFloat) {
    return SSemanFloat;
  }
  return SSemanNone;
}

SYLVA_EXPORT char *const  STokenTypeGetName(STokenType tokenType) {
  switch (tokenType) {
    case STokenComment:
      return "COMMENT";
    case STokenEOF:
      return "EOF";
    case STokenId:
      return "ID";
    case STokenVar:
      return "VAR";
    case STokenColon:
      return ":";
    case STokenEnd:
      return "END";
    case STokenNewLine:
      return "\\n";
    case STokenSemicolon:
      return ";";
    case STokenClass:
      return "CLASS";
    case STokenModule:
      return "MODULE";
    case STokenNative:
      return "NATIVE";
    case STokenRequire:
      return "REQUIRE";
    case STokenInclude:
      return "INCLUDE";
    case STokenStatic:
      return "STATIC";
    case STokenFunc:
      return "FUNC";
    case STokenSuper:
      return "SUPER";
    case STokenSelf:
      return "SELF";
    case STokenNil:
      return "NIL";
    case STokenTrue:
      return "TRUE";
    case STokenFalse:
      return "FALSE";
    case STokenInteger:
      return "INTEGER";
    case STokenFloat:
      return "FLOAT";
    case STokenString:
      return "STRING";
    case STokenPlus:
      return "+";
    case STokenMinus:
      return "-";
    case STokenMultiply:
      return "*";
    case STokenDivide:
      return "/";
    case STokenMod:
      return "%";
    case STokenEq:
      return "=";
    case STokenEqTo:
      return "==";
    case STokenLt:
      return "<";
    case STokenGt:
      return ">";
    case STokenGtEq:
      return ">=";
    case STokenLtEq:
      return "<=";
    case STokenNot:
      return "!";
    case STokenNotEq:
      return "!=";
    case STokenArrow:
      return "->";
    case STokenBitOr:
      return "|";
    case STokenBitAnd:
      return "&";
    case STokenOr:
      return "||";
    case STokenAnd:
      return "&&";
    case STokenBitShift:
      return "<<";
    case STokenBitUnshift:
      return ">>";
    case STokenQuestion:
      return "?";
    case STokenParenL:
      return "(";
    case STokenParenR:
      return ")";
    case STokenBracketL:
      return "[";
    case STokenBracketR:
      return "]";
    case STokenBraceL:
      return "{";
    case STokenBraceR:
      return "}";
    case STokenIf:
      return "IF";
    case STokenElse:
      return "ELSE";
    case STokenUnless:
      return "UNLESS";
    case STokenWhile:
      return "WHILE";
    case STokenBreak:
      return "BREAK";
    case STokenFor:
      return "FOR";
    case STokenIn:
      return "IN";
    case STokenDot:
      return ".";
    case STokenReturn:
      return "RETURN";
    default:
      return "???";
  }
}

SYLVA_EXPORT void STokenPrint(STokenRef token) {
  switch (token->semanType) {
    case SSemanNone:
      printf("<%s>", STokenTypeGetName(token->type));
      break;
    case SSemanInteger:
      printf("<%s,%ld>", STokenTypeGetName(token->type), token->value.integerValue);
    case SSemanFloat:
      printf("<%s,%lf>", STokenTypeGetName(token->type), token->value.floatValue);
    case SSemanString:
      printf("<%s,%s>", STokenTypeGetName(token->type), token->value.stringValue->string);
    default:
      break;
  }
}

STokenRef STokenCreate(STokenType type) {
  assert(STokenTypeGetSemanType(type) == SSemanNone);
  
  STokenRef token = malloc(sizeof(SToken));
  token->type = type;
  token->semanType = SSemanNone;
  token->value.integerValue = 0;
  return token;
}

STokenRef STokenCreateInteger(STokenType type, SLexInteger integer) {
  assert(STokenTypeGetSemanType(type) == SSemanInteger);
  
  STokenRef token = malloc(sizeof(SToken));
  token->type = type;
  token->semanType = SSemanInteger;
  token->value.integerValue = integer;
  return token;
}

STokenRef STokenCreateFloat(STokenType type, SLexFloat f) {
    assert(STokenTypeGetSemanType(type) == SSemanFloat);
  
  STokenRef token = malloc(sizeof(SToken));
  token->type = type;
  token->semanType = SSemanFloat;
  token->value.floatValue = f;
  return token;
}

STokenRef STokenCreateString(STokenType type, char *string) {
  assert(STokenTypeGetSemanType(type) == SSemanString);
  
  STokenRef token = malloc(sizeof(SToken));
  token->type = type;
  token->semanType = SSemanString;
  token->value.stringValue = SStringCreate(string);
  return token;
}


STokenRef STokenCreateStringWithLength(STokenType type, char *string, int length) {
  assert(STokenTypeGetSemanType(type) == SSemanString);
  
  STokenRef token = malloc(sizeof(SToken));
  token->type = type;
  token->semanType = SSemanString;
  token->value.stringValue = SStringCreateWithLength(string, length);
  return token;
}

void STokenDestroy(STokenRef token) {
  if (token->semanType == SSemanString) {
    SStringDestroy(token->value.stringValue);
  }
  free(token);
}
