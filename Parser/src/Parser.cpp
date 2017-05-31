/*
 * Parser.cpp
 *
 *  Created on: May 18, 2017
 *      Author: donmanu
 */

#include "../includes/Parser.h"

Parser::Parser(char* input) {
	this->scanner = new Scanner(input);
	this->current_token = NULL;
}

Parser::~Parser() {
	delete(this->scanner);
}

Node Parser::prog() {
	return new Node();
}

Node Parser::decls() {
	return new Node();
}

Node Parser::decl() {
	return new Node();
}

Node Parser::array() {
	return new Node();
}

Node Parser::statements() {
	return new Node();
}

Node Parser::statement() {
	return new Node();
}

Node Parser::exp() {
	return new Node();
}

Node Parser::exp2() {
	return new Node();
}

Node Parser::index() {
	return new Node();
}

Node Parser::op_exp() {
	return new Node();
}

Node Parser::op() {
	return new Node();
}
