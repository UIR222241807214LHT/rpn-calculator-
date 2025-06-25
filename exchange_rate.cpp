#include "exchange_rate.h"
#include "http_client.h"
#include <string>
#include <iostream>
#include <regex>
#include <stdexcept> // 保留，用于 std::runtime_error

// 简单从 JSON 响应中提取汇率
double parse_rate_from_response(const std::string &response)
{
    std::regex reg(R"("CNY"\s*:\s*([0-9.]+))");
    std::smatch match;
    if (std::regex_search(response, match, reg) && match.size() > 1)
    {
        return std::stod(match[1].str());
    }
    throw std::runtime_error("Failed to parse rate from response");
}

double get_usd_to_cny_rate()
{
    const std::string host = "open.er-api.com";
    const std::string path = "/v6/latest/USD";

    try
    {
        std::string response = http_get(host, path, 80);
        return parse_rate_from_response(response);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: Failed to get USD to CNY rate: " << e.what() << std::endl;
        throw;
    }
}