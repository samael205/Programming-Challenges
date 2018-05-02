#include "password.h"
#include <cctype>
#include <algorithm>

PasswordGenerator::PasswordGenerator() : passwordSettings(new bool[n]){
	passwordSettings[0] = true;
	password = "\0";
	length = 10;
}

PasswordGenerator::~PasswordGenerator(){}

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
	cout<<"Password Generator v1.2\n";
	cout<<"u.\t"<<checked(passwordSettings[0])<<" Upper Case"
		<<"\tStrength: "<<passwordStrength()<<"\n";
	cout<<"l.\t"<<checked(passwordSettings[1])<<" Lower Case\n";
	cout<<"n.\t"<<checked(passwordSettings[2])<<" Numbers\n";
	cout<<"s.\t"<<checked(passwordSettings[3])<<" Symbols\n";
	cout<<"t.\t"<<"Length: \033[1;31m"<<length<<"\033[0m\n";
	cout<<"g.\tGenerate\n";
	cout<<"e.\tExit\n";
	if(password.length() >= 10)
		cout<<"Your generated password: \e[1m"<<password<<"\e[0m\n";
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
				while(!(cin>>length))
					break;
				cin.clear();
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

static std::string availableChars(std::unique_ptr<bool[]> const & settings){
	string rule = "\0";

	if(settings[0])
		for(int i=65; i<91; i++)
			rule += (char)i;
	if(settings[1])
		for(int i=97; i<123; i++)
			rule += (char)i;	
	if(settings[2])
			rule += "1234567890";	
	if(settings[3])
			rule += "'!?$?%^&*()_-+={[}]:;@~#|<,>.?";	

	std::random_shuffle(rule.begin(), rule.end());	

	return rule;	
}

void PasswordGenerator::GeneratePassword(){
	password.clear();
	const std::string passwordRule = availableChars(passwordSettings);

	int ruleLength = passwordRule.length();
	const int requiredPasswordLength = 8;
	if(ruleLength <= requiredPasswordLength)
		return;

	for(int i=0; i<length; i++)	
		password += passwordRule[random(ruleLength)];	
}

string PasswordGenerator::passwordStrength(){
	string strength = "\0";
	bool hasUppercase, hasLowercase, hasNumbers, hasSymbols;
	hasUppercase = upperInPass();
	hasLowercase = lowerInPass();
	hasNumbers = numInPass();	
	hasSymbols = symbolInPass();

	int numberOfRulesAchieved = hasLowercase + hasUppercase + hasNumbers + hasSymbols;

	if(numberOfRulesAchieved == 1)
		strength = " \033[1;31mLOW\033[0m";
	else if(numberOfRulesAchieved == 2)
		strength = " \033[1;33mMEDIUM\033[0m";
	else if(numberOfRulesAchieved == 3)
		strength = " \033[1;32mGOOD\033[0m";
	else if(numberOfRulesAchieved == 4)
		strength = " \033[1;35mVERY GOOD\033[0m";

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
