/*
 * StringTab.h
 *
 *  Created on: Apr 5, 2017
 *      Author: donmanu
 */

#include "Information.h"

#ifndef SYMBOLTABLE_INCLUDES_STRINGTAB_H_
#define SYMBOLTABLE_INCLUDES_STRINGTAB_H_

class StringTabNode {
	public:
		StringTabNode(char* lexem);
		virtual ~StringTabNode();
		char* getLexem();
		int getLexemLength();

	private:
		char *lexem;
};

class StringTab {
	public:
		StringTab();
		virtual ~StringTab();
		char* insert(char* lexem, int size);
		StringTabNode* getFirstNode();
		int getNodeCount();

	private:
		void resize();
		StringTabNode* first_node;
		char* string_table;
		int node_count;
		int table_size;
		int free_space;
		char* free_ptr;
		static const int STRING_TABLE_SIZE = 128;
};

#endif /* SYMBOLTABLE_INCLUDES_STRINGTAB_H_ */
