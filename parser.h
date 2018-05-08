/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"
#include "reg.h"
#include <vector>
#include "lexical.h"
#include <iostream>

typedef struct Token_reg Token_reg;
struct Token_reg {
    std::string lexeme;
    REG* reg;
};

class Parser {
  private:
    LexicalAnalyzer lexer;
    std::vector<Token_reg*> tokenRegs;

    void syntax_error();
    Token expect(TokenType expected_type);
    Token peek();


    Token parse_input();
    void parse_char_list();
    void parse_tokens_section();
    void parse_token_list();
    void parse_token();
    REG* parse_expr();


  public:
    void ParseProgram();

    void printTokenRegs();
};



#endif

