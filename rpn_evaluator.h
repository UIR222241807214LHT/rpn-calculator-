// rpn_evaluator.h
#pragma once
#include <string>
#include <vector>

double evaluate_rpn(const std::vector<std::string> &tokens);
//添加常数pi和e（用 extern，只声明不定义）
extern const double PI;
extern const double E;
