/*
 * StateStaticDefinitions.h
 *
 *  Created on: Apr 27, 2017
 *      Author: arthur
 */

#ifndef AUTOMAT_INCLUDES_STATESTATICDEFINITIONS_H_
#define AUTOMAT_INCLUDES_STATESTATICDEFINITIONS_H_

#ifndef AUTOMAT_INCLUDES_STATE_H_
#error Error! State.h must be included before StateStaticDefinitions.h
#endif /* AUTOMAT_INCLUDES_STATE_H_ */

StateStart StateStart::instance;

StateError StateError::instance;

StateNumber StateNumber::instance;

StateIdentifier StateIdentifier::instance;

StateSingleSign StateSingleSign::instance;

StateAnd StateAnd::instance;

StateColon StateColon::instance;

StateColonEquals StateColonEquals::instance;

StateEquals StateEquals::instance;

StateEqualsColon StateEqualsColon::instance;

StateEqualsColonEquals StateEqualsColonEquals::instance;

StateCommentBegin StateCommentBegin::instance;

StateCommentEnd StateCommentEnd::instance;

StateUnknown StateUnknown::instance;

#endif /* AUTOMAT_INCLUDES_STATESTATICDEFINITIONS_H_ */
