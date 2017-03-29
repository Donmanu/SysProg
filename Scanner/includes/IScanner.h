/*
 * IScanner.h
 *
 *  Created on: Mar 24, 2017
 *      Author: donmanu
 */

#ifndef SCANNER_INCLUDES_ISCANNER_H_
#define SCANNER_INCLUDES_ISCANNER_H_

class IScanner {
public:
	IScanner() {}
	virtual ~IScanner() {}
	virtual void mkToken() = 0;
	virtual void ungetChar(int) = 0;
	virtual void stop() = 0;
};


#endif /* SCANNER_INCLUDES_ISCANNER_H_ */
