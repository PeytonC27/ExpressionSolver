#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <stack>

class Expression {
private:
    static const char binaryOperator[];
    static const std::string unaryOperator[];
    static const std::unordered_map<std::string, double> predefinedVariables;
    std::string expression;

    ///
    /// \brief Returns the precedence of a specific binary operator
    /// \param op the operator to check
    /// \return the operator's precedence
    ///
    int precedence(char op);

    ///
    /// \brief Checks if the second operator has a higher or equal precedence
    /// \param o1 the first operator
    /// \param o2 the second operator
    /// \return true if the second operator has higher/equal precedence, false otherwise
    ///
    bool hasPrecedence(char o1, char o2);

    ///
    /// \brief Applies an operation to two doubles
    /// \param b the second number
    /// \param a the first number
    /// \param op the operator to apply
    /// \return
    ///
    double applyOp(double b, double a, char op);


    ///
    /// \brief Applies a unary operation to a double
    /// \param value the double used by the operation
    /// \param op the unary operation to apply
    /// \return
    ///
    double applySpecialOp(double value, const std::string& op);

    ///
    /// \brief Checks if a char is an operator
    /// \param op the char to check
    /// \return
    ///
    bool isOp(char op);

    ///
    /// \brief Checks if a string is a unary operator
    /// \param op the string to check
    /// \return
    ///
    bool isSpecialOp(const std::string& op);

    ///
    /// \brief Cleans up an expression by elininating whitespace
    /// \param tokens the string to clean
    /// \return the cleaned up expression as a string
    ///
    std::string cleanTokens(const std::string& tokens);

    ///
    /// \brief Checks if the current number/variable needs to be negated
    /// \param tokens all tokens
    /// \param index the index being checked
    /// \param opstack the operation stack
    /// \return
    ///
    int negateCheck(std::string tokens, int index, std::stack<char>& opstack);

public:
    Expression(const std::string& expression);

    ///
    /// \brief Solves the expression with a variable map
    /// \param A map with all the <variable, value> pairs in this equation
    /// \return The expression returns 0 along with a message. If the message (string)
    ///         is empty, then no errors occurred
    ///
    std::tuple<double, std::string> evaluate(const std::unordered_map<std::string, double>& variableMap);

    ///
    /// \brief Solves the expression
    /// \return The expression returns 0 along with a message. If the message (string)
    ///         is empty, then no errors occurred
    ///
    std::tuple<double, std::string> evaluate();

    std::string toString();

    std::vector<std::string> getVars();
};
