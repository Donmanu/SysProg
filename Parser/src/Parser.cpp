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
	this->parse_tree = new ParseTree();
	this->current_node = this->parse_tree->getRoot();
}

Parser::~Parser() {
	delete this->scanner;
	//delete this->parse_tree;  // TODO Getting segFaults somewhere deep in the tree!! Got a Loop!?!
}

void Parser::parse() {
	this->prog();
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
void Parser::prog() {
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
	case TokenType::TokenInt:
	case TokenType::TokenWrite:
	case TokenType::TokenRead:
	case TokenType::TokenWhile:
	case TokenType::TokenIf:
	case TokenType::TokenCurlyBracesOpen:
	case TokenType::TokenStop: // TODO or break immediately?
		this->nonTerminalNode();
		this->decls();
		this->nonTerminalNode();
		this->statements();
		break;
	default:
		this->error();
	}
	this->current_node = this->current_node->getParent();
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
void Parser::decls() {
	switch (this->current_token.type) {
	case TokenType::TokenInt:
		this->nonTerminalNode();
		this->decl();
		if (this->current_token.type == TokenType::TokenSemiColon) {
			// TODO success: addToParseTree(this->current_token);
			this->terminalNode();
			this->nextToken();
			this->nonTerminalNode();
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
	case TokenType::TokenStop: // epsilon-DECLS
		break;
	default:
		this->error();
	}
	this->current_node = this->current_node->getParent();
}

/* DECL  ::=  "int"  ARRAY  identifier
 * ------------------------------------
 * "int"			-> int ARRAY identifier
 */
void Parser::decl() {
	if (this->current_token.type == TokenType::TokenInt) {
		// TODO success: addToParseTree(this->current_token);
		this->terminalInt();
		this->nextToken();
		this->nonTerminalNode();
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
	this->current_node = this->current_node->getParent();
}

/* ARRAY  ::=  "["  integer  "]"  |  €
 * ------------------------------------
 * "["				-> [ integer ]
 * "identifier"		-> €
 */
void Parser::array() {
	switch (this->current_token.type) {
	case TokenType::TokenSquareBracketsOpen:
		// TODO success: addToParseTree(this->current_token);
		this->terminalNode();
		this->nextToken();
		if (this->current_token.type == TokenType::TokenInteger) {
			this->terminalInt();
			this->nextToken();
			if (this->current_token.type == TokenType::TokenSquareBracketsClose) {
				// TODO success: addToParseTree(this->current_token);
				this->terminalNode();
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
	this->current_node = this->current_node->getParent();
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
void Parser::statements() {
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
	case TokenType::TokenWrite:
	case TokenType::TokenRead:
	case TokenType::TokenWhile:
	case TokenType::TokenIf:
	case TokenType::TokenCurlyBracesOpen:
		this->nonTerminalNode();
		this->statement();
		if (this->current_token.type == TokenType::TokenSemiColon) {
			// TODO success: addToParseTree(this->current_token);
			this->terminalNode();
			this->nextToken();
			this->nonTerminalNode();
			this->statements();
		} else {
			this->error();
		}
		break;
	case TokenType::TokenCurlyBracesClose:
	case TokenType::TokenStop:  // epsilon-STATEMENTS
		break;
	default:
		this->error();
	}
	this->current_node = this->current_node->getParent();
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
void Parser::statement() {
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
		// TODO success: addToParseTree(this->current_token);
		this->terminalId();
		this->nextToken();
		this->nonTerminalNode();
		this->index();
		if (this->current_token.type == TokenType::TokenColonEquals) {
			// TODO success: addToParseTree(this->current_token);
			this->terminalNode();
			this->nextToken();
			this->nonTerminalNode();
			this->exp();
		} else {
			this->error();
		}
		break;
	case TokenType::TokenWrite:
		// TODO success: addToParseTree(this->current_token);
		this->terminalNode();
		this->nextToken();
		if (this->current_token.type == TokenType::TokenParenthesisOpen) {
			// TODO success: addToParseTree(this->current_token);
			this->terminalNode();
			this->nextToken();
			this->nonTerminalNode();
			this->exp();
			if (this->current_token.type == TokenType::TokenParenthesisClose) {
				// TODO success: addToParseTree(this->current_token);
				this->terminalNode();
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
		this->terminalNode();
		this->nextToken();
		if (this->current_token.type == TokenType::TokenParenthesisOpen) {
			// TODO success: addToParseTree(this->current_token);
			this->terminalNode();
			this->nextToken();
			if (this->current_token.type == TokenType::TokenIdentifier) {
				this->terminalId();
				this->nextToken();
				this->nonTerminalNode();
				this->index();
				if (this->current_token.type == TokenType::TokenParenthesisClose) {
					// TODO success: addToParseTree(this->current_token);
					this->terminalNode();
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
		this->terminalNode();
		this->nextToken();
		if (this->current_token.type == TokenType::TokenParenthesisOpen) {
			// TODO success: addToParseTree(this->current_token);
			this->terminalNode();
			this->nextToken();
			this->nonTerminalNode();
			this->exp();
			if (this->current_token.type == TokenType::TokenParenthesisClose) {
				// TODO success: addToParseTree(this->current_token);
				this->terminalNode();
				this->nextToken();
				this->nonTerminalNode();
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
		this->terminalNode();
		this->nextToken();
		if (this->current_token.type == TokenType::TokenParenthesisOpen) {
			// TODO success: addToParseTree(this->current_token);
			this->terminalNode();
			this->nextToken();
			this->nonTerminalNode();
			this->exp();
			if (this->current_token.type == TokenType::TokenParenthesisClose) {
				// TODO success: addToParseTree(this->current_token);
				this->terminalNode();
				this->nextToken();
				this->nonTerminalNode();
				this->statement();
				if (this->current_token.type == TokenType::TokenElse) {
					// TODO success: addToParseTree(this->current_token);
					this->terminalNode();
					this->nextToken();
					this->nonTerminalNode();
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
		this->terminalNode();
		this->nextToken();
		this->nonTerminalNode();
		this->statements();
		if (this->current_token.type == TokenType::TokenCurlyBracesClose) {
			// TODO success: addToParseTree(this->current_token);
			this->terminalNode();
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
	this->current_node = this->current_node->getParent();
}

/* EXP  ::=  EXP2 OP_EXP
 * ---------------------------
 * "identifier"	-> EXP2 OP_EXP
 * "("			-> EXP2 OP_EXP
 * "-"			-> EXP2 OP_EXP
 * "!"			-> EXP2 OP_EXP
 * "integer"	-> EXP2 OP_EXP
 */
void Parser::exp() {
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
	case TokenType::TokenParenthesisOpen:
	case TokenType::TokenMinus:
	case TokenType::TokenExclamationMark:
	case TokenType::TokenInteger:
		this->nonTerminalNode();
		this->exp2();
		//this->nextToken(); already done in exp2
		this->nonTerminalNode();
		this->op_exp();
		break;
	default:
		this->error();
	}
	this->current_node = this->current_node->getParent();
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
void Parser::exp2() {
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
		this->terminalId();
		this->nextToken();
		this->nonTerminalNode();
		this->index();
		break;
	case TokenType::TokenParenthesisOpen:
		// TODO success: addToParseTree(this->current_token);
		this->terminalNode();
		this->nextToken();
		this->nonTerminalNode();
		this->exp();
		if (this->current_token.type == TokenType::TokenParenthesisClose) {
			// TODO success: addToParseTree(this->current_token);
			this->terminalNode();
			this->nextToken();
		} else {
			this->error();
		}
		break;
	case TokenType::TokenMinus:
	case TokenType::TokenExclamationMark:
		// TODO success: addToParseTree(this->current_token);
		this->terminalNode();
		this->nextToken();
		this->nonTerminalNode();
		this->exp2();
		break;
	case TokenType::TokenInteger:
		// TODO success: addToParseTree(this->current_token);
		this->terminalInt();
		this->nextToken();
		break;
	default:
		this->error();
	}
	this->current_node = this->current_node->getParent();
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
 * ":="		-> €
 */
void Parser::index() {
	switch (this->current_token.type) {
	case TokenType::TokenSquareBracketsOpen:
		// TODO success: addToParseTree(this->current_token);
		this->terminalNode();
		this->nextToken();
		this->nonTerminalNode();
		this->exp();
		if (this->current_token.type == TokenType::TokenSquareBracketsClose) {
			// TODO success: addToParseTree(this->current_token);
			this->terminalNode();
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
	case TokenType::TokenColonEquals:
		break;
	default:
		this->error();
	}
	this->current_node = this->current_node->getParent();
}

/* OP_EXP  ::=  OP  EXP  |  €
 * ---------------------------
 * "+"		-> OP EXP
 * "-"		-> OP EXP
 * "*"		-> OP EXP
 * ":"		-> OP EXP
 * "<"		-> OP EXP
 * ">"		-> OP EXP
 * "="		-> OP EXP
 * "&&"		-> OP EXP
 * "else" 	-> €
 * "]"		-> €
 * ")"		-> €
 * ";"		-> €
 */
void Parser::op_exp() {
	switch (this->current_token.type) {
	case TokenType::TokenPlus:
	case TokenType::TokenMinus:
	case TokenType::TokenStar:
	case TokenType::TokenColon:
	case TokenType::TokenLessThan:
	case TokenType::TokenGreaterThan:
	case TokenType::TokenEquals:
	case TokenType::TokenAndAnd:
		this->nonTerminalNode();
		this->op();
		//this->nextToken();
		this->nonTerminalNode();
		this->exp();
		break;
	case TokenType::TokenElse:
	case TokenType::TokenSquareBracketsClose:
	case TokenType::TokenParenthesisClose:
	case TokenType::TokenSemiColon:
		break;
	default:
		this->error();
	}
	this->current_node = this->current_node->getParent();
}

/* OP  ::=  "+"  |  "-"  |  "*"  |
 *          ":"  |  "<"  |  ">"  |
 *          "="  |  "=:="  |  "&&"
 */
void Parser::op() {
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
		this->terminalNode();
		nextToken();
		break;
	default:
		this->error();
	}
	this->current_node = this->current_node->getParent();
}

/* identifier */
void Parser::terminalId() {
	// TODO success: addToParseTree(this->current_token);
	Node* node = new NodeId(this->current_node, this->current_token.key->getInformation());
	this->addToParseTree(node);
}

/* integer */
void Parser::terminalInt() {
	// TODO success: addToParseTree(this->current_token);
	Node* node = new NodeInt(this->current_node, this->current_token.value);
	this->addToParseTree(node);
}

void Parser::terminalNode() {
	Node* node = new Node(this->current_node, this->current_token.type);
	this->addToParseTree(node);
}

void Parser::nonTerminalNode() {
	Node* node = new Node(this->current_node);
	this->addToParseTree(node);
}

void Parser::addToParseTree(Node* child) {
	// TODO: this->parse_tree->addNode(node); ?
	this->current_node->addChild(child);
	this->current_node = child;
}

void Parser::nextToken() {
	// increment a counter?
	// this->last_token = this->current_token; ?
	// do some other management?
	this->current_token = this->scanner->nextToken();
}

void Parser::error() {
	errno = EINVAL;
	printf("Unexpected token %s in line %d, column %d\n", TokenType::tokenNameShort[this->current_token.type], this->current_token.line, this->current_token.column);
	perror("Illegal Token Sequence");
	this->parse_tree->debugPrint();
	exit(EXIT_FAILURE);
}

void Parser::debugPrint() {
	this->scanner->debugPrint();
	this->parse_tree->debugPrint();
}
