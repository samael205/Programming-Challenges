#include "queue.h"
#include <fstream>
#include <iterator>
#include <algorithm>
#include <random>

std::mutex guard;

static std::mt19937 gen{std::random_device{}()};
template <class T>
T random(T min, T max){
	return std::uniform_int_distribution<T>{min, max}(gen);
}

string setname();
void simulate(Queue&);

typedef std::vector<Queue> vq;

vq queues_to_simulate(int,int);
vq queues_to_simulate(int);
vq queues_to_simulate();

void queue_threads(vq & queue);
int countFileLines(std::ifstream & file);

int main(void){	
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

int countFileLines(std::ifstream & file){
	int lines = std::count(std::istreambuf_iterator<char>(file),
			       std::istreambuf_iterator<char>(), '\n');
	return lines;
}

string setname(){
	std::ifstream file;
	file.open("names.txt", std::ios::out);
	if(!file.is_open())
		return "None";	
	int n = countFileLines(file);
	file.seekg(std::ios::beg);
	REP(i, random(0, n))
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
			queues[i].enqueue(Customer(setname(), random(1, 300)));
	}
	return queues;
}

vq queues_to_simulate(int n){
	vq queues;
	queues.reserve(n);
	int n_customers;
	REP(i, n){
		queues.push_back(Queue());
		n_customers = random(1, 100); 
		REP(j, n_customers)
			queues[i].enqueue(Customer(setname(), random(1, 300)));
	}
	return queues;
}

vq queues_to_simulate(){
	vq queues;
	int n = random(1, 20);
	int n_customers;
	queues.reserve(n);
	REP(i, n){
		queues.push_back(Queue());
		n_customers = random(1, 100); 
		REP(j, n_customers)
			queues[i].enqueue(Customer(setname(), random(1, 300)));
	}
	return queues;
}
