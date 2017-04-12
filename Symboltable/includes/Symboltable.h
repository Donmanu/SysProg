/*
 * Symboltable.h
 *
 *  Created on: Sep 26, 2012
 *      Author: knad0001
 */

#include <stdio.h>
#include <stdlib.h>

#include "StringTab.h"

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

class SymTabEntry {
	public:
		SymTabEntry();
		virtual ~SymTabEntry();
		bool hasNext();
		SymTabEntry* getNext();
		void setNext(SymTabEntry* next);
		void setStringTabNode(StringTabNode* node);
		char* getLexem();
		Key* getKey();
		void setKey(Key* key);

	private:
		SymTabEntry* next;
		StringTabNode* string_tab_node;
		Key* key;
};

class Symboltable {
	public:
		Symboltable();
		virtual ~Symboltable();
		Key insert(char* lexem, int value);
		Information lookup(Key key);

	private:
		void initSymbols();
		void resize();
		int hash(char* lexem);
		StringTab* string_table;
		SymTabEntry* entries;
		int table_size;
		int free_space;
		static const int SYMBOL_TABLE_SIZE = 128;
		static const int SALT = 31;

};

#endif /* SYMBOLTABLE_H_ */
