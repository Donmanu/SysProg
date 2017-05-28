/*
 * Symboltable.h
 *
 *  Created on: Sep 26, 2012
 *      Author: knad0001
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <err.h>

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
		Key* insert(char* lexem);
		Information lookup(Key key);

		SymTabEntry** entries;

		void debugPrint();

	private:
		void resize();
		void quickInsert(SymTabEntry* s);
		int hash(char* lexem);
		StringTab* string_table;

		int table_size;
		int free_space;
		static const int SYMBOL_TABLE_SIZE = 16; // should be at least enough for the keywords
		static const int SALT = 31;
		static const double LOADFACTOR = 0.75; // when to resize
};

#endif /* SYMBOLTABLE_H_ */
