/**
* File holding Schedule class, methods, and necessary values.
* @author	BERNSTEIN_JOHN
* @author	KLAPSTEIN_DANIEL
* @author	SMITH_EVAN
*/
#ifndef SCHEDULE_H
#define SCHEDULE_H 1
#define _CRT_SECURE_NO_WARNINGS 1

// LIBRARIES
#include "Schedule.h"

#include<chrono>
#include<ctime>
#include<fstream>
#include<iostream>
#include<stdexcept>
#include<string>
#include<thread>

#include<conio.h>
#include<time.h>

#include "Event.h"

// GLOBAL CONSTANTS / VARIABLES
// NONE

// CLASSES / STRUCTS

/**
* This struct represents a single Event Node in the Schedule class.
*/
struct Node
{
	Event e;
	Node *next;
};

/**
* Schedule class to hold an ordered list of Event objects and handle file I/O.
*/
class Schedule
{
public:
	Schedule(std::string);		// Constructor to load file.
	void pollingLoop();			// Loop to handle class functionality
	~Schedule();				// Class destructor
private:
	void addAlarm();			// Function to create a new alarm
	void alarmLoop();			// Loop to handle when an alarm is activated
	void removeNextAlarm();		// Function to delete the next alarm
	void saveFile();			// Function to save file
	void sortList();			// Function to order the list of alarms
	void userLoop();			// Loop to handle user input
	void viewNextAlarm();		// Function to output next alarm
	Node * mergeSort(Node*);	// Function to recursively sort linked list (do not manually execute)
	Node * merge(Node*, Node*);	// Function to merge two sorted linked lists (do not manually execute)
	bool isRunning, soundAlarm;
	Node *head;
	std::string filename;
	std::thread t1, t2;
};

/**
* Class constructor that loads existing file, sorts it, and stores the proper values.
* Re-saves file after closing.
* @param	string representing an existing file name
*/
Schedule::Schedule(std::string filename) : head(nullptr), filename(filename), isRunning(true), soundAlarm(false)
{
	std::ifstream fin;
	fin.open(filename);
	if (!fin.good()) throw std::invalid_argument("ERROR: File does not exist."); else;
	Node *nodeptr = nullptr;
	long long int alarm;
	std::string label;
	while (!fin.eof())
	{
		fin >> alarm;
		fin.ignore();
		std::getline(fin, label);
		if (!head)
		{
			head = new Node;
			head->e.setAlarm(alarm);
			head->e.setLabel(label);
			head->next = nullptr;
			nodeptr = head;
		}
		else
		{
			nodeptr->next = new Node;
			nodeptr = nodeptr->next;
			nodeptr->e.setAlarm(alarm);
			nodeptr->e.setLabel(label);
			nodeptr->next = nullptr;
		}
	}
	fin.close();
	sortList();
	// Delete expired alarms
	/*time_t current = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	Node *temp = head;
	while (head)
	{
		if (temp->e.getAlarmAsInt() < (long long) current)
		{
			head = head->next;
			delete temp;
			temp = head;
		}
		else;
	}*/
	saveFile();
}

/**
* Function to handle the main polling loop, includes thread implementation.
*/
void Schedule::pollingLoop()
{
	t1 = std::thread(&Schedule::userLoop, this);
	t2 = std::thread(&Schedule::alarmLoop, this);
	t1.join();
	t2.join();
}

/**
* Class destructor, saves file before deletion.
*/
Schedule::~Schedule()
{
	sortList();
	saveFile();
	while (head)
	{
		Node *nodeptr = head;
		head = head->next;
		delete nodeptr;
	}
}

/**
* Function to prompt the user to add a new alarm then add to list.
*/
void Schedule::addAlarm()
{
	int *date = new int[NUM_DATE_PARAMS];
	date[YEAR] = 1970;
	date[MONTH] = 1;
	date[DAY] = 1;
	date[HOUR] = 0;
	date[MINUTE] = 0;
	std::string label = "";
	// Enter event label
	std::cout << "Please enter the name of the Event: ";
	std::getline(std::cin, label);
	// Enter event date
	bool isGood = true;
	do
	{
		isGood = true;
		std::cout << "Please enter the year of the scheduled event [>1970]: ";
		std::cin >> date[YEAR];
		std::cin.ignore(1000, 10);
		if (date[YEAR] < 1970) { std::cout << "ERROR: Year mus be greater than 1970.\n"; isGood = false; }
		else;
		if ((date[YEAR] % 4 == 0 && date[YEAR] % 100 == 0) || date[YEAR] % 400 != 0) months[1] = 29; else months[1] = 28;
	} while (!isGood);
	do
	{
		isGood = true;
		std::cout << "Please enter the month of the scheduled event [1-12]: ";
		std::cin >> date[MONTH];
		std::cin.ignore(1000, 10);
		if (date[MONTH] < 1 || date[MONTH] > 12) { std::cout << "ERROR: Month must be an integer between 1 and 12.\n"; isGood = false; }
		else;
	} while (!isGood);
	do
	{
		isGood = true;
		std::cout << "Please enter the day of the scheduled event: ";
		std::cin >> date[DAY];
		std::cin.ignore(1000, 10);
		if (date[DAY] < 1 || date[DAY] > months[date[MONTH] - 1]) { std::cout << "ERROR: Day must be a valid calander date.\n"; isGood = false; }
		else;
	} while (!isGood);
	// Enter event time
	do
	{
		isGood = true;
		std::cout << "Please enter the hour of the scheduled event [0-23]: ";
		std::cin >> date[HOUR];
		if (date[HOUR] < 0 || date[HOUR] > 23)
		{
			std::cout << "ERROR: Hour must be a number between 0 and 23.\n";
			isGood = false;
		}
		else;
	} while (!isGood);
	do
	{
		isGood = true;
		std::cout << "Please enter the minute of the scheduled event [0-59]: ";
		std::cin >> date[MINUTE];
		if (date[MINUTE] < 0 || date[MINUTE] > 59)
		{
			std::cout << "ERROR: Hour must be a number between 0 and 59.\n";
			isGood = false;
		}
		else;
	} while (!isGood);
	// Check time for current event
	Node *newNode = new Node;
	newNode->e.setAlarm(date);
	newNode->e.setLabel(label);
	newNode->next = nullptr;
	time_t current = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	if (newNode->e.getAlarmAsInt() <= current)
	{
		std::cout << "ERROR: Event scheduled before current time.\n";
		delete newNode;
	}
	else
	{
		// Add event to list
		if (!head) head = newNode;
		else
		{
			newNode->next = head;
			head = newNode;
			sortList();
		}
		saveFile();
	}
}

/**
* Sub polling loop that checks if the next alarm has activated. If it has, it handles
* user input.
*/
void Schedule::alarmLoop()
{
	time_t current = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	struct tm * ptm = localtime(&current);
	while (isRunning)
	{
		current = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		ptm = localtime(&current);	// Syntax error
		if (current >= head->e.getAlarmAsInt())
		{
			soundAlarm = true;
		}
		else;
		while (soundAlarm)
		{
			std::cout << '\a';
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

/**
* Function to remove the next alarm in the list (if any).
*/
void Schedule::removeNextAlarm()
{
	if (!head) std::cout << "No scheduled event to remove.\n";
	else
	{
		Node *nodeptr = head;
		head = head->next;
		std::cout << "Deleted: " << nodeptr->e << std::endl;
		delete nodeptr;
		saveFile();
	}
}

/**
* Function to overwrite the file with the updated list.
*/
void Schedule::saveFile()
{
	std::ofstream fout;
	fout.open(filename, std::ofstream::out | std::ofstream::trunc); // Should be test.txt
	Node* nodeptr = head;
	while (nodeptr)
	{
		if (!nodeptr->next) fout << nodeptr->e.getAlarmAsInt() << "	" << nodeptr->e.getLabel();
		else fout << nodeptr->e.getAlarmAsInt() << "	" << nodeptr->e.getLabel() << std::endl;
		nodeptr = nodeptr->next;
	}
	fout.close();
}

/**
* Function to sort the list of Events.
*/
void Schedule::sortList()
{
	Node *list = mergeSort(head);
	head = list;
}

/**
* Function to handle user input in infinite loop.
*/
void Schedule::userLoop()
{
	int input;
	bool isTest = false;
	while (isRunning)
	{
		if (soundAlarm)
		{
			std::cout << "ALERT: " << head->e << std::endl;
			std::cout << "Type any entry to exit.\n";
			std::cin >> input;
			std::cin.ignore(1000, 10);
			soundAlarm = false;
			if (!isTest) removeNextAlarm();
			else isTest = false;
		}
		else
		{
			std::cout << "Please enter one of the following commands:\n";
			std::cout << "1 - View Next Alarm\n";
			std::cout << "2 - Add New Alarm\n";
			std::cout << "3 - Delete Next Alarm\n";
			std::cout << "4 - Test Next Alarm\n";
			std::cout << "0 - Exit\n";
			std::cin >> input;
			std::cin.ignore(1000, 10);
			switch (input)
			{
			case 1:
				viewNextAlarm();
				break;
			case 2:
				addAlarm();
				break;
			case 3:
				removeNextAlarm();
				break;
			case 4:
				isTest = true;
				soundAlarm = true;
				break;
			case 0:
				isRunning = false;
				break;
			default:
				std::cout << "ERROR: '" << input << "' is not a valid command.\n";
				break;
			};
		}
	}
}

/**
* Function to view the upcoming Event in the list (if any).
*/
void Schedule::viewNextAlarm()
{
	if (head) std::cout << "Next event: " << head->e << std::endl;
	else std::cout << "No upcoming events.\n";
}

/**
* Function called within sortList() to recursively sort linked list.
* @param	head pointer of list / sub-list
* @returns	pointer to the head of a sorted list or single element
*/
Node * Schedule::mergeSort(Node *list)
{
	if (!list || !list->next) return list; else; // Base case (list size is 1)
	// Divide list into two sublists
	Node *subhead_1, *subhead_2, *sublist_1, *sublist_2, *nodeptr;
	nodeptr = list;
	// Initialize sublist_1
	subhead_1 = sublist_1 = new Node;
	sublist_1->e.setAlarm(nodeptr->e.getAlarmAsInt());
	sublist_1->e.setLabel(nodeptr->e.getLabel());
	sublist_1->next = nullptr;
	nodeptr = nodeptr->next;
	delete list;
	list = nodeptr;
	// Initialize sublist_2
	subhead_2 = sublist_2 = new Node;
	sublist_2->e.setAlarm(nodeptr->e.getAlarmAsInt());
	sublist_2->e.setLabel(nodeptr->e.getLabel());
	sublist_2->next = nullptr;
	nodeptr = nodeptr->next;
	delete list;
	list = nodeptr;
	// Iterate and modify sublists
	while (nodeptr)
	{
		if (nodeptr)
		{
			sublist_1->next = new Node;
			sublist_1 = sublist_1->next;
			sublist_1->e.setAlarm(nodeptr->e.getAlarm());
			sublist_1->e.setLabel(nodeptr->e.getLabel());
			sublist_1->next = nullptr;
			nodeptr = nodeptr->next;
			delete list;
			list = nodeptr;
		} else;
		if (nodeptr)
		{
			sublist_2->next = new Node;
			sublist_2 = sublist_2->next;
			sublist_2->e.setAlarm(nodeptr->e.getAlarm());
			sublist_2->e.setLabel(nodeptr->e.getLabel());
			sublist_2->next = nullptr;
			nodeptr = nodeptr->next;
			delete list;
			list = nodeptr;
		} else;
	}
	// Return pointer to head of sorted linked list
	list = merge(mergeSort(subhead_1), mergeSort(subhead_2));
	return list;
}

/**
* Function to call within mergeSort() to merge two linked lists.
* @param	head pointer to a sublist to be sorted and merged
* @param	head pointer to the subsequent sublist that is to be sorted and merged
* @returns	head pointer to sorted list
*/
Node * Schedule::merge(Node *sublist_1, Node *sublist_2)
{
	Node *list = nullptr;
	if (sublist_1->e.getAlarmAsInt() < sublist_2->e.getAlarmAsInt()) list = sublist_1;
	else list = sublist_2;
	Node *nodeptr = list;
	do
	{
		if (!sublist_1 && sublist_2)
		{
			nodeptr->next = sublist_2;
			sublist_2 = sublist_2->next;
		}
		else if (sublist_1 && !sublist_2)
		{
			nodeptr->next = sublist_1;
			sublist_1 = sublist_1->next;
		}
		else if (sublist_1->e.getAlarmAsInt() < sublist_2->e.getAlarmAsInt())
		{
			nodeptr->next = sublist_1;
			sublist_1 = sublist_1->next;
		}
		else
		{
			nodeptr->next = sublist_2;
			sublist_2 = sublist_2->next;
		}
		nodeptr = nodeptr->next;
	} while (!sublist_1 && !sublist_2);
	return list;
}

#endif
