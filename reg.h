#ifndef __REG_H__
#define __REG_H__

#include <set>
#include <string>
#include <iostream>
#include <vector>

typedef struct REG_node REG_node;
typedef struct REG REG;

struct REG_node {

	struct REG_node* first_neighbor;
	char first_label;
	struct REG_node* second_neighbor;
	char second_label;

};

struct REG {
	
	struct REG_node* starting;
	struct REG_node* accepting;

};

struct REG * parse_regular_expression();

std::set<REG_node*> match_one_char(std::set<REG_node*> s, char c);

std::string match (REG* r, std::string s, int p);

void printReg(REG* r);

#endif