#pragma once
#include <string>

// 发起 HTTP GET 请求，返回响应体
std::string http_get(const std::string &host, const std::string &path, int port = 80);