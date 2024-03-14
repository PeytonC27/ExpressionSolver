#include "DExpression.h"
#include <iostream>
#include <unordered_map>

int main() {
    std::unordered_map<std::string, double> vars;
    vars["x"] = DExpression::PI;
    vars["m"] = 2;
    vars["b"] = 2.555;

    DExpression e("m * x + b");
    std::cout << e.evaluate(vars) << std::endl; 
}

