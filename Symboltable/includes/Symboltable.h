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
		unsigned int hash(char* lexem);

		SymTabEntry** entries;

		void debugPrint();

	private:
		void resize();
		void quickInsert(SymTabEntry* s);
		StringTab* string_table;

		unsigned int table_size;
		unsigned int free_space;
		static const unsigned int SYMBOL_TABLE_SIZE = 64; // should be at least big enough for the keywords: SYMBOL_TABLE_SIZE >= keywords / LOADFACTOR
		static const double LOADFACTOR = 0.75;            // fill level before resize (0.01 - 200.0 have almost same performance)
		// Multiplicative hashing (h = SEED; h += SALT * h + c) parameters:
		// Bernstein: 5381, 33
		// K&R      : 0   , 31
		// sdbm     : 0   , 65599
		static const unsigned int SEED = 5381;
		static const unsigned int SALT = 33;
};

#endif /* SYMBOLTABLE_H_ */
