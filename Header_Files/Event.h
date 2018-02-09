#ifndef EVENT_H
#define EVENT_H 1
#define _CRT_SECURE_NO_WARNINGS 1

#include<chrono>
#include<ctime>
#include<iostream>
#include<stdexcept>

const int MAX_NUM_HOURS = 24;
const int MAX_NUM_MINUTES = 60;
const int MAX_NUM_MONTHS = 12;
const int MIN_YEAR = 1970;
const int NUM_DATE_PARAMS = 5;
const int PST_ADJUST = 8 * 60 * 60;
int months[MAX_NUM_MONTHS] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
enum date {YEAR, MONTH, DAY, HOUR, MINUTE};

class Event
{
	public:
		Event();																		// Default constructor
		Event(std::chrono::seconds alrm, std::string lbl) : alarm(alrm), label(lbl) {}	// Constructor taking std::chrono::seconds
		Event(int time[NUM_DATE_PARAMS], std::string lbl);								// Constructor to take date array and convert to seconds
		std::chrono::seconds getAlarm() const {return alarm;}							// Getter for alarm
		std::string getLabel() const {return label;}									// Getter for label
		void setAlarm(std::chrono::seconds alrm) {alarm = alrm;}
		void setLabel(std::string lbl) {label = lbl;}
		Event operator=(const Event &e);												// Overload '=' operator
		friend std::ostream &operator<<(std::ostream &os, Event const &e);				// Overload '<<' operator
		~Event() {}
	private:
		int * getDate() const;
		void setAlarmFromDate(int time[NUM_DATE_PARAMS]);
		std::chrono::seconds alarm;
		std::string label;
};

Event::Event() : label("")
{
	std::chrono::seconds a (8 * 60 * 60);
	alarm = a;
}

Event::Event(int time[NUM_DATE_PARAMS], std::string lbl) : label(lbl)
{
	setAlarmFromDate(time);
}

Event Event::operator=(const Event &rhs)
{
	Event lhs;
	lhs.setAlarm(rhs.getAlarm());
	lhs.setLabel(rhs.getLabel());
	return lhs;
}

std::ostream &operator<<(std::ostream &os, Event const &e)
{
	/*
	std::time_t t = e.alarm.count();
	return os << "Alert: " << std::ctime(&t) << "" << e.getLabel() << std::endl;
	*/
	int *time = e.getDate();
	std::string minute = (time[MINUTE] < 10) ? "0" : "";
	return os << "Alert: " << time[YEAR] << "/" << time[MONTH] << "/" << time[DAY] << "  " << time[HOUR] << ":" <<
	minute << time[MINUTE] << std::endl << e.label << std::endl;
}

int * Event::getDate() const
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
	std::chrono::seconds t (seconds);
	alarm = t;
}

#endif
