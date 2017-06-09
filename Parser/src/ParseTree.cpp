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
	this->sibling = NULL;
	this->child = NULL;
	this->children = 0;
}
Node::~Node() {
	delete this->sibling;
	delete this->child;
}

void Node::addChild(Node* child) {
	Node * c = this->child;
	if (c == NULL) {
		c = child;
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

/*---------- ParseTree ------------*/

ParseTree::ParseTree() {
	//this->root;
}

ParseTree::~ParseTree() {
	delete &this->root; // triggers destructor chain
}

