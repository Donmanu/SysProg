/*
 * Scanner.h
 *
 *  Created on: Sep 26, 2012
 *      Author: knad0001
 */

#include "Token.h"
#include "IScanner.h"
#include "../../Automat/includes/Automat.h"
#include "../../Buffer/includes/Buffer.h"
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

	Token nextToken();
	void freeToken();

	void mkToken(TokenType::Type);
	void ungetChar(int count);
	void stop();
private:
	Automat* automat;
	Buffer* buffer;
	bool notoken;
	Token current_token;
};

#endif /* SCANNER_H_ */
