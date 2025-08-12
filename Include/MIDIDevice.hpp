#ifndef MIDICLASS_H
#define MIDICLASS_h

#include <windows.h>
#include <iostream>
#include <unordered_map>
#include "LogController.hpp"

/*
    creo la classe MIDI che eredita la classe LogControlelr
*/
class MIDI : public LogController{
    private:
        HMIDIIN midiHandle = {};
        MIDIINCAPS midiStructure = {};
        UINT id;
        std::unordered_map<int, void(*)(int, int, int)> FuncitionMap;
    public:
        MIDI() : LogController(){};
        UINT getId() const {return this->id;}
        void setId(UINT id) {this->id = id;}
        HMIDIIN* getMidiHandle() {return &midiHandle;}
        MIDIINCAPS& getMidiStructure() {return midiStructure;}
        void MidiClose();
        void setStructure(UINT id, MIDIINCAPS &midiStructure);
        void open();
        std::unordered_map<int,void(*)(int,int,int)> &getMap(){return this->FuncitionMap;}
        //void setMap(std::unordered_map<int,void(*)(int,int,int)> &map){this->map = map;}
        void Start();
        static void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
};


#endif