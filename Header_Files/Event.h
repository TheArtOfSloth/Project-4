/**
* File holding Event class, methods, and necessary values.
* @author	SMITH_EVAN
*/
#ifndef EVENT_H
#define EVENT_H 1
#define _CRT_SECURE_NO_WARNINGS 1

// LIBRARIES
#include "Event.h"

#include<chrono>
#include<iostream>
#include<stdexcept>
#include<string>

// GLOBAL CONSTANTS / VARIABLES
const int MAX_NUM_HOURS = 24;							// Integer to hold maximum number of hours
const int MAX_NUM_MINUTES = 60;							// Integer to hold maximum number of minutes
const int MAX_NUM_MONTHS = 12;							// Integer to hold maximum number of months
const int MIN_YEAR = 1970;							// Integer to hold the minimum year allowed
const int NUM_DATE_PARAMS = 5;							// Integer to hold constant for date array
const int PST_ADJUST = 8 * 60 * 60;						// Integer to hold time adjustment for epoch
int months[MAX_NUM_MONTHS] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };	// Integer array representing the number of days in each month
enum date { YEAR, MONTH, DAY, HOUR, MINUTE };					// Enumeration representing each parameter of a date array

// CLASSES / STRUCTS

/**
* This class holds and handles alarm entries as a time and a label.
*/
class Event
{
public:
	Event();																// Default constructor
	Event(std::chrono::seconds, std::string);			// Constructor taking seconds object and label
	Event(int time[NUM_DATE_PARAMS], std::string);			// Constructor taking date array and label
	Event(long long int, std::string);				// Constructor taking integer and label
	Event(Event&);							// Copy constructor
	void set(long long int, std::string);
	std::chrono::seconds getAlarm() const;				// Getter to return alarm as seconds object
	long long int getAlarmAsInt() const;				// Getter to return alarm as integer
	int * getAlarmAsDate() const;					// Getter to return alarm as integer array representing a date
	std::string getLabel() const;					// Getter to return label
	Event operator=(const Event &e);				// Overloaded '=' operator
	friend std::ostream &operator<<(std::ostream &os, Event const &e); // Overloaded '<<' operator
	~Event();																// Class destructor
private:
	void setAlarmFromDate(int time[NUM_DATE_PARAMS]);
	std::chrono::seconds alarm;
	std::string label;
};

// FUNCTIONS <Event>

/**
* Default constructor to initialize new Event with a blank label and an alarm
* that starts on January 1, 1970.
*/
Event::Event() : label("")
{
	std::chrono::seconds _alarm(PST_ADJUST);
	alarm = _alarm;
}

/**
* Constructor to initialize new Event.
* @param	chrono::seconds object representing time since epoch
* @param	(optional) string representing a label for the event
*/
Event::Event(std::chrono::seconds alarm, std::string label = "") : alarm(alarm), label(label) {}

/**
* Constructor to initialize a new Event.
* @param	integer array of size 5, representing a date and time
* @param	(optional) string representing a label for the Event
*/
Event::Event(int _alarm[NUM_DATE_PARAMS], std::string label = "") : label(label)
{
	setAlarmFromDate(_alarm);
}

/**
* Constructor to initialize new Event.
* @param	long long integer representing seconds since epoch (time adjust assumed)
* @param	(optional) string representing a label for the Event
*/
Event::Event(long long int _alarm, std::string label = "") : label(label)
{
	std::chrono::seconds temp(_alarm);
	alarm = temp;
}

/**
* Copy constructor to initialize new Event.
* @param	Event object to be copied
*/
Event::Event(Event& event) : alarm(event.getAlarmAsInt()), label(event.getLabel()) {}

/**
* Getter for the alarm.
* @returns	chrono::seconds object representing time since epoch
*/

/**
 * Setter function.
 * @param	integer representing seconds since epoch
 * @param	(optional) string representing a label for the event
 */
void Event::set(long long int _alarm, std::string _label)
{
	label = _label;
	std::chrono::seconds temp(_alarm);
	alarm = temp;
}

std::chrono::seconds Event::getAlarm() const
{
	return alarm;
}

/**
* Getter to return the alarm as a primitive.
* @returns long long int representing seconds since epoch
*/
long long int Event::getAlarmAsInt() const
{
	return alarm.count();
}

/**
* Function that computes the date from the number of seconds since the epoch.
* @returns	integer array of size 5 representing a date and time
*/
int * Event::getAlarmAsDate() const
{
	int *time = new int[NUM_DATE_PARAMS];
	time[YEAR] = 1970;
	time[MONTH] = 1;
	time[DAY] = 1;
	time[HOUR] = 0;
	time[MINUTE] = 0;
	long long int seconds = alarm.count() - PST_ADJUST;
	while (true)
	{
		if ((time[YEAR] % 4 == 0 && time[YEAR] % 100 == 0) || time[YEAR] % 400 != 0)
		{
			if (seconds <= 366 * 24 * 60 * 60) break;
			else
			{
				seconds -= 366 * 24 * 60 * 60;
				time[YEAR]++;
			}
		}
		else
		{
			if (seconds <= 365 * 24 * 60 * 60) break;
			else
			{
				seconds -= 365 * 24 * 60 * 60;
				time[YEAR]++;
			}
		}
	}
	if ((time[YEAR] % 4 == 0 && time[YEAR] % 100 == 0) || time[YEAR] % 400 != 0) months[1] = 29; else months[1] = 28;
	while (true)
	{
		if (seconds >= months[time[MONTH] - 1] * 24 * 60 * 60)
		{
			seconds -= months[time[MONTH] - 1] * 24 * 60 * 60;
			time[MONTH]++;
		}
		else break;
	}
	while (seconds >= 24 * 60 * 60)
	{
		seconds -= 24 * 60 * 60;
		time[DAY]++;
	}
	while (seconds >= 60 * 60)
	{
		seconds -= 60 * 60;
		time[HOUR]++;
	}
	while (seconds >= 60)
	{
		seconds -= 60;
		time[MINUTE]++;
	}
	return time;
}

/**
* Getter for the label.
* @returns label as a string
*/
std::string Event::getLabel() const
{
	return label;
}

/**
* Overloaded '=' operator to set an Event's parameters to equal another's member values.
*/
Event Event::operator=(const Event &rhs)
{
	//Event *lhs = new Event(&rhs);
	//Event *lhs = new Event(rhs.getAlarm(), rhs.getLabel());
	//return new Event(rhs.getAlarm(), rhs.getLabel());
	this->alarm = rhs.getAlarm();
	this->label = rhs.getLabel();
	return *this;
}

/**
* Overloaded '<<' operator to output an event in a single line.
*/
std::ostream &operator<<(std::ostream &os, Event const &e)
{
	int *time = e.getAlarmAsDate();
	std::string month = (time[MONTH] < 10) ? "0" : "";
	std::string day = (time[DAY] < 10) ? "0" : "";
	std::string hour = (time[HOUR] < 10) ? "0" : "";
	std::string minute = (time[MINUTE] < 10) ? "0" : "";
	return os << time[YEAR] << "/" << month << time[MONTH] << "/" << day << time[DAY] << "  "
		<< hour << time[HOUR] << ":" << minute << time[MINUTE] << " | " << e.label;
}

/**
* Private function to convert a date integer into the amount of seconds since the epoch and
* store the value in the alarm member function.
*/
void Event::setAlarmFromDate(int time[NUM_DATE_PARAMS])
{
	if (time[YEAR] < 1970) throw std::invalid_argument("ERROR: Date exists before epoch."); else;
	if (time[MONTH] > MAX_NUM_MONTHS || time[MONTH] < 1) throw std::invalid_argument("ERROR: Month doesn't exist."); else;
	if (time[HOUR] >= MAX_NUM_HOURS || time[HOUR] < 0) throw std::invalid_argument("ERROR: Hour must be less than 24."); else;
	if (time[MINUTE] >= MAX_NUM_MINUTES || time[MINUTE] < 0) throw std::invalid_argument("ERROR: Minute must be less than 60."); else;
	long long int seconds = PST_ADJUST;
	for (int year = 1970; year < time[YEAR]; year++)
	{
		seconds += 365 * 24 * 60 * 60;
		if ((time[YEAR] % 4 == 0 && time[YEAR] % 100 == 0) || time[YEAR] % 400 != 0) seconds += 24 * 60 * 60; else;
	}
	if ((time[YEAR] % 4 == 0 && time[YEAR] % 100 == 0) || time[YEAR] % 400 != 0) months[1] = 29; else months[1] = 28;
	if (time[DAY] > months[time[MONTH] - 1] || time[DAY] < 1) throw std::invalid_argument("ERROR: Date does not exist in month."); else;
	for (int month = 1; month < time[MONTH]; month++) seconds += months[month - 1] * 24 * 60 * 60;
	for (int day = 1; day < time[DAY]; day++) seconds += 24 * 60 * 60;
	for (int hour = 0; hour < time[HOUR]; hour++) seconds += 60 * 60;
	for (int minute = 0; minute < time[MINUTE]; minute++) seconds += 60;
	std::chrono::seconds t(seconds);
	alarm = t;
}

/**
* Class destructor.
*/
Event::~Event() {}

#endif
