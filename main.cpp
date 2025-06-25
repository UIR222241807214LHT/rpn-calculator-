#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "rpn_evaluator.h"

int main()
{
    std::cout << "Welcome to Reverse Polish Notation Calculator (+ - * / supported)" << std::endl;
    std::cout << "Enter 'exit' to quit the program." << std::endl;

    while (true)
    {
        std::cout << "Please enter an RPN expression (e.g. 3 4 + 2 *):" << std::endl;
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit")
        {
            std::cout << "Exiting calculator. Goodbye!" << std::endl;
            break; // 跳出循环，程序结束
        }

        // 分割输入字符串为 tokens
        std::istringstream iss(input);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token)
        {
            tokens.push_back(token);
        }

        try
        {
            double result = evaluate_rpn(tokens);
            std::cout << "Result: " << result << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}
