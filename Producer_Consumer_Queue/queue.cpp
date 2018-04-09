#include "queue.h"

Customer::Customer(){
	name = '\0';
	wait_time = 0;
}

Customer::Customer(const string & addname, const int addtime){
	name = addname;
	wait_time = addtime;
}

Customer & Customer::operator=(const Customer & absorb){
	this->wait_time = absorb.first();
	this->name = absorb.second();
}

bool Customer::operator<(const Customer & customer){
	if(customer.first() < this->wait_time)
			return true;
	return false;
}

bool Customer::operator>(const Customer & customer){
		if(customer.first() > this->wait_time)
			return true;
	return false;
}

std::ostream & operator<<(std::ostream & os, const Customer & customer){
	os<<customer.name<<" "<<time_format(customer.wait_time);
	return os;
}

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

void Queue::save(){
	std::stringstream sstream;
	sstream<<number_of_customers<<" customers\tsum: "<<time_format(allwaittime)
		<<"\tavg: "<<average<<"s\tslow: "<<longest_time<<"\tquick: "<<shortest_time;
	results.push_back(sstream.str());
}

void Queue::clearResults(){
	results.clear();
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

