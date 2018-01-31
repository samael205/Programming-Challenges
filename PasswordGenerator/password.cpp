#include "password.h"
#include <cctype>

PasswordGenerator::PasswordGenerator(){
	passwordSettings = new bool[n];
	password = "\0";
	length = 10;
}

PasswordGenerator::~PasswordGenerator(){
	delete [] passwordSettings;
}

const int PasswordGenerator::random(int n){
	return std::rand()%n;
}

void PasswordGenerator::Settings(){
	char choice ='\0';

	while(std::tolower(choice) != 'k'){
		Show();

		while(!(cin>>choice)){
			while(cin>>choice)
				break;
			cin.clear();
			continue;
		}
		
		setSettings(choice);		
	}
}

const void PasswordGenerator::Show(){
	cout << "\033[2J\033[1;1H";
	cout<<"Password Generator v1.0\n";
	cout<<"u.\t"<<checked(passwordSettings[0])<<" Upper Case"
		<<"\tStrength: "<<passwordStrength()<<"\n";
	cout<<"l.\t"<<checked(passwordSettings[1])<<" Lower Case\n";
	cout<<"n.\t"<<checked(passwordSettings[2])<<" Numbers\n";
	cout<<"s.\t"<<checked(passwordSettings[3])<<" Symbols\n";
	cout<<"t.\t"<<"Length: "<<length<<"\n";
	cout<<"g.\tGenerate\n";
	cout<<"e.\tExit\n";
	if(password.length() >= 10)
		cout<<"Your generated password: "<<password<<'\n';
	cout<<"-: "<<std::flush;
}

string PasswordGenerator::checked(bool setting){
	if(setting)
		return  "[X]";
	return "[ ]";
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
			Generate();
		break;
		case 't':
			std::cout<<"Set new password length: "<<std::flush;
			cin>>length;
			while(cin.get() != '\n' || length < 10 || length > 255){
				std::cout<<"Password length should be (10-255)\n"
					<<"Enter new Password: "<<std::flush;
				while(cin>>length)
					break;
				continue;
			}
		break;
		case 'e':
			std::exit(EXIT_SUCCESS);
		break;
		default:

		break;
	}
}

void PasswordGenerator::Generate(){
	string rules = "\0";
	password.clear();

	if(passwordSettings[0])
		for(int i=65; i<91; i++)
			rules += (char)i;
	if(passwordSettings[1])
		for(int i=97; i<123; i++)
			rules += (char)i;	
	if(passwordSettings[2])
			rules += "1234567890";	
	if(passwordSettings[3])
			rules += "'!?$?%^&*()_-+={[}]:;@~#|<,>.?";	

	int ruleLen = rules.length();
	if(ruleLen <= 1)
		return;
	for(int i=0; i<length; i++)	
		password += rules[random(ruleLen)];	
}

string PasswordGenerator::passwordStrength(){
	string strength = "\0";
	if(upperInPass())
		strength += "***";
	if(lowerInPass())
		strength += "***";
	if(numInPass())
		strength += "***";
	if(symbolInPass())
		strength += "***";
	if(strength.length() > 0 && strength.length() <= 3)
		strength += " \033[1;31mLOW\033[0m";
	else if(strength.length() > 3 && strength.length() <= 6)
		strength += " \033[1;33mMEDIUM\033[0m";
	else if(strength.length() > 6 && strength.length() <= 9)
		strength += " \033[1;32mGOOD\033[0m";
	else if(strength.length() > 9 && strength.length() <= 12)
		strength += " \033[1;35mVERY GOOD\033[0m";
	return strength;
}

bool PasswordGenerator::upperInPass(){
	REP(i, password.length())
		if(std::isupper(password[i]))
			return true;
	return false;	
}

bool PasswordGenerator::lowerInPass(){
	REP(i, password.length())
		if(std::islower(password[i]))
			return true;
	return false;
}

bool PasswordGenerator::numInPass(){
	REP(i, password.length())
		if(std::isdigit(password[i]))
			return true;
	return false;
}

bool PasswordGenerator::symbolInPass(){
	REP(i, password.length())
		if(std::ispunct(password[i]))
			return true;
	return false;
}
