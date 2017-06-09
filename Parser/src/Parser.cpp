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

Node Parser::parse() {
	this->nextToken();  // < TODO
	return this->prog();
}

/* PROG  ::=  DECLS  STATEMENTS
 * -------------------------------------
 * "identifier"		-> DECLS STATEMENTS
 * "int"			-> DECLS STATEMENTS
 * "write"			-> DECLS STATEMENTS
 * "read"			-> DECLS STATEMENTS
 * "while"			-> DECLS STATEMENTS
 * "if"				-> DECLS STATEMENTS
 * "{"				-> DECLS STATEMENTS
 */
Node Parser::prog() {
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
	case TokenType::TokenInt:
	case TokenType::TokenWrite:
	case TokenType::TokenRead:
	case TokenType::TokenWhile:
	case TokenType::TokenIf:
	case TokenType::TokenCurlyBracesOpen:
		this->decls();	// last action of function chain shall be nextToken();
		this->nextToken();
		this->statements();
		break;
	default:
		this->error();
	}

	return * new Node();
}

/* DECLS  ::=  DECL  ";"  DECLS  |  €
 * -----------------------------------
 * "int"			-> DECL ; DECLS
 * "identifier"		-> €
 * "write"			-> €
 * "read"			-> €
 * "while"			-> €
 * "if"				-> €
 * "{"				-> €
 */
Node Parser::decls() {
	switch (this->current_token.type) {
	case TokenType::TokenInt:
		this->decl();
		if (this->current_token.type == TokenType::TokenSemiColon) {
			// TODO success: addToParseTree(this->current_token);
			this->nextToken();
			this->decls();
		} else {
			this->error();
		}
		break;
	case TokenType::TokenIdentifier:
	case TokenType::TokenWrite:
	case TokenType::TokenRead:
	case TokenType::TokenWhile:
	case TokenType::TokenIf:
	case TokenType::TokenCurlyBracesOpen:
		break;
	default:
		this->error();
	}

	return * new Node();
}

/* DECL  ::=  "int"  ARRAY  identifier
 * ------------------------------------
 * "int"			-> int ARRAY identifier
 */
Node Parser::decl() {
	if (this->current_token.type == TokenType::TokenInt) {
		// TODO success: addToParseTree(this->current_token);
		this->nextToken();
		this->array();
		if (this->current_token.type == TokenType::TokenIdentifier) {
			this->terminalId();
			this->nextToken();
		} else {
			this->error();
		}
	} else {
		this->error();
	}

	return * new Node();
}

/* ARRAY  ::=  "["  integer  "]"  |  €
 * ------------------------------------
 * "["				-> [ integer ]
 * "identifier"		-> €
 */
Node Parser::array() {
	switch (this->current_token.type) {
	case TokenType::TokenSquareBracketsOpen:
		// TODO success: addToParseTree(this->current_token);
		this->nextToken();
		if (this->current_token.type == TokenType::TokenInteger) {
			this->terminalInt();
			this->nextToken();
			if (this->current_token.type == TokenType::TokenSquareBracketsClose) {
				// TODO success: addToParseTree(this->current_token);
				this->nextToken();
			} else {
				this->error();
			}
		} else {
			this->error();
		}
		break;
	case TokenType::TokenIdentifier:
		break;
	default:
		this->error();
	}

	return * new Node();
}

/* STATEMENTS  ::=  STATEMENT  ";"  STATEMENTS  |  €
 * --------------------------------------------------
 * "identifier"		-> STATEMENT ; STATEMENTS
 * "write"			-> STATEMENT ; STATEMENTS
 * "read"			-> STATEMENT ; STATEMENTS
 * "while"			-> STATEMENT ; STATEMENTS
 * "if"				-> STATEMENT ; STATEMENTS
 * "{"				-> STATEMENT ; STATEMENTS
 * "}"				-> €
 */
Node Parser::statements() {
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
	case TokenType::TokenWrite:
	case TokenType::TokenRead:
	case TokenType::TokenWhile:
	case TokenType::TokenIf:
	case TokenType::TokenCurlyBracesOpen:
		this->statement();
		if (this->current_token.type == TokenType::TokenSemiColon) {
			// TODO success: addToParseTree(this->current_token);
			this->nextToken();
			this->statements();
		} else {
			this->error();
		}
		break;
	case TokenType::TokenCurlyBracesClose:
		break;
	default:
		this->error();
	}

	return * new Node();
}

/* STATEMENT  ::=  identifier  INDEX  ":="  EXP  |
 *                 "write"  "("  EXP  ")"  |
 *                 "read"  "("  identifier  INDEX  ")"  |
 *                 "{"  STATEMENTS  "}"  |
 *                 "if"  "("  EXP  ")"  STATEMENT  "else"  STATEMENT  |
 *                 "while"  "("  EXP  ")"  STATEMENT
 * ---------------------------------------------------------------------
 * "identifier"		-> identifier INDEX := EXP
 * "write"			-> write ( EXP )
 * "read"			-> read ( identifier INDEX )
 * "while"			-> while ( EXP ) STATEMENT
 * "if"				-> if ( EXP ) STATEMENT else STATEMENT
 * "{"				-> { STATEMENTS }
 * "else"			-> €
 * ";"				-> €
 */
Node Parser::statement() {
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
		this->terminalId();
		this->nextToken();
		this->index();
		if (this->current_token.type == TokenType::TokenColonEquals) {
			// TODO success: addToParseTree(this->current_token);
			this->nextToken();
			this->exp();
		} else {
			this->error();
		}
		break;
	case TokenType::TokenWrite:
		// TODO success: addToParseTree(this->current_token);
		this->nextToken();
		if (this->current_token.type == TokenType::TokenParenthesisOpen) {
			// TODO success: addToParseTree(this->current_token);
			this->nextToken();
			this->exp();
			if (this->current_token.type == TokenType::TokenParenthesisClose) {
				// TODO success: addToParseTree(this->current_token);
				this->nextToken();
			} else {
				this->error();
			}
		} else {
			this->error();
		}
		break;
	case TokenType::TokenRead:
		// TODO success: addToParseTree(this->current_token);
		this->nextToken();
		if (this->current_token.type == TokenType::TokenParenthesisOpen) {
			// TODO success: addToParseTree(this->current_token);
			this->nextToken();
			if (this->current_token.type == TokenType::TokenIdentifier) {
				this->terminalId();
				this->nextToken();
				this->index();
				if (this->current_token.type == TokenType::TokenParenthesisClose) {
					// TODO success: addToParseTree(this->current_token);
					this->nextToken();
				} else {
					this->error();
				}
			} else {
				this->error();
			}
		} else {
			this->error();
		}
		break;
	case TokenType::TokenWhile:
		// TODO success: addToParseTree(this->current_token);
		this->nextToken();
		if (this->current_token.type == TokenType::TokenParenthesisOpen) {
			// TODO success: addToParseTree(this->current_token);
			this->nextToken();
			this->exp();
			if (this->current_token.type == TokenType::TokenParenthesisClose) {
				// TODO success: addToParseTree(this->current_token);
				this->nextToken();
				this->statement();
			} else {
				this->error();
			}
		} else {
			this->error();
		}
		break;
	case TokenType::TokenIf:
		// TODO success: addToParseTree(this->current_token);
		this->nextToken();
		if (this->current_token.type == TokenType::TokenParenthesisOpen) {
			// TODO success: addToParseTree(this->current_token);
			this->nextToken();
			this->exp();
			if (this->current_token.type == TokenType::TokenParenthesisClose) {
				// TODO success: addToParseTree(this->current_token);
				this->nextToken();
				this->statement();
				if (this->current_token.type == TokenType::TokenElse) {
					// TODO success: addToParseTree(this->current_token);
					this->nextToken();
					this->statement();
				} else {
					this->error();
				}
			} else {
				this->error();
			}
		} else {
			this->error();
		}
		break;
	case TokenType::TokenCurlyBracesOpen:
		// TODO success: addToParseTree(this->current_token);
		this->nextToken();
		this->statements();
		if (this->current_token.type == TokenType::TokenCurlyBracesClose) {
			// TODO success: addToParseTree(this->current_token);
			this->nextToken();
		} else {
			this->error();
		}
		break;
	case TokenType::TokenElse:
	case TokenType::TokenSemiColon:
		break;
	default:
		this->error();
	}
	return * new Node();
}

/* EXP  ::=  EXP2 OP_EXP
 * ---------------------------
 * "identifier"	-> EXP2 OP_EXP
 * "("			-> EXP2 OP_EXP
 * "-"			-> EXP2 OP_EXP
 * "!"			-> EXP2 OP_EXP
 * "integer"	-> EXP2 OP_EXP
 */
Node Parser::exp() {
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
	case TokenType::TokenParenthesisOpen:
	case TokenType::TokenMinus:
	case TokenType::TokenExclamationMark:
	case TokenType::TokenInteger:
		this->exp2();
		this->nextToken();
		this->op_exp();
		break;
	default:
		this->error();
	}

	return * new Node();
}

/* EXP2  ::=  "("  EXP  ")"  |
 *            identifier  INDEX  |
 *            integer  |
 *            "-"  EXP2  |
 *            "!"  EXP2
 * ------------------------------------
 * "identifier"		-> identifier INDEX
 * "("				-> ( EXP )
 * "-"				-> - EXP2
 * "!"				-> ! EXP2
 * "integer"		-> integer
 */
Node Parser::exp2() {
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
		this->terminalId();
		this->nextToken();
		this->index();
		break;
	case TokenType::TokenParenthesisOpen:
		// TODO success: addToParseTree(this->current_token);
		this->nextToken();
		this->exp();
		if (this->current_token.type == TokenType::TokenParenthesisClose) {
			// TODO success: addToParseTree(this->current_token);
			this->nextToken();
		} else {
			this->error();
		}
		break;
	case TokenType::TokenMinus:
	case TokenType::TokenExclamationMark:
		// TODO success: addToParseTree(this->current_token);
		this->nextToken();
		this->exp2();
		break;
	case TokenType::TokenInteger:
		// TODO success: addToParseTree(this->current_token);
		this->nextToken();
		break;
	default:
		this->error();
	}

	return * new Node();
}

/* INDEX  ::=  "["  EXP  "]"  |  €
 * --------------------------------
 * "["		-> [ EXP ]
 * ")"		-> €
 * ";"		-> €
 * "+"		-> €
 * "-"		-> €
 * "*"		-> €
 * ":"		-> €
 * "<"		-> €
 * ">"		-> €
 * "="		-> €
 * "&&"		-> €
 */
Node Parser::index() {
	switch (this->current_token.type) {
	case TokenType::TokenSquareBracketsOpen:
		// TODO success: addToParseTree(this->current_token);
		this->nextToken();
		this->exp();
		if (this->current_token.type == TokenType::TokenSquareBracketsClose) {
			// TODO success: addToParseTree(this->current_token);
			this->nextToken();
		} else {
			this->error();
		}
		break;
	case TokenType::TokenParenthesisClose:
	case TokenType::TokenSemiColon:
	case TokenType::TokenPlus:
	case TokenType::TokenMinus:
	case TokenType::TokenStar:
	case TokenType::TokenColon:
	case TokenType::TokenLessThan:
	case TokenType::TokenGreaterThan:
	case TokenType::TokenEquals:
	case TokenType::TokenAndAnd:
		break;
	default:
		this->error();
	}

	return * new Node();
}

/* OP_EXP  ::=  OP  EXP  |  €
 * ---------------------------
 * "else" 	-> €
 * "]"		-> €
 * ")"		-> €
 * ";"		-> €
 * "+"		-> OP EXP
 * "-"		-> OP EXP
 * "*"		-> OP EXP
 * ":"		-> OP EXP
 * "<"		-> OP EXP
 * ">"		-> OP EXP
 * "="		-> OP EXP
 * "&&"		-> OP EXP
 */
Node Parser::op_exp() {
	switch (this->current_token.type) {
	case TokenType::TokenElse:
	case TokenType::TokenSquareBracketsClose:
	case TokenType::TokenParenthesisClose:
	case TokenType::TokenSemiColon:
		break;
	case TokenType::TokenPlus:
	case TokenType::TokenMinus:
	case TokenType::TokenStar:
	case TokenType::TokenColon:
	case TokenType::TokenLessThan:
	case TokenType::TokenGreaterThan:
	case TokenType::TokenEquals:
	case TokenType::TokenAndAnd:
		this->op();
		this->nextToken();
		this->exp();
		break;
	default:
		this->error();
	}

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
		this->error();
	}
	return * new Node();
}

/* identifier */
Node Parser::terminalId() {
	// TODO success: addToParseTree(this->current_token);
	return * new Node();
}

/* integer */
Node Parser::terminalInt() {
	// TODO success: addToParseTree(this->current_token);
	return * new Node();
}

void Parser::nextToken() {
	// increment a counter?
	// this->last_token = this->current_token; ?
	// do some other management?
	this->current_token = this->scanner->nextToken();
}

void Parser::error() {
	errno = EINVAL;
	perror("Illegal Token Sequence");
	exit(EXIT_FAILURE);
}
