/*
 * Token.h
 *
 *  Created on: Mar 29, 2017
 *      Author: donmanu
 */

#ifndef SCANNER_INCLUDES_TOKEN_H_
#define SCANNER_INCLUDES_TOKEN_H_

#include "TokenType.h"

class Token {
	public:
		TokenType::Type type;
		int line;
		int column;
};

#endif /* SCANNER_INCLUDES_TOKEN_H_ */
