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

}

Node Parser::decls() {

}

Node Parser::decl() {

}

Node Parser::array() {

}

Node Parser::statements() {

}

Node Parser::statement() {

}

Node Parser::exp() {

}

Node Parser::exp2() {

}

Node Parser::index() {

}

Node Parser::op_exp() {

}

Node Parser::op() {

}
