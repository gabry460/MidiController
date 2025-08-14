#include <filesystem>
#include "JSONController.hpp"

std::filesystem::path JSONController::jsonPath = "../../Plugins/Plugins.json";
std::vector<std::string> functionArray = {};
nlohmann::json JSONController::j;

/*
    creo una funzione che inizializza il file di Json
*/
void JSONController::Init()
{
    static std::ofstream jsonFile(jsonPath, std::ios::trunc);

    if(!std::filesystem::exists(jsonPath))
    {
        std::cout << "\n[+] File JSON creato" << std::endl;
    }

    writeDefaults(jsonFile);
    jsonFile.close();
}

/*
    creo una funzione che scrive nel file i plugins di default
    std::ofstream& jsonFile -> il file Json
*/
void JSONController::writeDefaults(std::ofstream& jsonFile)
{
    JSONController::j["Volume.dll"] = {"Volume"};
    jsonFile << JSONController::j.dump(4);
}