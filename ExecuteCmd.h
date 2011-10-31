#ifndef EXECUTECMD_H
#define EXECUTECMD_H
#include "VldCmdCtrl.h"
#include "CmdTranslator.h"
#include "ToDoMngr.h"
using namespace std;

template <typename data_t>
void append_back (list<data_t>* main, list<data_t> sub);

class ExecuteCmd {
public:
	ExecuteCmd ();
	~ExecuteCmd ();

	void updateInput (string& input);
	string get_input ();
	//return a string of input that could not be able to access
	//return a string with a break point if execution cannot be done
	virtual bool execute ();
	string result ();

protected:
	string _input;
	string _result;
	ToDoMngr* _toDoMngr;
};

class Add: public CmdTrans, public ExecuteCmd {
public:
	Add (vector<cmd_pair>, ToDoMngr*);
	~Add ();
	bool execute ();
	static string MSG_CLASH;

private:
	static string MSG_ADDED;
	void insertBreakPoint ();
};

//think of wrong id??
class Edit: public CmdTrans, public ExecuteCmd {
public:
	Edit (vector<cmd_pair>, ToDoMngr*);
	~Edit ();
	bool execute ();

private:
	static string MSG_EDITED;
	void insertBreakPoint ();
};

class View: public CmdTrans, public ExecuteCmd {
public:
	View (vector<cmd_pair>, ToDoMngr*);
	~View ();
	bool execute ();
	void deactivateTraverse ();
	string next ();
	string prev ();
	string last ();
	string first ();

private:
	static string MSG_NO_NEXT;
	static string MSG_NO_PREV;
	static string MSG_WRONG_ID;

	int _taskId;
	bool _first;
	bool _last;
	TimePeriod _period;
	Time _time;
	view_t _viewType;
	bool _traverse;
	bool _activeListAccessible;
};

class Delete: public CmdTrans, public ExecuteCmd {
public:
	Delete (vector<cmd_pair>, ToDoMngr*);
	~Delete ();
	bool execute ();

private:
	static string MSG_DELETED;
};

class AccCtrl: public ExecuteCmd {
public:
	AccCtrl (ToDoMngr*);
	~AccCtrl ();
	bool execute ();
};
#endif