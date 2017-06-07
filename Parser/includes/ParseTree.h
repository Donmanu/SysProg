/*
 * ParseTree.h
 *
 *  Created on: May 18, 2017
 *      Author: donmanu
 */

#ifndef PARSER_INCLUDES_PARSETREE_H_
#define PARSER_INCLUDES_PARSETREE_H_

class Node {
	public:
		Node(void);
		virtual ~Node(void);

		void setLeftChild(Node*);
		Node* getLeftChild(void);
		void setRightChild(Node*);
		Node* getRightChild(void);

		bool isLeaf(void);

	private:
		Node* left;
		Node* right;
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
