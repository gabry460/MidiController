#include "MIDIDevice.hpp"
#include "PluginController.hpp"
#include <thread>
#include <windows.h>

/*
    creo una funzione che apre un dispositivo MIDI
*/
void MIDI::open()
{
    MMRESULT result = midiInOpen(this->getMidiHandle(), this->getId(), (DWORD_PTR)MIDI::MidiInProc, (DWORD_PTR)this, CALLBACK_FUNCTION);
    if(result == MMSYSERR_NOERROR)
    {
        std::cout << "[+] dispositivo aperto correttamente!" << std::endl;
        this->Log("open Midi device " + std::string((char*)this->getMidiStructure().szPname));
    }else
    {
        std::cerr << "[-] errore nell'apertura del dispositivo MIDI " << result << std::endl;
        this->LogError("open Midi device", result);
    }
    if(result != MMSYSERR_NOERROR) {
        std::cerr << "[-] Errore in midiInOpen: codice " << result << std::endl;
        this->midiHandle = nullptr; // Protezione extra
    }
}

/*
    creo la funzione callback che verrà eseguita ogni volta che ci sarà un input dal dispositivo MIDI
*/
void CALLBACK MIDI::MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    MIDI* pThis = reinterpret_cast<MIDI*>(dwInstance);
    uint8_t state = dwParam1;
    uint8_t control = dwParam1 >> 8;
    uint8_t data = dwParam1 >> 16;
    std::cout << (char*)pThis->getMidiStructure().szPname << " C: "<< (int)control << " D: " << (int)data << " S: " << (int)state << std::endl;
    if(pThis->getMap().find(control) == pThis->getMap().end() && (int)control != 0)
    {
        std::cout << "Inserisco nuovo control: " << (int)control << std::endl;
        pThis->getMap().insert_or_assign((int)control, nullptr);
    }
    switch(control)
    {
        case 71:
            for(auto& f : PluginController::getNamesMap())
            {
                if (f.second)
                {
                    f.second((int)control, (int)data, (int)state);
                } 
            }
            break;
    }

}

/*
    creo una funzione che setta la struttura del dispositivo MIDI
    UINT id -> l'id del dispositivo MIDI
    MIDIINCAPS midiStructure -> la struttura da riempire
*/
void MIDI::setStructure(UINT id, MIDIINCAPS &midiStructure)
{
    midiInGetDevCaps(this->getId(), &midiStructure, sizeof(midiStructure));
    this->midiStructure = midiStructure;
}

/*
    creo una funzione che inizia ad acquisire i dati dal dispositivo MIDI
*/
void MIDI::Start()
{   
    HMIDIIN* handle = this->getMidiHandle();
    MMRESULT res = midiInStart(*this->getMidiHandle());
    if(res == MMSYSERR_INVALHANDLE)
    {
        std::cerr << "[-] Invalid Handle" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(4));
        exit(EXIT_FAILURE);
    }
}

/*
    creo una funzione che chiude l'handle MIDI
*/
void MIDI::MidiClose()
{
    midiInClose(*this->getMidiHandle());
}