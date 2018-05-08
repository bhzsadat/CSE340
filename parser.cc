/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include "parser.h"

using namespace std;

void Parser::syntax_error () {
    cout << "Syntax Error\n";
    exit (1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect (TokenType expected_type) {
    Token t = lexer.GetToken ();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// this function simply checks the next token without
// consuming the input
// Written by Mohsen Zohrevandi
Token Parser::peek () {
    Token t = lexer.GetToken ();
    lexer.UngetToken (t);
    return t;
}

// Parsing

Token Parser::parse_input () {
    //input -> tokens_section INPUT_TEXT
    parse_tokens_section ();
    Token t = expect (INPUT_TEXT);

    //cout << t.lexeme << endl;

    LexicalAn lexicalAn (tokenRegs, t.lexeme);
    while (true) {
        std::string matched = lexicalAn.getToken ();
        if (matched == "") continue;
        if (matched == " ") continue;
        if (matched == "!") {
            std::cout << "ERROR" << std::endl;
            break;
        }
        if (matched == "?") {
            break;
        }
        std::cout << matched << std::endl;
    }
    //cout << lexicalAn.getToken() << endl;
    //cout << lexicalAn.getToken() << endl;

    return t;
}

/*void Parser::parse_alphabet_section()
{
    //alphabet_section -> char_list HASH

    parse_char_list();
    expect(HASH);
}
*/


void Parser::parse_tokens_section () {
    // tokens_section -> token_list HASH


    parse_token_list ();
    expect (HASH);
}

void Parser::parse_token_list () {
    // token_list -> token
    // token_list -> token COMMA token_list
    //string str;
    parse_token ();
    Token t = peek ();
    if (t.token_type == COMMA) {
        // token_list -> token COMMA token_list
        expect (COMMA);
        parse_token_list ();
    }
    else if (t.token_type == HASH) {
        // token_list -> token
    }
    else {
        syntax_error ();
    }

}

void Parser::parse_token () {
    // token -> ID expr

    Token_reg *newTokenReg = new Token_reg;

    Token t = expect (ID);
    newTokenReg->lexeme = t.lexeme;

    REG* reg = parse_expr ();
    newTokenReg->reg = reg;

    //printReg(reg);

    //std::cout << "After Printing!" << std::endl;

    tokenRegs.push_back (newTokenReg);

}

REG* Parser::parse_expr () {
    // expr -> CHAR
    // expr -> LPAREN expr RPAREN DOT LPAREN expr RPAREN
    // expr -> LPAREN expr RPAREN OR LPAREN expr RPAREN
    // expr -> LPAREN expr RPAREN STAR
    // expr -> UNDERSCORE

    Token t = lexer.GetToken ();
    if(t.token_type == CHAR) {
        //cout << "[Parser::parse_expr] read a char with value " << t.lexeme << endl;
        
        REG_node *start = new REG_node;
        REG_node *end = new REG_node;

        start->first_neighbor = end;     
        start->first_label = ((t.lexeme).c_str())[0];
        start->second_neighbor = NULL;
        start->second_label = '#';

        end->first_neighbor = NULL;
        end->second_neighbor = NULL;
        end->first_label = '#';
        end->second_label = '#';

        REG *reg = new REG;
        reg->starting = start;
        reg->accepting = end;

        return reg;

    }
    else if (t.token_type == UNDERSCORE) {

        //cout << "[Parser::parse_expr] read an underscore" << endl;

        REG_node *start = new REG_node;
        REG_node *end = new REG_node;

        start->first_neighbor = end;     
        start->first_label = UNDERSCORE;
        start->second_neighbor = NULL;
        start->second_label = '#';

        end->first_neighbor = NULL;
        end->second_neighbor = NULL;
        end->first_label = '#';
        end->second_label = '#';

        REG *reg = new REG;
        reg->starting = start;
        reg->accepting = end;

        return reg;
    }
    else if (t.token_type == LPAREN) {
        //cout << "[Parser::parse_expr] read an parantheses" << endl;
        // expr -> LPAREN expr RPAREN DOT LPAREN expr RPAREN
        // expr -> LPAREN expr RPAREN OR LPAREN expr RPAREN
        // expr -> LPAREN expr RPAREN STAR
        REG *reg1 = parse_expr ();
        expect (RPAREN);
        Token t2 = lexer.GetToken ();
        if (t2.token_type == DOT || t2.token_type == OR) {
            //cout << "[Parser::parse_expr] read an DOT or OR" << endl;
            expect (LPAREN);
            REG* reg2 = parse_expr ();
            expect (RPAREN);

            if (t2.token_type == DOT) {
                //cout << "[Parser::parse_expr] read an DOT" << endl;
                if (reg1->accepting->first_neighbor == NULL) {
                    reg1->accepting->first_neighbor = reg2->starting;
                    reg1->accepting->first_label = UNDERSCORE;
                } 
                else if (reg1->accepting->second_neighbor == NULL) {
                    reg1->accepting->second_neighbor = reg2->starting;
                    reg1->accepting->second_label = UNDERSCORE;
                }
                //reg1->accepting->second_label = UNDERSCORE;

                REG* newReg = new REG;
                newReg->starting = reg1->starting;
                newReg->accepting = reg2->accepting;

                return newReg;

            } 
            else if (t2.token_type == OR) {
                //cout << "[Parser::parse_expr] read an OR" << endl;
                REG_node* startNode = new REG_node;
                REG_node* endNode = new REG_node;

                startNode->first_neighbor = reg1->starting;
                startNode->first_label = UNDERSCORE;
                startNode->second_neighbor = reg2->starting;
                startNode->second_label = UNDERSCORE;

                endNode->first_neighbor = NULL;
                endNode->second_neighbor = NULL;

                if (reg1->accepting->first_neighbor == NULL) {
                    reg1->accepting->first_neighbor = endNode;
                    reg1->accepting->first_label = UNDERSCORE;
                } 
                else if (reg1->accepting->second_neighbor == NULL) {
                    reg1->accepting->second_neighbor = endNode;
                    reg1->accepting->second_label = UNDERSCORE;
                }
                if (reg2->accepting->first_neighbor == NULL) {
                    reg2->accepting->first_neighbor = endNode;
                    reg2->accepting->first_label = UNDERSCORE;
                } 
                else if (reg2->accepting->second_neighbor == NULL) {
                    reg2->accepting->second_neighbor = endNode;
                    reg2->accepting->second_label = UNDERSCORE;
                }

                REG* newReg = new REG;
                newReg->starting = startNode;
                newReg->accepting = endNode;

                return newReg;

            }
        }
        else if (t2.token_type == STAR) {
            //cout << "[Parser::parse_expr] read an *" << endl;

            REG_node* startNode = new REG_node;
            REG_node* endNode = new REG_node;

            endNode->first_neighbor = NULL;
            endNode->second_neighbor = NULL;

            startNode->first_neighbor = reg1->starting;
            startNode->first_label = UNDERSCORE;
            startNode->second_neighbor = endNode;
            startNode->second_label = UNDERSCORE;

            reg1->accepting->first_neighbor = endNode;
            reg1->accepting->first_label = UNDERSCORE;
            reg1->accepting->second_neighbor = reg1->starting;
            reg1->accepting->second_label = UNDERSCORE;

            REG* newReg = new REG;
            newReg->starting = startNode;
            newReg->accepting = endNode;

            return newReg;

        }

    }
    else {
        syntax_error ();
    }
}

void Parser::ParseProgram () {
    parse_input ();
    expect (END_OF_FILE);
}

void Parser::printTokenRegs () {

    for (int i = 0; i < tokenRegs.size(); i++){
        Token_reg* temp = tokenRegs.at(i);
        cout << temp->lexeme << endl;

    }
}

int main () {
    Parser parser;

    parser.ParseProgram ();
    //parser.printTokenRegs ();



}
