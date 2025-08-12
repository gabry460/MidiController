#include "UIController.hpp"

#define HEIGHT 350
#define WIDTH 450

int main()
{
    // creo e faccio parire l'UI
    UIController UI;
    UI.Init();
    UI.createWindow(WIDTH,HEIGHT,"MIDI CONTROLLER");
    UI.start();

    return 0;
}