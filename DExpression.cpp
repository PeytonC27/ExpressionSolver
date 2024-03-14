#include "DExpression.h"
#include <stack>
#include <string>
#include <cmath>
#include <limits>
#include <iostream>
#include <algorithm>

using std::string;
using std::cerr;
using std::endl;

const char DExpression::binaryOperator[] = {'+', '-', '*', '/', '^', '%'};
const string DExpression::unaryOperator[] = {"sqrt", "round", "abs", "sin", "cos", "tan", "asin", "acos", "atan", "rad2deg", "log", "ln"};
const std::unordered_map<std::string, double> DExpression::predefinedVariables = {
    {"PI", 3.141592653589793238462643383279502884197169399375105820974944592307816406286 },
    {"E",  2.718281828459045235360287471352662497757247093699959574966967627724076630353 }
};

DExpression::DExpression(const string& expression) {

    // evaluate the expression to make sure it's actually runnable, otherwise it should throw an error
    if (!checkValidity(expression))
        throw std::invalid_argument("Invalid expression");

    this->expression = expression;
}

double DExpression::evaluate() {
    std::unordered_map<string, double> variableMap;
    return evaluate(variableMap);
}

double DExpression::evaluate(const std::unordered_map<string, double>& variableMap) {
    size_t i;
    std::stack<double> values;
    std::stack<char> ops;
    std::stack<string> specialOps;
    string buffer = "";
    string tokens = cleanTokens(expression);

    double a, b;
    char c;

    for (i = 0; i < tokens.length(); i++) {

        // push open paren
        if (tokens[i] == '(')
            ops.push(tokens[i]);

        // push values on the values stack
        else if (std::isdigit(tokens[i])) {

            int negate = negateCheck(tokens, i, ops);
            while (i < tokens.length() && (std::isdigit(tokens[i]) || tokens[i] == '.')) {
                buffer += tokens[i];
                i++;
            }

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

                // evaluate the expression inside the map
                double res = DExpression(buffer).evaluate(variableMap);

                values.push(negate * applySpecialOp(
                    res,
                    specialOps.top())
                );

                specialOps.pop();
            }
            // variable was found
            else {
                if (variableMap.count(buffer) == 0 && predefinedVariables.count(buffer) == 0) {
                    throw std::invalid_argument(buffer + ": undefined variable");
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
        a = values.top();
        values.pop();
        b = values.top();
        values.pop();
        c = ops.top();
        ops.pop();

        values.push(applyOp(a, b, c));
    }

    return values.top();
}

bool DExpression::checkValidity(const string& expression) {
    size_t i;
    string buffer = "";
    string tokens = cleanTokens(expression);

    int openParen = 0, closeParen = 0;

    if (tokens.size() == 0)
        return false;

    for (i = 0; i < tokens.length(); i++) {

        // ignore whitespace
        if (tokens[i] == ' ')
            continue;

        // open paren, last character must be any operator EXCEPT a closing paren
        else if (tokens[i] == '(') {
            openParen++;

            if (i == 0)
                continue;
            
            if (tokens[i-1] == ')')
                return false;
        }

        // closing paren, no operators can come before it
        else if (tokens[i] == ')') {
            closeParen++;

            if (openParen < closeParen || (!isalnum(tokens[i-1]) && tokens[i-1] != '_'))
                return false;
        }

        // minus, can only have a num/var/op symbol. Three symbols in a row is not allowed
        else if (tokens[i] == '-') {
            if (i == 0)
                continue;

            else if (i == 1) {
                if (!isalnum(tokens[0]) && tokens[0] != '_')
                    return false; 
            }

            else {
                if (isalnum(tokens[i-1]) && tokens[i-1] == '_')
                    continue;
                
                else if (isOp(tokens[i-1]) && tokens[i-1] != '(' && tokens[i-1] != ')')
                    return isOp(tokens[i-2]);
            }
        }

        // other operations, simply can not have other ops before (except ')')
        else if (isOp(tokens[i])) {
            if (i == 0)
                return false;

            if (isOp(tokens[i-1]) && tokens[i-1] != ')')
                return false;
        }


        // found digit, loop through and check if it's valid
        else if (isdigit(tokens[i])) {

            while (i < tokens.length() && (isdigit(tokens[i]) || tokens[i] == '.')) {
                buffer += tokens[i];
                i++;
            }

            if (!isValidNumber(buffer))
                return false;

            buffer = "";
            i--;
        }

        // detect a word, check if it's a unary op or variable name
        else if (std::isalpha(tokens[i])) {

            // make sure a number didn't come first
            if (i > 0 && isdigit(tokens[i-1]))
                return false;

            while (i < tokens.length() && (isalnum(tokens[i]) || tokens[i] == '_')) {
                buffer += tokens[i];
                i++;
            }

            if (!isSpecialOp(buffer) && !isValidVarName(buffer))
                return false;

            buffer = "";
            i--;
        }

        // invalid character
        else {
            return false;
        }
    }

    if (openParen != closeParen)
        return false;

    return true;
}

int DExpression::precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    else if (op == '*' || op == '/' || op == '%')
        return 2;
    else if (op == '^')
        return 3;
    return 0;
}

bool DExpression::hasPrecedence(char o1, char o2) {
    return precedence(o1) <= precedence(o2);
}

double DExpression::applyOp(double b, double a, char op) {
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

double DExpression::applySpecialOp(double value, const string& op) {
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

bool DExpression::isOp(char op) {
    for (char c : binaryOperator)
        if (op == c)
            return true;
    return false;
}

bool DExpression::isSpecialOp(const string& op) {
    for (string s : unaryOperator)
        if (op == s)
            return true;
    return false;
}

string DExpression::cleanTokens(const string& tokens) {
    string result = "";
    for (size_t i = 0; i < tokens.length(); i++)
        if (tokens[i] != ' ')
            result += tokens[i];
    return result;
}

std::vector<string> DExpression::getVars() {
    string tokens = cleanTokens(expression);
    std::vector<string> vars;

    string buffer;

    for (size_t i = 0; i < tokens.size(); i++) {
        if (std::isalpha(tokens.at(i))) {

            while (i < tokens.size() && (std::isalnum(tokens[i]) || tokens[i] == '_')) {
                buffer.push_back(tokens.at(i));
                i++;
            }

            // find buffer in vars
            if (!predefinedVariables.count(buffer) && !isSpecialOp(buffer) && buffer != "" && !(std::find(vars.begin(), vars.end(), buffer) == vars.end()))
                vars.push_back(buffer);
            buffer = "";
        }
    }

    return vars;
}

int DExpression::negateCheck(string tokens, int index, std::stack<char>& opstack) {
    if ((index == 1 && tokens[index-1] == '-') || (index > 1 && tokens[index-1] == '-' && isOp(tokens[index-2]))) {
        opstack.pop();
        return -1;
    }
    return 1;
}

string DExpression::toString() { return expression; }

bool DExpression::isValidVarName(const std::string& s) {
    if (s.size() < 1)
        return false;

    // make sure it's not a special op name
    if (isSpecialOp(s))
        return false;

    // first is not letter or underscore check
    if (!std::isalpha(s[0]) && s[0] != '_')
        return false;

    // make sure the rest are nums, letters, or _
    for (size_t i = 1; i < s.size(); i++)
        if (!std::isalnum(s[i]) && s[0] != '_')
            return false;

    return true;
}

bool DExpression::isValidNumber(const std::string& s) {
    int dotCount = 0;

    for (size_t i = 0; i < s.size(); i++) {
        if (!std::isdigit(s[i]) && s[i] != '.')
            return false;

        if (s[i] == '.')
            dotCount++;
    }

    if (dotCount > 1)
        return false;

    return true;
}