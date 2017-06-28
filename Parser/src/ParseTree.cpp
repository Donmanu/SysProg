/*
 * ParseTree.cpp
 *
 *  Created on: Jun 4, 2017
 *      Author: arthur
 */

#define MAX_PRINT (400)

#include <stdlib.h>

#include "../includes/ParseTree.h"

/*----------------- Node -----------*/

Node::Node() {
	this->parent = NULL;
	this->sibling = NULL;
	this->child = NULL;
	this->children = 0;
	this->token_type = TokenType::TokenStop;
	this->rule_type = RuleType::terminal;
	this->data_type = DataType::noType;
}

Node::Node(Node* parent) {
	this->parent = parent;
	this->sibling = NULL;
	this->child = NULL;
	this->children = 0;
	this->token_type = TokenType::TokenStop;
	this->rule_type = RuleType::terminal;
	this->data_type = DataType::noType;
}

Node::Node(Node* parent, TokenType::Type token_type) {
	this->parent = parent;
	this->sibling = NULL;
	this->child = NULL;
	this->children = 0;
	this->token_type = token_type;
	this->rule_type = RuleType::terminal;
	this->data_type = DataType::noType;
}

Node::Node(Node* parent, RuleType::Type rule) {
	this->parent = parent;
	this->sibling = NULL;
	this->child = NULL;
	this->children = 0;
	this->token_type = TokenType::TokenStop;
	this->rule_type = rule;
	this->data_type = DataType::noType;
}

Node::Node(Node* parent, TokenType::Type token, RuleType::Type rule) {
	this->parent = parent;
	this->sibling = NULL;
	this->child = NULL;
	this->children = 0;
	this->token_type = token;
	this->rule_type = rule;
	this->data_type = DataType::noType;
}

Node::~Node() {
	delete this->child;
	delete this->sibling;
}

Node* Node::getParent() {
	return this->parent;
}

void Node::setParent(Node* node) {
	this->parent = node;
}

Node* Node::removeNode(Node* node) {
	// TODO: this is somewhat shitty...
	printf("deleting node...\n");
	Node* child = this->parent->getChild();
	Node* parent = this->parent;
	Node* prevChild = child;
	while (child->hasSibling()) {
		prevChild = child;
		child = child->getSibling();
	}
	prevChild->setSibling(NULL);
	delete child;
	memcpy(prevChild, this->parent->getChild(), this->parent->getChildren());
	this->parent->decrementChildren();

	printf("entering if statement with childnum: %i\n", this->parent->getChildren());
	if (this->parent->getChildren() == 0) {
		printf("setting child to null\n");
		this->parent->setChild(NULL);
	}

	this->setParent(NULL);
	return parent;
}

void Node::decrementChildren() {
	this->children--;
}

void Node::addChild(Node* child) {
	Node * c = this->child;
	if (c == NULL) {
		this->child = child;
	} else {
		while (c->hasSibling()) { // better for(i = children... ?
			c = c->getSibling();
		}
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

int Node::getChildren() {
	return this->children;
}

void Node::addSibling(Node* sibling) {
	Node * c = this->sibling;
	if (c == NULL) {
		this->sibling = sibling;
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

RuleType::Type Node::getRuleType() {
	return this->rule_type;
}

DataType::Type Node::getDataType() {
	return this->data_type;
}

void Node::setDataType(DataType::Type type) {
	this->data_type = type;
}

/*---------- NodeId ------------*/

NodeId::NodeId(Node* parent, Information* info) {
//	this->Node(TokenType::TokenIdentifier); // does not work like this
	this->parent = parent;
	this->sibling = NULL;
	this->child = NULL;
	this->children = 0;
	this->token_type = TokenType::TokenIdentifier;
	this->rule_type = RuleType::terminal;
	this->data_type = DataType::noType;
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
	this->token_type = TokenType::TokenInteger;
	this->rule_type = RuleType::terminal;
	this->data_type = DataType::noType;  // as we split parsing from type checking, we do not set intType already
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

ParseTree::ParseTree(Node* root) {
	this->root = root;
	this->depth = 0;
	this->printLine = new char[MAX_PRINT];
}

ParseTree::~ParseTree() {
	delete this->root; // triggers destructor chain
	delete[] this->printLine;
}

Node* ParseTree::getRoot() {
	return this->root;
}

void ParseTree::debugPrint() {
	printf("--- PARSE_TREE: ---\n");
	this->recursivePrint(this->root);
	printf("\n");
}

void ParseTree::recursivePrint(Node* thisRoot) {
	// pretty quick C&P solution from http://www.randygaul.net/2015/06/15/printing-pretty-ascii-trees/

	if (thisRoot->getRuleType() == RuleType::terminal)
		printf("[%s/%s]\n", TokenType::tokenNameMini[thisRoot->getTokenType()], DataType::dataName[thisRoot->getDataType()]);
	else
		printf("(%s/%s)\n", RuleType::ruleName[thisRoot->getRuleType()], DataType::dataName[thisRoot->getDataType()]); // should have data_type, but always == noType;

	Node* child = thisRoot->getChild();

	while (child)
	{
		Node* next = child->getSibling();
		printf("%s `--", this->printLine);
		this->indent(next ? '|' : ' ');
		this->recursivePrint(child);
		this->unIndent();
		child = child->getSibling();
	}
}

void ParseTree::indent(char c) {
    this->printLine[this->depth++] = ' ';
    this->printLine[this->depth++] = c;
    this->printLine[this->depth++] = ' ';
    this->printLine[this->depth++] = ' ';
    this->printLine[this->depth] = '\0';
    if (this->depth >= MAX_PRINT - 4) {
    	perror("WARNING! No more space for printing? Exiting for safety ...");
    	exit(EXIT_FAILURE);
    }
}

void ParseTree::unIndent() {
	this->printLine[this->depth -= 4] = '\0';
}
