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
	Node();
	virtual ~Node();

	void setLeftChild();
	Node* getLeftChild();
	void setRightChild();
	Node* getRightChild();

	bool isLeaf();

private:
	Node* left;
	Node* right;
};

class ParseTree {
public:
	ParseTree();
	virtual ~ParseTree();

	// ??
private:
	Node root;
};

#endif /* PARSER_INCLUDES_PARSETREE_H_ */
