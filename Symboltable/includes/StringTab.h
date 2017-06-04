/*
 * StringTab.h
 *
 *  Created on: Apr 5, 2017
 *      Author: donmanu
 */

#include "Information.h"
#include "StringTabNode.h"

#ifndef SYMBOLTABLE_INCLUDES_STRINGTAB_H_
#define SYMBOLTABLE_INCLUDES_STRINGTAB_H_

class StringTab {
	public:
		StringTab(void);
		virtual ~StringTab(void);

		const char* insert(const char* lexem, int size);
		StringTabNode* insert(const char* lexem);
		int getNodeCount(void);

		void debugPrint(void);

	private:
		void resize(void);
		void resizeUpdate(void);
		char* string_table;
		StringTabNode* first;
		StringTabNode* last;
		int node_count;
		int table_size;
		int free_space;
		char* free_ptr;
		static const int STRING_TABLE_SIZE = 64; // Shall be enough for at least all keywords: "write_read_if_IF_else_ELSE_while_WHILE_int_"
};

#endif /* SYMBOLTABLE_INCLUDES_STRINGTAB_H_ */
