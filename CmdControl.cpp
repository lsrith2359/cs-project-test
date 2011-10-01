#include "CmdControl.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

string LOST_FILE = "unable to find a file: ";
string INV_CMD = "unknown command: ";

CmdControl::CmdControl () {
	_validCmdFile = "vldCmd.txt";
	_flagError = NONE;
	_dateBeforeMonth = true;

	try {
		loadValidCmdList ();
	} catch (string message) {
		throw (message);
	}
}

CmdControl::CmdControl (string validCmdFile) {
	_validCmdFile = validCmdFile;
	_flagError = NONE;
	_dateBeforeMonth = true;

	try {
		loadValidCmdList ();
	} catch (string message) {
		throw (message);
	}
}

CmdControl::CmdControl (vector<cmd_pair> validCmd) {
	_validCmd = validCmd;
	_flagError = NONE;
	_dateBeforeMonth = true;
}

void CmdControl::loadValidCmdList ()
{
	ifstream inFile (_validCmdFile);

	if (inFile.is_open ()) {
		cmd_pair cmd;
		int cmdIndx;
		while (inFile >> cmdIndx && inFile >> cmd.str_cmd) {
			cmd.enum_cmd = convertToCommand (cmdIndx);
			_validCmd.push_back (cmd);
		}
	} else
		throw (LOST_FILE + _validCmdFile);
}

template <typename data_t>
void clear (queue<data_t>& Q) {
	while (!Q.empty ())
		Q.pop ();
}

void CmdControl::updateInput (string& input) {
	_input = input;
	clear (_cmdInput);
	clear (_dataInput);
	clear (_sequence);
	try {
		splitInput ();
	} catch (string message) {
		throw (message);
	}
};

void CmdControl::addInput (string& input) {
	_input = input;
	try {
		splitInput ();
	} catch (string message) {
		throw (message);
	}
};

template <typename data_t>
queue<data_t>& operator+= (queue<data_t>& Q1, queue<data_t> Q2) {
	while (!Q2.empty ()) {
		Q1.push (Q2.front ());
		Q2.pop ();
	}
	return Q1;
}

CmdControl::input_t CmdControl::getErrorFlag () {
	return _flagError;
}

void CmdControl::splitInput () {
	string temp;
	unsigned int end_pos = 0;

	while (!_input.empty ()) {
		if (_input[0] == '.' || _input[0] == '-') {
			end_pos = _input.find_first_of (" .-", 1);
			temp = _input.substr (0, end_pos);
			command cmd = translateCmd (temp);
			if (_flagError != CMD) {
				_sequence.push (CMD);
				_cmdInput.push (cmd);

				if (end_pos == string::npos || _input[end_pos] != ' ')
					_input.erase (0, end_pos);
				else
					_input.erase (0, end_pos + 1);
			} else
				throw (INV_CMD);
		} else {
			end_pos = _input.find_first_of (" .\n");
			temp = _input.substr (0, end_pos);
			_sequence.push (DATA);
			_dataInput.push (temp);

			if (end_pos != string::npos)
				_input.erase (0, end_pos + 1);
			else
				_input.erase (0, end_pos);
		}
	}
}

CmdControl::command CmdControl::translateCmd (string str) {
	int size = _validCmd.size ();
	bool found = false;
	command cmd = VOID;
	for (int i = 0; !found && i < size; i++) {
		if (str == _validCmd[i].str_cmd) {
			found = true;
			cmd = _validCmd[i].enum_cmd;
		}
	}

	if (!found)
		_flagError = CMD;
	
	return cmd;
}

string CmdControl::convertToString (command cmd) {
	string str;
	switch (cmd) {
	case (COSTOM):		str = "COSTOM";		break;
	case (FORCE):		str = "FORCE";		break;
	case (EXACT):		str = "EXACT";		break;
	case (SIMILAR):		str = "SIMILAR";	break;
	case (EACH):		str = "EACH";		break;
	case (COMMAND):		str = "COMMAND";	break;
	case (TIME):		str = "TIME";		break;
	case (DATE):		str = "DATE";		break;
	case (FROM):		str = "FROM";		break;
	case (TO):			str = "TO";			break;
	case (NAME):		str = "NAME";		break;
	case (VENUE):		str = "VENUE";		break;
	case (NOTE):		str = "NOTE";		break;
	case (ALERT):		str = "ALERT";		break;
	case (REPEAT):		str = "REPEAT";		break;
	case (HIGH):		str = "HIGH";		break;
	case (IMPT):		str = "IMPT";		break;
	case (NOMAL):		str = "NOMAL";		break;
	case (DAY):			str = "DAY";		break;
	case (WEEK):		str = "WEEK";		break;
	case (MONTH):		str = "MONTH";		break;
	case (ADD):			str = "ADD";		break;
	case (EDIT):		str = "EDIT";		break;
	case (DELETE):		str = "DELETE";		break;
	case (TABLE):		str = "TABLE";		break;
	case (VIEW):		str = "VIEW";		break;
	case (REMINDER):	str = "REMINDER";	break;
	case (NEXT):		str = "NEXT";		break;
	case (PREVIOUS):	str = "PREVIOUS";	break;
	case (FIRST):		str = "FIRST";		break;
	case (LAST):		str = "LAST";		break;
	case (UNDO):		str = "UNDO";		break;
	case (REDO):		str = "REDO";		break;
	case (HELP):		str = "HELP";		break;
	case (SORT):		str = "SORT";		break;
	case (SEARCH):		str = "SEARCH";		break;
	case (CLEAR):		str = "CLEAR";		break;
	case (RESET):		str = "RESET";		break;
	case (EXIT):		str = "EXIT";		break;
	default:			str = "VOID";		break;
	}
	return str;
}

CmdControl::command CmdControl::convertToCommand (int indx) {
	command cmd;
	switch (indx) {
	case 0:		cmd = COSTOM;	break;
	case 1:		cmd = FORCE;	break;
	case 2:		cmd = EXACT;	break;
	case 3:		cmd = SIMILAR;	break;
	case 4:		cmd = EACH;		break;
	case 5:		cmd = COMMAND;	break;
	case 6:		cmd = TIME;		break;
	case 7:		cmd = DATE;		break;
	case 8:		cmd = FROM;		break;
	case 9:		cmd = TO;		break;
	case 10:	cmd = NAME;		break;
	case 11:	cmd = VENUE;	break;
	case 12:	cmd = NOTE;		break;
	case 13:	cmd = ALERT;	break;
	case 14:	cmd = REPEAT;	break;
	case 15:	cmd = HIGH;		break;
	case 16:	cmd = IMPT;		break;
	case 17:	cmd = NOMAL;	break;
	case 18:	cmd = DAY;		break;
	case 19:	cmd = WEEK;		break;
	case 20:	cmd = MONTH;	break;
	case 21:	cmd = ADD;		break;
	case 22:	cmd = EDIT;		break;
	case 23:	cmd = DELETE;	break;
	case 24:	cmd = TABLE;	break;
	case 25:	cmd = VIEW;		break;
	case 26:	cmd = REMINDER;	break;
	case 27:	cmd = NEXT;		break;
	case 28:	cmd = PREVIOUS;	break;
	case 29:	cmd = FIRST;	break;
	case 30:	cmd = LAST;		break;
	case 31:	cmd = UNDO;		break;
	case 32:	cmd = REDO;		break;
	case 33:	cmd = HELP;		break;
	case 34:	cmd = SORT;		break;
	case 35:	cmd = SEARCH;	break;
	case 36:	cmd = CLEAR;	break;
	case 37:	cmd = RESET;	break;
	case 38:	cmd = EXIT;		break;
	default:	cmd = VOID;		break;
	}

	return cmd;
}

string CmdControl::executeCmd () {
	string str;
/*
	while (_sequence.empty () == false) {
		if (_sequence.front () == CMD) {
			str += convertToString (_cmdInput.front ());
			str += " ";
			_cmdInput.pop ();
		} else if (_sequence.front () == DATA) {
			str += _dataInput.front ();
			str += " ";
			_dataInput.pop ();
		} else
			str += "\nError\n";
		_sequence.pop ();
	}
*/
	_sequence.push (DATA);
	_dataInput.push ("12.00");
	_sequence.push (DATA);
	_dataInput.push ("PM");

	Time::clk_t clock = get_clock ();
	cout << clock << endl;
	cout << _sequence.size () << endl;

	return str;
}

Time::date_t CmdControl::get_date () {
	string strDate = _dataInput.front ();
	int size = strDate.size ();
	int date = 0;
	return date;
}

void CmdControl::convertToInt (string str, int& day, int& mnth) {
	day = 7;
	mnth = 0;
	if (str == "Sun" || str == "sun")
		day = 0;
	else if (str == "Mon" || str == "mon")
		day = 1;
	else if (str == "Tue" || str == "tue")
		day = 2;
	else if (str == "Wed" || str == "wed")
		day = 3;
	else if (str == "Thu" || str == "thu")
		day = 4;
	else if (str == "Fri" || str == "fri")
		day = 5;
	else if (str == "Sat" || str == "sat")
		day = 6;
	else if (str == "Jan" || str == "jan")
		mnth = 1;
	else if (str == "Feb" || str == "feb")
		mnth = 2;
	else if (str == "Mar" || str == "mar")
		mnth = 3;
	else if (str == "Apr" || str == "apr")
		mnth = 4;
	else if (str == "May" || str == "may")
		mnth = 5;
	else if (str == "Jun" || str == "jun")
		mnth = 6;
	else if (str == "Jul" || str == "jul")
		mnth = 7;
	else if (str == "Aug" || str == "aug")
		mnth = 8;
	else if (str == "Sep" || str == "sep")
		mnth = 9;
	else if (str == "Oct" || str == "oct")
		mnth = 10;
	else if (str == "Nov" || str == "nov")
		mnth = 11;
	else if (str == "Dec" || str == "dec")
		mnth = 12;
	else;
}

Time::clk_t CmdControl::get_clock () {
	string strClk = _dataInput.front ();
	int size = strClk.size ();
	int clock = 0;
	
	switch (size) {
	case 7:
	case 5:
		if ((strClk[2] == '.' || strClk[2] == ':') &&
			(strClk.substr (3, 2) >= "00" && strClk.substr (3, 2) <= "59"))
				clock += (strClk[3] - '0') * 10 + strClk[4] - '0';
		else
			_flagError = DATA;
	case 4:
	case 2:
		if (strClk.substr (0, 2) >= "00" && strClk.substr (0, 2) <= "23")
			clock += (strClk[0] - '0') * 1000 + (strClk[1] - '0') * 100;
		else
			_flagError = DATA;
		break;
	case 3:
	case 1:
		if (strClk[0] >= '0' && strClk[0] <= '9')
			clock += (strClk[0] - '0') * 100;
		else
			_flagError = DATA;
		break;
	default:
		_flagError = DATA;
		break;
	}

	if (_flagError != DATA) {
		switch (size) {
		case 5:
		case 2:
		case 1:
			_dataInput.pop ();
			_sequence.pop ();
			if (clock <= 1259 && notMorning ()) {
				if (clock != 1200)
					clock += 1200;
			}
			else if (clock <= 1259 && clock == 1200)
					clock += 1200;
			else;
			break;
		case 7:
		case 4:
		case 3:
			if (clock >= 1259)
				_flagError = DATA;
			else if (notMorning (strClk.substr (size - 2))) {
				if (clock != 1200)
					clock += 1200;
			}
			else
				if (clock == 1200)
					clock += 1200;

			if (_flagError != DATA) {
				_dataInput.pop ();
				_sequence.pop ();
			}
			break;
		}
	}

	if (clock > 2359)
		clock -= 2400;

	if (!Time::_valid_clock (clock))
		_flagError = DATA;

	if (_flagError == DATA)
		clock = 0;

	return clock;
}

bool CmdControl::notMorning (string str) {
	if (str.size () == 2 && (str[0] == 'p' || str[0] == 'P') && (str[1] == 'm' || str[1] == 'M'))
		return true;
	else if (str.size () == 2 && (str[0] == 'a' || str[0] == 'A') && (str[1] == 'm' || str[1] == 'M'))
		return false;
	else {
		_flagError = DATA;
		return false;
	}
}

bool CmdControl::notMorning () {
	if (_sequence.front () == DATA) {
		string str = _dataInput.front ();
		if (str.size () == 2 && (str[0] == 'p' || str[0] == 'P') && (str[1] == 'm' || str[1] == 'M')) {
			_sequence.pop ();
			_dataInput.pop ();
			return true;
		} else if (str.size () == 2 && (str[0] == 'a' || str[0] == 'A') && (str[1] == 'm' || str[1] == 'M')) {
			_sequence.pop ();
			_dataInput.pop ();
			return false;
		} else;
	}

	return false;
}

template <typename data_t>
int binary_find (vector<data_t>& vect, data_t& data) {
	return find (vect, data, 0, vect.size () - 1);
}

template <typename data_t>
int binary_find (vector<data_t>& vect, data_t& data, int min, int max) {
	int mid = (min + max) / 2;
	if (vect[mid] == data)
		return mid;
	else if (vect[mid] > data)
		return find (vect, data, min, mid - 1);
	else if (vect[mid] < data)
		return find (vect, data, mid + 1, max);
}