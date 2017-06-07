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
		~Parser(void);

		Node prog(void);
		Node decls(void);
		Node decl(void);
		Node array(void);
		Node statements(void);
		Node statement(void);
		Node exp(void);
		Node exp2(void);
		Node index(void);
		Node op_exp(void);
		Node op(void);

		Node terminalId(void);
		Node terminalInt(void);

		void nextToken(void);

	private:
		Scanner* scanner;
		Token current_token;
};

#endif /* PARSER_INCLUDES_PARSER_H_ */
