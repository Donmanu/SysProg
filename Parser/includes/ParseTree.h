/*
 * ParseTree.h
 *
 *  Created on: May 18, 2017
 *      Author: donmanu
 */

#ifndef PARSER_INCLUDES_PARSETREE_H_
#define PARSER_INCLUDES_PARSETREE_H_

#include "../../Scanner/includes/TokenType.h"

class Node {
	public:
		Node(void);
		Node(TokenType::Type token_type);
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

	private:
		int children;
		Node* child;
		Node* sibling;
		TokenType::Type token_type;
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
