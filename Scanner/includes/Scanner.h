/*
 * Scanner.h
 *
 *  Created on: Sep 26, 2012
 *      Author: knad0001
 */

#include "Token.h"
// Component 1: Automat
#include "../../Automat/includes/Automat.h"
#include "ScannerForAutomat.h"
// Component 2: Buffer
#include "../../Buffer/includes/Buffer.h"
// Component 3: Symboltable
#include "../../Symboltable/includes/Symboltable.h"

#ifndef SCANNER_H_
#define SCANNER_H_

class ScannerInterface {
	public:
		ScannerInterface() {}
		virtual ~ScannerInterface() {}

		virtual Token nextToken() = 0; //Token nextToken() = 0;
		virtual void freeToken() = 0;
};

class Scanner : public ScannerInterface, public IScanner {
public:
	Scanner(char*);
	virtual ~Scanner();

	// ScannerForParser interface:
	Token nextToken();
	void freeToken();

	// ScannerForAutomat interface:
	void mkToken(TokenType::Type);
	void ungetChar(int count);
	void stop(void);

	void debugPrint();

private:
	void initSymbols();
	void inline filterToken(TokenType::Type type);

	static const int KEYWORD_ARRAY_LENGTH = 9;

	bool notoken;
	Automat* automat;
	Buffer* buffer;
	Symboltable* symboltable;
	Key** keywords;
	Token current_token;

};

#endif /* SCANNER_H_ */
