#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<iomanip>
#include<ctime>
#include<chrono>
#include<time.h>
#include<thread>
using namespace std;

void usering(int a)
{
	cout << endl << "1 = again, else quit: " << endl;
	cin >> a;
	switch (a)
	{
	case 1:
		usering(a);
		break;
	default:
		break;
	}
};

void alarming()
{
	using chrono::system_clock;
	time_t tt = system_clock::to_time_t(system_clock::now());

	struct tm * ptm = localtime(&tt);
	cout << "Current time: " << put_time(ptm, "%X") << '\n';

	cout << "Waiting for the next minute to begin...\n";
	++ptm->tm_min; ptm->tm_sec = 0;
	this_thread::sleep_until(system_clock::from_time_t(mktime(ptm)));

	for (int i = 0; i < 5; i++)
	{
		cout << '\a';
	}
	cout << "It's time!" << endl;
};

int main()
{
	int b;
	thread first (alarming);
	thread second (usering, b);
	cout << "ready as ever..." << endl;
	first.join();
	second.join();
	cout << endl << "done";
	return 0;
}
