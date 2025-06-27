#include "rpn_evaluator.h"
#include "exchange_rate.h"

#include <cmath> // 加入数学函数支持：sin, cos, tan, log
#include <cstdlib>
#include <stack>
#include <stdexcept>

using namespace std;

// 声明常数
const double PI = 3.141592653589793;
const double E = 2.718281828459045;

// 计算逆波兰表达式的值
double evaluate_rpn(const vector<string> &tokens)
{
    stack<double> st;

    for (const auto &token : tokens) // 对tokens里的每个token元素按顺序读取且不修改
    {

        if (token == "+" || token == "-" || token == "*" || token == "/")
        {
            // 检查栈中是否有足够的操作数
            if (st.size() < 2)
                throw runtime_error("操作数不足");

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
                if (b == 0) // 除以0错误
                    throw runtime_error("不能除以0");
                res = a / b;
            }

            // 将结果压栈
            st.push(res);
        }
        else if (token == "sin" || token == "cos" || token == "tan" || token == "log")
        { // 添加的运算符部分
            if (st.empty())
                throw runtime_error("栈为空，无法执行以下运算：" + token); // 栈为空，无法执行单目运算符

            double a = st.top();
            st.pop();
            double res = 0;

            if (token == "sin")
                res = sin(a);
            else if (token == "cos")
                res = cos(a);
            else if (token == "tan")
                res = tan(a);
            else if (token == "log")
            {
                if (a <= 0)
                    throw runtime_error("log运算要求正数"); // log 运算要求正数
                res = log(a);
            }

            st.push(res);
        }
        else if (token == "$")
        { // 转换汇率部分
            if (st.empty())
                throw runtime_error("栈为空，无法执行汇率运算");
            double usd_amount = st.top();
            st.pop();

            double rate = 0.0;
            try
            {
                rate = get_usd_to_cny_rate();
            }
            catch (const exception &e)
            {
                throw runtime_error(string("查询汇率失败： ") + e.what());
            }

            double cny_amount = usd_amount * rate;
            st.push(cny_amount);
        }
        else
        { // 数字部分
            if (token == "pi")
            {
                st.push(PI);
            }
            else if (token == "e")
            {
                st.push(E);
            }
            else
            { // 将字符串转换为数字并压栈
                try
                {
                    size_t idx;
                    double val = stod(token, &idx);// string to double函数，idx 会记录成功解析的字符数
                    if (idx != token.size())
                    {
                        throw std::runtime_error("Invalid number: " + token);
                    }
                    st.push(val);
                }
                catch (...) // 万能捕获，可以捕获任意类型的异常
                {
                    throw runtime_error("无效的数字：" + token);
                }
            }
        }
    }

    // 最后栈中应该只剩一个结果
    if (st.size() != 1)
        throw std::runtime_error("无效的逆波兰式算式"); // 表达式格式错误

    return st.top();
}