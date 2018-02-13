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
#include"Event.h"

struct Node
{
	Event *event;
	Node *next;
};

class Schedule
{
public:
	Schedule(): list(nullptr) {}
	Schedule(std::string);
	void usering(int);
	void alarming();
	void threadTheNeedle();
	void pollingLoop();
protected:
	void enterEvent(std::string);
	void sortList();
	void notify();
	void saveFile();
private:
	Node *list;
	std::string eventname;
};

Schedule::Schedule(std::string filename)
{
	std::ifstream fin;
	std::fin.open(filename);
	if (!std::fin.good()) list = nullptr;
	else
	{
		Node *nodeptr = list;
		long long int time;
		std::string str;
		while(!fin.eof())
		{
			nodeptr = new Node;
			std::fin >> time;
			std::fin.ignore();
			std::getline(fin, str);
			std::chrono::seconds s (time);
			nodeptr->event = new Event(s, str);
			nodeptr->next = nullptr;
			nodeptr = nodeptr->next;
		}
		std::fin.close();
		sortList();
	}
}

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

void Schedule::sortList()
{
	Node *nodeptr = list;
	while (nodeptr)
	{
		Node *nextptr = nodeptr->next;
		while (nextptr)
		{
			if (nodeptr->event.getAlarm() > nextptr->event.getAlarm())
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
			cout << "1 = Snooze" << endl;
			cout << "2 = Dismiss" << endl;
			cout << "Other = exit" << endl;
			cin >> a;
			cout << endl;
			if (a == 1)
			{
				alarmGoingOff = false;
				//set alarm time for about 3 minutes in the future
			}
			else if (a == 2)
			{
				alarmGoingOff = false;
				//delete alarm event
			}
			else
			{
				alarmGoingOff = false;	//to stop the other thread
				isRunning = false;	//to exit both threads
			}
		}
		else
		{
			cout << "1 = View Next Alarm" << endl;
			cout << "2 = Add New Alarm" << endl;
			cout << "3 = Delete Next Alarm" << endl;
			cout << "4 = Exit Program" << endl;
			cin >> a;
			cout << endl;
			switch (a)
			{
			case 1:
			{
				//view next alarm
				break;
			}
			case 2:
			{
				//add new alarm
				break;
			}
			case 3:
			{
				//delete next alarm
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
};

void Schedule::alarming()
{
	using chrono::system_clock;
	time_t tt = system_clock::to_time_t(system_clock::now());
	struct tm * ptm = localtime(&tt);
	while (isRunning)
	{
		tt = system_clock::to_time_t(system_clock::now());
		ptm = localtime(&tt);
		if (placeholder)	//if current time==alarm time
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
};

void Schedule::threadTheNeedle()
{
	int b;
	thread first(this->alarming);
	thread second(this->usering, b);
	first.join();
	second.join();
};

#endif
