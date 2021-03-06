/*
 * State.h
 *
 *  Created on: Mar 24, 2017
 *      Author: donmanu
 */

#ifndef AUTOMAT_INCLUDES_STATE_H_
#define AUTOMAT_INCLUDES_STATE_H_


//#include "../../Scanner/includes/TokenType.h"
#include <stdio.h>
#include <string>
#include "Automat.h"

class Automat;

class State {
	public:
		State() {}
		virtual ~State() {}
		virtual void read(char c, Automat* m) = 0;
};

class StateStart : public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateStart instance;
};

class StateRestart: public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateRestart instance;
};

class StateNumber: public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateNumber instance;
};

class StateIdentifier: public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateIdentifier instance;
};

class StateAnd: public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateAnd instance;
};

class StateColon: public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateColon instance;
};

class StateEquals: public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateEquals instance;
};

class StateEqualsColon: public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateEqualsColon instance;
};

class StateCommentBegin: public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateCommentBegin instance;
};

class StateCommentEnd: public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateCommentEnd instance;
};

#endif /* AUTOMAT_INCLUDES_STATE_H_ */
