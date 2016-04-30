#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include "class.h"

using namespace std;

void function_1()
{
	std::cout << "Beauty is only skin-deep" << std::endl;
	while(true)
	{
		//int i = 0;
	}
}

class LogFile
{
	std::mutex _mu;
	ofstream _f;

public:
	LogFile()
	{
		_f.open("log.txt");
	}

	~LogFile();
	void shared_print(std::string msg, int id)
	{
		std::lock_guard<mutex> locker(_mu);
		_f << "From " << id << ": " << msg << endl;
	}

private:

};

LogFile::LogFile()
{

}

LogFile::~LogFile()
{}

int main()
{
	cout << std::this_thread::get_id() << endl;

	string str = "idk what to do";
	std::thread t1((Functor()), std::move(str));
	cout << t1.get_id() << endl;

	cout << std::thread::hardware_concurrency() << endl; // max threads on current system

	//t1.join(); // main thread waits for t1 to finish
	//t1.detach(); //t1 will be free, -- daemon process

	// RAII
	// Wrapper w(t1) with destructor that calls join
	try
	{
		for(int i = 0; i < 3; i++)
		{
			cout << "from main: " << str.size() << endl;
		}
	}
	catch(...)
	{
		t1.join();
		throw;
	}

	if(t1.joinable())
	{
		t1.join(); // will crash without test
	}
	return 0;
}