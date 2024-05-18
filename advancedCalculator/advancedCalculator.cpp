// INCLUDES //
#include <iostream>
#include <variant>
#include <format>
#include <functional>
#include <cmath>

constexpr char supported_operators[] = "+-*/^S"; // operators list

// Define an alias for type operation  
using operation_t = std::variant<std::plus<>, std::minus<>, std::multiplies<>, std::function<double(double, double)>>; //  using variant to  store different operation types

// Define a function to get the operation object based on the input
operation_t get_operation(char op) {
    switch (op) {
    case '+': return std::plus<>(); // addition op
    case '-': return std::minus<>(); // subtraction op
    case '*': return std::multiplies<>(); // multiplication op
    case '/': return [](double num1, double num2) { // division func
        if (num2 == 0) {
            throw std::invalid_argument("Division by zero");
        }
        return num1 / num2;
        };
    case 'S': return [](double num1, double _) { // square root func
        if (num1 < 0) {
            throw std::invalid_argument("Cannot take a square root of negative number");
        }
        return std::sqrt(num1);
        };
    case '^': return [](double num1, double num2) { return std::pow(num1, num2); };

    default: throw std::invalid_argument("Invalid operator"); // throw unsupported op
    }
}

// Define function to do the calculation using operation object
double calculate(double num1, double num2, const operation_t& op) {
    // using visit to call appropriate function based on the operator
    return std::visit([&](auto f) { return f(num1, num2); }, op);
}

int main() {
    char op;
    double num1, num2;

    // loop until user enters a valid operator
    while (strchr(supported_operators, op) == nullptr) {
        std::cout << "Enter operator (" << supported_operators << ")" << " (- 'S' means square root): ";
        std::cin >> op;
    }

    // Add square root exclusion
    if (op != 'S') {
        std::cout << "Enter two numbers: ";
        std::cin >> num1 >> num2;
    } else {
        std::cout << "Enter a number: ";
        std::cin >> num1;
        num2 = 0; // set num2 to 0 cuz you dont need a second number in square rooting
    }

    // Handle output
    try {
        const auto operation = get_operation(op);
        const auto result = calculate(num1, num2, operation);
        const auto formatted = std::format("{} {} {} = {}\n", num1, op, num2, result);

        std::cout << formatted;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}
