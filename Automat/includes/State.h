/*
 * State.h
 *
 *  Created on: Mar 24, 2017
 *      Author: donmanu
 */

#ifndef AUTOMAT_INCLUDES_STATE_H_
#define AUTOMAT_INCLUDES_STATE_H_

#define TAB_WIDTH (8)

#include "Automat.h"
#include "../../Scanner/includes/TokenType.h"
#include <stdio.h>
#include <string>

class Automat; // <- why after #include "Automat.h" ? TODO

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

class StateError: public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateError instance;
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

class StateSingleSign: public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateSingleSign instance;
};

class StateAnd: public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateAnd instance;
};

class StateAndAnd: public State {
	public:
		void read(char c, Automat *m);
		static State* makeState();

	private:
		static StateAndAnd instance;
};

class StateColon: public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateColon instance;
};

class StateColonEquals: public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateColonEquals instance;
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

class StateEqualsColonEquals: public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateEqualsColonEquals instance;
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

class StateUnknown: public State {
	public:
		void read(char c, Automat* m);
		static State* makeState();

	private:
		static StateUnknown instance;
};

#endif /* AUTOMAT_INCLUDES_STATE_H_ */
