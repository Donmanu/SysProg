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
		StringTabNode(Information* information);
		virtual ~StringTabNode();
		Key* getKey();
		Information* getInformation();
		StringTabNode* getNext();
		void setNext(StringTabNode* next);
		char* getLexem();
		int getSize();

	private:
		Key* key;
		Information* information;
		StringTabNode* next;
		char lexem[];

};

class StringTab {
	public:
		StringTab();
		virtual ~StringTab();
		char* insert(char* lexem, int size);
		StringTabNode* getEntry();
		int getNodeCount();

	private:
		void resize();
		StringTabNode* entries;
		int node_count;
		int table_size;
		int free_space;
		const int STRING_TABLE_SIZE = 128;
};


#endif /* SYMBOLTABLE_INCLUDES_STRINGTAB_H_ */
