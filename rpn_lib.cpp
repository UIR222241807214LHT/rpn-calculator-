#include "rpn_evaluator.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

using namespace std;

static string g_last_error = "";

extern "C"
{
    // 这个函数提供给Python调用，传入C风格字符串数组和长度
    // 返回计算结果double，遇异常返回NAN
    DLL_EXPORT double evaluate_rpn_c(const char **tokens, int size)
    {
        try
        {
            vector<string> vec_tokens;
            for (int i = 0; i < size; ++i)
            {
                vec_tokens.emplace_back(tokens[i]);
            }

            double result = evaluate_rpn(vec_tokens);
            return result;
        }
        catch (const exception &e)
        {
            // 遇到异常返回NaN，Python端调用时判断
            g_last_error = e.what(); // 这里保存错误消息
            return nan("");
        }
    }
    // 获取最后一次错误信息，返回一个 const char*，Python 端读取后再 decode
    DLL_EXPORT const char *get_last_error()
    {
        return g_last_error.c_str();
    }
    // 传入中缀表达式字符串，计算结果
    DLL_EXPORT double evaluate_infix_expr_c(const char *expr)
    {
        try
        {
            string s(expr);
            vector<string> rpn = infix_to_rpn(s);
            double result = evaluate_rpn(rpn);
            return result;
        }
        catch (const exception &e)
        {
            g_last_error = e.what();
            return nan("");
        }
    }
}