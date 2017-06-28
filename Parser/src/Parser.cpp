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
	this->is_epsilon_transition = false;
	this->came_from_statement = false;
}

Parser::~Parser() {
	delete this->scanner;
	//delete this->parse_tree;  // TODO Getting segFaults somewhere deep in the tree!! Got a Loop!?!
}

void Parser::parse() {
	this->current_rule = RuleType::prog;
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
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
	case TokenType::TokenInt:
	case TokenType::TokenWrite:
	case TokenType::TokenRead:
	case TokenType::TokenWhile:
	case TokenType::TokenIf:
	case TokenType::TokenCurlyBracesOpen:
		this->decls();
		this->statements();
		break;
	case TokenType::TokenStop:
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
	this->addNonTerminalNode();
	switch (this->current_token.type) {
	case TokenType::TokenInt:
		this->decl();
		if (this->current_token.type == TokenType::TokenSemiColon) {
			this->addTerminalNode();
			this->nextToken();
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
	case TokenType::TokenStop:
		this->removeFromParseTree(this->current_node);
		this->is_epsilon_transition = true;
		break;
	default:
		this->errorParse();
		break;
	}

	if (this->is_epsilon_transition) {
		this->is_epsilon_transition = false;
	} else {
		this->current_node = this->current_node->getParent();
	}
}

/* DECL  ::=  "int"  ARRAY  identifier
 * ------------------------------------
 * "int"			-> int ARRAY identifier
 * ";"				-> €
 */
void Parser::decl() {
	this->current_rule = RuleType::decl;
	this->addNonTerminalNode();
	switch (this->current_token.type) {
	case TokenType::TokenInt:
		this->addTerminalNode();
		this->nextToken();
		this->array();
		if (this->current_token.type == TokenType::TokenIdentifier) {
			this->addTerminalId();
			this->nextToken();
		} else {
			this->errorParse();
		}
		break;
	case TokenType::TokenSemiColon:
	case TokenType::TokenStop:
		this->removeFromParseTree(this->current_node);
		this->is_epsilon_transition = true;
		break;
	default:
		this->errorParse();
	}

	if (this->is_epsilon_transition) {
		this->is_epsilon_transition = false;
	} else {
		this->current_node = this->current_node->getParent();
	}
}

/* ARRAY  ::=  "["  integer  "]"  |  €
 * ------------------------------------
 * "["				-> [ integer ]
 * "identifier"		-> €
 */
void Parser::array() {
	this->current_rule = RuleType::array;
	this->addNonTerminalNode();
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
		this->removeFromParseTree(this->current_node);
		this->is_epsilon_transition = true;
		break;
	case TokenType::TokenStop:
		break;
	default:
		this->errorParse();
	}

	if (this->is_epsilon_transition) {
		this->is_epsilon_transition = false;
	} else {
		this->current_node = this->current_node->getParent();
	}
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
	this->addNonTerminalNode();
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
	case TokenType::TokenWrite:
	case TokenType::TokenRead:
	case TokenType::TokenWhile:
	case TokenType::TokenIf:
	case TokenType::TokenCurlyBracesOpen:
		this->statement();
		if (this->current_token.type == TokenType::TokenSemiColon) {
			this->addTerminalNode();
			this->nextToken();
//			this->came_from_statement = true;
			this->statements();
		} else {
			this->errorParse();
		}
		break;
	case TokenType::TokenCurlyBracesClose:
	case TokenType::TokenStop:
		this->removeFromParseTree(this->current_node);
		this->is_epsilon_transition = true;
		break;
	default:
		this->errorParse();
		break;
	}

	if (this->is_epsilon_transition) {
		printf("Current Node = STATEMENTS after delete\n");
		this->is_epsilon_transition = false;
	//} else if (this->came_from_statement){
	//	printf("Current Node = STATEMENTS after STATEMENT/STATEMENTS\n");
	//	this->came_from_statement = false; // TODO: check this
	} else {
		printf("Current Node = STATEMENTS without delete\n");
		this->current_node = this->current_node->getParent();
	}

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
	this->addNonTerminalNode();
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
		this->addTerminalId();
		this->nextToken();
		this->index();
		if (this->current_token.type == TokenType::TokenColonEquals) {
			this->addTerminalNode();
			this->nextToken();
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
			this->exp();
			if (this->current_token.type == TokenType::TokenParenthesisClose) {
				this->addTerminalNode();
				this->nextToken();
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
			this->exp();
			if (this->current_token.type == TokenType::TokenParenthesisClose) {
				this->addTerminalNode();
				this->nextToken();
				this->statement();
				if (this->current_token.type == TokenType::TokenElse) {
					this->addTerminalNode();
					this->nextToken();
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
//		this->came_from_statement = true;
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
 * ";"			-> €
 * "else"		-> €
 * ")"			-> €
 * "]"			-> €
 */
void Parser::exp() {
	this->current_rule = RuleType::exp;
	this->addNonTerminalNode();
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
	case TokenType::TokenParenthesisOpen:
	case TokenType::TokenMinus:
	case TokenType::TokenExclamationMark:
	case TokenType::TokenInteger:
		this->exp2();
		this->op_exp();
		break;
	case TokenType::TokenSemiColon:
	case TokenType::TokenElse:
	case TokenType::TokenParenthesisClose:
	case TokenType::TokenSquareBracketsClose:
		this->removeFromParseTree(this->current_node);
		this->is_epsilon_transition = true;
		break;
	default:
		this->errorParse();
	}

	if (this->is_epsilon_transition) {
		this->is_epsilon_transition = false;
	} else {
		this->current_node = this->current_node->getParent();
	}
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
 * "+"				-> €
 * "-"				-> €
 * "*"				-> €
 * ":"				-> €
 * "<"				-> €
 * ">"				-> €
 * "="				-> €
 * "&&"				-> €
 * "=:="			-> €
 * ";"				-> €
 * "else"			-> €
 * ")"				-> €
 * "]"				-> €
 */
void Parser::exp2() {
	this->current_rule = RuleType::exp2;
	this->addNonTerminalNode();
	switch (this->current_token.type) {
	case TokenType::TokenIdentifier:
		this->addTerminalId();
		this->nextToken();
		this->index();
		break;
	case TokenType::TokenParenthesisOpen:
		this->addTerminalNode();
		this->nextToken();
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
		this->exp2();
		break;
	case TokenType::TokenInteger:
		this->addTerminalInt();
		this->nextToken();
		break;
	case TokenType::TokenPlus:
	case TokenType::TokenStar:
	case TokenType::TokenColon:
	case TokenType::TokenLessThan:
	case TokenType::TokenGreaterThan:
	case TokenType::TokenEquals:
	case TokenType::TokenAndAnd:
	case TokenType::TokenEqualsColonEquals:
	case TokenType::TokenSemiColon:
	case TokenType::TokenElse:
	case TokenType::TokenParenthesisClose:
	case TokenType::TokenSquareBracketsClose:
		this->removeFromParseTree(this->current_node);
		this->is_epsilon_transition = true;
		break;
	default:
		this->errorParse();
	}

	if (this->is_epsilon_transition) {
		this->is_epsilon_transition = false;
	} else {
		this->current_node = this->current_node->getParent();
	}
}

/* INDEX  ::=  "["  EXP  "]"  |  €
 * --------------------------------
 * "["		-> [ EXP ]
 * ")"		-> €
 * "+"		-> €
 * "-"		-> €
 * "*"		-> €
 * ":"		-> €
 * "<"		-> €
 * ">"		-> €
 * "="		-> €
 * "&&"		-> €
 * ":="		-> €
 * "=:="	-> €
 * ";"		-> €
 * "else"	-> €
 * "]"		-> €
 */
void Parser::index() {
	this->current_rule = RuleType::index;
	this->addNonTerminalNode();
	switch (this->current_token.type) {
	case TokenType::TokenSquareBracketsOpen:
		this->addTerminalNode();
		this->nextToken();
		this->exp();
		if (this->current_token.type == TokenType::TokenSquareBracketsClose) {
			this->addTerminalNode();
			this->nextToken();
		} else {
			this->errorParse();
		}
		break;
	case TokenType::TokenParenthesisClose:
	case TokenType::TokenPlus:
	case TokenType::TokenMinus:
	case TokenType::TokenStar:
	case TokenType::TokenColon:
	case TokenType::TokenLessThan:
	case TokenType::TokenGreaterThan:
	case TokenType::TokenEquals:
	case TokenType::TokenAndAnd:
	case TokenType::TokenColonEquals:
	case TokenType::TokenEqualsColonEquals:
	case TokenType::TokenSemiColon:
	case TokenType::TokenElse:
	case TokenType::TokenSquareBracketsClose:
	case TokenType::TokenStop:
		this->removeFromParseTree(this->current_node);
		this->is_epsilon_transition = true;
		break;
	default:
		this->errorParse();
	}

	if (this->is_epsilon_transition) {
		this->is_epsilon_transition = false;
	} else {
		this->current_node = this->current_node->getParent();
	}
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
 * "=:="	-> OP EXP
 * "&&"		-> OP EXP
 * "else" 	-> €
 * "]"		-> €
 * ")"		-> €
 * ";"		-> €
 */
void Parser::op_exp() {
	this->current_rule = RuleType::op_exp;
	this->addNonTerminalNode();
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
		this->op();
		this->exp();
		break;
	case TokenType::TokenElse:
	case TokenType::TokenSquareBracketsClose:
	case TokenType::TokenParenthesisClose:
	case TokenType::TokenSemiColon:
	case TokenType::TokenStop:	// epsilon transition
		this->removeFromParseTree(this->current_node);
		this->is_epsilon_transition = true;
		break;
	default:
		this->errorParse();
	}

	if (this->is_epsilon_transition) {
		this->is_epsilon_transition = false;
	} else {
		this->current_node = this->current_node->getParent();
	}
}

/* OP  ::=  "+"  |  "-"  |  "*"  |
 *          ":"  |  "<"  |  ">"  |
 *          "="  |  "=:="  |  "&&"
 * --------------------------------
 * "+"				-> +
 * "-"				-> -
 * "*"				-> *
 * ":"				-> :
 * "<"				-> <
 * ">"				-> >
 * "="				-> =
 * "=:="			-> =:=
 * "&&"				-> &&
 * "("				-> €
 * "identifier"		-> €
 * "integer"		-> €
 * "-"				-> €	// obsolete?
 * "!"				-> €	// obsolete?
 */
void Parser::op() {
	this->current_rule = RuleType::op;
	this->addNonTerminalNode();
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
	case TokenType::TokenParenthesisOpen:
	case TokenType::TokenIdentifier:
	case TokenType::TokenInteger:
		// necessary?
		this->removeFromParseTree(this->current_node);
		this->is_epsilon_transition = true;
		break;
	default:
		this->errorParse();
	}

	if (this->is_epsilon_transition) {
		this->is_epsilon_transition = false;
	} else {
		this->current_node = this->current_node->getParent();
	}
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

/* This function adds a new node to the tree and also immediately goes to that node */
void Parser::addToParseTree(Node* child) {
	this->current_node->addChild(child);
	this->current_node = child;
}

void Parser::removeFromParseTree(Node* node) {
	printf("REMOVING NODE...\n");
	// TODO: this is somewhat shitty...
	this->current_node = this->current_node->removeNode(node);
}

void Parser::nextToken() {
	// increment a counter?
	// this->last_token = this->current_token; ?
	// do some other management?
	this->current_token = this->scanner->nextToken();
	if (this->current_token.line >= 8) {
		printf("debug this stuff!");
		this->debugPrint();
	}
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
	if (node == NULL) {
		return;
	}

	Node* child = node->getChild();
	bool has_array = false;
	bool has_index = false;
	bool has_op_exp = false;
	switch (node->getRuleType()) {
	case RuleType::prog:
		if (child != NULL) {
			if (child->getRuleType() == RuleType::decls) {
				this->checkType(child);
				if (child->hasSibling()) {
					child = child->getSibling();
				}
			}
			if (child->getRuleType() == RuleType::statements) {
				this->checkType(child);
			}
		}
		node->setDataType(DataType::noType);
		break;
	case RuleType::decls:
		if (child != NULL) {
			if (child->getRuleType() == RuleType::decl) {
				this->checkType(child);
				child = child->getSibling(); // :=
			}
			if (child->hasSibling()) {
				this->checkType(child->getSibling()); // decls
			}
		}
		node->setDataType(DataType::noType);
		break;
	case RuleType::decl:
		child = child->getSibling(); // array or identifier
		if (child->getRuleType() == RuleType::array) {
			this->checkType(child);
			has_array = true;
		}
		if (has_array) {
			if (child->getSibling()->getDataType() != DataType::noType) {
				perror("identifier already defined");
				node->setDataType(DataType::errorType);
			} else if (child->getDataType() == DataType::errorType) {
				node->setDataType(DataType::errorType);
			} else {
				node->setDataType(DataType::noType);
				if (child->getDataType() == DataType::arrayType) {
					child->getSibling()->setDataType(DataType::intArrayType);
				} else {
					child->getSibling()->setDataType(DataType::intType);
				}
			}
		} else {
			if (child->getDataType() != DataType::noType) {
				perror("identifier already defined");
				node->setDataType(DataType::errorType);
			} else {
				node->setDataType(DataType::noType);
				child->setDataType(DataType::intType);
			}
		}
		break;
	case RuleType::array:
		if (((NodeInt*)child)->getValue() > 0) {
			node->setDataType(DataType::arrayType);
		} else {
			perror("no valid dimension");
			node->setDataType(DataType::errorType);
		}
		break;
	case RuleType::statements:
		if (child != NULL) {
			this->checkType(child);
			child = child->getSibling(); // ;
			if (child->hasSibling()) {
				child = child->getSibling(); // statements
				if (child->getRuleType() == RuleType::statements) {
					this->checkType(child);
				}
			}
		}
		node->setDataType(DataType::noType);
		break;
	case RuleType::statement:
		if (child->getTokenType() == TokenType::TokenIdentifier) {
			if (child->getSibling()->getRuleType() == RuleType::index) {
				this->checkType(child->getSibling()->getSibling()->getSibling()); // exp
				this->checkType(child->getSibling()); // index
				has_index = true;
			} else {
				this->checkType(child->getSibling()->getSibling()); // exp
			}
			if (child->getDataType() == DataType::noType) {
				perror("identifier not defined");
				node->setDataType(DataType::errorType);
			}
			if (has_index) {
				if (child->getSibling()->getSibling()->getSibling()->getDataType() == DataType::intType &&
						(child->getDataType() == DataType::intArrayType && child->getSibling()->getDataType() == DataType::arrayType)) {
					node->setDataType(DataType::noType);
				} else {
					perror("incompatible types");
					node->setDataType(DataType::errorType);
				}
			} else {
				if (child->getSibling()->getSibling()->getDataType() == DataType::intType &&
						(child->getDataType() == DataType::intType)) {
					node->setDataType(DataType::noType);
				} else {
					perror("incompatible types");
					node->setDataType(DataType::errorType);
				}
			}
		} else if (child->getTokenType() == TokenType::TokenWrite) {
			this->checkType(child->getSibling()->getSibling());
			node->setDataType(DataType::noType);
		} else if (child->getTokenType() == TokenType::TokenRead) {
			if (child->getSibling()->getSibling()->getSibling()->getRuleType() == RuleType::index) {
				this->checkType(child->getSibling()->getSibling()->getSibling()); //index
				has_index = true;
			}
			if (child->getDataType() == DataType::noType) {
				perror("identifier not defined");
				node->setDataType(DataType::errorType);
			}
			if (has_index) {
				if (child->getSibling()->getSibling()->getDataType() == DataType::intArrayType
						&& child->getSibling()->getSibling()->getSibling()->getDataType() == DataType::arrayType) {
					node->setDataType(DataType::noType);
				} else {
					perror("incompatible types");
					node->setDataType(DataType::errorType);
				}
			} else {
				if (child->getSibling()->getSibling()->getDataType() == DataType::intType) {
					node->setDataType(DataType::noType);
				} else {
					perror("incompatible types");
					node->setDataType(DataType::errorType);
				}
			}
		} else if (child->getTokenType() == TokenType::TokenCurlyBracesOpen) {
			if (child->getSibling()->getRuleType() == RuleType::statements) {
				this->checkType(child->getSibling());
			}
			node->setDataType(DataType::noType);
		} else if (child->getTokenType() == TokenType::TokenIf) {
			this->checkType(child->getSibling()->getSibling()); // exp
			this->checkType(child->getSibling()->getSibling()->getSibling()->getSibling()); // statement 1
			this->checkType(child->getSibling()->getSibling()->getSibling()->getSibling()->getSibling()->getSibling()); // statement 2
			if (child->getSibling()->getSibling()->getDataType() == DataType::errorType) {
				node->setDataType(DataType::errorType);
			} else {
				node->setDataType(DataType::noType);
			}
		} else if (child->getTokenType() == TokenType::TokenWhile) {
			this->checkType(child->getSibling()->getSibling()); // exp
			this->checkType(child->getSibling()->getSibling()->getSibling()->getSibling()); // statement
			if (child->getSibling()->getSibling()->getDataType() == DataType::errorType) {
				node->setDataType(DataType::errorType);
			} else {
				node->setDataType(DataType::noType);
			}
		}
		break;
	case RuleType::exp:
		this->checkType(child);
		if (child->hasSibling()) {
			this->checkType(child->getSibling());
			has_op_exp = true;
		}
		if (has_op_exp) {
			if (child->getDataType() != child->getSibling()->getDataType()) {
				node->setDataType(DataType::errorType);
			} else {
				node->setDataType(child->getDataType());
			}
		} else {
			node->setDataType(child->getDataType());
		}
		break;
	case RuleType::exp2:
		if (child->getTokenType() == TokenType::TokenParenthesisOpen) {
			this->checkType(child->getSibling());
			node->setDataType(child->getSibling()->getDataType());
		} else if (child->getTokenType() == TokenType::TokenIdentifier) {
			if (child->hasSibling()) {
				this->checkType(child->getSibling());
				has_index = true;
			}
			if (child->getDataType() == DataType::noType) {
				perror("identifier not defined");
				node->setDataType(DataType::errorType);
			}
			if (has_index) {
				if (child->getDataType() == DataType::intArrayType
						&& child->getSibling()->getDataType() == DataType::arrayType) {
					node->setDataType(DataType::intType);
				} else {
					perror("no primitive type");
					node->setDataType(DataType::errorType);
				}
			} else {
				if (child->getDataType() == DataType::intType) {
					node->setDataType(child->getDataType());
				} else {
					perror("no primitive type");
					node->setDataType(DataType::errorType);
				}
			}
		} else if (child->getTokenType() == TokenType::TokenInteger) {
			node->setDataType(DataType::intType);
		} else if (child->getTokenType() == TokenType::TokenMinus) {
			this->checkType(child->getSibling());
			node->setDataType(child->getSibling()->getDataType());
		} else if (child->getTokenType() == TokenType::TokenExclamationMark) {
			this->checkType(child->getSibling());
			if (child->getSibling()->getDataType() != DataType::intType) {
				node->setDataType(DataType::errorType);
			} else {
				node->setDataType(DataType::intType);
			}
		}
		break;
	case RuleType::index:
		this->checkType(child->getSibling());
		if (child->getSibling()->getDataType() == DataType::errorType) {
			node->setDataType(DataType::errorType);
		} else {
			node->setDataType(DataType::arrayType);
		}
		break;
	case RuleType::op_exp:
		this->checkType(child);
		this->checkType(child->getSibling());
		node->setDataType(child->getSibling()->getDataType());
		break;
	case RuleType::op:
		switch (child->getTokenType()) {
		case TokenType::TokenPlus:
			child->setDataType(DataType::opPlus);
			break;
		case TokenType::TokenMinus:
			child->setDataType(DataType::opMinus);
			break;
		case TokenType::TokenStar:
			child->setDataType(DataType::opMult);
			break;
		case TokenType::TokenColon:
			child->setDataType(DataType::opDiv);
			break;
		case TokenType::TokenLessThan:
			child->setDataType(DataType::opLess);
			break;
		case TokenType::TokenGreaterThan:
			child->setDataType(DataType::opGreater);
			break;
		case TokenType::TokenEquals:
			child->setDataType(DataType::opEqual);
			break;
		case TokenType::TokenEqualsColonEquals:
			child->setDataType(DataType::opUnEqual);
			break;
		case TokenType::TokenAndAnd:
			child->setDataType(DataType::opAnd);
			break;
		default:
			return;
		}
		break;
	case RuleType::terminal:
		// obsolete?
		break;
	default:
		//obsolete?
		break;
	};

	/*
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
	*/
}

void Parser::makeCode() {
	this->makeCode(this->parse_tree->getRoot());
}

void Parser::makeCode(Node* node) {
	if (node == NULL) {
		return;
	}
	// 1) Check RuleType
	Node* child = node->getChild();
	bool has_array = false;
	bool has_statements = false;
	switch (node->getRuleType()) {
	case RuleType::prog:
		if (child != NULL) {
			if (child->getRuleType() == RuleType::decls) {
				this->makeCode(child); // decls
				if (child->hasSibling()) {
					child = child->getSibling(); //statements
				}
			}
			if (child->getRuleType() == RuleType::statements) {
				this->makeCode(child);
			} else {
				printf("NOP\n");
				// output << "NOP";
			}
		}
		printf("STP\n");
		// output << "STP";
		break;
	case RuleType::decls:
		if (child->getRuleType() == RuleType::decl) {
			this->makeCode(child);
			child = child->getSibling(); // ;
			if (child->hasSibling()) {
				child = child->getSibling(); // decls
				if (child->getRuleType() == RuleType::decls) {
					this->makeCode(child);
				}
			}
		}
		break;
	case RuleType::decl:
		printf("DS $ lexem\n");
		// output << "DS" << "$" << getLexem;
		while (child != NULL) {
			if (child->getRuleType() == RuleType::array) {
				has_array = true;
				this->makeCode(child);
			}
			child = child->getSibling();
		}
		if (!has_array) {
			printf("1\n");
			//output << 1;
		}
		break;
	case RuleType::array:
		printf("integer\n");
		//output << getInteger();
		break;
	case RuleType::statements:
		if (child != NULL) {
			if (child->getRuleType() == RuleType::statement) {
				this->makeCode(child); // statement
				child = child->getSibling(); // ;
				if (child->hasSibling()) {
					child = child->getSibling(); // statements
					if (child->getRuleType() == RuleType::statements) {
						has_statements = true;
						this->makeCode(child); // statements
					}
				}
			}
		}
		if (!has_statements) {
			printf("NOP\n");
			//output << "NOP";
		}
		break;
	case RuleType::statement:
		if (child->getTokenType() == TokenType::TokenIdentifier) {
			while (child->getRuleType() != RuleType::exp) {
				child = child->getSibling();
			}
			this->makeCode(child); // exp
			child = node->getChild(); // identifier
			printf("LA $ lexem\n");
			// output << "LA" << "$" << getIdentifier();
			child = child->getSibling(); // index or :=
			if (child->getRuleType() == RuleType::index) {
				this->makeCode(child); // index
			}
			printf("STR\n");
			// output << "STR";
		} else if (child->getTokenType() == TokenType::TokenWrite) {
			while (child->getRuleType() != RuleType::exp) {
				child = child->getSibling();
			}
			this->makeCode(); // exp
			printf("PRI\n");
			// output << "PRI";
		} else if (child->getTokenType() == TokenType::TokenRead) {
			printf("REA\n");
			// output << "REA";
			child = child->getSibling(); // (
			child = child->getSibling(); // identifier
			if (child->getTokenType() == TokenType::TokenIdentifier) {
				printf("LA $ lexem\n");
				// output << "LA" << "$" << getLexem();
			}
			child->getSibling();
			if (child->getRuleType() == RuleType::index) {
				this->makeCode(child); // index
			}
			printf("STR\n");
			// output << "STR";
		} else if (child->getTokenType() == TokenType::TokenCurlyBracesOpen) {
			child = child->getSibling(); // statements or }
			if (child->getRuleType() == RuleType::statements) {
				this->makeCode(child); // statements
			}
		} else if (child->getTokenType() == TokenType::TokenIf) {
			while (child->getRuleType() != RuleType::exp) {
				child = child->getSibling();
			}
			this->makeCode(child); // exp
			printf("JIN # label1\n");
			// output << "JIN" << "#" << label1;
			child = child->getSibling(); // )
			child = child->getSibling(); // statement
			this->makeCode(child); // statement
			printf("JMP # label2");
			// output << "JMP" << "#" << label2;
			printf("# label1 NOP\n");
			// output << "#" << label1 << "NOP";
			child = child->getSibling(); // else
			child = child->getSibling(); // statement
			this->makeCode(child); // statement
			printf("# label2 NOP\n");
			// output << "#" << label2 << "NOP";
		} else if (child->getTokenType() == TokenType::TokenWhile) {
			printf("# label1 NOP\n");
			// output << "#" << label1 << "NOP";
			while (child->getRuleType() != RuleType::exp) {
				child = child->getSibling();
			}
			this->makeCode(child); // exp
			printf("JIN # label2\n");
			// output << "JIN" << "#" << label2;
			child = child->getSibling(); // )
			child = child->getSibling(); // statement
			this->makeCode(child); // statement
			printf("JMP # label1\n");
			// output << "JMP" << "#" << label1;
			printf("# label2 NOP\n");
			// output << "#" << label2 << "NOP";
		}
		break;
	case RuleType::exp:
		if (!child->hasSibling()) {
			this->makeCode(child); // exp2
		} else if (child->getSibling()->getChild()->getChild()->getDataType() == DataType::opGreater) { // opGreater?
			this->makeCode(child->getSibling()); // op_exp
			this->makeCode(child); // exp2
			printf("LES\n");
			// output << "LES";
		} else if (child->getSibling()->getChild()->getChild()->getDataType() == DataType::opUnEqual) { // opUnequal?
			this->makeCode(child); // exp2
			this->makeCode(child->getSibling()); // op_exp
			printf("NOT\n");
			// output << "NOT";
		} else {
			this->makeCode(child); // exp2
			this->makeCode(child->getSibling()); // op_exp
		}
		break;
	case RuleType::exp2:
		if (child->getTokenType() == TokenType::TokenParenthesisOpen) {
			this->makeCode(child->getSibling()); // exp
		} else if (child->getTokenType() == TokenType::TokenIdentifier) {
			printf("LA $ lexem\n");
			// output << "LA" << "$" << getLexem;
			if (child->hasSibling()) {
				this->makeCode(child->getSibling()); // index
			}
			printf("LV\n");
			// output << "LV";
		} else if (child->getTokenType() == TokenType::TokenInteger) {
			printf("LC integer\n");
			// output << "LC" << getInteger;
		} else if (child->getTokenType() == TokenType::TokenMinus) {
			printf("LC 0\n");
			// output << "LC" << 0;
			this->makeCode(child->getSibling()); //exp2
			printf("SUB\n");
			// output << "SUB";
		} else if (child->getTokenType() == TokenType::TokenExclamationMark) {
			this->makeCode(child->getSibling()); // exp2
			printf("NOT\n");
			// output << "NOT";
		}
		break;
	case RuleType::index:
		if (child != NULL) {
			child = child->getSibling(); // exp
			this->makeCode(child); // exp
			printf("ADD\n");
			// output << "ADD";
		}
		break;
	case RuleType::op_exp:
		if (child != NULL) {
			if (child->getRuleType() == RuleType::exp) {
				this->makeCode(child); // exp
				child = child->getSibling(); // op
				this->makeCode(child); // op
			}
		}
		break;
	case RuleType::op:
		switch (child->getDataType()) {
		case DataType::opPlus:
			printf("ADD\n");
			// output << "ADD";
			break;
		case DataType::opMinus:
			printf("SUB\n");
			// output << "SUB";
			break;
		case DataType::opMult:
			printf("MUL\n");
			// output << "MUL";
			break;
		case DataType::opDiv:
			printf("DIV\n");
			// output << "DIV";
			break;
		case DataType::opLess:
			printf("LES\n");
			// output << "LES";
			break;
//		case DataType::opGreater:
			// noting?
//			break;
		case DataType::opEqual:
		case DataType::opUnEqual:
			printf("EQU\n");
			// output << "EQU"
			break;
		case DataType::opAnd:
			printf("AND\n");
			// output << "AND"
			break;
		default:
			printf("\n");
			break;
		};
		break;
//	case RuleType::terminal:
		// TODO: obsolete?
//		break;
	default:
		// TODO: obsolete?
		printf("\n");
		break;
	};
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
