/*
 * Scanner.h
 *
 *  Created on: Sep 26, 2012
 *      Author: knad0001
 */

#include "Token.h"
#include "IScanner.h"

#ifndef SCANNER_H_
#define SCANNER_H_

class ScannerInterface {
	public:
		ScannerInterface() {}
		virtual ~ScannerInterface() {}

		virtual void nextToken() = 0; //Token nextToken() = 0;
		virtual void freeToken() = 0;
};

class Scanner : public ScannerInterface, public IScanner {
public:
	Scanner(char*);
	virtual ~Scanner();

	void nextToken(); //Token nextToken();
	void freeToken();

	void mkToken(TokenType::Type token_type);
	void ungetChar(int count);
	void stop();
private:
	Automat* automat;
	Buffer* buffer;
	Symboltable* symTable;
};

#endif /* SCANNER_H_ */
