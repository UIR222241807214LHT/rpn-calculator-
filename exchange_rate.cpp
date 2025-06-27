#include "exchange_rate.h"
#include "http_client.h"
#include <string>
#include <iostream>
#include <regex>
#include <stdexcept> // 保留，用于 runtime_error

using namespace std;

// 简单从 JSON 响应中提取汇率
double parse_rate_from_response(const string &response)
{
    regex reg(R"("CNY"\s*:\s*([0-9.]+))"); // 从字符串 response 中提取 "CNY": 数字 这样的汇率信息。
    smatch match;
    if (regex_search(response, match, reg) && match.size() > 1)
    {
        return stod(match[1].str());
    }
    throw runtime_error("从网页提取汇率失败");
}

double get_usd_to_cny_rate()
{
    const string host = "open.er-api.com";
    const string path = "/v6/latest/USD";

    try
    {
        string response = http_get(host, path, 80);
        return parse_rate_from_response(response);
    }
    catch (const exception &e)
    {
        cerr << "错误：获取美元兑人民币汇率失败" << e.what() << endl;
        throw;
    }
}