/*
 * Parser.h
 *
 *  Created on: May 18, 2017
 *      Author: donmanu
 */

#ifndef PARSER_INCLUDES_PARSER_H_
#define PARSER_INCLUDES_PARSER_H_

#include "../../Scanner/includes/Scanner.h"
#include "ParseTree.h"

class Parser {
public:
	Parser(char*);
	~Parser();

	Node prog();
	Node decls();
	Node decl();
	Node array();
	Node statements();
	Node statement();
	Node exp();
	Node exp2();
	Node index();
	Node op_exp();
	Node op();

private:
	Scanner* scanner;
	Token* current_token;
};

#endif /* PARSER_INCLUDES_PARSER_H_ */
