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
#include <iostream>		// code file output
#include <fstream> 		// after iostream

class Parser {
	public:
		Parser(char*);
		~Parser(void);
		void parse(void);
		void checkType(void);
		void makeCode(char*);

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
		void removeFromParseTree(Node*);

		void nextToken(void);
		void errorParse(void);

		void checkType(Node*);
		void errorType(Node*);

		void makeCode(Node*);

		std::ofstream code_file;
		Scanner* scanner;
		Node* current_node;
		ParseTree* parse_tree;
		Token current_token;
		RuleType::Type current_rule;
		bool is_epsilon_transition;
		bool came_from_statement;
};

#endif /* PARSER_INCLUDES_PARSER_H_ */
