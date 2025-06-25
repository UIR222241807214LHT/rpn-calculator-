// main.cpp

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

// 声明 RPN 计算函数
double evaluate_rpn(const std::vector<std::string>& tokens);

int main() {
    // 提示用户输入逆波兰表达式
    std::cout << "Welcome to Reverse Polish Notation Calculator (+ - * / supported)" << std::endl;
    std::cout << "Please enter an RPN expression (e.g. 3 4 + 2 *):" << std::endl;

    std::string input;
    std::getline(std::cin, input);  // 读取一整行输入

    // 分割输入字符串为 tokens
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    try {
        double result = evaluate_rpn(tokens);
        std::cout << "Result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
