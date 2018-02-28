/**
* File holding Schedule class, methods, and necessary values.
* @author	BERNSTEIN_JOHN
* @author	KLAPSTEIN_DANIEL
* @author	SMITH_EVAN
* @author       DHILLON_NAVI
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
	int y;
	int b;
	int c;
	int d;
	int e;
	string f;
	isRunning = true;
	alarmGoingOff = false;
	placeholder = false;
	filename = a;
	fstream dataFile;
	dataFile.open(filename);
	Events *hold = new Events;
	//   CHECK THOROUGHLY!!!!!!!!!!!!!!!!!!!!!
	while (dataFile.good())
	{
		dataFile >> y >> b >> c >> d >> e >> f;	//LIKELY ISSUES HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		//Events *temp = new Events;
		//dataFile >> temp->when.tm_year >> temp->when.tm_mon >> temp->when.tm_mday >> temp->when.tm_hour >> temp->when.tm_min >> temp->alarmName;	//LIKELY ISSUES HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (!head)
		{
			addAlarm2(y, b, c, d, e, f);
			//dataFile >> head->when.tm_year >> head->when.tm_mon >> head->when.tm_mday >> head->when.tm_hour >> head->when.tm_min >> head->alarmName;	//LIKELY ISSUES HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			/*
			head->when.tm_year = temp->when.tm_year;
			head->when.tm_mon = temp->when.tm_mon;
			head->when.tm_mday = temp->when.tm_mday;
			head->when.tm_hour = temp->when.tm_hour;
			head->when.tm_min = temp->when.tm_min;
			head->alarmName = temp->alarmName;
			*/
			head->next = hold;
			
		}
		else
		{
			addAlarm3(hold, y, b, c, d, e, f);
			//hold = temp;
			//dataFile >> hold->when.tm_year >> hold->when.tm_mon >> hold->when.tm_mday >> hold->when.tm_hour >> hold->when.tm_min >> hold->alarmName;	//LIKELY ISSUES HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			/*
			hold->when.tm_year = temp->when.tm_year;
			hold->when.tm_mon = temp->when.tm_mon;
			hold->when.tm_mday = temp->when.tm_mday;
			hold->when.tm_hour = temp->when.tm_hour;
			hold->when.tm_min = temp->when.tm_min;
			hold->alarmName = temp->alarmName;
			*/
			hold = hold->next;
			hold = nullptr;
		}
	}
	//   CHECK THOROUGHLY!!!!!!!!!!!!!!!!!!!!!
	delete hold;
	dataFile.close();
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

//adds alarm
void doomClock::addAlarm()
{
	Events *temp = new Events;
	cout << "Please enter the name of the Alarm: ";
	cin >> temp->alarmName;
	cout << "please enter the Month of the alarm: ";
	cin >> temp->when.tm_mon;
	temp->when.tm_mon -= 1;
	cout << "please enter the day of the alarm: ";
	cin >> temp->when.tm_mday;
	cout << "please enter the year of the alarm: ";
	cin >> temp->when.tm_year;
	temp->when.tm_year -= 1900;
	cout << "please enter the hour of the alarm, from 0-23: ";
	cin >> temp->when.tm_hour;
	cout << "please enter the minute of the alarm, from 0-59: ";
	cin >> temp->when.tm_min;
	temp->next = head;
	head = temp;
	//sortList();-----------------------------------------------------------------------------------------
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
		std::cout << "Error opening file\n";
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
		cout << head->when.tm_hour << ":" << head->when.tm_min << " on " << (head->when.tm_mon)+1 << "/" << head->when.tm_mday << "/" << (head->when.tm_year)+1900 << endl;
	}

	else {
		cout << "There are no scheduled events" << endl;
	}
};

//runs user input
void doomClock::userLoop()
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
void doomClock::alarmLoop()
{
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
