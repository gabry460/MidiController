#ifndef UICONTROLLER_HPP
#define UICONTROLLER_HPP

#include <glfw3.h>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <MIDIDevice.hpp>
#include <windows.h>

/*
    creo la classe UIController
*/
class UIController{
    private:
        GLFWwindow* window = nullptr;
        std::thread t0;
        UINT id = 0;
        std::atomic<bool> running = false;
        std::unordered_map<UINT, MIDI> deviceMap;
    public:
        int Init();
        int createWindow(int width, int height, const char *title);
        void start();
        GLFWwindow* getWindow(){return this->window;}
        void setWindow(GLFWwindow* window){this->window = window;}
        ~UIController();

};


#endif