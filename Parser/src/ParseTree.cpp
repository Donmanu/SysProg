/*
 * ParseTree.cpp
 *
 *  Created on: Jun 4, 2017
 *      Author: arthur
 */

#include <stdlib.h>

#include "../includes/ParseTree.h"

/*----------------- Node -----------*/

Node::Node() {
	this->left = NULL;
	this->right = NULL;
}
Node::~Node() {
	delete this->left;
	delete this->right;
}

void Node::setLeftChild(Node* left) {
	this->left = left;
}

Node* Node::getLeftChild() {
	return this->left;
}

void Node::setRightChild(Node* right) {
	this->right = right;
}

Node* Node::getRightChild() {
	return this->right;
}

bool Node::isLeaf() {
	return !this->left && !this->right;
}

/*---------- ParseTree ------------*/

ParseTree::ParseTree() {
	//this->root;
}

ParseTree::~ParseTree() {
	delete &this->root; // triggers destructor chain
}

