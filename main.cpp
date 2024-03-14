#include "expression.h"
#include <iostream>

int main() {
    Expression e("1-sqrt(25)");
    std::cout << e.evaluate() << std::endl;
}

