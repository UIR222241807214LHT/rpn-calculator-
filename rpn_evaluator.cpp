// rpn_evaluator.cpp
#include "rpn_evaluator.h"
#include "exchange_rate.h"
#include <stack>
#include <stdexcept>
#include <cstdlib>
#include <cmath> // 加入数学函数支持：sin, cos, tan, log ,pi ,e

// 声明常数
const double PI = 3.141592653589793;
const double E = 2.718281828459045;

// 计算逆波兰表达式的值
double evaluate_rpn(const std::vector<std::string> &tokens)
{
    std::stack<double> st;

    for (const auto &token : tokens)
    {

        if (token == "+" || token == "-" || token == "*" || token == "/")
        {
            // 检查栈中是否有足够的操作数
            if (st.size() < 2)
                throw std::runtime_error("Insufficient operands"); // 操作数不足

            double b = st.top();
            st.pop(); // 第二个操作数
            double a = st.top();
            st.pop(); // 第一个操作数
            double res = 0;

            // 执行相应的运算
            if (token == "+")
                res = a + b;
            else if (token == "-")
                res = a - b;
            else if (token == "*")
                res = a * b;
            else if (token == "/")
            {
                if (b == 0)
                    throw std::runtime_error("Division by zero"); // 除以0错误
                res = a / b;
            }

            // 将结果压回栈中
            st.push(res);
        }
        else if (token == "sin" || token == "cos" || token == "tan" || token == "log")
        {
            if (st.empty())
                throw std::runtime_error("Stack is empty, cannot perform unary operator: " + token); // 栈为空，无法执行单目运算符

            double a = st.top();
            st.pop();
            double res = 0;

            if (token == "sin")
                res = std::sin(a);
            else if (token == "cos")
                res = std::cos(a);
            else if (token == "tan")
                res = std::tan(a);
            else if (token == "log")
            {
                if (a <= 0)
                    throw std::runtime_error("log operation requires a positive number"); // log 运算要求正数
                res = std::log(a);
            }

            st.push(res);
        }
        if (token == "$")
        {
            if (st.empty())
                throw std::runtime_error("Stack is empty, cannot perform '$' operator");
            double usd_amount = st.top();
            st.pop();

            double rate = 0.0;
            try
            {
                rate = get_usd_to_cny_rate();
            }
            catch (const std::exception &e)
            {
                throw std::runtime_error(std::string("Failed to get exchange rate: ") + e.what());
            }

            double cny_amount = usd_amount * rate;
            st.push(cny_amount);
        }
        else
        {
            if (token == "pi")
            {
                st.push(PI);
            }
            else if (token == "e")
            {
                st.push(E);
            }
            else
            {
                // 将字符串转换为数字并压栈
                try
                {
                    double val = std::stod(token);
                    st.push(val);
                }
                catch (...)
                {
                    throw std::runtime_error("Invalid number: " + token); // 非法数字
                }
            }
        }
    }

    // 最后栈中应该只剩一个结果
    if (st.size() != 1)
        throw std::runtime_error("Invalid RPN expression format"); // 表达式格式错误

    return st.top();
}
