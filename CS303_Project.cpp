#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
#include <vector>
#include <stdexcept>

using namespace std;

int order_of_ops(const string& op) {
    if (op == "||") return 1;
    if (op == "&&") return 2;
    if (op == "==" || op == "!=") return 3;
    if (op == "<" || op == ">" || op == "<=" || op == ">=") return 4;
    if (op == "+" || op == "-") return 5;
    if (op == "*" || op == "/" || op == "%") return 6;
    if (op == "^") return 7;
    if (op == "!" || op == "++" || op == "--" ) return 8;
    return 0;
}

bool isSymbol(const char& op) {
    return op == '+' || op == '-' || op == '*' || op == '/' ||
        op == '%' || op == '^' || op == '<' || op == '>' ||
        op == '=' || op == '&' || op == '|' || op == '!';
}

bool isOperator(const string& op) {
    return op == "++" || op == "--" || op == "+" || op == "-" || op == "*" || op == "/" || op == "%" ||
        op == "^" || op == "<" || op == ">" || op == "<=" || op == ">=" ||
        op == "==" || op == "!=" || op == "&&" || op == "||" || op == "!";
}

bool isUnaryOperator(const string& op) {
    return op == "++" || op == "--" || op == "!";
}

vector<string> parseInput(const string& expression) {
    vector<string> parsed_expression;
    string parser;
    if (isSymbol(expression[0])) {
        if (expression[0] != '!' && expression[0] != '+' && expression[0] != '-') { throw runtime_error("Expression cannot start with a binary operator"); }
        else if ((expression[0] == '+' || expression[0] == '-') && (expression[0] != expression[1])) { throw runtime_error("Expression cannot start with a binary operator"); }
        else if (expression[0] == '!' && isSymbol(expression[1])) { throw runtime_error("Expression cannot start with a binary operator"); }
    }
    for (int i = 0; i < expression.length(); ++i) {
        if (isalnum(expression[i])) {
            parser += expression[i];
        }
        else {
            if (!parser.empty()) {
                parsed_expression.push_back(parser);
                parser.clear();
            }
            if (expression[i] == '<' || expression[i] == '>' || expression[i] == '!' || expression[i] == '=') {
                parser += expression[i];
                if (i + 1 < expression.length()) {
                    if (expression[i + 1] == '=') {
                        parser += '=';
                        i++;
                    }
                }
            }
            else if ((expression[i] == '&' || expression[i] == '|') && i + 1 < expression.length() && expression[i] == expression[i + 1]) {
                if (expression[i] == expression[i + 2]) { throw runtime_error("Two binary operators in a row"); }
                else {
                    parser += expression[i];
                    parser += expression[i];
                    i++;
                }
            }
            else if ((expression[i] == '+' || expression[i] == '-') && i + 1 < expression.length() && expression[i] == expression[i + 1]) {
                if (!isalnum(expression[i+2]) && !(expression[i + 2] == expression[i + 3])) { throw runtime_error("A unary operand cannot be followed by a binary operator"); }
                else {
                    parser += expression[i];
                    parser += expression[i];
                    i++;
                }
            }
            else {
                parser += expression[i];
            }
            parsed_expression.push_back(parser);
            parser.clear();
        }
    }
    if (!parser.empty()) parsed_expression.push_back(parser);
    return parsed_expression;
}

vector<string> convertExpression(const vector<string>& expression) {
    stack<string> args;
    vector<string> converted;

    for (const auto& oper : expression) {
        if (isalnum(oper[0])) {
            converted.push_back(oper);
        }
        else if (oper == "(") {
            args.push(oper);
        }
        else if (oper == ")") {
            while (!args.empty() && args.top() != "(") {
                converted.push_back(args.top());
                args.pop();
            }
            if (args.empty()) {
                throw runtime_error("Mismatched parentheses");
            }
            args.pop(); // Discard the left parenthesis
        }
        else {
            while (!args.empty() && order_of_ops(args.top()) >= order_of_ops(oper)) {
                converted.push_back(args.top());
                args.pop();
            }
            args.push(oper);
        }
    }

    while (!args.empty()) {
        if (args.top() == "(") {
            throw runtime_error("Mismatched parentheses");
        }
        converted.push_back(args.top());
        args.pop();
    }

    return converted;
}

int calculate(const vector<string>& expression) {
    stack<int> args;

    for (const auto& oper : expression) {
        if (isalnum(oper[0])) {
            args.push(stoi(oper));
        }
        else if (isOperator(oper)) {
            if (isUnaryOperator(oper)) {
                int operand = args.top();
                args.pop();
                int result = operand;
                for (char unary : oper) {
                    if (unary == '-') {
                        result = -result;
                    }
                    else if (unary == '!') {
                        result = !result;
                    }
                    else if (unary == '+') {
                        result++;
                    }
                    else if (unary == '-') {
                        result--;
                    }
                }
                args.push(result);
            }
            else {
                int operand2 = args.top();
                args.pop();
                int operand1 = args.top();
                args.pop();
                int result;
                if (oper == "+") {
                    result = operand1 + operand2;
                }
                else if (oper == "-") {
                    result = operand1 - operand2;
                }
                else if (oper == "*") {
                    result = operand1 * operand2;
                }
                else if (oper == "/") {
                    if (operand2 == 0) {
                        throw runtime_error("Division by zero");
                    }
                    result = operand1 / operand2;
                }
                else if (oper == "%") {
                    result = operand1 % operand2;
                }
                else if (oper == "^") {
                    result = pow(operand1, operand2);
                }
                else if (oper == "<") {
                    result = operand1 < operand2;
                }
                else if (oper == ">") {
                    result = operand1 > operand2;
                }
                else if (oper == "<=") {
                    result = operand1 <= operand2;
                }
                else if (oper == ">=") {
                    result = operand1 >= operand2;
                }
                else if (oper == "==") {
                    result = operand1 == operand2;
                }
                else if (oper == "!=") {
                    result = operand1 != operand2;
                }
                else if (oper == "&&") {
                    result = operand1 && operand2;
                }
                else if (oper == "||") {
                    result = operand1 || operand2;
                }
                args.push(result);
            }
        }
    }

    if (args.size() != 1) {
        throw runtime_error("Invalid postfix expression");
    }

    return args.top();
}

int main() {
    string expression;
    while (true) {
        cout << "Enter an infix expression (Or type 'exit' to quit) >> ";
        getline(cin, expression);
        expression.erase(remove_if(expression.begin(), expression.end(), isspace), expression.end());
        if (expression == "exit") break;
        try {
            vector<string> parsed = parseInput(expression);
            vector<string> converted = convertExpression(parsed);
            int result = calculate(converted);
            cout << "Result: " << result << "\n\n";
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << '\n';
        }
    }
    return 0;
}
