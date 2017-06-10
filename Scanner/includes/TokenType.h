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

	/* Full translation for the enum with added padding, so every name has same width with TABSTOP = 8! */
	static const char* tokenNameAligned[] = {
		"TokenUnknown\t\t", // a.k.a. TokenError		//0
		"TokenInteger\t\t",								//1
		"TokenIdentifier\t\t",							//2
		"TokenPlus\t\t",								//3
		"TokenMinus\t\t",								//4
		"TokenColon\t\t",								//5
		"TokenStar\t\t",								//6
		"TokenLessThan\t\t",							//7
		"TokenGreaterThan\t",							//8
		"TokenEquals\t\t",								//9
		"TokenColonEquals\t",							//10
		"TokenEqualsColonEquals\t",						//11
		"TokenExclamationMark\t",						//12
		"TokenAndAnd\t\t",								//13
		"TokenSemiColon\t\t",							//14
		"TokenParenthesisOpen\t",						//15
		"TokenParenthesisClose\t",						//16
		"TokenSquareBracketsOpen\t",					//17
		"TokenSquareBracketsClose", // 24 chars = 6 \t	//18
		"TokenCurlyBracesOpen\t",						//19
		"TokenCurlyBracesClose\t",						//20
		"TokenWrite\t\t",								//21
		"TokenRead\t\t",								//22
		"TokenIf\t\t\t",								//23
		"TokenWhile\t\t",								//24
		"TokenElse\t\t",								//25
		"TokenInt\t\t",									//26
		"TokenStop\t\t"									//27
	};

	/* Full translation for the enum */
	static const char* tokenName[] = {
		"TokenUnknown", // a.k.a. TokenError	//0
		"TokenInteger",							//1
		"TokenIdentifier",						//2
		"TokenPlus",							//3
		"TokenMinus",							//4
		"TokenColon",							//5
		"TokenStar",							//6
		"TokenLessThan",						//7
		"TokenGreaterThan",						//8
		"TokenEquals",							//9
		"TokenColonEquals",						//10
		"TokenEqualsColonEquals",				//11
		"TokenExclamationMark",					//12
		"TokenAndAnd",							//13
		"TokenSemiColon",						//14
		"TokenParenthesisOpen",					//15
		"TokenParenthesisClose",				//16
		"TokenSquareBracketsOpen",				//17
		"TokenSquareBracketsClose", 			//18
		"TokenCurlyBracesOpen",					//19
		"TokenCurlyBracesClose",				//20
		"TokenWrite",							//21
		"TokenRead",							//22
		"TokenIf",								//23
		"TokenWhile",							//24
		"TokenElse",							//25
		"TokenInt",								//26
		"TokenStop"								//27
	};

	/* Short translation for the enum, max 15 chars wide */
	static const char* tokenNameShort[] = {
		"Unknown", // a.k.a. TokenError	//0
		"Integer",						//1
		"Identifier",					//2
		"Plus",							//3
		"Minus",						//4
		"Colon",						//5
		"Star",							//6
		"LessThan",						//7
		"GreaterThan",					//8
		"Equals",						//9
		"ColonEquals",					//10
		"EqualColonEqual",				//11
		"ExclamationMark",				//12
		"AndAnd",						//13
		"SemiColon",					//14
		"ParenthOpen",					//15
		"ParenthClose",					//16
		"SquarBrackOpen",				//17
		"SquarBrackClos", 				//18
		"CurlyBraceOpen",				//19
		"CurlyBraceClos",				//20
		"Write",						//21
		"Read",							//22
		"If",							//23
		"While",						//24
		"Else",							//25
		"Int",							//26
		"Stop"							//27
	};

	/* Short translation for the enum, max 4 chars wide */
	static const char* tokenNameMini[] = {
		"?", // a.k.a. TokenError	//0
		"123",						//1
		"abc",						//2
		"+",						//3
		"-",						//4
		":",						//5
		"*",						//6
		"<",						//7
		">",						//8
		"=",						//9
		":=",						//10
		"=:=",						//11
		"!",						//12
		"&&",						//13
		";",						//14
		"(",						//15
		")",						//16
		"[",						//17
		"]", 						//18
		"{",						//19
		"}",						//20
		"WRIT",						//21
		"READ",						//22
		"IF",						//23
		"WHIL",						//24
		"ELSE",						//25
		"INT",						//26
		"EOF"						//27
	};
}

#endif /* SCANNER_INCLUDES_TOKENTYPE_H_ */
