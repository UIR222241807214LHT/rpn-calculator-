@echo off
echo ���ڱ��� rpn_lib.dll...

:: ʹ�� g++ ������Դ�ļ������� DLL������ WinSock �⣬֧�� C++17 ��׼
g++ -std=c++17 -shared -o rpn_lib.dll ^
    rpn_lib.cpp ^
    rpn_evaluator.cpp ^
    exchange_rate.cpp ^
    http_client.cpp ^
    -I. -static-libgcc -static-libstdc++ -lws2_32

if %errorlevel% neq 0 (
    echo ����ʧ�ܣ����������Ϣ��
) else (
    echo ����ɹ��������� rpn_lib.dll
)
pause
