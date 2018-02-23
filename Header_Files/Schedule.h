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
	Schedule(std::string);			// Constructor to load file.
	void pollingLoop();			// Loop to handle class functionality
	~Schedule();				// Class destructor
protected:
	void addAlarm();			// Function to create a new alarm
	void alarmLoop();			// Loop to handle when an alarm is activated
	void removeNextAlarm();			// Function to delete the next alarm
	void saveFile();			// Function to save file
	void sortList();			// Function to order the list of alarms
	void viewNextAlarm();			// Function to output next alarm
private:
	bool isRunning, soundAlarm;
	Node *head;
	std::string filename;
	std::thread t1;
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
	Node *nodeptr = head;
	long long int alarm;
	std::string label;
	while(!fin.eof())
	{
		nodeptr = new Node;
		std::fin >> alarm;
		std::fin.ignore();
		std::getline(fin, label);
		nodeptr->event = new Event(alarm, label);
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
	bool printOutput = true;
	t1 = std::thread(&Schedule::alarmLoop, this);	// Execute background alarm loop
	while(isRunning)
	{
		if (soundAlarm)
		{
			printOutput = true;	// Re-print output
			t1.join(); // Spin lock until alarm is dealt with
			t1 = std::thread(&Schedule::alarmLoop, this); // Re-execute alarmLoop thread
		} else;
		if (printOutput)
		{
			printOutput = false;
			std::cout << "1 - View Next Alarm" << std::endl;
			std::cout << "2 - Add New Alarm" << std::endl;
			std::cout << "3 - Delete Next Alarm" << std::endl;
			std::cout << "0 - Exit Program" << std::endl;
			std::cout << "Enter command: ";
		} else;
		std::string command = "";
		while(std::getline(cin, command))
		{
			if (command.empty()) break;
			else
			{
				switch(command)
				{
					case "1":
						viewNextAlarm();
						break;
					case "2":
						addAlarm();
						break;
					case "3":
						removeNextAlarm();
						break;
					case "0":
						isRunning = false;
						break;
					default:
						std::cout << "ERROR: Invalid command.\n";
				} // switch(command)
				printOutput = true;
			} // else
		} // while(std::getline(cin, command))
	} // while(isRunning)
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
	char slash;
	date[NUM_DATE_PARAMS] = {1970, 1, 1, 0, 0};
	std::string label = "";
	// Enter event label
	std::cout << "Please enter the name of the Event: ";
	std::getline(cin, label);
	// Enter event date
	do
	{
		bool isGood = true;
		std::cout << "Please enter the date of the scheduled event [MM/DD/YYYY]: ";
		std::cin >> date[MONTH];
		if (std::cin.get() != '/')
		{
			std::cout << "ERROR: Date entered incorrectly.\n";
			isGood = false;
			continue;
		} else;
		std::cin >> date[DAY];
		if (std::cin.get() != '/')
		{
			std::cout << "ERROR: Date entered incorrectly.\n";
			isGood = false;
			continue;
		} else;
		std::cin >> date[YEAR];
		if (std::cin.get() != '/')
		{
			std::cout << "ERROR: Date entered incorrectly.\n";
			isGood = false;
			continue;
		} else;
		if (date[YEAR] < 1970) {std::cout << "ERROR: Year mus be greater than 1970.\n"; isGood = false;} else;
		if ((date[YEAR] % 4 == 0 && time[YEAR] % 100 == 0) || time[YEAR] % 400 != 0) months[1] = 29; else months[1] = 28;
		if (date[MONTH] < 1 || date[MONTH] > 12) {std::cout << "ERROR: Month must be an integer between 1 and 12.\n"; isGood = false;}
		else if (date[DAY] < 1 || date[DAY] > months[date[MONTH] - 1]) {std::cout << "ERROR: Day must be a valid calander date.\n"; isGood = false;}
		else;
	} while (!isGood)
	// Enter event time
	do
	{
		std::cout << "Please enter the time of the scheduled event in military time [HH:MM]: ";
		std::cin >> date[HOUR];
		if (std::cin.get() != ':')
		{
			std::cout << "ERROR: Time entered incorrectly.\n";
			isGood = false;
			continue;
		} else;
		std:cin >> date[MINUTE];
		bool isGood = true;
		if (date[HOUR] < 0 || date[HOUR] > 23)
		{
			std::cout << "ERROR: Hour must be between 0 and 23.\n";
			isGood = false;
		} else;
		if (date[MINUTE] < 0 || date[MINUTE] > 59)
		{
			std::cout << "ERROR: Minute must be between 0 and 59.\n";
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
		else if (newNode->event.getAlarmAsInt() < head->event.getAlarmAsInt())
		{
			newNode->next = head;
			head = newNode;
		}
		else
		{
			Node *prevptr = head;
			Node *nodeptr = head->next;
			while (nodeptr || newNode->event.getAlarmAsInt() > nodeptr->event.getAlarmAsInt())
			{
				nodeptr = nodeptr->next;
				prevptr = prevptr->next;
			}
			newNode->next = nodeptr;
			prevptr->next = newNode;
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
}

/**
 * Function to view the upcoming Event in the list (if any).
 */
void Schedule::viewNextAlarm()
{
	if (head) std::cout << "Next event: " << head-event << std::endl;
	else std::cout << "No upcoming events.\n";
}

#endif
