@echo off
echo 正在编译 rpn_lib.dll...

:: 使用 g++ 编译多个源文件，生成 DLL，链接 WinSock 库，支持 C++17 标准
g++ -std=c++17 -shared -o rpn_lib.dll ^
    rpn_lib.cpp ^
    rpn_evaluator.cpp ^
    exchange_rate.cpp ^
    http_client.cpp ^
    -I. -static-libgcc -static-libstdc++ -lws2_32

if %errorlevel% neq 0 (
    echo 编译失败，请检查错误信息！
) else (
    echo 编译成功，已生成 rpn_lib.dll
)
pause
