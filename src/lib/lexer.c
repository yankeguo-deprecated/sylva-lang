//
//  sylva-lexer.c
//  sylva
//
//  Created by Yanke Guo on 2016/9/20.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#define __SYLVA_SOURCE__
#include "sylva/string.h"
#include "sylva/lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sylva/lexer.h>
#include <sylva/string.h>

char *const SLexerErrorGetName(SLexerError error) {
  switch (error) {
  case SLexerErrorInvalidId:return "invalid identifier";
  case SLexerErrorInvalidSequence: return "invalid sequence";
  case SLexerErrorOK:
  default:return "ok";
  }
}

SLexerRef SLexerCreate(SStringRef source) {
  SLexerRef lexer = malloc(sizeof(SLexer));
  lexer->source = source;
  lexer->index = 0;
  return lexer;
};

STokenRef SLexerGetNextToken(SLexerRef lexer, SLexerError *err, SIndex *errIndex) {
  SIndex idx = lexer->index;

  //  Check EOF
  if (idx >= lexer->source->length) {
    return STokenCreate(STokenEOF);
  }

  //  Seek first non blank
  idx = SStringSeekNoBlank(lexer->source, lexer->index);
  if (idx == SIndexNotFound) {
    if (err) {
      *err = SLexerErrorOK;
    }
    lexer->index = lexer->source->length;
    return STokenCreate(STokenEOF);
  }

  //  Get first letter
  char first = lexer->source->string[idx];

  //  Check New Line
  if (first == '\n') {
    lexer->index = idx + 1;
    return STokenCreate(STokenNewLine);
  }

  if (first == '\r') {
    if (lexer->source->length > idx + 1 && lexer->source->string[idx + 1] == '\n') {
      lexer->index = idx + 2;
      return STokenCreate(STokenNewLine);
    }
  }

  //  Sharp started (comment)
  if (first == '#') {
    if (err) {
      *err = SLexerErrorOK;
    }
    lexer->index = lexer->source->length;
    return STokenCreateStringIL(STokenComment, lexer->source->string, idx + 1, lexer->source->length - idx - 1);
  }

  //  Alphabet started
  if (isalpha(first) || first == '_') {
    SIndex endIdx = SStringSeekNoAlphaNumberUnderscore(lexer->source, idx);
    if (endIdx == SIndexNotFound) {
      endIdx = lexer->source->length;
    }
    SStringRef sema = SStringCreateIL(lexer->source->string, idx, endIdx - idx);
    //  Check keywords
    STokenRef token = NULL;
    if (strcmp(sema->string, "class") == 0) {
      token = STokenCreate(STokenClass);
    } else if (strcmp(sema->string, "var") == 0) {
      token = STokenCreate(STokenVar);
    } else if (strcmp(sema->string, "weak") == 0) {
      token = STokenCreate(STokenWeak);
    } else if (strcmp(sema->string, "end") == 0) {
      token = STokenCreate(STokenEnd);
    } else if (strcmp(sema->string, "module") == 0) {
      token = STokenCreate(STokenModule);
    } else if (strcmp(sema->string, "native") == 0) {
      token = STokenCreate(STokenNative);
    } else if (strcmp(sema->string, "require") == 0) {
      token = STokenCreate(STokenRequire);
    } else if (strcmp(sema->string, "include") == 0) {
      token = STokenCreate(STokenInclude);
    } else if (strcmp(sema->string, "static") == 0) {
      token = STokenCreate(STokenStatic);
    } else if (strcmp(sema->string, "func") == 0) {
      token = STokenCreate(STokenFunc);
    } else if (strcmp(sema->string, "super") == 0) {
      token = STokenCreate(STokenSuper);
    } else if (strcmp(sema->string, "self") == 0) {
      token = STokenCreate(STokenSelf);
    } else if (strcmp(sema->string, "nil") == 0) {
      token = STokenCreate(STokenNil);
    } else if (strcmp(sema->string, "true") == 0) {
      token = STokenCreate(STokenTrue);
    } else if (strcmp(sema->string, "false") == 0) {
      token = STokenCreate(STokenFalse);
    } else if (strcmp(sema->string, "if") == 0) {
      token = STokenCreate(STokenIf);
    } else if (strcmp(sema->string, "else") == 0) {
      token = STokenCreate(STokenElse);
    } else if (strcmp(sema->string, "unless") == 0) {
      token = STokenCreate(STokenUnless);
    } else if (strcmp(sema->string, "while") == 0) {
      token = STokenCreate(STokenWhile);
    } else if (strcmp(sema->string, "break") == 0) {
      token = STokenCreate(STokenBreak);
    } else if (strcmp(sema->string, "for") == 0) {
      token = STokenCreate(STokenFor);
    } else if (strcmp(sema->string, "in") == 0) {
      token = STokenCreate(STokenIn);
    } else if (strcmp(sema->string, "return") == 0) {
      token = STokenCreate(STokenReturn);
    } else {
      //  No keyword found, make it ID
      token = STokenCreateString(STokenId, sema->string);
    }
    SStringDestroy(sema);
    if (err) {
      *err = SLexerErrorOK;
    }
    lexer->index = endIdx;
    return token;
  }

  //  Marks and operators
  {
    SIndex nextIdx = 0, endIdx = idx;
    STokenRef token = NULL;
    switch (first) {
    case ':': {
      endIdx += 1;
      token = STokenCreate(STokenColon);
    }
      break;
    case ';': {
      endIdx += 1;
      token = STokenCreate(STokenSemicolon);
    }
      break;
    case ',': {
      endIdx += 1;
      token = STokenCreate(STokenComma);
    }
      break;
    case '+': {
      endIdx += 1;
      token = STokenCreate(STokenPlus);
    }
      break;
    case '-': {
      endIdx += 1;
      nextIdx = SStringSeekNoBlank(lexer->source, endIdx);
      if (nextIdx != SIndexNotFound && lexer->source->string[nextIdx] == '>') {
        endIdx = nextIdx + 1;
        token = STokenCreate(STokenArrow);
      } else {
        token = STokenCreate(STokenMinus);
      }
    }
      break;
    case '*': {
      endIdx += 1;
      token = STokenCreate(STokenMultiply);
    }
      break;
    case '/': {
      endIdx += 1;
      token = STokenCreate(STokenDivide);
    }
      break;
    case '%': {
      endIdx += 1;
      token = STokenCreate(STokenMod);
    }
      break;
    case '=': {
      endIdx += 1;
      nextIdx = SStringSeekNoBlank(lexer->source, endIdx);
      if (nextIdx != SIndexNotFound && lexer->source->string[nextIdx] == '=') {
        SIndex nextNextIdx = SStringSeekNoBlank(lexer->source, nextIdx);
        if (nextNextIdx != SIndexNotFound && lexer->source->string[nextNextIdx] == '=') {
          endIdx = nextNextIdx + 1;
          token = STokenCreate(STokenIdentical);
        } else {
          endIdx = nextIdx + 1;
          token = STokenCreate(STokenEqTo);
        }
      } else {
        token = STokenCreate(STokenEq);
      }
    }
      break;
    case '<': {
      endIdx += 1;
      nextIdx = SStringSeekNoBlank(lexer->source, endIdx);
      if (nextIdx != SIndexNotFound && lexer->source->string[nextIdx] == '<') {
        endIdx = nextIdx + 1;
        token = STokenCreate(STokenBitLeftShift);
      } else if (nextIdx != SIndexNotFound && lexer->source->string[nextIdx] == '=') {
        endIdx = nextIdx + 1;
        token = STokenCreate(STokenLtEq);
      } else if (nextIdx != SIndexNotFound && lexer->source->string[nextIdx] == '>') {
        endIdx = nextIdx + 1;
        token = STokenCreate(STokenCompare);
      } else {
        token = STokenCreate(STokenLt);
      }
    }
      break;
    case '>': {
      endIdx += 1;
      nextIdx = SStringSeekNoBlank(lexer->source, endIdx);
      if (nextIdx != SIndexNotFound && lexer->source->string[nextIdx] == '>') {
        endIdx = nextIdx + 1;
        token = STokenCreate(STokenBitRightShift);
      } else if (nextIdx != SIndexNotFound && lexer->source->string[nextIdx] == '=') {
        endIdx = nextIdx + 1;
        token = STokenCreate(STokenGtEq);
      } else {
        token = STokenCreate(STokenGt);
      }
    }
      break;
    case '!': {
      endIdx += 1;
      nextIdx = SStringSeekNoBlank(lexer->source, endIdx);
      if (nextIdx != SIndexNotFound && lexer->source->string[nextIdx] == '=') {
        endIdx = nextIdx + 1;
        token = STokenCreate(STokenNotEq);
      } else {
        token = STokenCreate(STokenNot);
      }
    }
      break;
    case '|': {
      endIdx += 1;
      nextIdx = SStringSeekNoBlank(lexer->source, endIdx);
      if (nextIdx != SIndexNotFound && lexer->source->string[nextIdx] == '|') {
        endIdx = nextIdx + 1;
        token = STokenCreate(STokenOr);
      } else {
        token = STokenCreate(STokenBitOr);
      }
    }
      break;
    case '&': {
      endIdx += 1;
      nextIdx = SStringSeekNoBlank(lexer->source, endIdx);
      if (nextIdx != SIndexNotFound && lexer->source->string[nextIdx] == '&') {
        endIdx = nextIdx + 1;
        token = STokenCreate(STokenAnd);
      } else {
        token = STokenCreate(STokenBitAnd);
      }
    }
      break;
    case '^': {
      endIdx += 1;
      token = STokenCreate(STokenBitXor);
    }
      break;
    case '~': {
      endIdx += 1;
      token = STokenCreate(STokenTilde);
    }
      break;
    case '?': {
      endIdx += 1;
      token = STokenCreate(STokenQuestion);
    }
      break;
    case '(': {
      endIdx += 1;
      token = STokenCreate(STokenParenL);
    }
      break;
    case ')': {
      endIdx += 1;
      token = STokenCreate(STokenParenR);
    }
      break;
    case '[': {
      endIdx += 1;
      token = STokenCreate(STokenBracketL);
    }
      break;
    case ']': {
      endIdx += 1;
      token = STokenCreate(STokenBracketR);
    }
      break;
    case '{': {
      endIdx += 1;
      token = STokenCreate(STokenBraceL);
    }
      break;
    case '}': {
      endIdx += 1;
      token = STokenCreate(STokenBraceR);
    }
      break;
    case '.': {
      endIdx += 1;
      token = STokenCreate(STokenDot);
    }
      break;
    default:break;
    }

    if (token != NULL) {
      lexer->index = endIdx;
      return token;
    }
  }

  if (err)
    *err = SLexerErrorInvalidSequence;
  if (errIndex)
    *errIndex = idx;

  return NULL;
}

void SLexerReset(SLexerRef lexer) {
  lexer->index = 0;
}

void SLexerDestroy(SLexerRef lexer) {
  free(lexer);
}
