#include "expression.h"
#include <stack>
#include <string>
#include <cmath>
#include <limits>
#include <iostream>

using std::string;
using std::cerr;
using std::endl;

const char Expression::binaryOperator[] = {'+', '-', '*', '/', '^', '%'};
const string Expression::unaryOperator[] = {"sqrt", "round", "abs", "sin", "cos", "tan", "asin", "acos", "atan", "rad2deg", "log", "ln"};
const std::unordered_map<std::string, double> Expression::predefinedVariables = {
    {"PI", 3.141592653589793238462643383279502884197169399375105820974944592307816406286 },
    {"E",  2.718281828459045235360287471352662497757247093699959574966967627724076630353 }
};

Expression::Expression(const string& expression) {
    this->expression = expression;
}

int Expression::precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    else if (op == '*' || op == '/')
        return 2;
    else if (op == '^')
        return 3;
    return 0;
}

bool Expression::hasPrecedence(char o1, char o2) {
    return precedence(o1) <= precedence(o2);
}

double Expression::applyOp(double b, double a, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return std::pow(a, b);
        case '%': return (int)a % (int)b;
        default: return std::numeric_limits<double>::min();
    }
}

double Expression::applySpecialOp(double value, const string& op) {
    if (op == "sqrt") {
        return std::sqrt(value);
    }
    else if (op == "round") {
        if (value - (int)value >= 0.5) {
            return (int)value + 1;
        } else {
            return (int)value;
        }
    }
    else if (op == "abs") {
        return std::signbit(value) ? -value : value;
    }

    // trig
    else if (op == "sin")
        return std::sin(value);
    else if (op == "cos")
        return std::cos(value);
    else if (op == "tan")
        return std::tan(value);
    else if (op == "asin")
        return std::asin(value);
    else if (op == "acos")
        return std::acos(value);
    else if (op == "atan")
        return std::atan(value);

    // logs
    else if (op == "log")
        return std::log10(value);
    else if (op == "ln")
        return std::log(value);

    else if (op == "rad2deg")
        return 180.0 * value / predefinedVariables.at("PI");

    // unknown
    else
        return std::numeric_limits<double>::min();
}

bool Expression::isOp(char op) {
    for (char c : binaryOperator)
        if (op == c)
            return true;
    return false;
}

bool Expression::isSpecialOp(const string& op) {
    for (string s : unaryOperator)
        if (op == s)
            return true;
    return false;
}

string Expression::cleanTokens(const string& tokens) {
    string result = "";
    for (size_t i = 0; i < tokens.length(); i++)
        if (tokens[i] != ' ')
            result += tokens[i];
    return result;
}

std::tuple<double, string> Expression::evaluate(const std::unordered_map<string, double>& variableMap) {
    size_t i;
    std::stack<double> values;
    std::stack<char> ops;
    std::stack<string> specialOps;
    string buffer = "";
    string tokens = cleanTokens(expression);

    double a, b;
    char c;

    if (tokens.size() == 0)
        return std::tuple(0, "Cannot solve empty expression");

    for (i = 0; i < tokens.length(); i++) {

        // ignore whitespace
        if (tokens[i] == ' ')
            continue;

        // push open paren
        else if (tokens[i] == '(')
            ops.push(tokens[i]);

        // push values on the values stack
        else if (std::isdigit(tokens[i])) {

            int negate = negateCheck(tokens, i, ops);

            int dotCount = 0;
            while (i < tokens.length() && (std::isdigit(tokens[i]) || tokens[i] == '.')) {
                buffer += tokens[i];
                if (tokens[i] == '.')
                    dotCount++;
                i++;
            }

            if (dotCount > 1)
                return std::tuple(0, "Invalid number at pos " + std::to_string(i));

            values.push(negate * std::stod(buffer));
            buffer = "";
            i--;
        }

        // detect a word, check if it's a unary op or variable name
        else if (std::isalpha(tokens[i])) {

            int negate = negateCheck(tokens, i, ops);

            while (i < tokens.length() && (std::isalnum(tokens[i]) || tokens[i] == '_')) {
                buffer += tokens[i];
                i++;
            }

            if (isSpecialOp(buffer)) {
                specialOps.push(buffer);

                buffer = "";
                int open = 0, close = 0;

                while (i < tokens.length() && (open == 0 || open != close)) {
                    if (tokens[i] == '(')
                        open++;
                    if (tokens[i] == ')')
                        close++;
                    buffer += tokens[i];
                    i++;
                }

                // try to evaluate the expression inside the map
                std::tuple<double, string> res = Expression(buffer).evaluate(variableMap);
                if (std::get<1>(res) != "")
                    return std::tuple(0, "Inside " + specialOps.top() + ": " + std::get<1>(res));

                values.push(negate * applySpecialOp(
                    std::get<0>(res),
                    specialOps.top())
                );

                specialOps.pop();
            }
            // variable was found
            else {
                if (variableMap.count(buffer) == 0 && predefinedVariables.count(buffer) == 0) {
                    return std::tuple(0, "Variable " + buffer + " is undefined");
                }
                else if (variableMap.count(buffer) != 0) {
                    values.push(negate * variableMap.at(buffer));
                }
                else {
                    values.push(negate * predefinedVariables.at(buffer));
                }

            }
            buffer = "";
            i--;
        }

        // closing paren
        else if (tokens[i] == ')') {
            while (ops.top() != '(') {
                a = values.top();
                values.pop();
                b = values.top();
                values.pop();
                c = ops.top();
                ops.pop();

                values.push(applyOp(a, b, c));
            }

            ops.pop();
        }

        // evaluate operators that have higher precedence
        else if (isOp(tokens[i])) {
            while (!ops.empty() && hasPrecedence(tokens[i], ops.top()) && !isOp(tokens[i-1])) {
                a = values.top();
                values.pop();
                b = values.top();
                values.pop();
                c = ops.top();
                ops.pop();

                values.push(applyOp(a, b, c));
            }

            ops.push(tokens[i]);
        }
    }

    // clean up the rest of the operators/values
    while (!ops.empty()) {

        if (values.size() < 2)
            return std::tuple(0, "Too many operators were found");


        a = values.top();
        values.pop();
        b = values.top();
        values.pop();
        c = ops.top();
        ops.pop();

        values.push(applyOp(a, b, c));
    }

    return std::tuple(values.top(), "");
}

std::tuple<double, string> Expression::evaluate() {
    std::unordered_map<string, double> variableMap;
    return evaluate(variableMap);
}

std::vector<string> Expression::getVars() {
    string tokens = cleanTokens(expression);
    std::vector<string> vars;

    string buffer;

    for (size_t i = 0; i < tokens.size(); i++) {
        if (std::isalpha(tokens.at(i))) {

            while (i < tokens.size() && (std::isalnum(tokens[i]) || tokens[i] == '_')) {
                buffer.push_back(tokens.at(i));
                i++;
            }

            if (!predefinedVariables.contains(buffer) && !isSpecialOp(buffer) && buffer != "" && !(std::find(vars.begin(), vars.end(), buffer) != vars.end()))
                vars.push_back(buffer);
            buffer = "";
        }
    }

    return vars;
}

int Expression::negateCheck(string tokens, int index, std::stack<char>& opstack) {
    if ((index == 1 && tokens[index-1] == '-') || (index > 1 && tokens[index-1] == '-' && isOp(tokens[index-2]))) {
        opstack.pop();
        return -1;
    }
    return 1;
}

string Expression::toString() { return expression; }
