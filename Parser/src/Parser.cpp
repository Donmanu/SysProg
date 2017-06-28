/*
 * Parser.cpp
 *
 *  Created on: May 18, 2017
 *      Author: donmanu
 */

#include "../includes/Parser.h" // contains scanner.h

/*------------- Parser --------------*/

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
			if (((NodeId*)(child->getSibling()))->getInformation()->getType() != DataType::noType) {
				node->setDataType(DataType::errorType);
				perror("identifier already defined");
			} else if (child->getDataType() == DataType::errorType) {
				node->setDataType(DataType::errorType);
			} else {
				node->setDataType(DataType::noType);
				if (child->getDataType() == DataType::arrayType) {
					((NodeId*)(child->getSibling()))->getInformation()->setType(DataType::intArrayType);
				} else {
					((NodeId*)(child->getSibling()))->getInformation()->setType(DataType::intType);
				}
			}
		} else {
			if (((NodeId*)child)->getInformation()->getType() != DataType::noType) {
				node->setDataType(DataType::errorType);
				perror("identifier already defined");
			} else {
				node->setDataType(DataType::noType);
				((NodeId*)child)->getInformation()->setType(DataType::intType);
			}
		}
		break;
	case RuleType::array:
		child = child->getSibling(); // integer
		if (((NodeInt*)child)->getValue() > 0) {
			node->setDataType(DataType::arrayType);
		} else {
			node->setDataType(DataType::errorType);
			perror("no valid dimension");
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
			if (((NodeId*)child)->getInformation()->getType() == DataType::noType) {
				node->setDataType(DataType::errorType);
				perror("identifier not defined");
			}
			if (has_index) {
				if (child->getSibling()->getSibling()->getSibling()->getDataType() == DataType::intType &&
						(((NodeId*)child)->getInformation()->getType() == DataType::intArrayType && child->getSibling()->getDataType() == DataType::arrayType)) {
					node->setDataType(DataType::noType);
				} else {
					node->setDataType(DataType::errorType);
					perror("incompatible types");
				}
			} else {
				if (child->getSibling()->getSibling()->getDataType() == DataType::intType &&
						(((NodeId*)child)->getInformation()->getType() == DataType::intType)) {
					node->setDataType(DataType::noType);
				} else {
					node->setDataType(DataType::errorType);
					perror("incompatible types");
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
			if (((NodeId*)child->getSibling()->getSibling())->getInformation()->getType() == DataType::noType) {
				node->setDataType(DataType::errorType);
				perror("identifier not defined");
			}
			if (has_index) {
				if (((NodeId*)child->getSibling()->getSibling())->getInformation()->getType() == DataType::intArrayType
						&& child->getSibling()->getSibling()->getSibling()->getDataType() == DataType::arrayType) {
					node->setDataType(DataType::noType);
				} else {
					node->setDataType(DataType::errorType);
					perror("incompatible types");
				}
			} else {
				if (((NodeId*)child->getSibling()->getSibling())->getInformation()->getType() == DataType::intType) {
					node->setDataType(DataType::noType);
				} else {
					node->setDataType(DataType::errorType);
					perror("incompatible types");
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
			if (((NodeId*)child)->getInformation()->getType() == DataType::noType) {
				node->setDataType(DataType::errorType);
				perror("identifier not defined");
			}
			if (has_index) {
				if (((NodeId*)child)->getInformation()->getType() == DataType::intArrayType
						&& child->getSibling()->getDataType() == DataType::arrayType) {
					node->setDataType(DataType::intType);
				} else {
					node->setDataType(DataType::errorType);
					perror("no primitive type");
				}
			} else {
				if (((NodeId*)child)->getInformation()->getType() == DataType::intType) {
					node->setDataType(((NodeId*)child)->getInformation()->getType());
				} else {
					node->setDataType(DataType::errorType);
					perror("no primitive type");
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
}

void Parser::makeCode(char* inputFileName) {
	// Build output file name
	int outLen;
	char* outName;
	char* dotMark = strchr(inputFileName, '.');     // finds the FIRST occurrence of '.'. Good enough for now. Should also work on ".input"
	if (dotMark != NULL) {
		outLen = dotMark - inputFileName;
	} else {
		// no '.' in inputFileName -> take full name
		outLen = strlen(inputFileName);
	}
	outName = new char[outLen + 6];  // +1 '.', +4 "code", +1 '\0'

	//strcpy(outName, inputFileName); => WARNING! fails if len(input) > len(output)
	// so do it by hand instead:
	for (int i = 0; i < outLen; i++) {
		outName[i] = inputFileName[i];
	}
	// Reuse dotMark
	dotMark = outName + outLen;
	*(dotMark++) = '.';
	*(dotMark++) = 'c';
	*(dotMark++) = 'o';
	*(dotMark++) = 'd';
	*(dotMark++) = 'e';
	*(dotMark) = '\0';

	this->code_file.open(outName/*, ios::out*/);

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
				//printf("NOP\n");
				this->code_file << "NOP";
			}
		}
		//printf("STP\n");
		this->code_file << "STP";
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
		//printf("DS $ lexem\n");
		if (child->getSibling()->getRuleType() == RuleType::array) {
			this->code_file << "DS" << "$" << ((NodeId*)child->getSibling()->getSibling())->getInformation()->getLexem();
		} else {
			this->code_file << "DS" << "$" << ((NodeId*)child->getSibling())->getInformation()->getLexem();
		}
		while (child != NULL) {
			if (child->getRuleType() == RuleType::array) {
				has_array = true;
				this->makeCode(child);
			}
			child = child->getSibling();
		}
		if (!has_array) {
			//printf("1\n");
			this->code_file << 1;
		}
		break;
	case RuleType::array:
		//printf("integer\n");
		this->code_file << ((NodeInt*)child->getSibling())->getValue();
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
			//printf("NOP\n");
			this->code_file << "NOP";
		}
		break;
	case RuleType::statement:
		if (child->getTokenType() == TokenType::TokenIdentifier) {
			while (child->getRuleType() != RuleType::exp) {
				child = child->getSibling();
			}
			this->makeCode(child); // exp
			child = node->getChild(); // identifier
			//printf("LA $ lexem\n");
			this->code_file << "LA" << "$" << ((NodeId*)child)->getInformation()->getLexem();
			child = child->getSibling(); // index or :=
			if (child->getRuleType() == RuleType::index) {
				this->makeCode(child); // index
			}
			//printf("STR\n");
			this->code_file << "STR";
		} else if (child->getTokenType() == TokenType::TokenWrite) {
			while (child->getRuleType() != RuleType::exp) {
				child = child->getSibling();
			}
			this->makeCode(child); // exp
			//printf("PRI\n");
			this->code_file << "PRI";
		} else if (child->getTokenType() == TokenType::TokenRead) {
			//printf("REA\n");
			this->code_file << "REA";
			child = child->getSibling(); // (
			child = child->getSibling(); // identifier
			if (child->getTokenType() == TokenType::TokenIdentifier) {
				//printf("LA $ lexem\n");
				this->code_file << "LA" << "$" << ((NodeId*)child)->getInformation()->getLexem();
			}
			child->getSibling();
			if (child->getRuleType() == RuleType::index) {
				this->makeCode(child); // index
			}
			//printf("STR\n");
			this->code_file << "STR";
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
			//printf("JIN # label1\n");
			this->code_file << "JIN" << "#" << "label1"; // TODO label
			child = child->getSibling(); // )
			child = child->getSibling(); // statement
			this->makeCode(child); // statement
			//printf("JMP # label2");
			this->code_file << "JMP" << "#" << "label2"; // TODO label
			//printf("# label1 NOP\n");
			this->code_file << "#" << "label1" << "NOP"; // TODO label
			child = child->getSibling(); // else
			child = child->getSibling(); // statement
			this->makeCode(child); // statement
			//printf("# label2 NOP\n");
			this->code_file << "#" << "label2" << "NOP"; // TODO label
		} else if (child->getTokenType() == TokenType::TokenWhile) {
			//printf("# label1 NOP\n");
			this->code_file << "#" << "label1" << "NOP"; // TODO label
			while (child->getRuleType() != RuleType::exp) {
				child = child->getSibling();
			}
			this->makeCode(child); // exp
			//printf("JIN # label2\n");
			this->code_file << "JIN" << "#" << "label2"; // TODO label
			child = child->getSibling(); // )
			child = child->getSibling(); // statement
			this->makeCode(child); // statement
			//printf("JMP # label1\n");
			this->code_file << "JMP" << "#" << "label1"; // TODO label
			//printf("# label2 NOP\n");
			this->code_file << "#" << "label2" << "NOP"; // TODO label
		}
		break;
	case RuleType::exp:
		if (!child->hasSibling()) {
			this->makeCode(child); // exp2
		} else if (child->getSibling()->getChild()->getChild()->getDataType() == DataType::opGreater) { // opGreater?
			this->makeCode(child->getSibling()); // op_exp
			this->makeCode(child); // exp2
			//printf("LES\n");
			this->code_file << "LES";
		} else if (child->getSibling()->getChild()->getChild()->getDataType() == DataType::opUnEqual) { // opUnequal?
			this->makeCode(child); // exp2
			this->makeCode(child->getSibling()); // op_exp
			//printf("NOT\n");
			this->code_file << "NOT";
		} else {
			this->makeCode(child); // exp2
			this->makeCode(child->getSibling()); // op_exp
		}
		break;
	case RuleType::exp2:
		if (child->getTokenType() == TokenType::TokenParenthesisOpen) {
			this->makeCode(child->getSibling()); // exp
		} else if (child->getTokenType() == TokenType::TokenIdentifier) {
			//printf("LA $ lexem\n");
			this->code_file << "LA" << "$" << ((NodeId*)child)->getInformation()->getLexem();
			if (child->hasSibling()) {
				this->makeCode(child->getSibling()); // index
			}
			//printf("LV\n");
			this->code_file << "LV";
		} else if (child->getTokenType() == TokenType::TokenInteger) {
			//printf("LC integer\n");
			this->code_file << "LC" << ((NodeInt*)child)->getValue();
		} else if (child->getTokenType() == TokenType::TokenMinus) {
			//printf("LC 0\n");
			this->code_file << "LC" << 0;
			this->makeCode(child->getSibling()); //exp2
			//printf("SUB\n");
			this->code_file << "SUB";
		} else if (child->getTokenType() == TokenType::TokenExclamationMark) {
			this->makeCode(child->getSibling()); // exp2
			//printf("NOT\n");
			this->code_file << "NOT";
		}
		break;
	case RuleType::index:
		if (child != NULL) {
			child = child->getSibling(); // exp
			this->makeCode(child); // exp
			//printf("ADD\n");
			this->code_file << "ADD";
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
			//printf("ADD\n");
			this->code_file << "ADD";
			break;
		case DataType::opMinus:
			//printf("SUB\n");
			this->code_file << "SUB";
			break;
		case DataType::opMult:
			//printf("MUL\n");
			this->code_file << "MUL";
			break;
		case DataType::opDiv:
			//printf("DIV\n");
			this->code_file << "DIV";
			break;
		case DataType::opLess:
			//printf("LES\n");
			this->code_file << "LES";
			break;
//		case DataType::opGreater:
			// noting?
//			break;
		case DataType::opEqual:
		case DataType::opUnEqual:
			//printf("EQU\n");
			this->code_file << "EQU";
			break;
		case DataType::opAnd:
			//printf("AND\n");
			this->code_file << "AND";
			break;
		default:
			//printf("\n");
			break;
		};
		break;
//	case RuleType::terminal:
		// TODO: obsolete?
//		break;
	default:
		// TODO: obsolete?
		printf("\n");
		this->code_file << "\n";
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
