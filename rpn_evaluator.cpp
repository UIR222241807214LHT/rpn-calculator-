// rpn_evaluator.cpp

#include "rpn_evaluator.h"
#include <stack>
#include <stdexcept>
#include <cstdlib>

// 计算逆波兰表达式的值
double evaluate_rpn(const std::vector<std::string>& tokens) {
    std::stack<double> st;

    for (const auto& token : tokens) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            // 检查栈中是否有足够的操作数
            if (st.size() < 2) throw std::runtime_error("Insufficient operands");//操作数不足

            double b = st.top(); st.pop();  // 第二个操作数
            double a = st.top(); st.pop();  // 第一个操作数
            double res = 0;

            // 执行相应的运算
            if (token == "+") res = a + b;
            else if (token == "-") res = a - b;
            else if (token == "*") res = a * b;
            else if (token == "/") {
                if (b == 0) throw std::runtime_error("Division by zero");//除以0错误
                res = a / b;
            }

            // 将结果压回栈中
            st.push(res);
        } else {
            // 将字符串转换为数字并压栈
            try {
                double val = std::stod(token);
                st.push(val);
            } catch (...) {
                throw std::runtime_error("Invalid number: " + token);//非法数字
            }
        }
    }

    // 最后栈中应该只剩一个结果
    if (st.size() != 1) throw std::runtime_error("Invalid RPN expression format");//表达式格式错误

    return st.top();
}
