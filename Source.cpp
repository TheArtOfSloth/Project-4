#include "Schedule.h"
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
#include<stdexcept>
using namespace std;

int main()
{
	string a;
	a = "timestamps.txt";
	Schedule b(a);
	b.pollingLoop();
	return 0;
}
