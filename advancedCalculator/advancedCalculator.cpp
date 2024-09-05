// INCLUDES //
#include <iostream>
#include <variant>
#include <format>
#include <functional>

// Operator lists
constexpr char supported_operators[] = "+-*/^R%!";
constexpr char single_operators[] = "R!";

// Define an alias for type operation using variant
using operation_t = std::variant<std::plus<>, std::minus<>, std::multiplies<>, std::function<double(double, double)>>;

// Calculator functions
static operation_t get_operator(char op) {
	switch (op)
	{
	case '+': return std::plus<>(); // addition op
	case '-': return std::minus<>(); // substraction op
	case '*': return std::multiplies<>(); // multiplication op
	case '^': return [](double num1, double num2) { return std::pow(num1, num2); }; // pow op
	case '/': return [](double num1, double num2) { return num2 == 0 ? throw std::invalid_argument("Division by 0") : num1 / num2;  }; // division op
	case 'R': return [](double num1, double _) { return num1 < 0 ? throw std::invalid_argument("Cannot find a square root of a negative number!") : std::sqrt(num1); }; // square root op
	case '%': return [](double num1, double num2) { return num2 == 0 ? throw std::invalid_argument("Division by zero") : static_cast<int>(num1) % static_cast<int>(num2); }; // modulo op
	case '!': return [](double num1, double _) { if (num1 < 0) throw std::invalid_argument("Negative number"); std::function<double(double)> fact = [&](double n) -> double {return (n <= 1) ? 1 : n * fact(n - 1);};return fact(num1);}; // factorial op
	default: throw std::invalid_argument("Invalid operator"); // throw an error about unsupported operator
	}
}

inline static double calculate(double num1, double num2, operation_t op) {
	return std::visit([&](auto f) {return f(num1, num2); }, op); // using std::visit to call appropriate function based on the operator
}

int main() {
	char op;
	double num1, num2;
	bool singleOp = false;

	// loop until user enters a valid operator
	do {
		std::cout << "Enter an operator (";
		for (uint8_t i = 0; i < sizeof(supported_operators) - 2; ++i) {
			std::cout << supported_operators[i] << ", ";
		}
		std::cout << supported_operators[sizeof(supported_operators) - 2] << "): ";

		std::cin >> op;
	} while (strchr(supported_operators, op) == nullptr);

	// Making an one number for factorial and square root
	if (!strchr(single_operators, op)) {
		std::cout << "Enter two numbers: ";
		std::cin >> num1 >> num2;
	} else {
		std::cout << "Enter a number: ";
		std::cin >> num1;
		num2 = 0;
		singleOp = true;
	}

	// Handle output
	try {
		const operation_t operation = get_operator(op);
		const double result = calculate(num1, num2, operation);
		const std::string formatted = !singleOp ? std::format("{} {} {} = {}", num1, op, num2, result) : std::format("{} = {}", num1, result);

		std::cout << formatted;
	}
	// Catch any errors that might be thrown
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
	}
}
