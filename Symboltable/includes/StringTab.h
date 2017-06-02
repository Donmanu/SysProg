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
		StringTabNode(const char* lexem);
		virtual ~StringTabNode();
		const char* getLexem();
		int getLexemLength();

	private:
		const char *lexem;
		// StringTabNode* next?
};

class StringTab {
	public:
		StringTab();
		virtual ~StringTab();
		char* insert(const char* lexem, int size);
		StringTabNode* getFirstNode();
		int getNodeCount();

		void debugPrint();

	private:
		void resize();
		StringTabNode* first_node;
		char* string_table;
		int node_count;
		int table_size;
		int free_space;
		char* free_ptr;
		static const int STRING_TABLE_SIZE = 16;
};

#endif /* SYMBOLTABLE_INCLUDES_STRINGTAB_H_ */
