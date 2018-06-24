#include "queue.h"
#include <iomanip>

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
	return customer.first() < this->wait_time;	
}

bool Customer::operator>(const Customer & customer){
	return customer.first() > this->wait_time;	
}

std::ostream & operator<<(std::ostream & os, const Customer & customer){
	os<<customer.name<<" "<< std::setw(8)<<time_format(customer.wait_time);
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
	sstream<<std::left<<std::setw(3)<<" customers: "<<number_of_customers<<'\t'
	       <<std::setw(3)<<"sum: "<<time_format(allwaittime)<<" "
               <<std::setw(3)<<"average: "<<time_format(average)<<" "
	       <<std::setw(9)<<"slowest: "<<std::setw(9)<<longest_time<<" " 
	       <<std::setw(9)<<"quickest: "<<std::setw(9)<<shortest_time;
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
	int seconds = time%3600;
	int minutes = seconds/60;
	int hours = time/3600;
	seconds %= 60;
	
	std::string hourFormat, minuteFormat, secondFormat;
	hourFormat = hours >= 10 ? std::to_string(hours) : "0" + std::to_string(hours);
	minuteFormat = minutes >= 10 ? std::to_string(minutes) : "0" + std::to_string(minutes);
	secondFormat = seconds >= 10 ? std::to_string(seconds) : "0" + std::to_string(seconds);
	
	return hourFormat + ":" + minuteFormat + ":" + secondFormat;
}

