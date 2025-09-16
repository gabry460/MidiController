#include <filesystem>
#include "JSONController.hpp"

std::filesystem::path JSONController::jsonPath = "../../Plugins/Plugins.json";
std::vector<std::string> functionArray = {};
nlohmann::json JSONController::json;

/*
    creo una funzione che inizializza il file di Json
*/
void JSONController::Init()
{
    static std::ifstream jsonFilestream(jsonPath);

    if (!std::filesystem::exists(jsonPath))
    {
        std::cout << "\n[+] File JSON creato" << std::endl;
    }

    jsonFilestream >> JSONController::getJson(); // carico il contenuto del file in memoria
}

/*
    creo una funzione che scrive nel file i plugins di default
    std::ofstream& jsonFile -> il file Json
*/
void JSONController::writeDefaults(std::ofstream &jsonFile)
{
    JSONController::json["Volume.dll"] = {"Volume", "StopAudio", "NextTrack", "PrevTrack", "VolumeUp", "VolumeDown", "Mute", "Zoom"};
    jsonFile << JSONController::getJson().dump(4);
}