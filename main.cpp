#include "expression.h"
#include <iostream>

void test(std::string expression) {
    try {
        Expression e(expression);
        std::cout << expression << " was valid" << std::endl;
    } catch (std::exception e) {
        std::cout << expression << " was invalid" << std::endl;
    }
}

int main() {

    // valid
    test("5+6");
    test("PI+6");
    test("5+-6");
    test("x+y*z+balls");

    // invalid
    test("5-+6");
    test("(5+6");
    test("5+6)");
    test("PI+9E");
    test("this is a:sentence");

    Expression e("1-sqrt(25)");
    std::cout << e.evaluate() << std::endl;
}

