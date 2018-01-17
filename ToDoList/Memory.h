#ifndef MEMORY_H_
#define MEMORY_H_

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>
#include <sstream>

using std::cin;
using std::cout;
using std::ios;

#define REP(i, n) for(int i=0; i<n; i++)
#define VAR(i, x) typeof(x) i = x
#define FOREACH(i, n) for(VAR(i, n.begin()); i != n.end(); i++)

typedef std::vector<std::pair<std::string, std::string>> vis;
typedef std::vector<std::string> datas;

class Tasks{
private:
	std::string linia;
	vis tasks;
	datas date_task;
	std::ifstream plik;
public:
	Tasks();
	Tasks(const vis & v);
	~Tasks();

	const void Check() const;
	void AddTask();
	
protected:
	void NewTask(std::string & t);
	void EraseContent_File();
	void Split(const std::string & s);
	void Check_Progress();
	const void Menu() const;
	const void Clear() const { cout<< "\033[2J\033[1;1H"; }
};

Tasks::Tasks() : tasks(0){
	plik.open("tasks.txt");
	while(std::getline(plik, linia)){
		Split(linia);
		tasks.push_back(std::make_pair<std::string, std::string>(date_task[0].c_str(), date_task[1].c_str()));
		date_task.clear();
	}
	plik.close();
	EraseContent_File();
}
Tasks::~Tasks(){
	std::ofstream write_to_file;
	write_to_file.open("tasks.txt", ios::in) ;
	for(auto task : tasks){
		write_to_file<<task.first<<";"<<task.second<<'\n';
	}
	write_to_file.close();
}
const void Tasks::Menu() const{
	REP(i, 40)
		cout<<"-";
	cout<<"\nA. Add New Task\n";
	cout<<"R. Remove\n";
	cout<<"P. Check Progress\n";
	cout<<"E. Exit\n";
	cout<<"-: "<<std::flush;
}
void Tasks::Check_Progress(){
	int taskNumber;
	char done;
	while(std::cout<<"write number of task to check\n"<<": "<<std::flush && !(cin>>taskNumber)){
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	if(taskNumber > 0 && taskNumber <= tasks.size()){
		Clear();
		cout<<tasks[taskNumber-1].second<<" is done?\n";
		cout<<"(D/N): "<<std::flush;
		cin>>done;
		while(cin.get() != '\n' && !(cin)){
			continue;
		}
		if(std::tolower(done) == 'd')
			tasks[taskNumber-1].first = "DONE";
		else if (std::tolower(done) == 'n')
			tasks[taskNumber-1].first = "";
		else
			while(cin.get() != '\n')
				continue;
	}
}
void Tasks::AddTask(){
	std::string date, job;
	char option;
	Check();
	do{
		Menu();
		while(!(cin>>option)){
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		switch(std::tolower(option)){
			case 'a':
				Check();
				cin.ignore();
				cout<<"Task: ";
				std::getline(cin, job, '\n');
				NewTask(job);
			break;	
			case 'r':
				int index;
				Check();
				cout<<"Remove: "<<std::flush;
				cin>>index;
				if(index > 0 && index <= tasks.size())
					tasks.erase(tasks.begin() + (index-1));
			break;
			case 'p':
				Check();
				Check_Progress();
			break;
		}
		Check();
	}while(option != 'e');
}
void Tasks::EraseContent_File(){
	std::ofstream erase;
	erase.open("tasks.txt", ios::out | ios::trunc);
	erase.close();
}
void Tasks::Split(const std::string & s){
	std::stringstream in(s);
	std::string split;
	while(std::getline(in, split, ';'))
		date_task.push_back(split);
}
const void Tasks::Check() const{
	Clear();
	int i = 1;
	for(auto task : tasks)
		cout<<i++<<": "<<task.first<<" - "<<task.second<<'\n';
}
void Tasks::NewTask(std::string & t){
	tasks.push_back(std::make_pair<std::string, std::string>("", t.c_str()));
}

#endif
