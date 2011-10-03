#include "CmdControl.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

string CmdControl::INV_CMD = "unknown command: ";
string CmdControl::INV_DATA = "invalid data input: ";
string CmdControl::MSG_DELETED = " is deleted.";
string CmdControl::MSG_ADDED = " is added.";
string CmdControl::MSG_CLEAR = "All your tasks have been deleted!";
string CmdControl::MSG_CLASH = " is clashed with the following:\n";
string CmdControl::MSG_ERROR = "\n\tERROR!!\n";
string CmdControl::MSG_NO_NEXT = "This is the last!";
string CmdControl::MSG_NO_PREV = "This is the first!";
string CmdControl::MSG_WRONG_DATE = "Invalid date entered!";
string CmdControl::MSG_WRONG_ID = "Invalid index entered!";
string CmdControl::MSG_WRONG_TABLE = "Invalid timetable name entered!";
string CmdControl::MSG_WRONG_PERIOD = "Invalid period entered!";


CmdControl::CmdControl () {
	_validCmdFile = "vldCmd.txt";
	_dfltCmdFile = "dfltCmd.txt";
	_validDayMnth = "vldDM.txt";

	_flagError = NONE;
	_force = false;
	_activeListAccessible = false;
//	_search = ToDoMngr::search_t::SEACH;
// these two following parts are to be modified
	string str = get_vldCmdList (_validCmd, standAloneCmdEndPos);
	_dayMonth = get_dayMonth ();

	if (_validCmd.empty ())
		cout << "empty" << endl;
}

void CmdControl::updateInput (string& input) {
	clear ();
	_input = input;
	_flagError = NONE;
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

CmdControl::input_t CmdControl::getErrorFlag () {
	return _flagError;
}

void CmdControl::splitInput () {
	string temp;
	unsigned int end_pos = 0;

	checkIfStandAloneCmd ();
	while (!_input.empty ()) {
		if (_input[0] == '.' || _input[0] == '-') {
			end_pos = _input.find_first_of (" .-", 1);
			temp = _input.substr (0, end_pos);
			command cmd = translateCmd (temp);
			if (_flagError != CMD) {
				push (cmd);
				if (end_pos == string::npos || _input[end_pos] != ' ')
					_input.erase (0, end_pos);
				else
					_input.erase (0, end_pos + 1);
			} else
				throw (INV_CMD);
		} else {
			end_pos = _input.find_first_of (" .\n");
			while (end_pos != string::npos && _input[end_pos] == '.' && _input[end_pos - 1] != ' ')
				end_pos = _input.find_first_of (" .\n", end_pos + 1);
			temp = _input.substr (0, end_pos);
			push (temp);
			if (end_pos != string::npos)
				_input.erase (0, end_pos + 1);
			else
				_input.erase (0, end_pos);
		}
	}
}

string CmdControl::executeEditCmd (int indx) {
	command pair;
	pair.enum_cmd = convertToCommand (indx);
	string str;
	promptToGetValidInput (str);
	str.erase ();

	if (_sequence->front () == DATA) {
		pair.str_cmd = _dataInput->front ();
		pop ();
		str = ">> " + pair.str_cmd + " indicate " + convertToString (pair.enum_cmd);
		
		vector<cmd_pair>::iterator iter;
		bool existed = search_vldCmd (pair.str_cmd, iter);
		if (!existed) {
			_validCmd.push_back (pair);
		} else if (pair.enum_cmd != iter->enum_cmd) {
				iter->enum_cmd = pair.enum_cmd;
		} else;
	}
	return str;
}

string CmdControl::executeCmd () {
	string str;

	while (_sequence->empty () == false) {
		if (_sequence->front () == CMD) {
			str += convertToString (_cmdInput->front ());
			str += " ";
			_cmdInput->pop ();
		} else if (_sequence->front () == DATA) {
			str += _dataInput->front ();
			str += " ";
			_dataInput->pop ();
		} else
			str += "\nError\n";
		_sequence->pop ();
	}
/*
	do {
		cin >> str;
		_sequence.push (DATA);
		_dataInput.push (str);
//		cout << _sequence.size () << endl;
	} while (str != "end");

//	Time::clk_t clock = get_clock ();
//	cout << clock << endl;
	Time time;
	time = get_time ();
	cout << time.string_date () << " " << time.string_clock () << endl;
	cout << _sequence.size () << endl;
*/
	return str;
}
/*
string CmdControl::executeCmd (command cmd) {
	string str;

	switch (cmd) {
	case ADD:
		str = executeADD ();
		break;
	case EDIT:
		str = executeEDIT ();
		break;
	case DELETE:
		str = executeDELETE ();
		break;
	case TABLE:
		str = executeTABLE ();
		break;
	case VIEW:
		str = executeVIEW ();
		break;
	case REMINDER:
		str = _toDoMngr.reminder ();
	case FIRST:
		while (executePREV ());
		break;
	case LAST:
		while (executeNEXT ());
		break;
	case NEXT:
		if (!executeNEXT ())
			str = MSG_NO_NEXT;
		break;
	case PREVIOUS:
		if (!executePREV ())
			str = MSG_NO_PREV;
		break;
	case UNDO:
		_toDoMngr.undo ();
		break;
	case REDO:
		_toDoMngr.redo ();
		break;
	case SORT:
		break;
	case CLEAR:
		_toDoMngr.clear ();
		str = MSG_CLEAR;
		break;
	case RESET:
		reset ();
		break;
	case HELP:
		if (_sequence.front () == CMD && _cmdInput.front () == COMMAND) {
			pop ();
			str = _toDoMngr.help ("cmd");
		} else if (_sequence.front () == DATA) {
			str = _toDoMngr.help (_dataInput.front ());
			pop ();
		} else {
			str = _toDoMngr.help ("");
		}
		break;
	case EXIT:
		if (_sequence.front () == CMD && _cmdInput.front () == TABLE) {
			pop ();
			_tableName.erase ();
		} else {
			update_dayMonth (_dayMonth);
			_toDoMngr.exit ();
		}
		break;
	default:
		break;
	}

	if (cmd == VIEW)
		_activeListAccessible = true;
	else
		_activeListAccessible = false;

	return str;
}

string CmdControl::executeTABLE () {
	string str;
	pop ();

	if (_sequence.front () == DATA && !getTableName ()) {
		string tableName = _dataInput.front ();
		pop ();

		if (_sequence.front () == CMD && (_cmdInput.front () == FROM || _cmdInput.front () == TO)) {
			tabPeriod:
				TimePeriod period = get_period ();
			
			if (_flagError != DATA) {
				_toDoMngr.newTable (tableName, period);
				_tableName = tableName;
			} else {
				if (promptToGetValidInput (MSG_WRONG_PERIOD))
					goto tabPeriod;
			}
		}
	} else {
		str = _toDoMngr.viewTableNames ();
	}
	return str;
}

string CmdControl::executeDELETE () {
	string str;
	switch (_sequence.front ()) {
	case DATA:
		if (_activeListAccessible) {
			delID:
				string data = _dataInput.front ();
				int taskId = convertToInt (data);

			Task task = _toDoMngr.erase (taskId);
			if (task.get_index () == 0) {
				if (promptToGetValidInput (MSG_WRONG_ID))
					goto delID;
			} else {
				str = ToDoMngr::view (task) + MSG_DELETED;
				pop ();
			}
		}
		break;
	case CMD:
		switch (_cmdInput.front ()) {
		case FROM:
		case TO:
			str = executeFunction (_toDoMngr.erase);
			break;
		case TABLE:
			pop ();
			while (!getTableName ())
				promptToGetValidInput (MSG_WRONG_TABLE);
			
			_toDoMngr.erase (_tableName);
			break;
		default:
			break;
		}
	default:
		break;
	}
	return str;
}

string CmdControl::executeVIEW () {
	string str;

	ToDoMngr::view_t viewType = ToDoMngr::view_t::DAILY;
	if (_sequence.front () == CMD) {
		switch (_cmdInput.front ()) {
		case DAY:
			viewType = ToDoMngr::view_t::DAILY;
			pop ();
			break;
		case WEEK:
			viewType = ToDoMngr::view_t::WEEKLY;	
			pop ();
			break;
		case MONTH:
			viewType = ToDoMngr::view_t::MONTHLY;
			pop ();
			break;
		default:
			break;
		}
	}

	switch (_sequence.front ()) {
	case DATA:
		if (_activeListAccessible) {
			viewID:
				string data = _dataInput.front ();
				int taskId = convertToInt (data);

			str = _toDoMngr.view (taskId);
			if (str.empty ()) {
				if (promptToGetValidInput (MSG_WRONG_ID))
					goto viewID;
			} else {
				_dataInput.pop ();
				_sequence.pop ();
			}
		}
		break;
	case CMD:
		switch (_cmdInput.front ()) {
		case FROM:
		case TO:
			str = executeFunction (_toDoMngr.view);
			break;
		case TIME:
		case DATE:
			pop ();
			viewTime:
				Time time;
				time.modify_date (get_date ());
			
			if (_flagError != DATA) {
				str = _toDoMngr.view (viewType, time);		
			} else {
				if (promptToGetValidInput (MSG_WRONG_DATE))
					goto viewTime;  
			}
			break;
		case TABLE:
			pop ();
			while (!getTableName ()) {
				promptToGetValidInput (MSG_WRONG_TABLE);
			}
				
			str = _toDoMngr.view (viewType, _tableName);
			break;
		default:
			break;
		}
		break;
	default:
		if (!_tableName.empty ())
			str = _toDoMngr.view (viewType, _tableName);
		break;
	}

	return str;
}

string CmdControl::executeADD () {
	string str;

	if (_sequence.front () == CMD && _cmdInput.front () == FORCE) {
		pop ();
		_force = true;
	}
	Task task = get_task ();
		
	add:
		*_taskList = _toDoMngr.add (_tableName, task, _force);

	if (!_force && !_taskList->empty ()) {
		string message = ToDoMngr::view (task) + MSG_CLASH + ToDoMngr::view (taskList);
		if (promptToContinue (message)) {
			_force = true;
			goto add;
		}
	} else if (_force && !_taskList->empty ()) {
		str = MSG_ERROR;
	} else {
		str = ToDoMngr::view (task) + MSG_ADDED;
	}
		
	_taskList->clear ();
	return str;
}

string CmdControl::executeFunction (void* function (TimePeriod)) {
	string str;
	getPeriod:
		TimePeriod period = get_period ();
			
	if (_flagError != DATA) {
		str = _toDoMngr.view (period);		
	} else {
		if (promptToGetValidInput (MSG_WRONG_PERIOD))
			goto getPeriod;  
	}
}
*/
Time CmdControl::get_time () {
	Time time;
	if (_flagError == DATA)
		_flagError = NONE;

	time.modify_date (get_date ());
	time.modify_clock (get_clock ());

	if (_flagError != DATA && time.get_date () == Time::INF_DATE && time.get_clock () != Time::INF_CLOCK) {
		_flagError = NONE;
		Time curr;
		curr.current_time ();

		if (time.get_clock () < curr.get_clock ())
			curr.modify_date (curr.get_date () + 1000000);

		time.modify_date (curr.get_date ());
	}

	return time;
}

Time::date_t CmdControl::get_date () {
	string strDate = _dataInput->front ();
	int size = strDate.size ();
	int Day = 7;
	int tempDay, tempMnth, tempYear;
	int day = -1;
	int mnth = -1;
	int year = -1;

	if (_flagError == DATA)
		_flagError = NONE;

	if (size == 3) {
		convertToInt (strDate, Day, mnth);
		if (Day < 7)
			pop ();
	} else if (size > 3) {
		convertToInt (strDate.substr (0, 3), Day, mnth);
		if (Day < 7)
			_dataInput->front ().erase (0, 3);
	} else;
	mnth = -1;

	while (_sequence->front () == DATA && (day == -1 || mnth == -1 || year == -1)) {
		get_date (tempDay, tempMnth, tempYear);
		if (tempDay != -1 || tempMnth != -1 || tempYear != -1)
			pop ();
		else
			break;

		if (day == -1)
			day = tempDay;
		else {
			if (tempDay != -1 && mnth == -1)
				mnth = tempDay;
			
			if (mnth != -1 && mnth > 12 && day != -1 && day <= 12)
				_dayMonth = false;

			if (!_dayMonth) {
				tempDay = day;
				day = mnth;
				mnth = tempDay;
			}
		}

		if (mnth == -1 && tempMnth != -1)
			mnth = tempMnth;
		if (year == -1)
			year = tempYear;
	}
	
	Time currTime;
	currTime.current_time ();
	if (Day < 7) {
		if (day == -1 && mnth == -1 && year == -1) {
			int currDay = currTime.get_day ();
			currTime = currTime + ((Day + 7 - currDay) % 7) * Time::DAY;
		} else if (day != -1 && mnth == -1 && year == -1) {
			int currDate = currTime.get_date ();
			currDate += (day - currDate / 1000000) * 1000000;				//substitute with user's day
			try {
				currTime.modify_date (currDate);

				while (currTime.get_day () != Day) {
					if (!(++currTime)) {
						_flagError = DATA;
						break;
					}
				}
			} catch (string msg) {
				_flagError = DATA;
			}
		} else if (day != -1 && mnth != -1 && year == -1) {
			int currDate = currTime.get_date ();
			currDate = day * 1000000 + mnth * 10000 + currDate % 10000;	//substitute with user's day and mnth
			try {
				currTime.modify_date (currDate);
				while (currTime.get_day () != Day) {
					if (currTime.get_date () % 10000 == 0) {
						_flagError = DATA;
						break;
					} else {
						currDate++;
						currTime.modify_date (currDate);						//increment by one year
					}
				}
			} catch (string msg) {
				_flagError = DATA;
			}
		} else if (day != -1 && mnth != -1 && year != -1) {
			try {
				currTime.modify_date (day * 1000000 + mnth * 10000 + year);
				
				if (currTime.get_day () != Day)
					_flagError = DATA;
			} catch (string msg) {
				_flagError = DATA;
			}
		} else
			_flagError = DATA;
	} else {
		if (day == -1 && mnth == -1 && year == -1) {
			_flagError = DATA;
		} else if (day != -1 && mnth == -1 && year == -1) {
			int currDate = currTime.get_date ();
			currDate += (day - currDate / 1000000) * 1000000;				//substitute with user's day
			try {
				currTime.modify_date (currDate);
			} catch (string msg) {
				_flagError = DATA;
			}
		} else if (day != -1 && mnth != -1 && year == -1) {
			int currDate = currTime.get_date ();
			currDate = day * 1000000 + mnth * 100000 + currDate % 10000;	//substitute with user's day and mnth
			try {
				currTime.modify_date (currDate);
			} catch (string msg) {
				_flagError = DATA;
			}
		} else if (day != -1 && mnth != -1 && year != -1) {
			try {
				currTime.modify_date (day * 1000000 + mnth * 10000 + year);
			} catch (string msg) {
				_flagError = DATA;
			}
		} else
			_flagError = DATA;
	}
	if (_flagError == DATA)
		return Time::INF_DATE;
	else
		return currTime.get_date ();
}

void CmdControl::get_date (int& day, int& mnth, int& year) {
	string strDate = _dataInput->front ();
	int size = strDate.size ();
	Time::date_t date;
	day = -1;
	mnth = -1;
	year = -1;

	switch (size) {
	case 10:
		if ((strDate[2] == '.' || strDate[2] == '/' || strDate[2] == '-') &&
			(strDate[5] == '.' || strDate[5] == '/' || strDate[5] == '-')) {
				_dataInput.front ().erase (5, 1);
				_dataInput.front ().erase (2, 1);
				get_date (day, mnth, year);
		}
		break;
	case 8:
		if ((strDate[2] == '.' || strDate[2] == '/' || strDate[2] == '-') && 
			(strDate[5] == '.' || strDate[5] == '/' || strDate[5] == '-')) {
				_dataInput.front ().erase (5, 1);
				_dataInput.front ().erase (2, 1);
				get_date (day, mnth, year);
		} else {
			date = convertToInt (strDate);
			if (date <= Time::INF_DATE) {
				if (_dayMonth) {
					day = date / 1000000;
					mnth = date / 10000 - day * 100;
					year = date % 10000;					
				} else {
					mnth = date / 1000000;
					day = date / 10000 - mnth * 100;
					year = date % 10000;
				}
			}
		}
		break;
	case 6:
		date = convertToInt (strDate);
		if (date <= Time::INF_DATE) {
			if (_dayMonth) {
				day = date / 10000;
				mnth = date / 100 - day * 100;
			} else {
				mnth = date / 10000;
				day = date / 100 - mnth * 100;
			}
			
			year = date % 100;
			if (year < 70)
				year += 2000;
			else
				year += 1900;
		}
		break;
	case 9:
		convertToInt (strDate.substr (0, 3), day, mnth);
		if (mnth != 0) {
			date = convertToInt (strDate.substr (3));
			day = date / 10000;
			year = date % 10000;
		} else {
			convertToInt (strDate.substr (2, 3), day, mnth);
			if (mnth != 0) {
				day = convertToInt (strDate.substr (0, 2));
				year = convertToInt (strDate.substr (size - 4));
			} else {
				convertToInt (strDate.substr (4, 3), day, mnth);
				if (mnth != 0) {
					day = convertToInt (strDate.substr (size - 2));
					year = convertToInt (strDate.substr (0, 4));
				} else {
					day = -1;
					mnth = -1;
					year = -1;
				}
			}
		}
		break;
	case 5:
		convertToInt (strDate.substr (0, 3), day, mnth);
		if (mnth != 0) {
			day = convertToInt (strDate.substr (3));
		} else {
			convertToInt (strDate.substr (2, 3), day, mnth);
			if (mnth != 0) {
				day = convertToInt (strDate.substr (0, 2));
			} else {
				day = -1;
				mnth = -1;
				year = -1;
			}
		}
		break;
	case 4:
		year = convertToInt (strDate);
		break;
	case 3:
		convertToInt (strDate, day, mnth);
		day = -1;
		if (mnth == 0)
			mnth = -1;
		break;
	case 2:
		day = convertToInt (strDate);
		break;
	default:
		break;
	}

	switch (size) {
	case 10:
	case 8:
	case 6:
		if (_flagError != DATA && mnth > 12 && day <= 12) {
			int temp = mnth;
			mnth = day;
			day = temp;
			_dayMonth = !_dayMonth;
		}
	}
}

int CmdControl::convertToInt (string str) {
	int num = 0;
	int size = str.size ();

	for (int i = 0; i < size; i++) {
		if (str[i] < '0' || str[i] > '9') {
			num = -1;
			break;
		}
		num *= 10;
		num += str[i] - '0';
	}

	return num;
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
	string strClk = _dataInput->front ();
	int size = strClk.size ();
	int clock = 0;
	if (_flagError == DATA)
		_flagError = NONE;

	switch (size) {
	case 7:
	case 5:
		if ((strClk[2] == '.' || strClk[2] == ':') &&
			(strClk.substr (3, 2) >= "00" && strClk.substr (3, 2) <= "59"))
				clock += (strClk[3] - '0') * 10 + strClk[4] - '0';
		else
			_flagError = DATA;
	case 4:
		if (strClk.substr (0, 2) >= "00" && strClk.substr (0, 2) <= "23")
			clock += (strClk[0] - '0') * 1000 + (strClk[1] - '0') * 100;
		else
			_flagError = DATA;
		break;
	case 3:
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
			pop ();
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

			if (_flagError != DATA)
				pop ();
			break;
		}
	}

	if (clock > 2359)
		clock -= 2400;

	if (!Time::_valid_clock (clock))
		_flagError = DATA;

	if (_flagError == DATA)
		clock = Time::INF_CLOCK;

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
	if (_sequence->front () == DATA) {
		string str = _dataInput->front ();
		if (str.size () == 2 && (str[0] == 'p' || str[0] == 'P') && (str[1] == 'm' || str[1] == 'M')) {
			pop ();
			return true;
		} else if (str.size () == 2 && (str[0] == 'a' || str[0] == 'A') && (str[1] == 'm' || str[1] == 'M')) {
			pop ();
			return false;
		} else;
	}
	return false;
}

/*
CmdControl::CmdControl (string validCmdFile) {
	_validCmdFile = validCmdFile;
	_flagError = NONE;
	_dayMonth = true;

	try {
		loadValidCmdList ();
	} catch (string message) {
		throw (message);
	}
}

CmdControl::CmdControl (vector<cmd_pair> validCmd) {
	_validCmd = validCmd;
	_flagError = NONE;
	_dayMonth = true;
}
*/

void CmdControl::checkIfStandAloneCmd () {
	command cmd;
	bool found = search_standAloneCmd (_input, cmd);

	if (found && cmd != COMMAND) {
		push (cmd);
		_input.erase ();
	} else if (cmd == COMMAND) {
		push (HELP);
		push (COMMAND);
		_input.erase ();
	} else;
}

VldCmdCtrl::command CmdControl::translateCmd (string str) {
	command cmd;
	bool found = search_vldCmd (str, cmd);
	
	if (!found)
		_flagError = CMD;
	else {
		command prev = VOID;
		if (!_cmdInput->empty ())
			prev = _cmdInput->back ();

		bool vldExt = checkIfVldExtension (cmd, prev);

		if (!vldExt)
			_flagError = CMD
	}

	return cmd;
}

void CmdControl::activate2ndQs () {
	_sequence = &_2ndSeq;
	_cmdInput = &_2ndCmd;
	_dataInput = &_2ndData;
}

void CmdControl::deactivate2ndQs () {
	_sequence = &_1stSeq;
	_cmdInput = &_1stCmd;
	_dataInput = &_1stData;
}

void CmdControl::push (command cmd) {
	_cmdInput->push (cmd);
	_sequence->push (CMD);
}

void CmdControl::push (string data) {
	_dataInput->push (data);
	_sequence->push (CMD);
}

bool CmdControl::pop () {
	if (!_sequence->empty ()) {
		switch (_sequence->front ()) {
		case DATA:
			_dataInput->pop ();
			break;
		case CMD:
			_cmdInput->pop ();
			break;
		default:
			break;
		}
		_sequence->pop ();

		if (_sequence->empty ())
			deactivate2ndQs ();

		return true;
	} else
		return false;
}

void CmdControl::clear () {
	clear (*_sequence);
	clear (*_cmdInput);
	clear (*_dataInput);
}

template <typename data_t>
void CmdControl::clear (queue<data_t>& Q) {
	while (!Q.empty ())
		Q.pop ();
}

template <typename data_t>
queue<data_t>& operator+= (queue<data_t>& Q1, queue<data_t> Q2) {
	while (!Q2.empty ()) {
		Q1.push (Q2.front ());
		Q2.pop ();
	}
	return Q1;
}