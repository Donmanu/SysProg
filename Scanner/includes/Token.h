/*
 * Token.h
 *
 *  Created on: Mar 29, 2017
 *      Author: donmanu
 */

#ifndef SCANNER_INCLUDES_TOKEN_H_
#define SCANNER_INCLUDES_TOKEN_H_

#include "TokenType.h"
#include "../../Symboltable/includes/Information.h"

class Key;

struct Token {
	public:
		TokenType::Type type;
		int line;
		int column;
		int value;
		Key* key;
};

#endif /* SCANNER_INCLUDES_TOKEN_H_ */
