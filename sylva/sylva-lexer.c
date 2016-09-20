//
//  sylva-lexer.c
//  sylva
//
//  Created by Yanke Guo on 2016/9/20.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#include "sylva.h"
#include "sylva-lexer.h"

#include <stdlib.h>
#include <string.h>

sylva_lexer_ref sylva_lexer_create(int tokens_len) {
	sylva_lexer_ref lexer = malloc(sizeof(sylva_lexer));
	lexer->tokens_count = 0;
	lexer->tokens_capacity = tokens_len;
	lexer->tokens = malloc(sizeof(sylva_token) * tokens_len);
	return lexer;
}

sylva_lexer_error sylva_lexer_scan(sylva_lexer_ref lexer, char * source, int source_len, int * err_pos) {
	return sylva_lexer_error_ok;
}

void sylva_lexer_clear(sylva_lexer_ref lexer) {
	memset(lexer->tokens, 0, sizeof(sylva_token) * lexer->tokens_count);
	lexer->tokens_count = 0;
}

void sylva_lexer_destroy(sylva_lexer_ref lexer) {
	free(lexer->tokens);
	free(lexer);
}