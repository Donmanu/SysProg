/*
 * Symboltable.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: knad0001
 */

#include "../includes/Symboltable.h"

Symboltable::Symboltable() {
	// TODO Auto-generated constructor stub
	entries = new StringTab[SYMBOL_TABLE_SIZE];
	table_size = SYMBOL_TABLE_SIZE;
	free_space = SYMBOL_TABLE_SIZE;
}

Symboltable::~Symboltable() {
	// TODO Auto-generated destructor stub
	delete(entries);
}

Key Symboltable::insert(char* lexem, int value) {
	// TODO: implement this
	return NULL;
}

Information Symboltable::lookup(Key key) {
	// TODO: implement this
	return NULL;
}

void Symboltable::initSymbols() {
	// TODO: init keywords
}

void Symboltable::resize() {
	int new_table_size = table_size + SYMBOL_TABLE_SIZE;
	StringTab* temp = new StringTab[new_table_size];
	memcpy(temp, entries, new_table_size);
	table_size = new_table_size;
	free_space += SYMBOL_TABLE_SIZE;
	entries = temp; // TODO: check this
}

int Symboltable::hash(char* lexem) {
	int value = 0;
	int i = 0;

	while (lexem != '\0') {
		value += lexem[i];
		i++;
	}

	return value % table_size;
}
