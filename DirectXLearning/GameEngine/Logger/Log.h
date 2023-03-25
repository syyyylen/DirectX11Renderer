#pragma once
#include <iostream>
#include <string>
#include <vector>

class Log
{
public:
    Log();
    ~Log();

    void LogMsg(std::string msg);

    void LogAllMessages();

public:
    static Log* Get();

private:
    std::vector<std::string> m_messages;
};

#define LOG(x) Log::Get()->LogMsg(x)
