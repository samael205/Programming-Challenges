#include "queue.h"

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
