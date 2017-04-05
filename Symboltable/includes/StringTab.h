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
		StringTabNode(char* lexem); // TODO: insert parameter lexem
		virtual ~StringTabNode();
		StringTabNode* getNext();
		void setNext(StringTabNode* next);
		char* getLexem();
		int getLexemLength();

	private:
		StringTabNode* next;
		char *lexem;

};

class StringTab {
	public:
		StringTab();
		virtual ~StringTab();
		char* insert(char* lexem, int size);
		StringTabNode* getNode();
		int getNodeCount();

	private:
		void resize();
		StringTabNode* first_node;
		int node_count;
		int table_size;
		int free_space;
		static const int STRING_TABLE_SIZE = 128;
};

#endif /* SYMBOLTABLE_INCLUDES_STRINGTAB_H_ */
