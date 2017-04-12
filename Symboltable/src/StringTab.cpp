/*
 * StringTab.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: donmanu
 */

#include "../includes/StringTab.h"

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

StringTab::StringTab() {
	first_node = NULL;
	node_count = 0;
	table_size = 0;
	free_space = 0;
}

StringTab::~StringTab() {
	delete(first_node);
}

char* StringTab::insert(char* lexem, int size) {
	char c = ' ';
	return &c;
}

StringTabNode* StringTab::getNode() {
	return this->first_node;
}

int StringTab::getNodeCount() {
	return 0;
}

void StringTab::resize() {

}
