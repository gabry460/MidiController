#ifndef JSONCONTROLLER_H
#define JSONCONTROLLER_H

#include <iostream>
#include <fstream>
#include "json.hpp"

/*
    creo la classe JSONController
*/
class JSONController{
    private:
        static std::filesystem::path jsonPath;
        static nlohmann::json j;
    public:
        static nlohmann::json &getJ(){return j;}
        static void Init();
        static void writeDefaults(std::ofstream& jsonFile);

};


#endif