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

std::mutex guard;

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
	Customer(){
		name = '\0';
		wait_time = 0;
	}
	Customer(const string & addname, const int addtime){
		name = addname;
		wait_time = addtime;
	}

	const int & first() const { return wait_time; }
	const string & second() const { return name; }

	Customer & operator=(const Customer & absorb){
		this->wait_time = absorb.first();
		this->name = absorb.second();
	}
	bool operator>(const Customer & larger){
		if(larger.first() > this->wait_time)
			return true;
		return false;
	}
	bool operator<(const Customer & smaller){
		if(smaller.first() < this->wait_time)
			return true;
		return false;
	}
	friend std::ostream & operator<<(std::ostream & os, const Customer & customer){
		os<<customer.name<<" "<<time_format(customer.wait_time);
		return os;
	}
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

	friend void simulate(Queue & queue);
	friend void summary(const Queue & queue);
	
protected:
	void save();		
};
