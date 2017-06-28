/*
 * RuleType.h
 *
 *  Created on: Jun 10, 2017
 *      Author: arthur
 */

#ifndef RULETYPE_H_
#define RULETYPE_H_

// for name array:
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace RuleType {

	enum Type {
		prog,		// 0
		decls,		// 1
		decl,		// 2
		array,		// 3
		statements,	// 4
		statement,	// 5
		exp,		// 6
		exp2,		// 7
		index,		// 8
		op_exp,		// 9
		op,			// 10

		terminal	// 11
	};

	/* Short translation for the enum */
	static const char* ruleName[] = {
		"prog",			// 0
		"decls",		// 1
		"decl",			// 2
		"array",		// 3
		"statements",	// 4
		"statement",	// 5
		"exp",			// 6
		"exp2",			// 7
		"index",		// 8
		"op_exp",		// 9
		"op",			// 10
		"TERM"			// 11
	};
}

#endif /* RULETYPE_H_ */
