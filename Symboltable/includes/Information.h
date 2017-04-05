/*
 * Information.h
 *
 *  Created on: Apr 5, 2017
 *      Author: donmanu
 */

#include <unistd.h>

#ifndef SYMBOLTABLE_INCLUDES_INFORMATION_H_
#define SYMBOLTABLE_INCLUDES_INFORMATION_H_

class Key {
	public:
		Key();
		virtual ~Key();
		Information* getInformation();
		void setInformation(Information* information);

	private:
		Information* information;

};

class Information {
	public:
		Information(char* lexem, int value);
		virtual ~Information();
		bool compareLexem(char* lexem);
		char* getLexem();
		int getValue();
		void setLexem(char* lexem);
		void setValue(int value);

	private:
		char *lexem;
		int value;

};

#endif /* SYMBOLTABLE_INCLUDES_INFORMATION_H_ */
