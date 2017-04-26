/*
 * StringTab.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: donmanu
 */

#include "../includes/StringTab.h"

StringTabNode::StringTabNode(char* lexem) {
	this->lexem = lexem;
}

StringTabNode::~StringTabNode() {
}

char* StringTabNode::getLexem() {
	return this->lexem;
}

int StringTabNode::getLexemLength() {
	return strlen(this->lexem);
}

StringTab::StringTab() {
	this->first_node = NULL;
	this->string_table = new char[STRING_TABLE_SIZE];

	for (int i = 0; i < STRING_TABLE_SIZE; i++) {
		this->string_table[i] = NULL;
	}

	this->node_count = 0;
	this->table_size = STRING_TABLE_SIZE;
	this->free_space = STRING_TABLE_SIZE;
	this->free_ptr = &(this->string_table[0]);
}

StringTab::~StringTab() {
	delete(first_node);
//	delete[] this->string_table;
}

char* StringTab::insert(char* lexem, int size) {
	// TODO: re-think this
	char* tmp = this->free_ptr;
	if (this->free_space < size) {
		this->resize();
	}

	this->node_count++;
	memcpy(tmp, lexem, sizeof(size + 1));
	this->free_ptr[size] = '\0';
	this->free_ptr += size + 1;
	this->free_space -= size + 1;

	return tmp;
}

StringTabNode* StringTab::getFirstNode() {
	return this->first_node;
}

int StringTab::getNodeCount() {
	return this->node_count;
}

void StringTab::resize() {
	int new_table_size = this->table_size + STRING_TABLE_SIZE;
	char* temp = new char[new_table_size];
	memcpy(temp, this->string_table, sizeof(this->table_size));
	this->table_size = new_table_size;
	this->free_space += STRING_TABLE_SIZE;
	delete[] this->string_table;
	this->string_table = temp;
}
