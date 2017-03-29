/*
 * Token.h
 *
 *  Created on: Mar 29, 2017
 *      Author: donmanu
 */

#ifndef SCANNER_INCLUDES_TOKEN_H_
#define SCANNER_INCLUDES_TOKEN_H_

enum TokenType {
	TokenUnknown,
	TokenInteger,
	TokenIdentifier,
	TokenPlus,
	TokenMinus,
	TokenColon,
	TokenStar,
	TokenLessThan,
	TokenGreaterThan,
	TokenEquals,
	TokenColonEquals,
	TokenEqualsColonEquals,
	TokenExclamationMark,
	TokenAndAnd,
	TokenSemiColon,
	TokenBracketsOpen,
	TokenBracketsClose,
	TokenSquareBracketsOpen,
	TokenSquareBracketsClose,
	TokenCurlyBracketsOpen,
	TokenCurlyBracketsClose,
	TokenIf,
	TokenWhile
};

class Token {
	public:
		TokenType type;
		int line;
		int column;
};

#endif /* SCANNER_INCLUDES_TOKEN_H_ */
