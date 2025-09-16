#ifndef JSONCONTROLLER_H
#define JSONCONTROLLER_H

#include <iostream>
#include <fstream>
#include "json.hpp"

/*
    creo la classe JSONController
*/
class JSONController
{
private:
    static std::filesystem::path jsonPath;
    static nlohmann::json json;

public:
    static nlohmann::json &getJson() { return json; }
    static void Init();
    static void writeDefaults(std::ofstream &jsonFile);
};

#endif