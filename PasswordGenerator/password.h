#ifndef PASSWORD_H_
#define PASSWORD_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <ctime>
#include <memory>

#define REP(i, n) for(int i=0; i<n; i++)

using std::string;
using std::cout;
using std::cin;

class PasswordGenerator{
public:
	PasswordGenerator();
	~PasswordGenerator();

	void GeneratePassword();
	void Settings();
	const void Show();
protected:
	void setSettings(const char);
	
	string checked(bool);
	string passwordStrength();

	int calculateNumberOfRules();
private:
	enum { n = 4 };
	std::unique_ptr<bool[]> passwordSettings;
	string password;
	int length;
};

#endif
