#ifndef PLUGINCONTROLLER_H
#define PLUGINCONTROLLER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include "JSONController.hpp"
#include "compileMode.hpp"

/*
    creo la classe PluginsController che è eredita la classe JSONController
*/
class PluginController : public JSONController
{
private:
    static std::unordered_map<std::string, void (*)(int, int, int)> NamesMap;
    static std::vector<std::string> filesName;

#ifdef RELEASE
    static inline const std::filesystem::path pluginsPath = "./Plugins/";
#else
    static inline const std::filesystem::path pluginsPath = "./../../Plugins/";
#endif

public:
    static void Init();
    static const std::filesystem::path &getPluginPath() { return pluginsPath; }
    static std::vector<std::string> &getFileNames() { return filesName; }
    static std::unordered_map<std::string, void (*)(int, int, int)> &getNamesMap() { return NamesMap; }
};

#endif