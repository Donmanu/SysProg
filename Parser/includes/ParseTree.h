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
		Node(TokenType::Type);
		virtual ~Node(void);

		void addChild(Node*);
		void setChild(Node*);
		Node* getChild(void);

		void addSibling(Node*);
		void setSibling(Node*);
		Node* getSibling(void);

		bool isLeaf(void);
		bool hasSibling(void);
		//bool isOnlyChild(void); ??

	protected:
		int children;
		Node* child;
		Node* sibling;
		TokenType::Type token_type;
};

class NodeId : Node {
	public:
		NodeId(Information*);
		virtual ~NodeId();
		Information* getInformation();
		void setInformation(Information*);

	private:
		Information* information;
};

class NodeInt : Node {
	public:
		NodeInt(int);
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

		// The root node could represent a parse tree in itself
		// TODO offer some helpful functions here, to work with the whole tree

	private:
		Node root;
};

#endif /* PARSER_INCLUDES_PARSETREE_H_ */
