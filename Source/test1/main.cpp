#include <iostream>
#include <string>
#include <thread>

using namespace std;

void function_1()
{
	std::cout << "Beauty is only skin-deep" << std::endl;
	while(true)
	{
		//int i = 0;
	}
}

class Functor
{
public:
	void operator()()
	{
		for(int i = 0; i < 100; i++)
		{
			std::cout << "From Functor: " << i << std::endl;
		}
	}

	void operator()(std::string& msg)
	{
		msg = "trust is the mother of deceit";
		for(int i = 0; i < 3; i++)
		{
			std::cout << "From Functor: " << msg << std::endl;
		}
	}
};

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