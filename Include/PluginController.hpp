#ifndef PLUGINCONTROLLER_H
#define PLUGINCONTROLLER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <JSONController.hpp>

/*
    creo la classe PluginsController che è eredita la classe JSONController
*/
class PluginController : public JSONController
{
    private:
        static std::vector<void (*)(int, int, int)> functionArray;
        static std::unordered_map<std::string, void (*)(int, int, int)> NamesMap;
        static std::vector<std::string> filesName;
        static std::filesystem::path pluginsPath;
    public:
        static void Init();
        static std::unordered_map<std::string, void (*)(int, int, int)>& getNamesMap() {return NamesMap;}

};

#endif