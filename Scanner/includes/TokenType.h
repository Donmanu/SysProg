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
		TokenUnknown, // a.k.a. TokenError				//0
		TokenInteger,									//1
		TokenIdentifier,								//2
		TokenPlus,										//3
		TokenMinus,										//4
		TokenColon,										//5
		TokenStar,										//6
		TokenLessThan,									//7
		TokenGreaterThan,								//8
		TokenEquals,									//9
		TokenColonEquals,								//10
		TokenEqualsColonEquals,							//11
		TokenExclamationMark,							//12
		TokenAndAnd,									//13
		TokenSemiColon,									//14
		TokenParenthesisOpen,							//15
		TokenParenthesisClose,							//16
		TokenSquareBracketsOpen,						//17
		TokenSquareBracketsClose,						//18
		TokenCurlyBracesOpen,							//19
		TokenCurlyBracesClose,							//20
		TokenWrite,										//21
		TokenRead,										//22
		TokenIf,										//23
		TokenWhile,										//24
		TokenElse,										//25
		TokenInt,										//26

		TokenStop // Metatoken: No more tokens to come	//27
	};
}

#endif /* SCANNER_INCLUDES_TOKENTYPE_H_ */
