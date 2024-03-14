# Doublr

Doublr is a simple expression-solving class designed to be easy to add and use to projects. Doublr currently supports the following:

## Basic Math
- PEMDAS operations (+, -, *, /, ^)
- Remainder operation (%)
- Negation ("-5--5" is valid, but "5-+6" is not)
- Built-in variables ("PI" and "E")

## Built-in Functions
Trig operations:
- sin(x) and asin(x)
- cos(x) and acos(x)
- tan(x) and atan(x)
- rad2deg(x)
Logarithms
- ln(x)
- log(x)
And some other useful functions
- abs(x)
- round(x)
- sqrt(x)

## How to Use
Once Doublr is integrated into your project, creating an expression and solving one is as simple as this:
```cpp
DExpression e("sqrt(abs(10-35))");
e.evaluate();
```

And if variables are needed, an `unordered_map<std::string, double>` is needed to fill in these variables:
```cpp
std::unordered_map<string, double> vars;
vars["x"] = DExpression::PI;
vars["m"] = 2;
vars["b"] = 2.555;

DExpression e("m * x + b");
e.evaluate(vars);
```
