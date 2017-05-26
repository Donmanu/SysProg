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

SymTabEntry::SymTabEntry(Key* key, StringTabNode* node) {
	this->next = NULL;
	this->string_tab_node = node;
	this->key = key;
}

SymTabEntry::~SymTabEntry() {
	delete this->next;
	delete this->string_tab_node;
	delete this->key;
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
	this->entries = new SymTabEntry[SYMBOL_TABLE_SIZE];
	this->string_table = new StringTab[SYMBOL_TABLE_SIZE];
	this->table_size = SYMBOL_TABLE_SIZE;
	this->free_space = SYMBOL_TABLE_SIZE;
}

Symboltable::~Symboltable() {
	delete[] this->entries;
	delete[] this->string_table;
}

bool Symboltable::containsEntry(SymTabEntry* entry, char* lexem) {
	if (entry->getKey() != NULL) {
		return entry->getKey()->getInformation()->compareLexem(lexem);
	}
	return false;
}

Key* Symboltable::insert(char* lexem) {
	if (lexem == NULL) {
		errno = EINVAL; // invalid arg
		perror("NULL given to Symboltable::insert()");
		return NULL; // also bad, but meh ...
	}
	int hash = this->hash(lexem);
	SymTabEntry* current = &(this->entries[hash]);

	if (this->containsEntry(current, lexem)) {
		return current->getKey();
	}

	Information* information = new Information(lexem);
	Key* key = new Key(information);
	StringTabNode* node = new StringTabNode(lexem);

	if (current->getKey() == NULL) {
		current->setKey(key);
		current->setStringTabNode(node);
		this->entries[hash] = *current;
	} else {
		while (current->hasNext()) {
			current = current->getNext();
		}
		SymTabEntry* next = new SymTabEntry(key, node);
		current->setNext(next);
	}

	this->string_table->insert(lexem, node->getLexemLength());

	return key;
}

Information Symboltable::lookup(Key key) { // wouldn't Symboltable::lookup(char* lexem) be more usefull??
	return *key.getInformation();
}

void Symboltable::resize() {
	int new_table_size = this->table_size * 2;
	SymTabEntry* temp = new SymTabEntry[new_table_size];
	memcpy(temp, this->entries, this->table_size);   // TODO Wrong! Need to rehash all entries with new modulo table_size
	this->table_size = new_table_size;
	this->free_space += SYMBOL_TABLE_SIZE;
	delete[] this->entries;
	this->entries = temp;
}

int Symboltable::hash(char* lexem) {
	// hope it's not NULL here. Check beforehand!
	int hash = 0;
	int i = 0;

	while (lexem[i] != '\0') {
		hash += (SALT * lexem[i]) % this->table_size;
		i++;
	}

	return hash % this->table_size;
}
