#ifndef LOGCONTROLLER_H
#define LOGCONTROLLER_H

#include <fstream>

/*
    creo la classe LogController
*/
class LogController{
    private:
        static std::ofstream stream;
    public:
        static void Init();
        static void Close();
        static void Log(std::string message);
        static void LogError(std::string message, int errorCode);
        static void clear();

};


#endif