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
#include<iomanip>
#include<iostream>
#include<limits>
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
	Event event;
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
	Node * mergeSort(Node*)		// Function to recursively sort linked list (do not manually execute)
	Node * merge(Node*, Node*);	// Function to merge two sorted linked lists (do not manually execute)
	bool isRunning, soundAlarm, plceholder;
	Node *head;
	std::string filename,;
	std::thread t1, t2;
};

/**
 * Class constructor that loads existing file, sorts it, and stores the proper values.
 * Re-saves file after closing.
 * @param	string representing an existing file name
 */
Schedule::Schedule(std::string filename) : head(nullptr), filename(filename), isRunning(true), soundAlarm(false), placeholder(false)
{
	std::fstream fin;
	fin.open(filename);
	if (!fin.good()) throw std::invalid_argument("ERROR: File does not exist."); else;
	Node *nodeptr = head;
	long long int alarm;
	std::string label;
	while(fin.good())
	{
		nodeptr = new Node;
		std::fin >> alarm;
		std::fin.ignore();
		std::getline(fin, label);
		nodeptr->event.set(alarm, label);
		nodeptr->next = nullptr;
		nodeptr = nodeptr->next;
	}
	std::fin.close();
	sortList();
	// Delete expired alarms
	nodeptr = head;
	time_t current = system_clock::to_time_t(system_clock::now());
	while(head)
	{
		if (nodeptr->event.getAlarmAsInt() < current)
		{
			head = head->next;
			delete nodeptr;
			nodeptr = head;
		}
		else break;
	}
	saveFile();
}

/**
 * Function to handle the main polling loop, includes thread implementation.
 */
void Schedule::pollingLoop()
{
	t1 = thread(&poller::userLoop, this);
	t2 = thread(&poller::alarmLoop, this);
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
	while(head)
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
	date[NUM_DATE_PARAMS] = {1970, 1, 1, 0, 0};
	std::string label = "";
	// Enter event label
	std::cout << "Please enter the name of the Event: ";
	std::getline(cin, label);
	// Enter event date
	bool isGood = true;
	do
	{
		isGood = true;
		std::cout << "Please enter the year of the scheduled event [>1970]: ";
		std::cin >> date[YEAR];
		std::cin.ignore(1000,10);
		if (date[YEAR] < 1970) {std::cout << "ERROR: Year mus be greater than 1970.\n"; isGood = false;} else;
		if ((date[YEAR] % 4 == 0 && time[YEAR] % 100 == 0) || time[YEAR] % 400 != 0) months[1] = 29; else months[1] = 28;
	} while (!isGood)
	do
	{
		isGood = true;
		std::cout << "Please enter the month of the scheduled event [1-12]: ";
		std::cin >> date[MONTH];
		std::cin.ignore(1000,10);
		if (date[MONTH] < 1 || date[MONTH] > 12) {std::cout << "ERROR: Month must be an integer between 1 and 12.\n"; isGood = false;} else;
	} while (!isGood)
	do
	{
		isGood = true;
		std::cout << "Please enter the day of the scheduled event: ";
		std::cin >> date[DAY];
		std::cin.ignore(1000,10);
		if (date[DAY] < 1 || date[DAY] > months[date[MONTH] - 1]) {std::cout << "ERROR: Day must be a valid calander date.\n"; isGood = false;} else;
	} while (!isGood)
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
		} else;
	} while (!isGood)
	do
	{
		isGood = true;
		std::cout << "Please enter the minute of the scheduled event [0-59]: ";
		std::cin >> date[MINUTE];
		if (date[MINUTE] < 0 || date[MINUTE] > 59)
		{
			std::cout << "ERROR: Hour must be a number between 0 and 59.\n";
			isGood = false;
		} else;
	} while (!isGood)
	// Check time for current event
	Event *event = new Event(date, label);
	time_t current = system_clock::to_time_t(system_clock::now());
	if (event.getAlarmAsInt() <= current)
	{
		std::cout << "ERROR: Event scheduled before current time.\n";
		delete event;
	}
	else
	{
		// Add event to list
		Node *newNode = new Node;
		newNode->event = event;
		newNode->next = nullptr;
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
		if (placeholder /* || conditions are met */)
		{
			placeholder = false;
			soundAlarm = true;
		} else;
		while (soundAlarm)
		{
			std::cout << '\a';
			std::thread::this_thread::sleep_for(std::chrono::seconds(1));
		}
		std::thread::this_thread::sleep_for(std::chrono::seconds(1));
	}
	/*
	using std::chrono::system_clock;
	while(!soundAlarm)
	{
		time_t current = system_clock::to_time_t(system_clock::now());
		if (!head) soundAlarm = false;
		else if (current >= head->event.getAlarmAsInt()) soundAlarm = true;
		else soundAlarm = false;
		this_thread::sleep_for(chrono::seconds(1));
	}
	while (soundAlarm)
	{
		bool printAlarm = true;
		if (printAlarm)
		{
			std::cout << "Alert: " << list->event << std::endl;
			std::cout << "Press any key to dismiss...\n";
			printAlarm = false;
		} else;
		std::string exit = "";
		while (std::getline(cin, exit))
		{
			std::cout << "\a";
			this_thread::sleep_for(chrono::seconds(1));
			if (exit.empty()) break;
			else
			{
				soundAlarm = false;
				Node *nodeptr = head;
				head = head->next;
				delete nodeptr;
				std::cin.ignore();
				saveFile();
			}
		}
	}
	*/
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
		std::cout << "Deleted: " << nodeptr->event << std::endl;
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
	fout.open(filename); // Should be test.txt
	Node* nodeptr = head;
	while (nodeptr)
	{
		fout << nodeptr->event.getAlarmAsInt() << "	" << nodeptr->event.getLabel() << std::endl;
		nodeptr = nodeptr->next;
	}
}

/**
 * Function to sort the list of Events.
 */
void Schedule::sortList()
{
	head = mergeSort();
	/*
	Node *nodeptr = list;
	while (nodeptr)
	{
		Node *nextptr = nodeptr->next;
		while (nextptr)
		{   //Error
			if (nodeptr->event->getAlarm() > nextptr->event->getAlarm())
			{
				Event *temp = new Event();
				temp = nodeptr->event;
				nodeptr->event = nextptr->event;
				nextptr->event = temp;
				delete temp;
			}
			else;
			nextptr = nextptr->next;
		}
		nodeptr = nodeptr->next;
	}
	*/
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
			std::cout << "ALERT: " << head->event << std::endl;
			std::cout << "Type any entry to exit.\n";
			std::cin >> input;
			std::cin.ignore(1000,10);
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
			std::cin.ignore(1000,10);
			switch(input)
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
	if (head) std::cout << "Next event: " << head->event << std::endl;
	else std::cout << "No upcoming events.\n";
}

/**
 * Function called within sortList() to recursively sort linked list.
 * @param	(optional) head pointer of list / sub-list
 * @returns	pointer to the head of a sorted list or single element
 */
Node * Schedule::mergeSort(Node *list)
{
	if (!list || !list->next) return list; // Base case (list size is 1)
	else
	{
		// Divide list into two sublists
		Node *nodeptr = list;
		Node *sublist_1 = nodeptr;
		Node *sublist_2 = (nodeptr = nodeptr->next);
		while (nodeptr)
		{
			if (nodeptr) sublist_1 = (nodeptr = nodeptr->next); else;
			if (nodeptr) sublist_2 = (nodeptr = nodeptr->next); else;
		}
	}
	// Return pointer to head of sorted linked list
	return merge(mergeSort(sublist_1), mergeSort(sublist_2));
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
	if (sublist_1->event.getAlarmAsInt() < sublist_2->event.getAlarmAsInt()) list = sublist_1;
	else list = sublist_2;
	Node *nodeptr = list;
	while (sublist_1 || sublist_2)
	{
		if (!sublist_1 && sublist_2)
		{
			nodeptr = sublist_2;
			sublist_2 = sublist_2->next;
		}
		else if (sublist_1 && !sublist_2)
		{
			nodeptr = sublist_1;
			sublist_1 = sublist_1->next;
		}
		else if (sublist_1->event.getAlarmAsInt() < sublist_2->event.getAlarmAsInt())
		{
			nodeptr = sublist_1;
			sublist_1 = sublist_1->next;
		}
		else
		{
			nodeptr = sublist_2;
			sublist_2 = sublist_2->next;
		}
		nodeptr = nodeptr->next;
	}
	return list;
}

#endif
