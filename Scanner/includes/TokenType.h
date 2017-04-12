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
		TokenBracketsOpen,
		TokenBracketsClose,
		TokenSquareBracketsOpen,
		TokenSquareBracketsClose,
		TokenCurlyBracketsOpen,
		TokenCurlyBracketsClose,
		TokenIf,
		TokenWhile
	};
}

#endif /* SCANNER_INCLUDES_TOKENTYPE_H_ */
