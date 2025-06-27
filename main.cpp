#include <windows.h> // 设置UTF-8
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "rpn_evaluator.h"

using namespace std;

int main()
{
    SetConsoleOutputCP(CP_UTF8); // 将控制台代码页设置为 UTF-8
    cout << "欢迎使用逆波兰式计算器 (支持 + - * /  sin cos tan log pi e)" << endl;
    cout << "输入'exit'以退出程序" << endl;

    while (true) // 程序永远循环直到break退出
    {
        cout << "请输入一个符合逆波兰式的算式" << endl;
        string input;
        getline(cin, input); // getline(输入流,结果保存)

        if (input == "exit")
        {
            cout << "退出计算器，再见!" << endl;
            break;
        }

        // 字符串输入流类，从字符串中读取
        istringstream iss(input); // iss为变量名，用字符串input初始化它
        vector<string> tokens;    // 保存所有标识符
        string token;             // 循环保持每一个标识符
        while (iss >> token)      // 操作符">>"自动以 空格 制表符 换行符 为分隔符，从输入流中读取一个token
        {
            tokens.push_back(token); // 把当前的 token 追加到 tokens 向量（数组）的末尾，
        }
        try // try-catch
        {   // try中放可能发生错误的代码块，尝试运行
            double result = evaluate_rpn(tokens);
            cout << "结果是：" << result << endl;
        }
        catch (const exception &e)
        { // catch捕获try中throw的异常，防止程序崩溃
            cerr << "" << e.what() << std::endl;
        }
    }

    return 0;
}