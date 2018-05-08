#include "lexical.h"

LexicalAn::LexicalAn (std::vector<Token_reg*> list, std::string s) {

	//std::cout << "[LexicalAn::LexicalAn]list size is " << list.size () << " and input is " << s << std::endl;
	this->s = s;
	this->list = list;

}

std::string LexicalAn::getToken() {

	std::string max = "";
	int maxIndex = 0;

	if (s[p] == ' ') {
		p++;
		return "";
	} 
	
	for (int i = 0; i < list.size(); i++) {
		REG* currentReg = (list.at(i))->reg;
		std::string matched = match (currentReg, s, p);
		//std::cout << "[LexicalAn::getToken]Here we matched with " << matched << " for token " << (list.at(i))->lexeme << std::endl;
		if (matched.length() > max.length()) {
			max = matched;
			maxIndex = i;
		}
	}


	//if (max.length() == 0) std::cout << "Current final position is " << p << std::endl;
	if (max.length() == 0 && p != s.length()) return "!";
	if (max.length() == 0 && p == s.length()) return "?";

	p += max.length();

	return (list.at(maxIndex))->lexeme + " , " + "\"" + max + "\"";

}