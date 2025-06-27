#include "rpn_evaluator.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <cmath> // 如果你rpn_evaluator.cpp用到了cmath，最好也包含

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

using namespace std;

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
            return nan("");
        }
    }
}
