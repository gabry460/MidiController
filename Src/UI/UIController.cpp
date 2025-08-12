#include <iostream>
#include <glfw3.h>
#include <windows.h>
#include <thread>
#include <atomic>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "imgui.h"
#include "json.hpp"
#include "PluginController.hpp"
#include <unordered_map>
#include "UIController.hpp"
#include "LogController.hpp"

std::thread t1;
std::atomic<bool> threadRunning = true;
std::string current_item_name = "Nessuna";
int i = 0;

/*
    creo una funzione che inizializza l'UI
*/
int UIController::Init()
{
    if (!glfwInit())
    {
        return -1;
    }
    // Configura OpenGL 3.3 core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromFileTTF("./../../font/arial.ttf", 20.0f);
    ImGui::StyleColorsClassic();
    ImVec4* colors = ImGui::GetStyle().Colors;
    ImVec4 titleColor = ImVec4(0.0862745098039216f, 0.0941176470588235f, 0.1098039215686275f, 1.00f);

    colors[ImGuiCol_TitleBg]         = titleColor; 
    colors[ImGuiCol_TitleBgActive]   = titleColor; 
    colors[ImGuiCol_WindowBg]        = ImVec4(0.0862745098039216f, 0.0941176470588235f, 0.1098039215686275f, 1.00f); 
    colors[ImGuiCol_Button]          = ImVec4(0.086f, 0.098f, 0.110f, 1.0f);
    colors[ImGuiCol_ButtonHovered]   = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive]    = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);

    return 0;
}

/*
    creo una funzione che crea la finestra di glfw
    int width -> larghezza della finetra
    int height -> altezza della finestra
    const char* title -> il titolo della finestra
*/
int UIController::createWindow(int width, int height, const char *title)
{
    this->setWindow(glfwCreateWindow(width, height, title, nullptr, nullptr));
    glfwMakeContextCurrent(this->getWindow());
    glfwSetWindowPos(this->getWindow(), 300,200);
    // Inizializza backend
    ImGui_ImplGlfw_InitForOpenGL(this->getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    return 0;
}

/*
    creo una funzione che fa partire la visualizzazione della finestra di imgui
*/
void UIController::start()
{
    bool connect = false;
    PluginController::Init();
    LogController::Init();
    std::atomic<UINT> devCount = 0;
    
    t1 = std::thread([&devCount]()->void{
        while(threadRunning)
        {
            UINT count = 0;
            if(midiInGetNumDevs() > 0)
            {  
                for(unsigned int i = 0; i < midiInGetNumDevs(); i++)
                {
                    MIDIINCAPS caps;
                    if (midiInGetDevCaps(i, &caps, sizeof(caps)) == MMSYSERR_NOERROR) {
                        count++;
                    }
                }
            }
            devCount = count;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
    
    // ciclo principale della finestra
    while (!glfwWindowShouldClose(window)) {
        glfwWaitEventsTimeout(0.01);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        UINT currentDevCount = devCount.load();
        if(currentDevCount > 0)
        {   
            //dev.setMap(this->FuncitionMap);
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(450, 350));
            float pad = 4.0f;
            float button_size = 20.0f;
            ImGui::Begin("Gestione MIDI", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);
            ImGui::Text("Midi Controller");
            ImGui::SameLine(390);
            ImGui::SetCursorPosY(4);
            if(ImGui::Button("-", ImVec2(20,0)))
            {
                glfwIconifyWindow(this->getWindow());
            }
            if(glfwGetWindowAttrib(this->getWindow(), GLFW_ICONIFIED))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(150));
            }
            ImGui::SameLine(420);
            ImGui::SetCursorPosY(4);
            if(ImGui::Button("x", ImVec2(20,0)))
            {
                glfwSetWindowShouldClose(this->getWindow(), GLFW_TRUE);
            }
            // Colonna sinistra: lista dispositivi
            ImGui::BeginChild("left_panel", ImVec2(180, 300), true);
            static std::atomic<int> selected_device = 0; // 0 = MIDI Keyboard, 1 = Launchpad, 2 = MIDI Mixer
            MIDIINCAPS caps;
            
            for (int i = 0; i < currentDevCount; i++)
            {
                midiInGetDevCaps(i, &caps, sizeof(caps));
                if (ImGui::Selectable((const char *)caps.szPname, selected_device == i) && caps.szPname != NULL)
                {
                        MIDI& dev = deviceMap[i];
                        selected_device = i;
                }
            }
            ImGui::EndChild();

            ImGui::SameLine();

            // Colonna destra: dettagli dispositivo
            ImGui::BeginChild("right_panel", ImVec2(253, 100), true);
            ImGui::Text("Dispositivo: %s", (const char *)caps.szPname);

            if(connect)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
                ImGui::Text("Connesso");
                ImGui::PopStyleColor();
            }else{
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                ImGui::Text("Disconnesso");
                ImGui::PopStyleColor();
            }

            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(38, 42, 47, 255));
            const char *str = (connect == true) ? "Disconnetti" : "connetti";
            // Bottone disconnessione
            if (ImGui::Button(str, ImVec2(-1, 0)))
            {
                if(connect == true)
                {
                    running = false;
                    if (t0.joinable()) t0.join();
                    deviceMap[0].MidiClose();
                    std::cout << "dispositivo disconnesso" << std::endl;
                    connect = false;
                }else{
                    deviceMap[selected_device].setId(selected_device);
                    MIDIINCAPS str = deviceMap[selected_device].getMidiStructure();
                    deviceMap[selected_device].setStructure(deviceMap[selected_device].getId(), deviceMap[selected_device].getMidiStructure());
                    deviceMap[selected_device].open();
                    running = true;
                    std::cout << "dispositivo connesso" << std::endl;
                    this->t0 = std::thread([&deviceMap = deviceMap, this]() {
                        while (running) {
                            deviceMap[selected_device].Start();
                            std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        }
                    });
                    connect = true;
                }
            }
            ImGui::PopStyleColor();
            
            ImGui::Spacing();
            ImGui::EndChild();
            ImGui::SetCursorPos(ImVec2(196,137));
            ImGui::Text("Comandi disponibili:");
            ImGui::SetCursorPos(ImVec2(196,160));
            ImGui::BeginChild("functionPanel", ImVec2(252,174), true, ImGuiWindowFlags_NoScrollbar);

            static int current_item = 0;
            std::string testo;
            int posText = 10;
            int posCombo = 7;
            for(auto& [Devicekey, Devicevalue] : deviceMap[selected_device].getMap())
            {
                testo = std::to_string(Devicekey) + ":";
                ImGui::SetCursorPosY(posText);
                ImGui::Text(testo.c_str());
                ImGui::SameLine(80);
                ImGui::SetNextItemWidth(160);
                ImGui::SetCursorPosY(posCombo);
                std::string c = std::string("##command ") + std::to_string(i);
                if (ImGui::BeginCombo(c.c_str(), current_item_name.c_str(), ImGuiComboFlags_NoArrowButton))
                {
                    int idx = 0;
                    for (auto& [Pluginkey, Pluginvalue] : PluginController::getNamesMap()) {
                        bool is_selected = (current_item == idx);
                        if (ImGui::Selectable(Pluginkey.c_str(), is_selected)) {
                            current_item = idx;
                            current_item_name = Pluginkey; // salva anche la stringa
                        }
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                        idx++;
                    }
                    ImGui::EndCombo();
                }
                i++;
                posText += 30;
                posCombo += 30;
            }

            ImGui::EndChild();
            ImGui::End();          
        }
        
            
        // renderizzo la finestra
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0862745098039216f, 0.0980392f, 0.11372549f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
    }
}


/*
    creo il distruttore della classe
*/
UIController::~UIController()
{
    threadRunning = false;
    if(t1.joinable())
    {
        t1.join();
    }
    if(t0.joinable())
    {
        this->running = false;
        t0.join();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(this->getWindow());
    glfwTerminate();
}