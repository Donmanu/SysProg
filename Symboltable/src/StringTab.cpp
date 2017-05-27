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
	memset(this->string_table, 0, this->STRING_TABLE_SIZE); // to be on the save site
	this->node_count = 0;
	this->table_size = STRING_TABLE_SIZE;
	this->free_space = STRING_TABLE_SIZE;
	this->free_ptr = &(this->string_table[0]);
}

StringTab::~StringTab() {
	delete[] string_table;
	delete first_node;
}

char* StringTab::insert(char* lexem, int size) {
	// TODO: good to go?
	while (this->free_space < size + 1) {
		this->resize();
	}

	this->node_count++;
	memcpy(this->free_ptr, lexem, size);
	//this->free_ptr[size] = '\0';
	this->free_ptr += size + 1;
	this->free_space -= size + 1;

	return this->free_ptr;
}

StringTabNode* StringTab::getFirstNode() {
	return this->first_node;
}

int StringTab::getNodeCount() {
	return this->node_count;
}

void StringTab::resize() {
	int new_table_size = this->table_size * 2;
	if (new_table_size >= 1024 * 1024 * 1024 /* 1 GB */)
		printf("WARNING! The String table reaches gigabytes in size!");
	char* temp = new char[new_table_size];
	memcpy(temp, this->string_table, this->table_size);
	this->free_space += this->table_size;
	this->table_size = new_table_size;
	delete[] this->string_table;
	this->string_table = temp;
	this->free_ptr = &(this->string_table[this->table_size - this->free_space]);
	memset(this->free_ptr, 0, this->free_space);
}

void StringTab::debugPrint() {
	for (int e = 0; e < this->table_size; e++) {
		//printf("%c", this->string_table[e]); // also print all the '\0'  -> in Eclipse, nothing is shown AFTER the first '\0'. In the console, nothing is printed AS '\0'
		if (this->string_table[e] == '\0') {
			printf("_");
		} else {
			printf("%c", this->string_table[e]);
		}
	}
	printf("\n"); // flushes the out stream
}
