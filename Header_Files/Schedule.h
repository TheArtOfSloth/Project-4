#ifndef SCHEDULE_H
#define SCHEDULE_H 1

#include<chrono>
#include<conio.h>
#include<ctime>
#include<fstream>
#include<iostream>

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

#endif
