/*
 * Symboltable.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: knad0001
 */

#include "../includes/Symboltable.h"

Symboltable::Symboltable() {
	// TODO Auto-generated constructor stub
	entries = new SymTabEntry[SYMBOL_TABLE_SIZE];
	string_table = new StringTab();
	table_size = SYMBOL_TABLE_SIZE;
	free_space = SYMBOL_TABLE_SIZE;
}

Symboltable::~Symboltable() {
	// TODO Auto-generated destructor stub
	delete(entries);
	delete(string_table);
}

Key Symboltable::insert(char* lexem, int value) {
	// TODO: implement this
	return Key();
}

Information Symboltable::lookup(Key key) {
	// TODO: implement this
	return Information("ABC", 3);
}

void Symboltable::initSymbols() {
	// TODO: init keywords
}

void Symboltable::resize() {
	int new_table_size = table_size + SYMBOL_TABLE_SIZE;
	SymTabEntry* temp = new SymTabEntry[new_table_size];
	memcpy(temp, entries, new_table_size);
	table_size = new_table_size;
	free_space += SYMBOL_TABLE_SIZE;
	delete[] entries;
	entries = temp; // TODO: check this
}

int Symboltable::hash(char* lexem) {
	int hash = 0;
	int i = 0;

	while (lexem != '\0') {
		hash += SALT * lexem[i];
		i++;
	}

	return hash % table_size;
}
