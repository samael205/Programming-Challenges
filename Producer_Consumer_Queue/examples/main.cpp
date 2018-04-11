#include "queue.h"
#include <fstream>
#include <iterator>
#include <algorithm>

const int random(int n) { return rand()%n+10; }

std::mutex guard;

string setname();

void simulate(Queue&);

typedef std::vector<Queue> vq;

vq queues_to_simulate(int,int);
vq queues_to_simulate(int);
vq queues_to_simulate();

void queue_threads(vq & queue);
int countFileLines(std::fstream & file);

int main(void){
	std::srand(std::time(0));
	vq test = queues_to_simulate(5, 8);
	queue_threads(test);
	summary(test.back());

	cout<<"\n\n";
	test.back().clearResults();
	test = queues_to_simulate(10);
	queue_threads(test);
	summary(test.back());

	cout<<"\n\n";
	test.back().clearResults();
	test = queues_to_simulate();
	queue_threads(test);
	summary(test.back());
}

void queue_threads(vq & queue){
	std::vector<std::future<void>> threads;
	FOREACH(it, queue)
		threads.push_back(std::async(std::launch::async, simulate, std::ref(*it)));
	FOREACH(it, threads)
		it->get();
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


int countFileLines(std::fstream & file){
	file.unsetf(std::ios_base::skipws);
	int lines = std::count(std::istream_iterator<char>(file),
			std::istream_iterator<char>(), '\n');
	return lines;
}


string setname(){
	std::fstream file;
	file.open("names.txt");
	if(!file.is_open())
		exit(EXIT_FAILURE);
	file.seekg(std::ios::beg);
	int n = countFileLines(file);
	REP(i, rand()%n)
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::string name;
	file >> name;
	file.close();
	return name;	
}

vq queues_to_simulate(int n, int n_customers){
	vq queues;
	queues.reserve(n);
	REP(i, n){
		queues.push_back(Queue());
		REP(j, n_customers)
			queues[i].enqueue(Customer(setname(), random(65)));
	}
	return queues;
}

vq queues_to_simulate(int n){
	vq queues;
	queues.reserve(n);
	int n_customers;
	REP(i, n){
		queues.push_back(Queue());
		n_customers = rand()%30+1;
		REP(j, n_customers)
			queues[i].enqueue(Customer(setname(), random(65)));
	}
	return queues;
}

vq queues_to_simulate(){
	vq queues;
	int n = rand()%40+1;
	int n_customers;
	queues.reserve(n);
	REP(i, n){
		queues.push_back(Queue());
		n_customers = rand()%30+1;
		REP(j, n_customers)
			queues[i].enqueue(Customer(setname(), random(65)));
	}
	return queues;
}
