#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>
#include <future>
#include <mutex>

using std::cout;
using std::string;

typedef long long ll;
typedef std::vector<string> vs;

#define REP(i, n) for(int i=0; i<n; i++)
#define VAR(i, x) __typeof(x) i = x
#define FOREACH(i, x) for(VAR(i, x.begin()); i != x.end(); i++)

string time_format(ll);

class Customer{
private:
	int wait_time;
	string name;
public:
	Customer();
	Customer(const string & addname, const int addtime);
	
	const int & first() const { return wait_time; }
	const string & second() const { return name; }

	Customer & operator=(const Customer & absorb);
		
	bool operator>(const Customer & larger);
	bool operator<(const Customer & smaller);
			
	friend std::ostream & operator<<(std::ostream & os, const Customer & customer);			
};

class Queue{
private:
	static vs results;
	enum { n = 30 };
	struct Node{
		Customer person;
		struct Node * next;
	};
	struct Node * head;
	struct Node * rear;

	int current, number_of_customers;
	const int maximum;

	ll allwaittime;
	double average;

	Customer longest_time;
	Customer shortest_time;

public:
	Queue();
	~Queue();

	bool isfull() { return current == maximum; }
	bool isempty() { return current == 0; }

	void enqueue(const Customer&);
	void dequeue(Customer &);

	void clearResults();

	friend void simulate(Queue & queue);
	friend void summary(const Queue & queue);
	
protected:
	void save();	
};

