#include "queue.h"

#include <fstream>

const int random(int n) { return rand()%n+10; }

string setname();

typedef std::vector<Queue> vq;
vq queues_to_simulate(int,int);
vq queues_to_simulate(int);
vq queues_to_simulate();

void queue_threads(vq & queue){
	std::vector<std::future<void>> threads;
	FOREACH(it, queue)
		threads.push_back(std::async(std::launch::async, simulate, std::ref(*it)));
	FOREACH(it, threads)
		it->get();
}

int main(void){
	std::srand(std::time(0));
	vq test = queues_to_simulate(10);
	queue_threads(test);
	cout<< "\033[2J\033[1;1H";
	summary(test.back());
}

string setname(){
	std::fstream file;
	file.open("names.txt");
	if(!file.is_open())
		exit(EXIT_FAILURE);
	file.seekg(std::ios::beg);
	REP(i, rand()%99+1)
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