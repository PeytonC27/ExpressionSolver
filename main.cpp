#include "DExpression.h"
#include <iostream>

int main() {
    DExpression e("1-sqrt(25)");
    std::cout << e.evaluate() << std::endl;
}

