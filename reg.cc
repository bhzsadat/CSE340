#include "reg.h"
#include <cstdlib>
#include <set>
#include "lexer.h"
#include <iostream>


struct REG * parse_regular_expression () {

	return NULL;

}

std::set<REG_node*> match_one_char (std::set<REG_node*> s, char c) {

	std::set<REG_node*> sPrime;
	std::set<REG_node*>::iterator it_first;
	std::set<REG_node*>::iterator it_second;

		for (auto n : s) {
			//std::cout << "c is " << c << " labels are " << n->first_label << "," << n->second_label << std::endl; 
			//it_first = s.find(n->first_neighbor);
			if (n->first_label == c ) {
				//std::cout << "Been able to go to another first node with label " << c << std::endl;
				sPrime.insert (n->first_neighbor);
			}
			//it_second = s.find(n->second_neighbor);
			if (n->second_label == c ) {
				//std::cout << "Been able to go to another second node with label " << c << std::endl;
				sPrime.insert (n->second_neighbor);
			}
		}
	if (sPrime.empty()) {
		//std::cout << "Sprime is already empty!" << std::endl;
		return sPrime;
	}

	bool changed = true;
	while (changed) {
		changed = false;
		std::set<REG_node*>::iterator it;
		for (int i  = 0 ; i < 20; i++) {
			for (it = sPrime.begin(); it != sPrime.end(); ++it){
				REG_node *n = *it;
			//for (auto n : sPrime) {
				//std::cout << "Check for one node in sPrime and see if we can goo anywhere with _" << std::endl;
				//if (n == NULL) std::cout << "woops it's null" << std::endl;
				if (n->first_neighbor != NULL) {
	 				it_first = sPrime.find(n->first_neighbor);
					if (n->first_label == UNDERSCORE && it_first == sPrime.end()) {
						//std::cout << "Inserted to sprime with first neighbor" << std::endl;
						sPrime.insert (n->first_neighbor);
						changed = true;
						//it = sPrime.begin();
					}
				}
				if (n->second_neighbor != NULL) {
					it_second = sPrime.find(n->second_neighbor);
					if (n->second_label == UNDERSCORE && it_second == sPrime.end()) {
						//std::cout << "Inserted to sprime with second neighbor" << std::endl;
						sPrime.insert (n->second_neighbor);
						changed = true;
						//it = sPrime.begin();
					}
				}
			}
		}
		
	}

	//std::cout << "At the end the sprime size is " << sPrime.size() << std::endl;

	return sPrime;

}

std::string match (REG* r, std::string s, int p) {
	std::string str= "";
	std::set<REG_node*> totalSet;
	totalSet.insert(r->starting);
	int totalMatch = 0;
	int start = p;

	std::set<REG_node*>::iterator it;

	for (int i = 0; i < 20; i++) {
		for (it = totalSet.begin(); it != totalSet.end(); it++){
			//std::cout << "Searchiiiiing!" << std::endl;
			std::set<REG_node*>::iterator it_first;
			std::set<REG_node*>::iterator it_second;
			//for (auto n : totalSet) {
			REG_node *n = *it;
			//std::cout << "Extracted " << n << std::endl;
			it_first = totalSet.find(n->first_neighbor);
			if (n->first_label == UNDERSCORE && it_first == totalSet.end()) {
				//std::cout << "Inserting first " << n->first_neighbor << std::endl;
				totalSet.insert(n->first_neighbor);
				it = totalSet.begin();
			}
			it_second = totalSet.find(n->second_neighbor);
			if (n->second_label == UNDERSCORE && it_second == totalSet.end()) {
				//std::cout << "Inserting second " << n->second_neighbor << std::endl;
				totalSet.insert (n->second_neighbor);
				it = totalSet.begin();
			}
		}
	}

	//std::cout << "[match]Size of set after initialization is " << totalSet.size() << std::endl;

	while (p < s.length()) {
		std::set<REG_node*>::iterator it_first;
		char temp = s[p];
		if (temp == ' ') {
			s.substr(start, totalMatch);
		}
		//std::cout << "[match]Read one char as " << temp << std::endl;
		totalSet = match_one_char(totalSet, temp);
		it_first = totalSet.find(r->accepting);
		p++;

		if (it_first != totalSet.end()) {
			//std::cout << "[match]Found a final node at this stage!" << std::endl;
			totalMatch = p - start;
		}

		if (totalSet.empty()) {
			break;
		}
	}


	return s.substr(start, totalMatch);
	
}


/*
void printReg(REG* r) {

	//std::cout << "Start Printing" << std::endl;

	std::vector<REG_node*> current;
	std::vector<REG_node*> processed;
	current.push_back (r->starting);

	int counter = 0;
	while (true){
		//std::cout << "Before Back()" << std::endl;
		REG_node* reg_node = current.back();
		//std::cout << "Got a regnode" << std::endl;
		if (current.size() == 0) break;
		//std::cout << "It's not null" << std::endl;
		current.pop_back();
		processed.push_back (reg_node);
		//std::cout << "Reg node being pushed back into the processed!" << std::endl;
		//if (reg_node->first_neighbor == NULL) std::cout << "woops! Reg node is null :(" << std::endl;
		//else std::cout << "Reg node is not null" << std::endl;
		if (reg_node->first_neighbor != NULL) {
			//std::cout << "first neighbor is not null" << std::endl;
			//std::cout << "reg node " << counter 
			//	 << " goes to first neighbor node with label " 
			//	 << reg_node->first_label << std::endl;
			if (std::find(processed.begin(), processed.end(), reg_node->first_neighbor) == processed.end()) {
				current.push_back (reg_node->first_neighbor);
			} 
		}
		if (reg_node->second_neighbor != NULL) {
			//std::cout << "second neighbor is not null" << std::endl;
			//std::cout << "reg node " << counter 
			//	 << " goes to second neighbor node with label " 
			//	 << reg_node->second_label << std::endl;
			if (std::find(processed.begin(), processed.end(), reg_node->second_neighbor) == processed.end()) {
				current.push_back (reg_node->second_neighbor);
			} 
		}
		counter++;
	}

}
*/