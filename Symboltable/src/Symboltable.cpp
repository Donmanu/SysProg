/*
 * Symboltable.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: knad0001
 */

#include "../includes/Symboltable.h"

SymTabEntry::SymTabEntry() {
	this->next = NULL;
	this->string_tab_node = NULL;
	this->key = NULL;
}

SymTabEntry::~SymTabEntry() {
	delete(this->next);
	delete(this->string_tab_node);
//	delete(this->key);
}

bool SymTabEntry::hasNext() {
	return this->next != NULL;
}

SymTabEntry* SymTabEntry::getNext() {
	return this->next;
}

void SymTabEntry::setNext(SymTabEntry* next) {
	this->next = next;
}

void SymTabEntry::setStringTabNode(StringTabNode* node) {
	this->string_tab_node = node;
}

char* SymTabEntry::getLexem() {
	return this->key->getInformation()->getLexem();
}

Key* SymTabEntry::getKey() {
	return this->key;
}

void SymTabEntry::setKey(Key* key) {
	this->key = key;
}

Symboltable::Symboltable() {
	// TODO Auto-generated constructor stub
	this->entries = new SymTabEntry[SYMBOL_TABLE_SIZE];
	this->string_table = new StringTab[SYMBOL_TABLE_SIZE];
	this->table_size = SYMBOL_TABLE_SIZE;
	this->free_space = SYMBOL_TABLE_SIZE;
	this->initSymbols();
}

Symboltable::~Symboltable() {
	// TODO Auto-generated destructor stub
	delete[] this->entries;
	delete[] this->string_table;
}

Key Symboltable::insert(char* lexem, int value) {
	SymTabEntry current = this->entries[this->hash(lexem)];

	while (current.hasNext()) {
		if (current.getKey() == NULL) {
			current = *(current.getNext());
		} else {
			return *(current.getKey());
		}
	}

	Information* information = new Information(lexem, value);
	Key* key = new Key(information);

	StringTabNode* node = new StringTabNode(lexem);
	SymTabEntry* next = new SymTabEntry();

	next->setKey(key);
	next->setStringTabNode(node);
	current.setNext(next);

	this->string_table->insert(lexem, node->getLexemLength());

	return *key;
}

Information Symboltable::lookup(Key key) {
	return *key.getInformation();
}

void Symboltable::initSymbols() {
	// TODO: replace value parameters with something useful
	printf("inserting if...\n");
	this->insert("if", 0);
	printf("inserting IF...\n");
	this->insert("IF", 0);
	printf("inserting while...\n");
	this->insert("while", 0);
	printf("inserting WHILE...\n");
	this->insert("WHILE", 0);
	printf("Success!\n");
}

void Symboltable::resize() {
	int new_table_size = this->table_size + SYMBOL_TABLE_SIZE;
	SymTabEntry* temp = new SymTabEntry[new_table_size];
	memcpy(temp, this->entries, sizeof(this->table_size));
	this->table_size = new_table_size;
	this->free_space += SYMBOL_TABLE_SIZE;
	delete[] this->entries;
	this->entries = temp; // TODO: check this
}

int Symboltable::hash(char* lexem) {
	int hash = 0;
	int i = 0;

	while (lexem[i] != '\0') {
		hash += SALT * lexem[i];
		i++;
	}

	return hash % this->table_size;
}
