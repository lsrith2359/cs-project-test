#ifndef TODOMNGR_H
#define TODOMNGR_H
#include "Task.h"
#include "DataStorage.h"
#include <list>
#include <queue>
#include <stack>
#include <string>
using namespace std;

enum searchType {EXACT, SIMILAR, INDIVIDUAL};

class ToDoMngr {
private:
	static string _storageFile;
	static DataStorage _dataStorage;

public:
	ToDoMngr ();

//Ris:
	string view (Task* task);
	// return a string of the view of the specific task
	string view (string viewType);
	// return a string of the view of the specific tasks, by day, by week or by table name
	// have to consider how to display the clashed tasks
	// for each task shown, numering them from 1 to the end and save as a vector of address of template
	// if user enter view 5, we execute view (templateVect[4]); 
	string reminder ();
	// return a string of the view of the reminder whenever ToDoMngr is executed;
	string alert ();
	// return a string to pop up whenever a task is set to be alert at any specified time
	
	void help (string command);
	// view help based on specific command, view full help if command == NULL;
	// for V0.1, suppose that the command is always NULL.
	
	//***Switch User*** function prototype to be completed

	void undo ();
	void redo ();

//Ben:
	Task addTask (Task task, bool forceAdd);
	// add a task to the dataStorage
	// if forceAdd is true, just add and return a fresh Template
	// else return the clashed task

	list<Task> addTask (Task task, RepeatType repeat, bool forceAdd);
	// add a task to the dataStorage based on repeat type

	void editTask (//argument to be specified//);
	Task editTask (//argument to be specified//);
	list<Task> editTask (//argument to be specified//);
	void eraseTask (//argument to be specified//);
	// functions overloading are considerable

	bool newTable (string name, TimePeriod period);
	// create a new timetable, save it with this name
	// return false if the name is already exist
	list<Task> addInTable (Task task);
	// add tasks periodically to the table
	// return a list of clashed tasks if any
	void editInTable (//argument to be specified//);
	void eraseInTable (//argument to be specified//);
	// function overloading are considerable
	// edit or erase all task periodically
	void eraseTable (string name);
	// erase the whole Table with the name specified
	list<TimePeriod> eraseTable (TimePeriod period);
	// return a list of periods of a similar period if cannot find the table with the time period
	// return a clear list if be able to erase

};
#endif