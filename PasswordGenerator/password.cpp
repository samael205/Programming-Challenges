#include "password.h"
#include <cctype>
#include <algorithm>
#include <random>

PasswordGenerator::PasswordGenerator() : passwordSettings(new bool[n]){
	passwordSettings[0] = true;
	password = "\0";
	length = 10;
}

PasswordGenerator::~PasswordGenerator() { }

void PasswordGenerator::Settings(){
	char choice ='\0';

	while(std::tolower(choice) != 'k'){
		Show();
		while(!(cin>>choice)){
			while(cin>>choice) break;
			cin.clear();
			continue;
		}
		setSettings(choice);		
	}
}

const void PasswordGenerator::Show(){
	cout << "\033[2J\033[1;1H"
	     <<"Password Generator v1.3\n"
	     <<"u.\t"<<checked(passwordSettings[0])<<" Upper Case"
	     <<"\tStrength: "<<passwordStrength()<<"\n"
	     <<"l.\t"<<checked(passwordSettings[1])<<" Lower Case\n"
	     <<"n.\t"<<checked(passwordSettings[2])<<" Numbers\n"
	     <<"s.\t"<<checked(passwordSettings[3])<<" Symbols\n"
	     <<"t.\t"<<"Length: \033[1;31m"<<length<<"\033[0m\n"
	     <<"g.\tGenerate\n"
	     <<"e.\tExit\n";
	if(password.length() >= 10) cout<<"Your generated password: \e[1m"<<password<<"\e[0m\n";
	cout<<"-: "<<std::flush;
}

string PasswordGenerator::checked(bool checked){
	return checked ? "[X]" : "[ ]";
}

void PasswordGenerator::setSettings(const char set){
	switch(std::tolower(set)){
		case 'u':
			passwordSettings[0] = !passwordSettings[0];
			break;
		case 'l':
			passwordSettings[1] = !passwordSettings[1];
			break;
		case 'n':
			passwordSettings[2] = !passwordSettings[2];
			break;
		case 's':
			passwordSettings[3] = !passwordSettings[3];
			break;
		case 'g':
			GeneratePassword();
			break;
		case 't':
			std::cout<<"Set new password length: "<<std::flush;
			cin>>length;
			while(cin.get() != '\n' || length < 10 || length > 255){
				std::cout<<"Password length should be (10-255)\n"
					 <<"Enter new Password: "<<std::flush;
				while(!(cin>>length)) break;
				cin.clear();
				continue;
			}
			break;
		case 'e':
			std::exit(EXIT_SUCCESS);
		default:
			break;
	}
}

static std::string availableChars(std::unique_ptr<bool[]> const & settings){
	string rule = "\0";

	if(settings[0]) for(int i=65; i<91; i++) rule += (char)i;
	if(settings[1]) for(int i=97; i<123; i++) rule += (char)i;	
	if(settings[2]) rule += "1234567890";	
	if(settings[3]) rule += "'!?$?%^&*()_-+={[}]:;@~#|<,>.?";	

	return rule;	
}

static std::mt19937 gen{std::random_device{}()};
template <class T>
T random(T min, T max){
	return std::uniform_int_distribution<T>{min, max}(gen);
}

void PasswordGenerator::GeneratePassword(){
	password.clear();
	auto passwordRule = availableChars(passwordSettings);
	REP(i, length) 
		password += passwordRule[random(0, (int)passwordRule.length())];
}

string PasswordGenerator::passwordStrength(){
	string strength = "\0";

	int numberOfRulesAchieved = calculateNumberOfRules();

	if(numberOfRulesAchieved == 1) strength = " \033[1;31mLOW\033[0m";
	else if(numberOfRulesAchieved == 2) strength = " \033[1;33mMEDIUM\033[0m";
	else if(numberOfRulesAchieved == 3) strength = " \033[1;32mGOOD\033[0m";
	else if(numberOfRulesAchieved == 4) strength = " \033[1;35mVERY GOOD\033[0m";

	return strength;
}

int PasswordGenerator::calculateNumberOfRules(){ 
	bool lower, upper, punct, digit;
	lower = upper = punct = digit = false;
	REP(i, password.length()){ 
		if(std::islower(password[i])) lower = true;
		if(std::isupper(password[i])) upper = true;
		if(std::ispunct(password[i])) punct = true;
		if(std::isdigit(password[i])) digit = true;
	}
	return lower + upper + punct + digit;
}
