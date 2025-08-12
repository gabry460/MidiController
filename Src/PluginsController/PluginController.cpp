#include <iostream>
#include <filesystem>
#include <vector>
#include <windows.h>
#include "JSONController.hpp"
#include "PluginController.hpp"

std::filesystem::path PluginController::pluginsPath = "./../../Plugins/";
std::unordered_map<std::string, void (*)(int, int, int)> PluginController::NamesMap = {};
std::vector<void (*)(int, int, int)> PluginController::functionArray = {};
std::vector<std::string> PluginController::filesName = {};
static void LoadPlugins(std::vector<std::string> filesNames, std::unordered_map<std::string, void (*)(int, int, int)> &functionArray);
/*
    creo una funzione che inizializza i plugin
*/
void PluginController::Init()
{
    std::cout << "[+] sto inizializzando i plugin.." << std::endl;
    int counter = 0;
    try {
        // cerco tutti i file dei plugins (escluso Plugins.json) 
        for (auto const& entry : std::filesystem::directory_iterator(pluginsPath)) {
            if (entry.is_regular_file() && entry.path().filename() != "Plugins.json") {
                counter++;
            }
        }
        if(counter == 0)
        {
            std::cout << "[-] nessun plugin trovato all'interno della directory ./Plugins.. Installa dei plugins e riprova" << std::endl;
            exit(0);
        }
        else{
            std::cout << "[+] " << counter << " plugins trovati:" << std::endl;
            // ogni nome del file lo inserisco in un vettore
            for (auto const& entry : std::filesystem::directory_iterator(pluginsPath)) 
            {
                if (entry.is_regular_file() && entry.path().filename() != "Plugins.json") {
                    filesName.push_back(entry.path().filename().string());
                }
            }
            // stampo tutto il vettore
            for(int i = 0; i < filesName.size(); i++)
            {
                std::cout << "[->]  " << filesName[i] << std::endl;
            }
            LoadPlugins(PluginController::filesName, PluginController::getNamesMap());
            PluginController::getNamesMap().insert_or_assign("Nessuna", nullptr);
        }
    } catch (std::filesystem::filesystem_error const& ex) {
        std::cerr << "[-] Errore: " << ex.what() << "\n";
        return;
    }

}

/*
    creo una funzione che serve a caricare i plugin
    std::vector<std::string> filesNames -> un vettore di stringhe che contiene i nomi dei file
    std::vector<void (*)(int, int, int)> &aunctionArray -> una referenza ad un vettore di puntatori a funzione che conterrà tutte le funzioni dei plugins
*/
static void LoadPlugins(std::vector<std::string> filesNames, std::unordered_map<std::string, void (*)(int, int, int)> &NameArray)
{
    std::cout << "[+] sto caricando i plugin..." << std::endl;
    // inizializzo il file json
    JSONController::Init();
    // per ogni file carico la rispettiva funzione e la inserisco in functionArray
    for (auto& [key, value] : JSONController::j.items()) {
        std::string c = "../../Plugins/" + key;
        HMODULE lib = LoadLibraryA(c.c_str());
        for (const auto& func : value) {
            void (*Func)(int, int, int) = (void (*)(int, int, int))GetProcAddress(lib, func.get<std::string>().c_str());
            if (Func) {
                NameArray.insert_or_assign(func.get<std::string>(), Func);
            } else {
                std::cerr << "[-] Funzione non trovata: " << func.get<std::string>().c_str() << std::endl;
            }
        }
    }
    std::cout << "[+] plugin caricati correttamente" << std::endl;

}