#include <iostream>
#include <cmath>
#include "callable.hpp"

int main() {
    callable<double, double, double>::register_func(
        "pow",
        [](const std::vector<double>& v) {
            return std::pow(v[0], v[1]);
        }
    );
    callable<double, double, double>::register_func(
        "nroot",
        [](const std::vector<double>& v) {
            return std::pow(v[0], 1.0 / v[1]);
        }
    );
    callable<double, double, double> sum({"x", "y"}, "x + y");
    callable<double, double, double> sub({"x", "y"}, "x - y");
    callable<double, double, double> mul({"x", "y"}, "x * y");
    callable<double, double, double> div({"x", "y"}, "x / y");
    callable<double, double, double> pow({"x", "y"}, "pow(x,y)");
    callable<double, double, double> nroot({"x", "y"}, "nroot(x,y)");
    double a;
    double b;
    std::cout << "Ingrese dos numeros: \n> ";
    std::cin >> a;
    std::cout << "> ";
    std::cin >> b;
    std::cout << "sum: " << sum(a, b) << "\n";
    std::cout << "sub: " << sub(a, b) << "\n";
    std::cout << "mul: " << mul(a, b) << "\n";
    std::cout << "div: " << div(a, b) << "\n";
    std::cout << "pow: " << pow(a, b) << "\n";
    std::cout << "nroot: " << nroot(a, b) << "\n";
    return 0;
}
