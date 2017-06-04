/*
 * Information.h
 *
 *  Created on: Apr 5, 2017
 *      Author: donmanu
 */

#include <stdio.h>  // printf()
#include <stdlib.h> // exit()
#include <string.h> // strlen(), strcpy()
#include <error.h>  // perrer()
#include <errno.h>  // EINVAL ...
#include <err.h>    // warn(), err()

#include "../../Symboltable/includes/StringTabNode.h"

#ifndef SYMBOLTABLE_INCLUDES_INFORMATION_H_
#define SYMBOLTABLE_INCLUDES_INFORMATION_H_

class Information {
	public:
		Information(StringTabNode*);
		virtual ~Information();
		bool compareLexem(const char* lexem);
		const char* getLexem();
		void incrementOccurrences(void);
		void decrementOccurrences(void);
		int getOccurrences();

	private:
		StringTabNode* lexemWrapper;
		int occurrences;
};

class Key {
	public:
		Key(unsigned int, Information* information);
		virtual ~Key();
		void setInformation(Information*);
		Information* getInformation();
		void setHash(unsigned int);
		unsigned int getHash();

	private:
		Information* information;
		unsigned int hash;
};

#endif /* SYMBOLTABLE_INCLUDES_INFORMATION_H_ */
