/*
 * StringTab.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: donmanu
 */

#include "../includes/StringTabNode.h"
#include "../includes/StringTab.h"

/*------------------ StringTabNode -------------------*/

StringTabNode::StringTabNode(const char* lexem) {
	this->lexem = lexem;
	this->next = NULL;
}

StringTabNode::~StringTabNode() {
	// leave this->lexem, not our responsibility!
	// leave this->next, not our responsibility!
}

const char* StringTabNode::getLexem() {
	return this->lexem;
}

void StringTabNode::updatePointer(long int offset) {
	this->lexem += offset;
}

StringTabNode* StringTabNode::getNext(void) {
	return this->next;
}

void StringTabNode::setNext(StringTabNode* next) {
	this->next = next;
}


/*------------------ StringTab -------------------*/

StringTab::StringTab() {
	this->string_table = new char[StringTab::STRING_TABLE_SIZE];
	memset(this->string_table, 0, StringTab::STRING_TABLE_SIZE); // to be on the save site
	this->node_count = 0;
	this->table_size = StringTab::STRING_TABLE_SIZE;
	this->free_space = StringTab::STRING_TABLE_SIZE;
	this->free_ptr = &(this->string_table[0]);
	this->first = NULL;
	this->last = NULL;
}

StringTab::~StringTab() {
	delete[] string_table;
}

/*char* StringTab::insert(const char* lexem, int size) {
	while (this->free_space < size + 1) {
		this->resize();
	}

	this->node_count++;
	memcpy(this->free_ptr, lexem, size);
	this->free_ptr += size + 1;
	this->free_space -= size + 1;

	return this->free_ptr; // after next resize, this pointer will be useless ...
}*/

StringTabNode* StringTab::insert(const char* lexem) {
	int size = strlen(lexem);
	while (this->free_space < size + 1) {
		// the loop may take unnecessarily long, if {node_count} is high, but lexems are short (many updates) and new lexem is long (multiple runs)
		this->resizeUpdate();
	}
	memcpy(this->free_ptr, lexem, size);
	StringTabNode* result = new StringTabNode(this->free_ptr); // Responsibility for deleting is at caller!
	this->free_ptr += size + 1;
	this->free_space -= size + 1;

	if (this->node_count == 0) {
		// on first insert
		this->first = result;
		this->last = result;
	} else {
		this->last->setNext(result);
		this->last = result;
	}

	this->node_count++;
	return result;
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

	this->free_space += this->table_size; // only correct if * 2
	this->table_size = new_table_size;

	delete[] this->string_table;
	this->string_table = temp;

	this->free_ptr = &(this->string_table[this->table_size - this->free_space]);
	memset(this->free_ptr, 0, this->free_space);
}

void StringTab::resizeUpdate(void) {
	int new_table_size = this->table_size * 2;

	if (new_table_size >= 1024 * 1024 * 1024 /* 1 GB */)
		printf("WARNING! The String table reaches gigabytes in size!");

	char* new_table = new char[new_table_size];
	memcpy(new_table, this->string_table, this->table_size);

	this->free_space += this->table_size; // only correct if * 2
	this->table_size = new_table_size;
	delete[] this->string_table;

	StringTabNode* current = first;
	long int offset = new_table - this->string_table;
	while (current) {
		current->updatePointer(offset);
		current = current->getNext();
	}

	this->string_table = new_table;

	this->free_ptr = & this->string_table[this->table_size - this->free_space];
	memset(this->free_ptr, 0, this->free_space);
}

void StringTab::debugPrint() {
	bool wasLastNull = false;
	printf("\n--- STRING_TABLE: ---\n");
	for (int e = 0; e < this->table_size; e++) {
		if (this->string_table[e] == '\0') {
			if (wasLastNull) {
				printf("_...");
				break; // save on the rest.
			} else {
				printf("_");
				wasLastNull = true;
			}
		} else {
			printf("%c", this->string_table[e]);
			wasLastNull = false;
		}
	}

	printf("\n\n  -- STRING_TABLE, overall statistics --\n");
	printf("  Entries   : %d\n", this->node_count);
	printf("  Table_size: %d B\n\n", this->table_size);
	printf("  Free_space: %d B\n\n", this->free_space);
}
