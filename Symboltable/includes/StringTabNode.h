/*
 * StringTabNode.h
 *
 *  Created on: Jun 4, 2017
 *      Author: arthur
 */

#ifndef STRINGTABNODE_H_
#define STRINGTABNODE_H_

class StringTabNode {
	public:
		StringTabNode(const char* lexem);
		virtual ~StringTabNode(void);

		const char*    getLexem(void);
		StringTabNode* getNext(void);
		void           setNext(StringTabNode*);
		void           updatePointer(long int);

	private:
		const char *lexem;
		StringTabNode* next;
};

#endif /* STRINGTABNODE_H_ */
