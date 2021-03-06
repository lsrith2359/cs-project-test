#include "Time.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

string Time::INVALID_DATE = "The date is invalid!";
string Time::INVALID_TIME = "The time is invalid!";
Time::clk_t Time::DFLT_CLOCK = -1;				//default clock
Time::date_t Time::DFLT_DATE = 0;				//default date
Time::clk_t Time::INF_CLOCK = 2400;				//infinite clock
Time::date_t Time::INF_DATE = 31129999;			//infinite date
int Time::DAY = 1440;							//1 day = 1440 mins
int Time::MAX_YEAR = 2100;
int Time::MIN_YEAR = 1970;

Time::Time ()
{
	_clk = DFLT_CLOCK;
	_date = DFLT_DATE;
	_mins = 0;
}

Time::Time (date_t date, clk_t clk)
{
	if (_valid_date (date))
		_date = date;
	else
		_date = DFLT_DATE;

	if (_valid_clock (clk))
		_clk = clk;
	else
		_clk = DFLT_CLOCK;

	convert_to_mins ();
}

bool Time::_valid_date (date_t date)
{
	if (date == INF_DATE || date == DFLT_DATE)
		return true;

	bool valid_date = true;
	date_t max_day, day, mth, year;
	day = date / 1000000;
	mth = (date % 1000000) / 10000;
	year = (date % 10000);

	if (day >= 1 && (mth >= 1 && mth <= 12) && (year >= MIN_YEAR && year <= MAX_YEAR))
	{
		switch (mth)
		{
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				max_day = 31;
				break;
			case 4:
			case 6:
			case 9:
			case 11:
				max_day = 30;
				break;
			case 2:
				if (LeapYear (year))
					max_day = 29;
				else
					max_day = 28;
				break;
			default:
				max_day = 0;
		}

		if (day <= max_day)
			valid_date = true;
		else
			valid_date = false;
	}
	else
		valid_date = false;
		
	return valid_date;
}

bool Time::_valid_clock (clk_t clk)
{
	if (clk == INF_CLOCK || clk == DFLT_CLOCK)
		return true;

	clk_t hour, min;
	hour = clk / 100;
	min = clk % 100;

	if ((hour >= 00 && hour <= 23) && (min >= 00 && min <= 59))
		return true;
	else
		return false;
}

int Time::get_day ()
{
	int numDays = count_days ();
	return (numDays + 3) % 7;
}

void Time::modify_date (date_t new_date)
{
	if (_valid_date (new_date)) {
		_date = new_date;
		_mins = count_days () * DAY;
		if (_clk != INF_CLOCK && _clk != DFLT_CLOCK)
			_mins += _clk % 100 + (_clk / 100) * 60;
	} else {
		throw (INVALID_DATE);
	}
}

void Time::modify_clock (clk_t new_clk)
{
	if (_clk != INF_CLOCK && _clk != DFLT_CLOCK)
		_mins -= _clk % 100 + (_clk / 100) * 60;

	if (_valid_clock (new_clk)) {
		_clk = new_clk;

		if (_clk != INF_CLOCK && _clk != DFLT_CLOCK)
			_mins += _clk % 100 + (_clk / 100) * 60;
	} else {
		throw (INVALID_TIME);
	}
}

void Time::current_time ()
{
	time_t curr_time;
	time (&curr_time);
	string curr_time_str = ctime (&curr_time);
	istringstream str (curr_time_str);
	
	string month, clk;
	int year, day;
	str >> month >> month;
	str >> day >> clk >> year;

	int mth;
	if (month == "Jan")			mth = 1;
	else if (month == "Feb")	mth = 2;
	else if (month == "Mar")	mth = 3;
	else if (month == "Apr")	mth = 4;
	else if (month == "May")	mth = 5;
	else if (month == "Jun")	mth = 6;
	else if (month == "Jul")	mth = 7;
	else if (month == "Aug")	mth = 8;
	else if (month == "Sep")	mth = 9;
	else if (month == "Oct")	mth = 10;
	else if (month == "Nov")	mth = 11;
	else if (month == "Dec")	mth = 12;
	else						mth = 0;

	_date = day * 1000000 + mth * 10000 + year;
	_clk = (clk[0] - '0') * 1000 + (clk[1] - '0') * 100 + (clk[3] - '0') * 10 + (clk[4] - '0');
	convert_to_mins ();
}

Time::clk_t Time::get_clock ()
{
	return _clk;
}

Time::date_t Time::get_date ()
{
	return _date;
}

string Time::string_date ()
{
	if (_date == DFLT_DATE) {
		return "               ";
	} else if (_date == INF_DATE) {
		return "               ";
	} else {
		ostringstream str;
		str << display_day (get_day ()) << " " << setw (2) << setfill ('0')
			<< _date / 1000000 << " " << display_month ((_date % 1000000) / 10000)
			<< " " << _date % 10000;
		return str.str ();
	}
}

string Time::display_month (int mth)
{
	string month;
	switch (mth) {
	case 1:		month = "Jan";	break;
	case 2:		month = "Feb";	break;
	case 3:		month = "Mar";	break;
	case 4:		month = "Apr";	break;
	case 5:		month = "May";	break;
	case 6:		month = "Jun";	break;
	case 7:		month = "Jul";	break;
	case 8:		month = "Aug";	break;
	case 9:		month = "Sep";	break;
	case 10:	month = "Oct";	break;
	case 11:	month = "Nov";	break;
	case 12:	month = "Dec";	break;
	default:	month = "Err";	break;
	}
	return month;
}

string Time::display_day (int day)
{
	string dayStr;
	switch (day) {
	case 0:		dayStr = "Sun";	break;
	case 1:		dayStr = "Mon";	break;
	case 2:		dayStr = "Tue";	break;
	case 3:		dayStr = "Wed";	break;
	case 4:		dayStr = "Thu";	break;
	case 5:		dayStr = "Fri";	break;
	case 6:		dayStr = "Sat";	break;
	default:	dayStr = "Err";	break;
	}
	return dayStr;
}

string Time::string_clock ()
{
	if (_clk == DFLT_CLOCK) {
		return "        ";
	} else if (_clk == INF_CLOCK) {
		return "        ";
	} else {
		ostringstream str;
		clk_t hour, min;
		string AMPM;

		min = _clk % 100;
		if (_clk < 100) {
			hour = 12;
			AMPM = "AM";
		} else if (_clk < 1200) {
			hour = _clk / 100;
			AMPM = "AM";
		} else if (_clk < 1300) {
			hour = _clk / 100;
			AMPM = "PM";
		} else {
			hour = _clk / 100 - 12;
			AMPM = "PM";
		}

		str << setw(2) << setfill ('0') << hour << ":" << setw(2) <<
			setfill ('0') << min << " " << AMPM;
		return str.str ();
	}
}

string Time::string_clock_24 () {
	if (_clk == DFLT_CLOCK) {
		return "        ";
	} else if (_clk == INF_CLOCK) {
		return "        ";
	} else {
		ostringstream str;
		str << setw(2) << setfill ('0') << _clk / 100 << ":" << setw(2) << setfill ('0') << _clk % 100 << "  ";
		return str.str ();
	}
}

bool Time::isAfter (date_t start_date, date_t end_date)
{
	bool _isAfter;

	if (start_date == end_date)
		_isAfter = false;
	else if (start_date % 10000 < end_date % 10000)
		_isAfter = true;
	else if (start_date % 10000 > end_date % 10000)
		_isAfter = false;
	else
	{
		if (start_date % 1000000 < end_date % 1000000)
			_isAfter = true;
		else if (start_date % 1000000 > end_date % 1000000)
			_isAfter = false;
		else
		{
			if (start_date < end_date)
				_isAfter = true;
			else
				_isAfter = false;
		}
	}

	return _isAfter;
}

bool Time::operator> (Time time)
{
	return this->_mins > time._mins;
}

bool Time::operator== (Time time)
{
	return this->_date == time._date && this->_clk == time._clk; 
}

bool Time::operator!= (Time time)
{
	return this->_date != time._date || this->_clk != time._clk;
}

bool Time::operator< (Time time)
{
	return this->_mins < time._mins;
}

int Time::operator- (Time time)
{
	return this->_mins - time._mins;
}

Time& Time::operator+ (int mins)
{
	Time sum;
	if (this->_clk != DFLT_CLOCK && this->_clk != INF_CLOCK)
		sum._mins = this->_mins + mins;
	else
		sum._mins = this->_mins + mins - mins % DAY;

	sum.convert_from_mins ();
	return sum;
}

Time& Time::operator- (int mins)
{
	Time minus;
	if (this->_clk != DFLT_CLOCK && this->_clk != INF_CLOCK)
		minus._mins = this->_mins - mins;
	else
		minus._mins = this->_mins - mins - mins % DAY;

	minus.convert_from_mins ();
	return minus;
}

Time& Time::operator= (Time time)
{
	this->_date = time._date;
	this->_clk = time._clk;
	this->_mins = time._mins;
	return *this;
}

bool Time::operator++ () {
	int mnth = _date / 10000 - (_date / 1000000) * 100;
	int year = _date % 10000;
	int day = _date / 1000000;

	if (mnth == 12) {
		mnth = 1;
		year += 1;
	} else {
		mnth += 1;
	}

	int max_day = days_in_month (mnth, year);
	if (day > max_day)
		day = max_day;

	if (year <= MAX_YEAR) {
		_date = day * 1000000 + mnth * 10000 + year;
		convert_to_mins ();
		return true;
	} else
		return false;
}

bool Time::operator-- () {
	int mnth = _date / 10000 - (_date / 1000000) * 100;
	int year = _date % 10000;
	int day = _date / 1000000;

	if (mnth == 1) {
		mnth = 12;
		year -= 1;
	} else {
		mnth -= 1;
	}

	int max_day = days_in_month (mnth, year);
	if (day > max_day)
		day = max_day;

	if (year >= MIN_YEAR) {
		_date = day * 1000000 + mnth * 10000 + year;
		convert_to_mins ();
		return true;
	} else
		return false;
}

void Time::convert_to_mins ()
{
	_mins = 0;
	if (_date != DFLT_DATE && _date != INF_DATE) {
		_mins += count_days () * DAY;
		if (_clk != DFLT_CLOCK && _clk != INF_CLOCK)
			_mins += (_clk / 100) * 60 + _clk % 100;
	}
}

void Time::convert_from_mins ()
{
	short int min = _mins % 60;
	short int hour = ((_mins - min) % DAY) / 60;
	unsigned int numDays = (_mins - hour * 60 - min) / DAY;
	int year, mnth, day;

	for (year = 1970; numDays > 365 ; year++)
		if (LeapYear (year))
			numDays -= 366;
		else
			numDays -= 365;
	
	if (year <= MAX_YEAR) {
		for (mnth = 1; numDays > days_in_month (mnth, year); mnth++)
			numDays -= days_in_month (mnth, year);
	
		day = numDays;
		_clk = hour * 100 + min;
		_date = day * 1000000 + mnth * 10000 + year;
	} else {
		_clk = INF_CLOCK;
		_date = INF_DATE;
	}
}

int Time::count_days ()
{
	short int day = _date / 1000000;
	short int mnth = (_date % 1000000) / 10000;
	short int year = _date % 10000;
	int numDays = day;
	switch (mnth)
	{
		case 12:	numDays += 30;
		case 11:	numDays += 31;
		case 10:	numDays += 30;
		case 9:		numDays += 31;
		case 8:		numDays += 31;
		case 7:		numDays += 30;
		case 6:		numDays += 31;
		case 5:		numDays += 30;
		case 4:		numDays += 31;
		case 3:		if (LeapYear (year))
						numDays += 29;
					else
						numDays += 28;
		case 2:		numDays += 31;
		case 1:		break;
		default:	break;
	}

	for (int i = 1970; i < year; i++)
		if (LeapYear (i))
			numDays += 366;
		else
			numDays += 365;
	
	return numDays;
}

int Time::days_in_month (int mnth, int year)
{
	int days;
	switch (mnth) {
	case 1:		days = 31;	break;
	case 2:		if (LeapYear (year))
					days = 29;
				else
					days = 28;
				break;
	case 3:		days = 31;	break;
	case 4:		days = 30;	break;
	case 5:		days = 31;	break;
	case 6:		days = 30;	break;
	case 7:		days = 31;	break;
	case 8:		days = 31;	break;
	case 9:		days = 30;	break;
	case 10:	days = 31;	break;
	case 11:	days = 30;	break;
	case 12:	days = 31;	break;
	default:	days = 0;	break;
	}
	return days;
}

bool Time::LeapYear (int year)
{
	if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
		return true;
	else
		return false;
}
