/*
 * Information.h
 *
 *  Created on: Apr 5, 2017
 *      Author: donmanu
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <err.h>

#ifndef SYMBOLTABLE_INCLUDES_INFORMATION_H_
#define SYMBOLTABLE_INCLUDES_INFORMATION_H_

class Information {
	public:
		Information(char* lexem);
		virtual ~Information();
		bool compareLexem(char* lexem);
		char* getLexem();
		void incrementOccurrences(void);
		int getOccurrences();

	private:
		char *lexem;
		int occurrences;
};

class Key {
	public:
		Key(Information* information);
		virtual ~Key();
		Information* getInformation();
		void setInformation(Information* information);

	private:
		Information* information;

};

#endif /* SYMBOLTABLE_INCLUDES_INFORMATION_H_ */
