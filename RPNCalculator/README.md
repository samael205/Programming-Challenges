# Description

Solve typical math equation like
 
```
3 + 2 - (2 / 3) * 3
```

### How

build postfix expression

```cpp
std::string Interface::buildPostfixExpression(const std::string & equation);
```
and use reverse Polish notation to calculate 

```cpp
double Interface::rpn(const std::string& expression);
```

# Installation

- Copy main.cpp from examples

- Compile

```sh
g++ -std=c++11 main.cpp calculator.h calculator.cpp -o RPNCalculator
```
