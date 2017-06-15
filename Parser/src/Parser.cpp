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
	this->current_rule = RuleType::prog;
	this->current_node = NULL;
	this->parse_tree = NULL;
}

Parser::~Parser() {
	delete this->scanner;
	//delete this->parse_tree;  // TODO Getting segFaults somewhere deep in the tree!! Got a Loop!?!
}

void Parser::parse() {
	this->current_node = new Node(NULL, TokenType::TokenStop, this->current_rule); // init Root
	this->parse_tree = new ParseTree(this->current_node);
	this->current_token = this->scanner->nextToken();
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
	this->current_rule = RuleType::prog;
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
	case TokenType::TokenInt:
	case TokenType::TokenWrite:
	case TokenType::TokenRead:
	case TokenType::TokenWhile:
	case TokenType::TokenIf:
	case TokenType::TokenCurlyBracesOpen:
	case TokenType::TokenStop: // TODO or break immediately?
		this->decls();
		this->statements();
		break;
	default:
		this->errorParse();
	}
	this->current_node = this->current_node->getParent(); // back to the roots
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
	this->current_rule = RuleType::decls;
	switch (this->current_token.type) {
	case TokenType::TokenInt:
		this->addNonTerminalNode();
		this->decl();
		if (this->current_token.type == TokenType::TokenSemiColon) {
			this->addTerminalNode();
			this->nextToken();
			this->addNonTerminalNode();
			this->decls();
		} else {
			this->errorParse();
		}
		break;
	case TokenType::TokenIdentifier:
	case TokenType::TokenWrite:
	case TokenType::TokenRead:
	case TokenType::TokenWhile:
	case TokenType::TokenIf:
	case TokenType::TokenCurlyBracesOpen: // TODO how to check if they close?
	case TokenType::TokenStop: // epsilon-DECLS
		this->addNonTerminalNode();
		break;
	default:
		this->errorParse();
	}
	this->current_node = this->current_node->getParent();
}

/* DECL  ::=  "int"  ARRAY  identifier
 * ------------------------------------
 * "int"			-> int ARRAY identifier
 */
void Parser::decl() {
	this->current_rule = RuleType::decl;
	if (this->current_token.type == TokenType::TokenInt) {
		this->addTerminalNode();
		this->nextToken();
		this->addNonTerminalNode();
		this->array();
		if (this->current_token.type == TokenType::TokenIdentifier) {
			this->addTerminalId();
			this->nextToken();
		} else {
			this->errorParse();
		}
	} else {
		this->errorParse();
	}
	this->current_node = this->current_node->getParent();
}

/* ARRAY  ::=  "["  integer  "]"  |  €
 * ------------------------------------
 * "["				-> [ integer ]
 * "identifier"		-> €
 */
void Parser::array() {
	this->current_rule = RuleType::array;
	switch (this->current_token.type) {
	case TokenType::TokenSquareBracketsOpen:
		this->addTerminalNode();
		this->nextToken();
		if (this->current_token.type == TokenType::TokenInteger) {
			this->addTerminalInt();
			this->nextToken();
			if (this->current_token.type == TokenType::TokenSquareBracketsClose) {
				this->addTerminalNode();
				this->nextToken();
			} else {
				this->errorParse();
			}
		} else {
			this->errorParse();
		}
		break;
	case TokenType::TokenIdentifier:
		break;
	default:
		this->errorParse();
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
	this->current_rule = RuleType::statements;
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
	case TokenType::TokenWrite:
	case TokenType::TokenRead:
	case TokenType::TokenWhile:
	case TokenType::TokenIf:
	case TokenType::TokenCurlyBracesOpen:
		this->addNonTerminalNode();
		this->statement();
		if (this->current_token.type == TokenType::TokenSemiColon) {
			this->addTerminalNode();
			this->nextToken();
			this->addNonTerminalNode();
			this->statements();
		} else {
			this->errorParse();
		}
		break;
	case TokenType::TokenCurlyBracesClose:
	case TokenType::TokenStop:  // epsilon-STATEMENTS
		this->addNonTerminalNode();
		break;
	default:
		this->errorParse();
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
	this->current_rule = RuleType::statement;
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
		this->addTerminalId();
		this->nextToken();
		this->addNonTerminalNode();
		this->index();
		if (this->current_token.type == TokenType::TokenColonEquals) {
			this->addTerminalNode();
			this->nextToken();
			this->addNonTerminalNode();
			this->exp();
		} else {
			this->errorParse();
		}
		break;
	case TokenType::TokenWrite:
		this->addTerminalNode();
		this->nextToken();
		if (this->current_token.type == TokenType::TokenParenthesisOpen) {
			this->addTerminalNode();
			this->nextToken();
			this->addNonTerminalNode();
			this->exp();
			if (this->current_token.type == TokenType::TokenParenthesisClose) {
				this->addTerminalNode();
				this->nextToken();
			} else {
				this->errorParse();
			}
		} else {
			this->errorParse();
		}
		break;
	case TokenType::TokenRead:
		this->addTerminalNode();
		this->nextToken();
		if (this->current_token.type == TokenType::TokenParenthesisOpen) {
			this->addTerminalNode();
			this->nextToken();
			if (this->current_token.type == TokenType::TokenIdentifier) {
				this->addTerminalId();
				this->nextToken();
				this->addNonTerminalNode();
				this->index();
				if (this->current_token.type == TokenType::TokenParenthesisClose) {
					this->addTerminalNode();
					this->nextToken();
				} else {
					this->errorParse();
				}
			} else {
				this->errorParse();
			}
		} else {
			this->errorParse();
		}
		break;
	case TokenType::TokenWhile:
		this->addTerminalNode();
		this->nextToken();
		if (this->current_token.type == TokenType::TokenParenthesisOpen) {
			this->addTerminalNode();
			this->nextToken();
			this->addNonTerminalNode();
			this->exp();
			if (this->current_token.type == TokenType::TokenParenthesisClose) {
				this->addTerminalNode();
				this->nextToken();
				this->addNonTerminalNode();
				this->statement();
			} else {
				this->errorParse();
			}
		} else {
			this->errorParse();
		}
		break;
	case TokenType::TokenIf:
		this->addTerminalNode();
		this->nextToken();
		if (this->current_token.type == TokenType::TokenParenthesisOpen) {
			this->addTerminalNode();
			this->nextToken();
			this->addNonTerminalNode();
			this->exp();
			if (this->current_token.type == TokenType::TokenParenthesisClose) {
				this->addTerminalNode();
				this->nextToken();
				this->addNonTerminalNode();
				this->statement();
				if (this->current_token.type == TokenType::TokenElse) {
					this->addTerminalNode();
					this->nextToken();
					this->addNonTerminalNode();
					this->statement();
				} else {
					this->errorParse();
				}
			} else {
				this->errorParse();
			}
		} else {
			this->errorParse();
		}
		break;
	case TokenType::TokenCurlyBracesOpen:
		this->addTerminalNode();
		this->nextToken();
		this->addNonTerminalNode();
		this->statements();
		if (this->current_token.type == TokenType::TokenCurlyBracesClose) {
			this->addTerminalNode();
			this->nextToken();
		} else {
			this->errorParse();
		}
		break;
	case TokenType::TokenElse:
	case TokenType::TokenSemiColon:
		this->addTerminalNode();
		break;
	default:
		this->errorParse();
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
	this->current_rule = RuleType::exp;
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
	case TokenType::TokenParenthesisOpen:
	case TokenType::TokenMinus:
	case TokenType::TokenExclamationMark:
	case TokenType::TokenInteger:
		this->addNonTerminalNode();
		this->exp2();
		this->addNonTerminalNode();
		this->op_exp();
		break;
	default:
		this->errorParse();
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
	this->current_rule = RuleType::exp2;
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
		this->addTerminalId();
		this->nextToken();
		this->index();
		this->addNonTerminalNode();
		break;
	case TokenType::TokenParenthesisOpen:
		this->addTerminalNode();
		this->nextToken();
		this->addNonTerminalNode();
		this->exp();
		if (this->current_token.type == TokenType::TokenParenthesisClose) {
			this->addTerminalNode();
			this->nextToken();
		} else {
			this->errorParse();
		}
		break;
	case TokenType::TokenMinus:
	case TokenType::TokenExclamationMark:
		this->addTerminalNode();
		this->nextToken();
		this->addNonTerminalNode();
		this->exp2();
		break;
	case TokenType::TokenInteger:
		this->addTerminalInt();
		this->nextToken();
		break;
	default:
		this->errorParse();
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
	this->current_rule = RuleType::index;
	switch (this->current_token.type) {
	case TokenType::TokenSquareBracketsOpen:
		this->addTerminalNode();
		this->nextToken();
		this->addNonTerminalNode();
		this->exp();
		if (this->current_token.type == TokenType::TokenSquareBracketsClose) {
			this->addTerminalNode();
			this->nextToken();
		} else {
			this->errorParse();
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
		this->addTerminalNode();
		break;
	default:
		this->errorParse();
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
	this->current_rule = RuleType::op_exp;
	switch (this->current_token.type) {
	case TokenType::TokenPlus:
	case TokenType::TokenMinus:
	case TokenType::TokenStar:
	case TokenType::TokenColon:
	case TokenType::TokenLessThan:
	case TokenType::TokenGreaterThan:
	case TokenType::TokenEquals:
	case TokenType::TokenAndAnd:
		this->addNonTerminalNode();
		this->op();
		this->addNonTerminalNode();
		this->exp();
		break;
	case TokenType::TokenElse:
	case TokenType::TokenSquareBracketsClose:
	case TokenType::TokenParenthesisClose:
	case TokenType::TokenSemiColon:
		break;
	default:
		this->errorParse();
	}
	this->current_node = this->current_node->getParent();
}

/* OP  ::=  "+"  |  "-"  |  "*"  |
 *          ":"  |  "<"  |  ">"  |
 *          "="  |  "=:="  |  "&&"
 */
void Parser::op() {
	this->current_rule = RuleType::op;
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
		this->addTerminalNode();
		nextToken();
		break;
	default:
		this->errorParse();
	}
	this->current_node = this->current_node->getParent();
}


/* identifier */
void Parser::addTerminalId() {
	Node* node = new NodeId(this->current_node, this->current_token.key->getInformation());
	this->current_node->addChild(node); // add, but don't follow
}

/* integer */
void Parser::addTerminalInt() {
	Node* node = new NodeInt(this->current_node, this->current_token.value);
	this->current_node->addChild(node); // add, but stay in parent node
}

void Parser::addTerminalNode() {
	Node* node = new Node(this->current_node, this->current_token.type);
	this->current_node->addChild(node); // add, but stay in parent node
}

void Parser::addNonTerminalNode() {
	Node* node = new Node(this->current_node, this->current_rule);
	this->addToParseTree(node); // add and descent
}

/* This function adds a new node to the tree also also immediately goes to that node */
void Parser::addToParseTree(Node* child) {
	// TODO: this->parse_tree->addNode(current_node); ?
	this->current_node->addChild(child);
	this->current_node = child;
}

void Parser::nextToken() {
	// increment a counter?
	// this->last_token = this->current_token; ?
	// do some other management?
	this->current_token = this->scanner->nextToken();
}

void Parser::errorParse() {
	this->parse_tree->debugPrint();
	errno = EINVAL;
	printf("Unexpected token '%s' in line %d, column %d while parsing %s\n",
			TokenType::tokenNameMini[this->current_token.type],
			this->current_token.line,
			this->current_token.column,
			RuleType::ruleName[this->current_rule]);
	perror("Illegal Token Sequence");
	exit(EXIT_FAILURE);
}

void Parser::checkType() {
	this->checkType(this->parse_tree->getRoot());
}

void Parser::checkType(Node* node) {
	// 1) Check this type
	switch (node->getRuleType()) {
	case RuleType::terminal:
		// TODO do identifier type check
		break;
	case RuleType::op: // TODO do we have a problem with negative numbers here?
		// As long as DataType and TokenType would stay in according sync,
		// we could just calculate it like "DataType = TokenType - 5;" or so ...
		switch (node->getTokenType()) {
		case TokenType::TokenPlus:
			node->setDataType(DataType::opPlus);
			break;
		case TokenType::TokenMinus:
			node->setDataType(DataType::opMinus);
			break;
		case TokenType::TokenStar:
			node->setDataType(DataType::opMult);
			break;
		case TokenType::TokenColon:
			node->setDataType(DataType::opDiv);
			break;
		case TokenType::TokenLessThan:
			node->setDataType(DataType::opLess);
			break;
		case TokenType::TokenGreaterThan:
			node->setDataType(DataType::opGreater);
			break;
		case TokenType::TokenEquals:
			node->setDataType(DataType::opEqual);
			break;
		case TokenType::TokenEqualsColonEquals:
			node->setDataType(DataType::opUnEqual);
			break;
		case TokenType::TokenAndAnd:
			node->setDataType(DataType::opAnd);
			break;
		default:
			this->errorType(node);
		}
		break;
	default:
		// noType is already set as default
		// do nothing
		break;
	}

	// 2) Check children
	Node* ch = node->getChild();
	while (ch != NULL) {
		this->checkType(ch);
		ch = ch->getSibling();
	}
}

void Parser::errorType(Node* node) {
	this->debugPrint();
	printf("TypeError at token %s in line %d, column %d\n", TokenType::tokenNameShort[node->getTokenType()], -1, -1); // TODO we don't have line/col info on every node!
	errno = EINVAL;
	perror("TypeError");
	exit(errno);
}

void Parser::debugPrint() {
	//this->scanner->debugPrint();
	this->parse_tree->debugPrint();
}
