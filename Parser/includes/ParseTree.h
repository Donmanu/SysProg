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

class Node {
	public:
		Node(void);
		Node(Node*);
		Node(Node*, TokenType::Type);
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

	protected:
		int children;
		Node* parent;
		Node* child;
		Node* sibling;
		TokenType::Type token_type;
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
		ParseTree(void);
		virtual ~ParseTree(void);

		Node* getRoot(void);
		// The root node could represent a parse tree in itself
		// TODO offer some helpful functions here, to work with the whole tree

		void debugPrint();

	private:
		void recursivePrint(Node*);
		void push(char);
		void pop();
		Node* root;

		char* printDepth;
		int di;
};

#endif /* PARSER_INCLUDES_PARSETREE_H_ */
