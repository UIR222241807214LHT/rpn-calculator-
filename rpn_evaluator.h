#pragma once
#include <string>
#include <vector>

double evaluate_rpn(const std::vector<std::string> &tokens);
// 添加常数pi和e（用extern只声明不定义）
// extern告诉编译器：“这两个变量是在别的地方定义的，这里只是声明，让别的文件知道它们存在。”
// 为了防止在多个源文件中重复定义这两个变量，导致链接错误。
extern const double PI;
extern const double E;