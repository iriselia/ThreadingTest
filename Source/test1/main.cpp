#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include <future>
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

	~LogFile()
	{

	}
	void shared_print(std::string msg, int id)
	{
		std::lock_guard<mutex> locker(_mu);
		_f << "From " << id << ": " << msg << endl;
	}

private:

};

class Factorial
{
public:

	int operator()(int num)
	{
		for (int i = 1; i <= num; i++)
		{
			num = num * i;
		}
		return num;
	}

	int memberfunc(int num)
	{
		return this->operator()(num);
	}
private:

};

int factorial(int num)
{
	for (int i = 1; i <= num; i++)
	{
		num = num * i;
	}
	return num;
}

int main()
{
	Factorial f;
	/* thread invocation template */
	std::thread t1(f, 6); //copy of f invoked in new thread
	std::thread t2(std::ref(f), 6); // ref of f in main thread
	std::thread t3(std::move(f), 6); // turn f into a rvalue ref and move construct it in new thread
	std::thread t4(Factorial(), 6); // temp factorial in both main and new thread (? unsure)
	std::thread t5([](int x) { return x*x; }, 6); // call lambda
	std::thread t6(factorial, 6); // lower case factorial, global function
	std::thread t7(&Factorial::memberfunc, f, 6); // memberfunc invoked on a copy of f
	std::thread t8(&Factorial::memberfunc, &f, 6); // memberfunc invoked on ref of f


	/* mutex */
	std::mutex mu;
	{
		std::lock_guard<mutex> locker(mu);
	}
	std::unique_lock<mutex> ulocker(mu);

	/* condition variable */
	std::condition_variable cond;

	/* future and promise */
	std::promise<int> p;
	std::future<int> fu = p.get_future();

	/* async() */
	std::future<int> fu2 = async(factorial, 6);

	/* packaged task */
	std::packaged_task<int(int)> t(factorial);
	std::future<int> fu3 = t.get_future();
	t(6);


	cout << std::this_thread::get_id() << endl;

	string str = "idk what to do";
	//std::thread t1((Functor()), std::move(str));
	//cout << t1.get_id() << endl;

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
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();

	return 0;
}