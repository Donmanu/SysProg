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
		SymTabEntry(Key* key, StringTabNode* node);
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
		Key insert(char* lexem);
		Information lookup(Key key);

		SymTabEntry* entries;

	private:
		void resize();
		int hash(char* lexem);
		bool containsEntry(SymTabEntry* entry, char* lexem);
		StringTab* string_table;

		int table_size;
		int free_space;
		static const int SYMBOL_TABLE_SIZE = 8;
		static const int SALT = 31;

};

#endif /* SYMBOLTABLE_H_ */
