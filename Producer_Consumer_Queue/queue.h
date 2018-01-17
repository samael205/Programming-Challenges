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
	void save(){
		std::stringstream sstream;
		sstream<<number_of_customers<<" customers\tsum: "<<time_format(allwaittime)<<"\tavg: "<<average<<"s\tslow: "<<longest_time<<"\tquick: "<<shortest_time;
		results.push_back(sstream.str());
	}
};

vs Queue::results = vs(0);

Queue::Queue() : maximum(n), longest_time(), shortest_time() {
	current = 0;
	head = rear = nullptr;
	average = 0.0;
	allwaittime = 0;
}

Queue::~Queue(){
	Node * remove;
	while(head != nullptr){
		remove = head;
		head = head->next;
		delete head;
	}
}

void Queue::enqueue(const Customer & newcustomer){
	if(isfull())
		return;
	Node * addcustomer = new Node;
	addcustomer->person = newcustomer;
	addcustomer->next = nullptr;
	current++;
	number_of_customers = current;
	if(head == nullptr)
		head = addcustomer;
	else
		rear->next = addcustomer;
	rear = addcustomer;
}

void Queue::dequeue(Customer & leaver){
	if(head == nullptr)
		return;
	leaver = head->person;
	Node * remove = head;
	head = head->next;
	delete remove;
	current--;
	if(isempty())
		head = nullptr;
}	

void simulate(Queue & queue){
	Customer * temp;
	queue.shortest_time = queue.head->person;
	queue.longest_time = queue.head->person;
	while(!queue.isempty()){
		double time = 0;
		if(time <= 0){
			temp = new Customer;
			queue.dequeue(*temp);
			if(*temp < queue.longest_time)
				queue.longest_time = *temp;
			if(*temp > queue.shortest_time)
				queue.shortest_time = *temp;
			time = temp->first();
			queue.allwaittime+= time;
			delete temp;
		}
		if(time > 0)
			time--;
	}
	queue.average = queue.allwaittime/queue.number_of_customers;
	guard.lock();
	queue.save();
	guard.unlock();
}

void summary(const Queue & queue){
	auto wait = std::chrono::milliseconds(3);
	for(auto it = queue.results.begin(); it != queue.results.end(); it++){
		cout<<*it<<"\n";
		std::this_thread::sleep_for(wait);
	}
}

string time_format(ll time){
	int min = time/60;
		int seconds = time%60;
		if(seconds == 0 && min != 0)
			return std::to_string(min) + "m ";
		else if(seconds != 0 && min == 0)
			return std::to_string(seconds) + "s ";
		else 
			return std::to_string(min) + "m " + std::to_string(seconds) + "s ";
}
