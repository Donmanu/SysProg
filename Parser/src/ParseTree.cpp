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
	this->token_type = TokenType::TokenStop;
}

Node::Node(TokenType::Type token_type) {
	this->sibling = NULL;
	this->child = NULL;
	this->children = 0;
	this->token_type = token_type;
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

/*---------- NodeId ------------*/

NodeId::NodeId(Information* info) {
//	this->Node(TokenType::TokenIdentifier); // does not work like this
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

NodeInt::NodeInt(int value) {
//	this->Node(TokenType::TokenInteger); // does not work like this
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
	//this->root;
}

ParseTree::~ParseTree() {
	delete &this->root; // triggers destructor chain
}

