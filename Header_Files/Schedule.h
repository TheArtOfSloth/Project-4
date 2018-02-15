#ifndef SCHEDULE_H
#define SCHEDULE_H 1
#define _CRT_SECURE_NO_WARNINGS 1

#include<chrono>
#include<conio.h>
#include<ctime>
#include<fstream>
#include<iostream>
#include<iomanip>
#include<time.h>
#include<thread>
#include <string>

#include"Event.h"

using namespace std;

struct Node
{
	Event *event;
	Node *next;
};

class Schedule
{
public:
	Schedule() : list(nullptr) {}
	Schedule(std::string& filename);
	void usering(int);
	void alarming();
	void threadTheNeedle();
protected:
	void addAlarm();
	void deleteNextAlarm();
	void sortList();
	bool isRunning;
	bool alarmGoingOff;
	bool placeholder;
	void saveFile();
	std::string setFileName();
private:
	Node * list;
	std::string eventname;
	std::string filename;
};

std::string Schedule::setFileName() {
	filename = "test.txt";
	return filename;
}

Schedule::Schedule(std::string& filename)
{
	std::ifstream fin;
	fin.open(filename);
	if (!fin.good()) list = nullptr;
	else
	{
		Node *nodeptr = list;
		long long int time;
		std::string str;
		while (!fin.eof())
		{
			nodeptr = new Node;
			fin >> time;
			fin.ignore();
			getline(fin, str);
			std::chrono::seconds s(time);
			nodeptr->event = new Event(s, str);
			nodeptr->next = nullptr;
			nodeptr = nodeptr->next;
		}
		//fin.close();
		sortList();
	}
}

/*
void pollingLoop()
{
Schedule *schedule = new Schedule("test.txt");
while (list->event.getAlarm() < std::chrono::system_clock::now().time_since_epoch() || !list)
{
Node *prevptr = list;
list = list->next;
delete prevptr;
}
saveFile();
while (eventname != "EXIT")
{
std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
std::chrono::duration epoch now.time_since_epoch();
std::chrono::seconds secs = std::chrono::duration_cast<std::chrono::seconds>(epoch);
if (secs.count() >= list->event.getAlarm().count())
{
notify();
system("pause");
}
else;
std::string input = "";
while (getline(std::cin, input))
{
if (input.empty()) break;
else
{
eventname = input;
std::cout << input << std::endl << "Enter name of new event or exit: ";
enterEvent(input);
}
}
}
}
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

void Schedule::usering(int a)
{
	while (isRunning)
	{
		if (alarmGoingOff)
		{
			cout << "Alarm going off!" << endl;
			cout << "1 = Dismiss" << endl;
			cout << "Other = exit" << endl;
			cin >> a;
			cout << endl;
			if (a == 1)
			{
				alarmGoingOff = false;
				Node *nodeptr = list;
				list = list->next;
				delete nodeptr;
				saveFile();
			}
			else if (a == 2) alarmGoingOff = false;
			else
			{
				alarmGoingOff = false;	//to stop the other thread
				isRunning = false;	//to exit both threads
			}
		}
		else
		{
			cout << "1 - View Next Alarm" << endl;
			cout << "2 - Add New Alarm" << endl;
			cout << "3 - Delete Next Alarm" << endl;
			cout << "4 - Exit Program" << endl;
			cin >> a;
			cout << endl;
			switch (a)
			{
			case 1:
			{
				if (!list->next) std::cout << "No other scheduled event.\n";
				else std::cout << list->next->event << std::endl;
				break;
			}
			case 2:
			{
				addAlarm();
				break;
			}
			case 3:
			{
				deleteNextAlarm();
				break;
			}
			case 4:
			{
				isRunning = false;
				break;
			}
			case 5:
			{
				placeholder = true;
				break;
			}
			default:
			{
				isRunning = false;
				break;
			}
			}
		}
	}
}

void Schedule::alarming()
{
	using chrono::system_clock;
	time_t tt = system_clock::to_time_t(system_clock::now());
	struct tm * ptm = localtime(&tt);
	while (isRunning)
	{
		tt = system_clock::to_time_t(system_clock::now());
		ptm = localtime(&tt);    //Syntax error
		if (placeholder || tt >= (time_t)list->event->getAlarm().count())	//if current time==alarm time
		{
			alarmGoingOff = true;
			placeholder = false;
		}
		while (alarmGoingOff)
		{
			cout << '\a';
			this_thread::sleep_for(chrono::seconds(1));
		}
		this_thread::sleep_for(chrono::seconds(1));
	}
}

void Schedule::threadTheNeedle()
{

	isRunning = true;
	alarmGoingOff = false;
	placeholder = false;
	//int b;
	//thread first(this->alarming);
	//Syntax Error with threading
	//thread second(this->usering, b);
	//first.join();
	//second.join();
}

void Schedule::deleteNextAlarm()
{
	if (!list->next) std::cout << "No scheduled event.\n";
	else
	{
		Node *nodeptr = list->next;
		list->next = nodeptr->next;
		delete nodeptr;
	}
}

void Schedule::saveFile() {
	std::fstream file;
	file.open(filename); //Should be test.txt.
	Node* nodeptr = list;
	while (file.good()) {
		while (nodeptr != nullptr) {
			nodeptr = nodeptr->next;
		}
	}
	sortList();
}

void Schedule::addAlarm()
{
	int *date = new int[NUM_DATE_PARAMS];
	std::string lbl;
	std::cout << "Enter name of event: ";
	std::getline(cin, lbl);
	std::cout << "Enter year: ";
	std::cin >> date[YEAR];
	std::cin.ignore(1000, 10);
	std::cout << "Enter month (1 - 12): ";
	std::cin >> date[MONTH];
	std::cin.ignore(1000, 10);
	std::cout << "Enter day: ";
	std::cin >> date[DAY];
	std::cin.ignore(1000, 10);
	std::cout << "Enter hour (0 - 23): ";
	std::cin >> date[HOUR];
	std::cin.ignore(1000, 10);
	std::cout << "Enter minute(0 - 59): ";
	std::cin >> date[MINUTE];
	std::cin.ignore(1000, 10);
	Node *add = new Node;
	add->event = new Event(date, lbl);
	add->next = nullptr;
	Node *nodeptr = list;
	while (nodeptr->next) nodeptr = nodeptr->next;
	nodeptr->next = add;
	sortList();
	saveFile();
}

#endif
