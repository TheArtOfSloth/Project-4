/**
* File holding Schedule class, methods, and necessary values.
* @author	BERNSTEIN_JOHN
* @author	KLAPSTEIN_DANIEL
* @author	SMITH_EVAN
* @author   DHILLON_NAVI
* @author	TARIN
*/
#ifndef DOOMCLOCK_H
#define DOOMCLOCK_H 1
#define _CRT_SECURE_NO_WARNINGS 1

// LIBRARIES
#include <cstdlib>
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
#include <iomanip>
#include <sstream>
#include <memory>

using namespace std;

/**
* This struct represents a single Event Node in the Schedule class.
*/
struct Events
{
	string alarmName;
	time_t alarmTime;
	struct std::tm when;
	Events *next;
};
/*
used to store data and run threads.
*/
class doomClock
{
public:
	doomClock(string);
	~doomClock();
	void pollingLoop();
private:
	void addAlarm();
	void addAlarm2(int, int, int, int, int, string);
	void addAlarm3(Events*, int, int, int, int, int, string);
	void deleteNextAlarm();
	void saveFile();
	void save(string&, string&, string&, string&, string&, string&);
	void viewNextAlarm();
	void userLoop();
	void alarmLoop();
	void sortList();
	void processFile(Events*, Events*, string);
	Events *mergeSort(Events*);
	Events *merge(Events*, Events*);
	thread t1;
	thread t2;
	bool isRunning;
	bool alarmGoingOff;
	bool placeholder;
	int command1;
	string command2;
	string filename;
	Events *head;
};

//class constructor
doomClock::doomClock(string a)
{
	isRunning = true;
	alarmGoingOff = false;
	placeholder = false;
	filename = a;
	//fstream dataFile;
	//dataFile.open(filename);
	Events *hold = new Events;
	//   CHECK THOROUGHLY!!!!!!!!!!!!!!!!!!!!!
	//processFile(head, hold, a);
	//sortList();---------------------------------------------------------------------------------------------------
};

//class destructor
doomClock::~doomClock()
{
	while (head)
	{
		Events *nodeptr = new Events;
		nodeptr = head;
		head = head->next;
		delete nodeptr;
	}
};

void doomClock::processFile(Events *a, Events *b, string d)
{
	string filename = d;
	fstream dataFile;
	dataFile.open(filename);
	while (dataFile.good())
	{
		Events *temp = new Events;	//LIKELY ISSUES HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		dataFile >> temp->when.tm_year >> temp->when.tm_mon >> temp->when.tm_mday >> temp->when.tm_hour >> temp->when.tm_min >> temp->alarmName;	//LIKELY ISSUES HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		temp->when.tm_sec = 0;
		a = temp;
		b->next = a;
		a = b;
	};
	dataFile.close();
	a = a->next;
};

//adds alarm
void doomClock::addAlarm()
{
	Events *temp = new Events;
	cout << "Please enter the name of the alarm: ";
	cin >> temp->alarmName;
	cout << "Please enter the month of the alarm: ";
	cin >> temp->when.tm_mon;
	temp->when.tm_mon -= 1;
	cout << "Please enter the day of the alarm: ";
	cin >> temp->when.tm_mday;
	cout << "Please enter the year of the alarm: ";
	cin >> temp->when.tm_year;
	temp->when.tm_year -= 1900;
	cout << "Please enter the hour of the alarm, from 0-23: ";
	cin >> temp->when.tm_hour;
	cout << "Please enter the minute of the alarm, from 0-59: ";
	cin >> temp->when.tm_min;
	temp->when.tm_sec = 0;
	temp->next = head;
	head = temp;
	//sortList();//-----------------------------------------------------------------------------------------
};

void doomClock::addAlarm2(int a, int b, int c, int d, int e, string f)
{
	Events *temp = new Events;
	temp->alarmName = f;
	temp->when.tm_mon = b;
	temp->when.tm_mday = c;
	temp->when.tm_year = a;
	temp->when.tm_hour = d;
	temp->when.tm_min = e;
	temp->next = head;
	head = temp;
};

void doomClock::addAlarm3(Events *z, int a, int b, int c, int d, int e, string f)
{
	z->alarmName = f;
	z->when.tm_mon = b;
	z->when.tm_mday = c;
	z->when.tm_year = a;
	z->when.tm_hour = d;
	z->when.tm_min = e;
	z->next = head;
	head = z;
};

//deletes next alarm
void doomClock::deleteNextAlarm()
{
	Events *nodeptr = new Events;
	nodeptr = head;

	head = head->next;
	delete nodeptr;
};

//saves list to file
//John - Do this.
void doomClock::saveFile()
{
	Events *viewer = new Events;
	viewer = head;
	string sa;
	string sb;
	string sc;
	string sd;
	string se;
	while (viewer != NULL)
	{
		sa = (viewer->when.tm_year);
		sb = (viewer->when.tm_mon);
		sc = (viewer->when.tm_mday);
		sd = (viewer->when.tm_hour);
		se = (viewer->when.tm_min);
		save(sa, sb, sc, sd, se, viewer->alarmName);
		viewer = viewer->next;
	}
}

void doomClock::save(std::string& yr,
	std::string& mon,
	std::string& day,
	std::string& hr,
	std::string& min,
	std::string& msg
) {
	std::fstream file;
	file.open(filename, std::ios::out | std::ios::app | std::ios::binary);
	if (!file.is_open()) {
		std::cout << "Error opening file.\n";
	}
	else {
		file << yr << " " << mon << " " << day << " " << hr << " " << min << msg << "\n";
	}
	file.close();
}

//views next alarm
//Tarin did this.
void doomClock::viewNextAlarm()
{
	if (head != NULL) {
		cout << "The next scheduled event is " << head->alarmName << endl;
		cout << "It is scheduled for ";
		cout << head->when.tm_hour << ":" << head->when.tm_min << " on " << (head->when.tm_mon) + 1 << "/" << head->when.tm_mday << "/" << (head->when.tm_year) + 1900 << endl;
	}

	else {
		cout << "There are no scheduled events." << endl;
	}
};

//runs user input
void doomClock::userLoop()
{
	while (isRunning)
	{
		if (alarmGoingOff)
		{
			cout << "ALARM TIME!!! Type any entry to exit." << endl;
			cin >> command1;
			alarmGoingOff = false;
			deleteNextAlarm();
		}
		else
		{
			
			cout <<  "1 - View Next Alarm" << endl;
			cout <<  "2 - Add New Alarm" << endl;
			cout <<  "3 - Delete Next Alarm" << endl;
			cout <<  "4 - Exit" << endl;
			cout <<  "5 - Test Dummy Alarm" << endl;
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
void doomClock::alarmLoop()
{
	Events* viewer = new Events;
	Events* viewed = new Events;
	viewer = head;
	viewed = viewer;
	using chrono::system_clock;
	time_t tt = system_clock::to_time_t(system_clock::now());
	struct tm * ptm = localtime(&tt);
	while (isRunning)
	{
		tt = system_clock::to_time_t(system_clock::now());
		ptm = localtime(&tt);    //Syntax error
		if (placeholder /*|| (head && (mktime((&head->when)) <= tt))*/)
		{
			placeholder = false;
			alarmGoingOff = true;
		};
		if (head && (mktime((&head->when)) <= tt))
			alarmGoingOff = true;
		while (viewer)
		{
			if (viewer && (mktime((&viewer->when)) <= tt))
			{
				alarmGoingOff = true;
				viewed->next = viewer->next;
				viewer->next = head;
				head = viewer;
				break;
			}

		}
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
void doomClock::pollingLoop()
{
	t1 = thread(&doomClock::userLoop, this);
	t2 = thread(&doomClock::alarmLoop, this);
	t1.join();
	t2.join();
};

/*
EVAN'S SORTING METHODS
*/

void doomClock::sortList()
{
	head = mergeSort(head);
}

Events * doomClock::mergeSort(Events *list)
{
	if (!list || !list->next) return list; else; // Base case (list size is 1)
												 // Divide list into two sublists
	Events *nodeptr = list;
	Events *sublist_1 = nodeptr;
	Events *sublist_2 = (nodeptr = nodeptr->next);
	while (nodeptr)
	{
		if (nodeptr) sublist_1 = (nodeptr = nodeptr->next); else;
		if (nodeptr) sublist_2 = (nodeptr = nodeptr->next); else;
	}
	// Return pointer to head of sorted linked list
	list = merge(mergeSort(sublist_1), mergeSort(sublist_2));
	return list;
}

Events * doomClock::merge(Events *sublist_1, Events *sublist_2)
{
	Events *list = nullptr;
	if (mktime(&sublist_1->when) < mktime(&sublist_2->when)) list = sublist_1;
	else list = sublist_2;
	Events *nodeptr = list;
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
		else if (mktime(&sublist_1->when) < mktime(&sublist_2->when))
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
	} while (sublist_1 || sublist_2);
	return list;
}

/*
EVAN'S SORTING METHODS
*/
#endif
