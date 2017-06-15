/*
 * ParseTree.h
 *
 *  Created on: May 18, 2017
 *      Author: donmanu
 */

#ifndef PARSER_INCLUDES_PARSETREE_H_
#define PARSER_INCLUDES_PARSETREE_H_

#include "../../Scanner/includes/TokenType.h"
#include "../../Symboltable/includes/Information.h"
#include "RuleType.h"
#include "DataType.h"

class Node {
	public:
		Node(void);
		Node(Node*);
		Node(Node*, TokenType::Type);
		Node(Node*, RuleType::Type);
		Node(Node*, TokenType::Type, RuleType::Type);
		virtual ~Node(void);

		Node* getParent();

		void addChild(Node*);
		void setChild(Node*);
		Node* getChild(void);

		void addSibling(Node*);
		void setSibling(Node*);
		Node* getSibling(void);

		bool isLeaf(void);
		bool hasSibling(void);
		//bool isOnlyChild(void); ??

		TokenType::Type getTokenType(void);
		RuleType::Type getRuleType(void);
		DataType::Type getDataType(void);

	protected:
		int children;
		Node* parent;
		Node* child;
		Node* sibling;
		TokenType::Type token_type;
		RuleType::Type rule_type;
		DataType::Type data_type;
};

class NodeId : public Node {
	public:
		NodeId(Node*, Information*);
		virtual ~NodeId();
		Information* getInformation();
		void setInformation(Information*);

	private:
		Information* information;
};

class NodeInt : public Node {
	public:
		NodeInt(Node*, int);
		virtual ~NodeInt();
		int getValue();
		void setValue(int);

	private:
		int value;
};

class ParseTree {
	public:
		ParseTree(Node*);
		virtual ~ParseTree(void);

		Node* getRoot(void);
		// The root node could represent a parse tree in itself
		// TODO offer some helpful functions here, to work with the whole tree

		void debugPrint();

	private:
		void recursivePrint(Node*);
		void indent(char);
		void unIndent();
		Node* root;

		char* printLine;
		int depth;
};

#endif /* PARSER_INCLUDES_PARSETREE_H_ */
