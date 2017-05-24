/*
 * TokenType.h
 *
 *  Created on: Mar 29, 2017
 *      Author: donmanu
 */

#ifndef SCANNER_INCLUDES_TOKENTYPE_H_
#define SCANNER_INCLUDES_TOKENTYPE_H_

namespace TokenType {
	enum Type {
		TokenUnknown, // a.k.a. TokenError
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
		TokenParenthesisOpen,
		TokenParenthesisClose,
		TokenSquareBracketsOpen,
		TokenSquareBracketsClose,
		TokenCurlyBracesOpen,
		TokenCurlyBracesClose,
		TokenWrite,
		TokenRead,
		TokenIf,
		TokenWhile,
		TokenElse,
		TokenInt,

		TokenStop // Metatoken: No more tokens to come
	};
}

#endif /* SCANNER_INCLUDES_TOKENTYPE_H_ */
