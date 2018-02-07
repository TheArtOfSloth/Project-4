#ifndef EVENT_H
#define EVENT_H 1
#define _CRT_SECURE_NO_WARNINGS 1

#include<chrono>
#include<ctime>
#include<iostream>

class Event
{
	public:
		Event();
		Event(std::chrono::seconds alrm, std::string lbl) : alarm(alrm), label(lbl) {}
		std::chrono::seconds getAlarm() const {return alarm;}
		std::string getLabel() const {return label;}
		Event operator=(const Event &e);
		friend std::ostream &operator<<(std::ostream &os, Event const &e);
		~Event() {}
	private:
		std::chrono::seconds alarm;
		std::string label;
};

Event::Event() : label("")
{
	std::chrono::seconds a (8 * 60 * 60);
	alarm = a;
}

Event operator=(const Event &lhs, const Event &rhs)
{
	lhs.alarm = rhs.alarm;
	lhs.label = rhs.label;
	return;
}

std::ostream &operator<<(std::ostream &os, Event const &e)
{
	std::time_t t = e.alarm.count();
	return os << "Alert: " << std::ctime(&t) << "" << e.getLabel() << std::endl;
}

#endif
