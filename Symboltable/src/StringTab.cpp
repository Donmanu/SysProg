/*
 * StringTab.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: donmanu
 */

#include "StringTab.h"

StringTabNode::StringTabNode(Information* information) {
	this->information = information;
	this->key = NULL;
	this->next = NULL;
	this->lexem = information->getLexem();
}

StringTabNode::~StringTabNode() {
	delete(this->information);
	delete(this->key);
	delete(this->next);
	delete(this->lexem);
}

Key* StringTabNode::getKey() {
	return this->key;
}

Information* StringTabNode::getInformation() {
	return this->information;
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

int StringTabNode::getSize() {
	return strlen(this->lexem);
}
