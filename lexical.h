
#ifndef __LEXICAL_H__
#define __LEXICAL_H__

#include <vector>
#include <string>

#include "reg.h"
#include "parser.h"

typedef struct Token_reg Token_reg;

class LexicalAn {
public:
	LexicalAn (std::vector<Token_reg*> list, std::string s);

	std::string getToken ();

private:
	std::string s;
	std::vector<Token_reg*> list;
	int p = 0;
};

#endif