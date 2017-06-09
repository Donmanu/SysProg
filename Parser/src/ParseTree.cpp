/*
 * ParseTree.cpp
 *
 *  Created on: Jun 4, 2017
 *      Author: arthur
 */

#define MAX_PRINT (2056)

#include <stdlib.h>

#include "../includes/ParseTree.h"

/*----------------- Node -----------*/

Node::Node() {
	this->parent = NULL;
	this->sibling = NULL;
	this->child = NULL;
	this->children = 0;
	this->token_type = TokenType::TokenStop;
}

Node::Node(Node* parent) {
	this->parent = parent;
	this->sibling = NULL;
	this->child = NULL;
	this->children = 0;
	this->token_type = TokenType::TokenStop;
}

Node::Node(Node* parent, TokenType::Type token_type) {
	this->parent = parent;
	this->sibling = NULL;
	this->child = NULL;
	this->children = 0;
	this->token_type = token_type;
}

Node::~Node() {
	delete this->sibling;
	delete this->child;
}

Node* Node::getParent() {
	return this->parent;
}

void Node::addChild(Node* child) {
	Node * c = this->child;
	if (c == NULL) {
		this->child = child;
	} else {
		while (c->hasSibling()) // better for(i = children... ?
			c = c->getSibling();
		c->setSibling(child);
	}
	this->children++;
}

void Node::setChild(Node* child) {
	this->child = child;
}

Node* Node::getChild() {
	return this->child;
}

void Node::addSibling(Node* sibling) {
	Node * c = this->sibling;
	if (c == NULL) {
		c = sibling;
	} else {
		while (c->hasSibling())
			c = c->getSibling();
		c->setSibling(sibling);
	}
}

void Node::setSibling(Node* sibling) {
	this->sibling = sibling;
}

Node* Node::getSibling() {
	return this->sibling;
}

bool Node::isLeaf() {
	return !this->child;
}

bool Node::hasSibling() {
	return !!this->sibling;
}

TokenType::Type Node::getTokenType() {
	return this->token_type;
}

/*---------- NodeId ------------*/

NodeId::NodeId(Node* parent, Information* info) {
//	this->Node(TokenType::TokenIdentifier); // does not work like this
	this->parent = parent;
	this->sibling = NULL;
	this->child = NULL;
	this->children = 0;
	this->token_type = TokenType::TokenIdentifier;
	this->information = info;
}

NodeId::~NodeId() {
	this->~Node();
}

Information* NodeId::getInformation() {
	return this->information;
}

void NodeId::setInformation(Information* info) {
	this->information = info;
}

/*---------- NodeInt ------------*/

NodeInt::NodeInt(Node* parent, int value) {
//	this->Node(TokenType::TokenInteger); // does not work like this
	this->parent = parent;
	this->sibling = NULL;
	this->child = NULL;
	this->children = 0;
	this->token_type = TokenType::TokenIdentifier;
	this->value = value;
}

NodeInt::~NodeInt() {
	this->~Node();
}

int NodeInt::getValue() {
	return this->value;
}

void NodeInt::setValue(int value) {
	this->value = value;
}

/*---------- ParseTree ------------*/

ParseTree::ParseTree() {
	this->root = new Node();
	this->printDepth = new char[MAX_PRINT];
}

ParseTree::~ParseTree() {
	delete this->root; // triggers destructor chain
	delete[] this->printDepth;
}

Node* ParseTree::getRoot() {
	return this->root;
}

void ParseTree::debugPrint() {
	printf("  --- ParseTree: ---\n");
	this->recursivePrint(this->root);
	printf("\n");
}

void ParseTree::recursivePrint(Node* thisRoot) {
	// pretty quick C&P solution from http://www.randygaul.net/2015/06/15/printing-pretty-ascii-trees/

	printf("(%s)\n", TokenType::tokenNameShort[thisRoot->getTokenType()]);

	Node* child = thisRoot->getChild();

	while (child)
	{
		Node* next = child->getSibling();
		printf("%s `--", this->printDepth);
		this->push(next ? '|' : ' ');
		this->recursivePrint(child);
		this->pop();
		child = child->getSibling();
	}
}

void ParseTree::push(char c) {
    this->printDepth[di++] = ' ';
    this->printDepth[di++] = c;
    this->printDepth[di++] = ' ';
    this->printDepth[di++] = ' ';
    this->printDepth[di] = '\0';
    if (di >= MAX_PRINT - 4) {
    	perror("No more space for printing ...");
    	exit(EXIT_FAILURE);
    }
}

void ParseTree::pop() {
	this->printDepth[di -= 4] = 0;
}
