#include <iostream>
#include <ctime>
#include <cerrno>
#include <cstring>
#include "LogController.hpp"

std::ofstream LogController::stream;

/*
    creo una funzione che inizializza il LogController
*/
void LogController::Init() {
    stream.open("log.txt", std::ios::app);
    if (!stream.is_open()) {
        std::cerr << "[-] errore nell'apertura del file" << std::endl;
    }
}

/*
    creo una funzione che chiude il file di log
*/
void LogController::Close() {
    std::cout << "[+] sto chiudendo il file di log" << std::endl;
    stream.close();
}

/*
    creo una funzione che logga un messaggio
    std::string message -> il messaggio da inserire nel file
*/
void LogController::Log(std::string message) {
    if (!stream.is_open()) return;

    time_t timestamp;
    time(&timestamp);
    std::string timeStr = ctime(&timestamp);
    timeStr.pop_back();
    stream << "[INFO] " << timeStr << ": " << message << "\n";
    stream.flush();
}

/*
    creo una funzione che logga un messaggio di errore
    std::string message -> il messaggio da inserire nel file 
    int errorCode -> il codice dell'errore
*/
void LogController::LogError(std::string message, int errorCode) {
    if (!stream.is_open()) return;

    time_t timestamp;
    time(&timestamp);
    std::string timeStr = ctime(&timestamp);
    timeStr.pop_back();
    stream << "[ERROR] " << timeStr << ": " << message << " code: " << errorCode << "\n";
    stream.flush();
}

/*
    creo una funzione che pulisce il file di log
*/
void LogController::clear() {
    if (stream.is_open()) stream.close();
    if (std::remove("log.txt") == 0) {
        std::cout << "file eliminato correttamente" << std::endl;
    } else {
        std::cerr << "errore nell'eliminazione del file " << std::strerror(errno) << std::endl;
    }
}