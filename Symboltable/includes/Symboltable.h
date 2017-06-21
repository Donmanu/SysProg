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
		// Con- and destructors
		SymTabEntry(Key*);
		virtual ~SymTabEntry(void);

		// getter, setter
		bool         hasNext(void);
		SymTabEntry* getNext(void);
		void         setNext(SymTabEntry*);
		Key*         getKey(void);
		void         setKey(Key*);

		// shorthands on methods of Key
		bool         compareLexem(const char*);
		const char*  getLexem(void);

	private:
		SymTabEntry* next;
		Key* key;
};

class Symboltable {
	public:
		Symboltable();
		virtual ~Symboltable();

		Key*         insert(const char* lexem);
		Information  lookup(Key key);
		unsigned int hash(const char* lexem);

		void         debugPrint();

	private:
		void resize();
		void reinsert(SymTabEntry* s);

		StringTab* string_table;
		SymTabEntry** entries;
		unsigned int table_size;
		static const unsigned int SYMBOL_TABLE_SIZE = 64; // should be at least big enough for the keywords: SYMBOL_TABLE_SIZE >= keywords / LOADFACTOR
		static const double LOADFACTOR = 0.75;            // fill level to be reached before resize happens
		// Multiplicative hashing (h = SEED; h += SALT * h + c) parameters:
		// Bernstein: 5381, 33
		// K&R      : 0   , 31
		// sdbm     : 0   , 65599
		//static const unsigned int SEED = 5381;
		//static const unsigned int SALT = 33;
};

#endif /* SYMBOLTABLE_H_ */
