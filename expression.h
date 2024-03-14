#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <stack>

/// @author Peyton Crua
///
/// @brief  A simple expression solving class designed to be easy to use. As of now
///         this is a small project with minimal features, but plans to integrate more 
///         are to come.
///
/// @date March 13, 2024
///
class Expression {
private:
    static const char binaryOperator[];
    static const std::string unaryOperator[];
    static const std::unordered_map<std::string, double> predefinedVariables;
    std::string expression;

    ///
    /// @brief Returns the precedence of a specific binary operator
    /// @param op the operator to check
    /// @return the operator's precedence
    ///
    int precedence(char op);

    ///
    /// @brief Checks if the second operator has a higher or equal precedence
    /// @param o1 the first operator
    /// @param o2 the second operator
    /// @return true if the second operator has higher/equal precedence, false otherwise
    ///
    bool hasPrecedence(char o1, char o2);

    ///
    /// @brief Applies an operation to two doubles
    /// @param b the second number
    /// @param a the first number
    /// @param op the operator to apply
    /// @return
    ///
    double applyOp(double b, double a, char op);


    ///
    /// @brief Applies a unary operation to a double
    /// @param value the double used by the operation
    /// @param op the unary operation to apply
    /// @return
    ///
    double applySpecialOp(double value, const std::string& op);

    ///
    /// @brief Checks if a char is an operator (+, -, etc)
    /// @param op the char to check
    /// @return
    ///
    bool isOp(char op);

    ///
    /// @brief Checks if a string is a unary operator (ln, log, sin, cos, etc)
    /// @param op the string to check
    /// @return
    ///
    bool isSpecialOp(const std::string& op);

    ///
    /// @brief Cleans up an expression by elininating whitespace
    /// @param tokens the string to clean
    /// @return the cleaned up expression as a string
    ///
    std::string cleanTokens(const std::string& tokens);

    ///
    /// @brief Checks if the current number/variable needs to be negated
    /// @param tokens all tokens
    /// @param index the index being checked
    /// @param opstack the operation stack
    /// @return
    ///
    int negateCheck(std::string tokens, int index, std::stack<char>& opstack);

    ///
    /// @brief Used to check the validity of an expression
    /// @param expression the expression to check
    /// @return 
    ///
    bool checkValidity(const std::string& expression);

    ///
    /// @brief Checks the validity of a variable name
    /// @param s the string to check
    /// @return
    ///
    bool isValidVarName(const std::string& s);

    ///
    /// @brief Checks if a string is a valid double
    /// @param s the string to check
    /// @return 
    ///
    bool isValidNumber(const std::string& s);

public:
    ///
    /// @brief  Creates an immutable expression, throwing an "invalid_argument" exception if
    ///         the passed in expression is invalid. Important notes about expressions:
    ///
    ///                 a) Variable names are restricted to a letter/underscore followed by letters/numbers/underscores
    ///                 b) Whitespace is completely ignored
    ///
    /// @param expression the expression to parse
    /// @throw if the expression is invalid, an "invalid_argument" exception is thrown
    ///
    Expression(const std::string& expression);

    ///
    /// @brief Solves the expression with a variable map
    /// @param variableMap map with all the <variable, value> pairs in this equation
    /// @return the value of the expression
    /// @throw if a variable is undefined, an "invalid_argument" exception is thrown
    ///
    double evaluate(const std::unordered_map<std::string, double>& variableMap);

    ///
    /// @brief Solves the expression without using a variable map
    /// @return The expression returns 0 along with a message. If the message (string)
    ///         is empty, then no errors occurred
    /// @throw if a variable is undefined, an "invalid_argument" exception is thrown
    ///
    double evaluate();

    /// @brief Returns the expression as a string
    /// @return 
    std::string toString();

    /// @brief Grabs all the variables inside this equation
    /// @return 
    std::vector<std::string> getVars();
};
