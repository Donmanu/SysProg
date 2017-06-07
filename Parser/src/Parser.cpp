/*
 * Parser.cpp
 *
 *  Created on: May 18, 2017
 *      Author: donmanu
 */

#include "../includes/Parser.h" // contains scanner.h

/*------------- Parser --------------*/

/*
TokenUnknown,                   //0
TokenInteger,					//1
TokenIdentifier,				//2
TokenPlus,						//3
TokenMinus,						//4
TokenColon,						//5
TokenStar,						//6
TokenLessThan,					//7
TokenGreaterThan,				//8
TokenEquals,					//9
TokenColonEquals,				//10
TokenEqualsColonEquals,			//11
TokenExclamationMark,			//12
TokenAndAnd,					//13
TokenSemiColon,					//14
TokenParenthesisOpen,			//15
TokenParenthesisClose,			//16
TokenSquareBracketsOpen,		//17
TokenSquareBracketsClose,		//18
TokenCurlyBracesOpen,			//19
TokenCurlyBracesClose,			//20
TokenWrite,						//21
TokenRead,						//22
TokenIf,						//23
TokenWhile,						//24
TokenElse,						//25
TokenInt,						//26
TokenStop						//27
*/

Parser::Parser(char* input) {
	this->scanner = new Scanner(input);
	// TODO some try/catch error and args handling ...
	// go:
	this->current_token = this->scanner->nextToken();
	this->prog(); // TODO what to do with the return? parseTree.root = ?
}

Parser::~Parser() {
	delete this->scanner;
}

/* PROG  ::=  DECLS  STATEMENTS */
Node Parser::prog() {
	// nextToken();  < TODO
	// this->decls();      // last action of function chain shall be nextToken();
	// this->statements();
	return * new Node();
}

/* DECLS  ::=  DECL  ";"  DECLS  |  € */
Node Parser::decls() {
	return * new Node();
}

/* DECL  ::=  "int"  ARRAY  identifier */
Node Parser::decl() {
	return * new Node();
}

/* ARRAY  ::=  "["  integer  "]"  |  € */
Node Parser::array() {
	return * new Node();
}

/* STATEMENTS  ::=  STATEMENT  ";"  STATEMENTS  |  € */
Node Parser::statements() {
	return * new Node();
}

/* STATEMENT  ::=  identifier  INDEX  ":="  EXP  |
 *                 "write"  "("  EXP  ")"  |
 *                 "read"  "("  identifier  INDEX  ")"  |
 *                 "{"  STATEMENTS  "}"  |
 *                 "if"  "("  EXP  ")"  STATEMENT  "else"  STATEMENT  |
 *                 "while"  "("  EXP  ")"  STATEMENT
 */
Node Parser::statement() {
	return * new Node();
}

/* EXP  ::=  EXP2  OP  EXP */
Node Parser::exp() {
	return * new Node();
}

/* EXP2  ::=  "("  EXP  ")"  |
 *            identifier  INDEX  |
 *            integer  |
 *            "-"  EXP2  |
 *            "!"  EXP2
 */
Node Parser::exp2() {
	return * new Node();
}

/* INDEX  ::=  "["  EXP  "]"  |  € */
Node Parser::index() {
	return * new Node();
}

/* OP_EXP  ::=  OP  EXP  |  € */
Node Parser::op_exp() {
	return * new Node();
}

/* OP  ::=  "+"  |  "-"  |  "*"  |
 *          ":"  |  "<"  |  ">"  |
 *          "="  |  "=:="  |  "&&"
 */
Node Parser::op() {
	switch (this->current_token.type) {
	case TokenType::TokenPlus:
	case TokenType::TokenMinus:
	case TokenType::TokenStar:
	case TokenType::TokenColon:
	case TokenType::TokenLessThan:
	case TokenType::TokenGreaterThan:
	case TokenType::TokenEquals:
	case TokenType::TokenEqualsColonEquals:
	case TokenType::TokenAndAnd:
		// TODO success: addToParseTree(this->current_token);
		nextToken();
		break;
	default:
		// TODO error
		break;
	}
	return * new Node();
}

/* identifier */
Node Parser::terminalId() {
	return * new Node();
}

/* integer */
Node Parser::terminalInt() {
	return * new Node();
}

void Parser::nextToken() {
	// increment a counter?
	// this->last_token = this->current_token; ?
	// do some other management?
	this->current_token = this->scanner->nextToken();
}
