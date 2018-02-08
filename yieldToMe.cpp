#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<iomanip>
#include<ctime>
#include<chrono>
#include<time.h>
#include<thread>
using namespace std;

bool alarmGoingOff = false;
bool isRunning = true;
bool placeholder;	//holds place of comparison statements

void usering(int a)
{
	while (isRunning)
	{
		if (alarmGoingOff)
		{
			cout << "Alarm going off! 1=snooze, 2=dismiss, other=exit" << endl;
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
			cout << "1=view next alarm, 2=add new alarm," << endl;
			cout << "3=delete next alarm, 4 = exit program." << endl;
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

void alarming()
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

int main()
{
	int b;
	thread first(alarming);
	thread second(usering, b);
	cout << "ready as ever..." << endl;
	first.join();
	second.join();
	cout << endl << "done";
	return 0;
}
