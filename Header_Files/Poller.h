/**
* File holding Schedule class, methods, and necessary values.
* @author	BERNSTEIN_JOHN
* @author	KLAPSTEIN_DANIEL
* @author	SMITH_EVAN
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
used to stare data and run threads.
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
	void viewNextAlarm();
	void userLoop();
	void alarmLoop();
	thread t1;
	thread t2;
	bool isRunning;
	bool alarmGoingOff;
	bool placeholder;
	string command;
	string filename;
	Node *head;
};

//class constructor
poller::poller(string a)
{
	isRunning = true;
	alarmGoingOff = false;
	placeholder = false;
	filename = a;
	//open file
	//read file line-by-line into Node list
	//close file
};

//class destructor
poller::~poller()
{
	while(head)
		{
			Node *nodeptr = head;
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

	do
	{
		bool isGood = true;
		cout << "Please enter the date of the Alarm [MM/DD/YYYY]: ";
		cin >> date[MONTH];
			if (cin.get() != '/')
			{
				cout << " Error : Incorret Date!" << endl;
				isGood = false;
			}
		cin >> date[DAY];
			if (cin.get() != '/')
			{
				cout << " Error : Incorret Date!" << endl;
				isGood = false;
			}
		cin >> date[YEAR];
			if (cin.get() != '/')
			{
				cout << " Error : Incorret Date!" << endl;
				isGood = false;
			}

			if (date[YEAR] < 1970)
			{
				cout << "Error : Year must be greater than 1970. " << endl;
				isGood = false;
			}

			if ((date[YEAR] % 4 == 0 && date[YEAR] % 100 == 0) || date[YEAR] % 400 != 0) 
				months[1] = 29; else months[1] = 28;


	}


};

//deletes next alarm
void poller::deleteNextAlarm()
{
	Node *nodeptr = head;
	head = head->next;
	delete nodeptr;
};

//sorts alarm list
void poller::sortAlarms()
{
	//pick a sorting method
	//use it
};

//views next alarm
void poller::viewNextAlarm()
{
	//output head pointer event data
};

//runs user input
void poller::userLoop()
{
};

//runs the alarm
void poller::alarmLoop()
{
};

//runs the threads
void poller::pollingLoop()
{
};
#endif
