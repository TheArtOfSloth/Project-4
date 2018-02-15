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
#include<stdexcept>
#include"Event.h"
#include"Schedule.h"
using namespace std;

int main()
{
	string c = "timestamps.txt";
	Schedule b(c);
	b.threadTheNeedle();
	return 0;
}
