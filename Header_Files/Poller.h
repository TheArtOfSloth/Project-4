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
	//dereference nodes, from last to first
};

//adds alarm
void poller::addAlarm()
{
	//get user input
	//process user input
	//feed user input into event class functions
	//create new node for linking the new event
	//step through list until a node is reached which is later than the new event
	//link it in
};

//deletes next alarm
void poller::deleteNextAlarm()
{
	//create temporary pointer to head
	//set head to point to the next node
	//delete what was formerly head
	//dereference temporary pointer
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
