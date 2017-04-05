/*
 * Symboltable.h
 *
 *  Created on: Sep 26, 2012
 *      Author: knad0001
 */

#include "StringTab.h"

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

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
		StringTab* entries;
		int table_size;
		int free_space;
		const int SYMBOL_TABLE_SIZE = 128;

};

#endif /* SYMBOLTABLE_H_ */
