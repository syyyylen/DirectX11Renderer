#include "Log.h"

Log::Log()
{
}

Log::~Log()
{
}

void Log::LogMsg(std::string msg)
{
   std::cout << msg << std::endl;

   m_messages.push_back(msg);
}

void Log::LogAllMessages()
{
   std::cout << "-------- Log Messages History : -----------" << std::endl;
   std:: cout << std::endl;
   
   for(auto msg : m_messages)
   {
      std::cout << msg << "\n";
   }

   std::cout << "-------------------------------------" << std::endl;
}

Log* Log::Get()
{
   static Log log;
   return &log;
}
