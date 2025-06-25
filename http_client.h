#pragma once
#include <string>

std::string http_get(const std::string &host, const std::string &path, int port = 80);
