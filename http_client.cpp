#include "http_client.h"
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstring>

#ifdef _WIN32 // 包含 Windows 的 socket 编程头文件
#include <winsock2.h>
#include <ws2tcpip.h> // 为 IPv6 做准备
#pragma comment(lib, "ws2_32.lib")
#else // 包含 Unix/Linux 的 socket 编程头文件
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#endif

using namespace std;

string http_get(const string &host, const string &path, int port)
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        throw runtime_error("调用WSAStartup失败");
#endif

    struct hostent *server = gethostbyname(host.c_str()); // 基于 IPv4 的域名解析方式
    if (!server)
        throw runtime_error("无法解析主机名");

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        throw runtime_error("创建套接字失败");

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        throw runtime_error("连接失败");

    ostringstream req;
    req << "GET " << path << " HTTP/1.1\r\n";
    req << "Host: " << host << "\r\n";
    req << "Connection: close\r\n\r\n";

    string request = req.str();
    send(sock, request.c_str(), static_cast<int>(request.size()), 0);

    char buffer[4096];
    ostringstream response;
    int bytes;
    while ((bytes = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytes] = '\0';
        response << buffer;
    }

#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif

    return response.str();
}