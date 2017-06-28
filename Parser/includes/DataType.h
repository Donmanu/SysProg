/*
 * DataType.h
 *
 *  Created on: Jun 15, 2017
 *      Author: arthur
 */

#ifndef DATATYPE_H_
#define DATATYPE_H_

// for name array:
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace DataType {

	enum Type {
		noType,		// 0

		intType,		// 1
		intArrayType,	// 2
		arrayType,		// 3

		opPlus,		// 4
		opMinus,	// 5
		opMult,		// 6
		opDiv,		// 7
		opLess,		// 8
		opGreater,	// 9
		opEqual,	// 10
		opUnEqual,	// 11
		opAnd,		// 12

		errorType	// 13
	};

	/* Short translation for the enum */
	static const char* dataName[] = {
		"none",		// 0
		"int",		// 1
		"intArray",	// 2
		"array",	// 3
		"opPlus",	// 4
		"opMinus",	// 5
		"opMult",	// 6
		"opDiv",	// 7
		"opLess",	// 8
		"opGreater",// 9
		"opEqual",	// 10
		"opUnEqu",	// 11
		"opAnd",	// 12
		"ERROR"		// 13
	};
}

#endif /* DATATYPE_H_ */
