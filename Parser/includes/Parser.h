/*
 * Parser.h
 *
 *  Created on: May 18, 2017
 *      Author: donmanu
 */

#ifndef PARSER_INCLUDES_PARSER_H_
#define PARSER_INCLUDES_PARSER_H_

#include "../../Scanner/includes/Scanner.h"
#include "ParseTree.h"  // incl "RuleType.h"

class Parser {
	public:
		Parser(char*);
		~Parser(void);
		void parse(void);
		void checkType(void);
		// TODO void makeCode(char*);

		void debugPrint(void);

	private:
		void prog(void);
		void decls(void);
		void decl(void);
		void array(void);
		void statements(void);
		void statement(void);
		void exp(void);
		void exp2(void);
		void index(void);
		void op_exp(void);
		void op(void);

		void addTerminalId(void);
		void addTerminalInt(void);
		void addTerminalNode(void);
		void addNonTerminalNode(void);
		void addToParseTree(Node*);

		void nextToken(void);
		void errorParse(void);

		void checkType(Node*);
		void errorType(Node*);

		Scanner* scanner;
		Node* current_node;
		ParseTree* parse_tree;
		Token current_token;
		RuleType::Type current_rule;
};

#endif /* PARSER_INCLUDES_PARSER_H_ */
