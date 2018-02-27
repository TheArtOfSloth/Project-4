/**
* File holding Schedule class, methods, and necessary values.
* @author	BERNSTEIN_JOHN
* @author	KLAPSTEIN_DANIEL
* @author	SMITH_EVAN
* @author       DHILLON_NAVI
* @author	TARIN
*/
#ifndef POLLER_H
#define POLLER_H 1
#define _CRT_SECURE_NO_WARNINGS 1

// LIBRARIES
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

using namespace std;

/**
* This struct represents a single Event Node in the Schedule class.
*/
struct Node
{
	Event event;
	Node *next;
};

/*
used to store data and run threads.
*/
class poller
{
public:
	poller(string);
	~poller();
	void pollingLoop();
private:
	void addAlarm();
	void deleteNextAlarm();
	void sortAlarms();
	void saveFile();
	void viewNextAlarm();
	void userLoop();
	void alarmLoop();
	thread t1;
	thread t2;
	bool isRunning;
	bool alarmGoingOff;
	bool placeholder;
	int command1;
	string command2;
	string filename;
	Node *head;
};

//class constructor
poller::poller(string a)
{
	string labelHold;
	long long int timeStamp;
	isRunning = true;
	alarmGoingOff = false;
	placeholder = false;
	filename = a;
	fstream dataFile;
	dataFile.open(filename);
	head = nullptr;
	Node *hold = new Node;
	hold = nullptr;
	//   CHECK THOROUGHLY!!!!!!!!!!!!!!!!!!!!!
	while (dataFile.good())
	{
		dataFile >> timeStamp >> labelHold;
		Node *temp = new Node;
		temp->event = Event(timeStamp, labelHold);	//LIKELY ISSUES HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (!head)
		{
			head = temp;
			head->next = hold;
		}
		else
		{
			hold = temp;
			hold = hold->next;
			hold = nullptr;
		}
	}
	//   CHECK THOROUGHLY!!!!!!!!!!!!!!!!!!!!!
	delete hold;
	dataFile.close();
};


//class destructor
poller::~poller()
{
	while (head)
	{
		Node *nodeptr = new Node;
		nodeptr = head;
		head = head->next;
		delete nodeptr;
	}
};

//adds alarm
void poller::addAlarm()
{
	bool isGood = true;
	int *date = new int[NUM_DATE_PARAMS];
	date[YEAR] = 1970; date[MONTH] = 1; date[DAY] = 1; date[HOUR] = 0; date[MINUTE] = 0;
	string label = "";

	cout << "Please enter the name of the Alarm: ";
	getline(cin, label);

	while (isGood)
	{
		bool isGood = true;
		cout << "Please enter the date of the Alarm [MM/DD/YYYY]: ";
		cin >> date[MONTH];
		if (cin.get() != '/')
		{
			cout << " Error : Incorrect Date!" << endl;
			isGood = false;
		}
		cin >> date[DAY];
		if (cin.get() != '/')
		{
			cout << " Error : Incorrect Date!" << endl;
			isGood = false;
		}
		cin >> date[YEAR];
		if (cin.get() != '/')
		{
			cout << " Error : Incorrect Date!" << endl;
			isGood = false;
		}

		if (date[YEAR] < 1970)
		{
			cout << "Error : Year must be greater than 1970. " << endl;
			isGood = false;
		}

		if ((date[YEAR] % 4 == 0 && date[YEAR] % 100 == 0) || date[YEAR] % 400 != 0)
			months[1] = 29;
		else months[1] = 28;

		if (date[MONTH] < 1 || date[MONTH] > 12)
		{
			cout << "Error: Month must be between 1 and 12." << endl;
			isGood = false;

		}
		else if (date[MONTH] < 1 || date[DAY] > months[date[MONTH] - 1])
		{
			cout << "Error: Day must be between 1 and 31. " << endl;
			isGood = false;
		}

		while (isGood)
		{
			cout << "Please enter the time of the scheduled event in 24 hour time [HH:MM]: " << endl;
			cin >> date[HOUR];
			if (cin.get() != ':')
			{
				cout << "Error: Time entered incorrectly! " << endl;
			}
			cin >> date[MINUTE];
			if (date[HOUR] < 0 || date[HOUR] > 23)
			{
				cout << "Error: Hour must be between 1 and 23! " << endl;
				isGood = false;
			}
			if (date[MINUTE] < 0 || date[MINUTE] > 59)
			{
				cout << "Error: Hour must be between 1 and 59! " << endl;
				isGood = false;
			}

		}

		Event *eventA = new Event(date, label);
		using chrono::system_clock;
		time_t current = system_clock::to_time_t(system_clock::now());

		if (eventA->getAlarmAsInt() <= current)
		{
			cout << "Error: Alarm schedulded in the past. " << endl;
			delete eventA;
		}

		else
		{
			Node *newNode = new Node;
			newNode->event = *eventA;
			newNode->next = head;
			head = newNode;
			sortAlarms();

			if (!head)
			{
				head = newNode;
			}
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



		}


	}
};

	//deletes next alarm
	void poller::deleteNextAlarm()
	{
		Node *nodeptr = new Node;
		nodeptr = head;
		head = head->next;
		delete nodeptr;
	};

	//sorts alarm list
	//Evan - Do this.
	void poller::sortAlarms()
	{
		//pick a sorting method
		//use it
	};

	//saves list to file
	//John - Do this.
	void poller::saveFile()
	{
	};

	//views next alarm
	//Tarin did this.
	void poller::viewNextAlarm()
	{
		if (head != NULL) {
			cout << "The next scheduled event is " << head->event.getLabel() << endl;
			cout << "It is scheduled for ";
			int* dateArray = head->event.getAlarmAsDate();
			cout << dateArray[HOUR] << ":" << dateArray[MINUTE] << " on " << dateArray[MONTH] << "/" << dateArray[DAY] << "/" << dateArray[YEAR] << endl;
		}

		else {
			cout << "There are no scheduled events" << endl;
		}
	};

	//runs user input
	void poller::userLoop()
	{
		while (isRunning)
		{
			if (alarmGoingOff)
			{
				cout << "ALARM TIME!!! type any entry to exit" << endl;
				cin >> command1;
				alarmGoingOff = false;
				deleteNextAlarm();
			}
			else
			{
				cout << "1 - view next alarm" << endl << "2 - add new alarm" << endl;
				cout << "3 - delete next alarm" << endl << "4 - exit" << endl;
				cout << "5 - test dummy alarm" << endl;
				cin >> command1;
				cout << endl;
				switch (command1)
				{
				case 1:
				{
					viewNextAlarm();
					break;
				};
				case 2:
				{
					addAlarm();
					break;
				};
				case 3:
				{
					deleteNextAlarm();
					break;
				};
				case 4:
				{
					isRunning = false;
					break;
				};
				case 5:
				{
					placeholder = true;
					break;
				};
				default:
				{
					cout << "Invalid option. Try again." << endl;
					break;
				};
				};
			}
		};
	};

	//runs the alarm
	void poller::alarmLoop()
	{
		using chrono::system_clock;
		time_t tt = system_clock::to_time_t(system_clock::now());
		struct tm * ptm = localtime(&tt);
		while (isRunning)
		{
			tt = system_clock::to_time_t(system_clock::now());
			ptm = localtime(&tt);    //Syntax error
			if (placeholder/* || conditions are met*/)
			{
				placeholder = false;
				alarmGoingOff = true;
			};
			if (alarmGoingOff)
			{
				while (alarmGoingOff)
				{
					cout << '\a';
					this_thread::sleep_for(chrono::seconds(1));
				};
			}
			this_thread::sleep_for(chrono::seconds(1));
		}
	};

	//runs the threads
	void poller::pollingLoop()
	{
		t1 = thread(&poller::userLoop, this);
		t2 = thread(&poller::alarmLoop, this);
		t1.join();
		t2.join();
	};
#endif
