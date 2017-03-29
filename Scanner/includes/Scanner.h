/*
 * Scanner.h
 *
 *  Created on: Sep 26, 2012
 *      Author: knad0001
 */

#include "Token.h"

#ifndef SCANNER_H_
#define SCANNER_H_

class ScannerInterface {
	public:
		ScannerInterface();
		virtual ~ScannerInterface();

		virtual Token nextToken() = 0;
		virtual void freeToken() = 0;
};

class Scanner : public ScannerInterface, IScanner {
public:
	Scanner();
	virtual ~Scanner();

	Token nextToken();
	void freeToken();

	void mkToken(TokenType token_type);
	void ungetChar(int count);
	void stop();
};

#endif /* SCANNER_H_ */
