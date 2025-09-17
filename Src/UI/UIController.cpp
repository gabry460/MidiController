#include <iostream>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "imgui.h"
#include "json.hpp"
#include "PluginController.hpp"
#include "UIController.hpp"
#include "LogController.hpp"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>

std::thread t1;
std::atomic<bool> threadRunning = true;
std::unordered_map<std::string, std::string> comboMap;
std::string current_item_name = "Nessuna";

/*
    creo una funzione che inizializza l'UI
*/
int UIController::Init()
{
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("./../../font/arial.ttf", 20.0f);
    ImGui::StyleColorsClassic();
    ImVec4 *colors = ImGui::GetStyle().Colors;
    ImVec4 titleColor = ImVec4(0.0862745098039216f, 0.0941176470588235f, 0.1098039215686275f, 1.00f);

    colors[ImGuiCol_TitleBg] = titleColor;
    colors[ImGuiCol_TitleBgActive] = titleColor;
    colors[ImGuiCol_WindowBg] = ImVec4(0.0862745098039216f, 0.0941176470588235f, 0.1098039215686275f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.086f, 0.098f, 0.110f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);

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
    glfwSetWindowPos(this->getWindow(), 300, 200);
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
    bool dragging = false;
    bool connect = false;
    PluginController::Init();
    LogController::Init();
    std::atomic<UINT> devCount = 0;
    double curX;
    double curY;

    t1 = std::thread([&devCount]() -> void
                     {
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
            Sleep(1000);
        } });

    int c = 0;
    // ciclo principale della finestra
    while (!glfwWindowShouldClose(window))
    {
        glfwWaitEventsTimeout(0.01);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // logica per il riposizionamento della finestra

        glfwGetCursorPos(this->window, &curX, &curY);
        if (!glfwGetWindowAttrib(this->getWindow(), GLFW_ICONIFIED) && curX <= 385 && curY <= 40 && glfwGetMouseButton(this->getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            HWND hwnd = glfwGetWin32Window(window);
            ReleaseCapture();
            SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        }

        UINT currentDevCount = devCount.load();
        int state = glfwGetMouseButton(this->getWindow(), GLFW_MOUSE_BUTTON_LEFT);

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(450, 350));
        float pad = 4.0f;
        float button_size = 20.0f;
        ImGui::Begin("Gestione MIDI", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);

        ImGui::Text("Midi Controller");
        ImGui::SameLine(388);
        ImGui::SetCursorPosY(4);
        if (ImGui::Button("-", ImVec2(30, 30)))
        {
            glfwIconifyWindow(this->getWindow());
        }
        if (glfwGetWindowAttrib(this->getWindow(), GLFW_ICONIFIED))
        {
            Sleep(150);
        }
        ImGui::SameLine(420);
        ImGui::SetCursorPosY(4);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(255, 0, 0, 255));
        if (ImGui::Button("x", ImVec2(30, 30)))
        {
            glfwSetWindowShouldClose(this->getWindow(), GLFW_TRUE);
        }
        ImGui::PopStyleColor();
        // Colonna sinistra: lista dispositivi
        ImGui::BeginChild("left_panel", ImVec2(180, 300), true);
        MIDIINCAPS caps;
        static std::atomic<int> selected_device = 0; // 0 = MIDI Keyboard, 1 = Launchpad, 2 = MIDI Mixer
        if (currentDevCount > 0)
        {
            for (int i = 0; i < currentDevCount; i++)
            {
                midiInGetDevCaps(i, &caps, sizeof(caps));
                if (ImGui::Selectable((const char *)caps.szPname, selected_device == i) && caps.szPname != NULL)
                {
                    MIDI &dev = deviceMap[i];
                    selected_device = i;
                }
            }
        }
        else
        {
            ImGui::SetCursorPosY(10);
            ImGui::Text("No Device Found");
        }
        ImGui::EndChild();
        ImGui::SameLine();

        // Colonna destra: dettagli dispositivo
        ImGui::BeginChild("right_panel", ImVec2(253, 100), true);
        char *s = (currentDevCount > 0) ? (char *)caps.szPname : (char *)"No Device";
        ImGui::Text("Dispositivo: %s", s);

        if (connect && currentDevCount > 0)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
            ImGui::Text("Connesso");
            ImGui::PopStyleColor();
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            ImGui::Text("Disconnesso");
            ImGui::PopStyleColor();
        }

        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(38, 42, 47, 255));
        if (devCount > 0)
        {

            const char *str = (connect == true) ? "Disconnetti" : "connetti";
            // Bottone disconnessione
            if (ImGui::Button(str, ImVec2(-1, 0)))
            {
                if (connect == true)
                {
                    running = false;
                    if (t0.joinable())
                        t0.join();
                    deviceMap[0].MidiClose();
                    std::cout << "dispositivo disconnesso" << std::endl;
                    connect = false;
                }
                else if (connect == false && currentDevCount > 0)
                {
                    deviceMap[selected_device].setId(selected_device);
                    MIDIINCAPS str = deviceMap[selected_device].getMidiStructure();
                    deviceMap[selected_device].setStructure(deviceMap[selected_device].getId(), deviceMap[selected_device].getMidiStructure());
                    deviceMap[selected_device].open();
                    running = true;
                    std::cout << "dispositivo connesso" << std::endl;
                    this->t0 = std::thread([&deviceMap = deviceMap, this]()
                                           {
                        while (running) {
                            deviceMap[selected_device].Start();
                            Sleep(10);
                        } });
                    connect = true;
                }
            }
        }
        ImGui::PopStyleColor();

        ImGui::Spacing();
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(196, 137));
        ImGui::Text("Comandi disponibili:");
        ImGui::SetCursorPos(ImVec2(196, 160));
        ImGui::BeginChild("functionPanel", ImVec2(252, 174), true, ImGuiWindowFlags_NoScrollbar);

        static int current_item = 0;
        std::string testo;
        int posText = 10;
        int posCombo = 7;
        for (auto &[Devicekey, Devicevalue] : deviceMap[selected_device].getMap())
        {
            testo = std::to_string(Devicekey) + ":";
            ImGui::SetCursorPosY(posText);
            ImGui::Text(testo.c_str());
            ImGui::SameLine(80);
            ImGui::SetNextItemWidth(160);
            ImGui::SetCursorPosY(posCombo);
            std::string c = std::string("##command ") + std::to_string(Devicekey);
            comboMap.try_emplace(c, "Nessuna");
            if (ImGui::BeginCombo(c.c_str(), comboMap.at(c).c_str(), ImGuiComboFlags_NoArrowButton))
            {
                int idx = 0;
                for (auto &[Pluginkey, Pluginvalue] : PluginController::getNamesMap())
                {
                    bool is_selected = (current_item == idx);
                    if (ImGui::Selectable(Pluginkey.c_str(), is_selected))
                    {
                        current_item = idx;
                        comboMap[c] = Pluginkey;
                        deviceMap[selected_device].getMap().insert_or_assign(Devicekey, Pluginvalue);
                        for (auto &[key, value] : deviceMap[currentDevCount].getMap())
                        {
                            if (value == NULL)
                            {
                                std::cout << "key: " << key << " value: NULL";
                            }
                            else
                            {
                                std::cout << "key: " << key << " value: Puntatore";
                            }
                        }
                        std::cout << std::endl;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                    idx++;
                }
                ImGui::EndCombo();
            }
            posText += 30;
            posCombo += 30;
        }

        ImGui::EndChild();
        ImGui::End();

        // renderizzo la finestra
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(this->getWindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0862745098039216f, 0.0980392f, 0.11372549f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(this->getWindow());
    }
}

/*
    creo il distruttore della classe
*/
UIController::~UIController()
{

    threadRunning = false;
    if (t1.joinable())
    {
        t1.join();
    }
    if (t0.joinable())
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
