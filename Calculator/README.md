# Description

Solve typical math equation like

```sh
2+3-2(2/4)*3
```

### How

Convert infix expression to postfix

```cp
std::string infixToPostix(const std::string & expression);
```

Solve them using stack

```cp
double solve(const std::string & expression);
```

# Installation

- Compile

```sh
$ g++ -std=c++11 *.cpp *.h -o calculator
```

