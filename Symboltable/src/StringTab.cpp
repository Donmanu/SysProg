/*
 * StringTab.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: donmanu
 */

#include "StringTab.h"

StringTabNode::StringTabNode(char* lexem) {
	this->next = NULL;
	this->lexem = lexem;
}

StringTabNode::~StringTabNode() {
	delete(this->next);
	delete(this->lexem);
}

StringTabNode* StringTabNode::getNext() {
	return this->next;
}

void StringTabNode::setNext(StringTabNode* next) {
	this->next = next;
}

char* StringTabNode::getLexem() {
	return this->lexem;
}

int StringTabNode::getLexemLength() {
	return strlen(this->lexem);
}
