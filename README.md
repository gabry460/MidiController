# MIDI Controller Interface

This software is designed to manage MIDI controllers and interface with your system through a simple graphical UI.  
It delivers high performance and allows any user (with basic programming knowledge) to create custom plugins for their device.

---

## Installation
1. Clone or download the repository to your computer.  
2. Navigate to the `Bin/Release` folder.  
3. Run the executable file.

---

## Usage
- The graphical interface is built with **Dear ImGui** and **OpenGL APIs**.  
- The **left panel** displays the list of connected MIDI devices.  
- The **right panels** show device details and a button to start the connection (spawning a new thread).  
- The **bottom panel** lists the available controller commands (after configuration: press all buttons and move all analog controls to detect them).  
- Each command has a **combo box** to assign a function from the loaded plugins.

---

## How to Add a Plugin
1. Place the `.dll` file containing your plugin functions into the `/Plugins/` folder.  
2. Edit the `.json` file (see the included example) to specify the file name and each function name (case-sensitive).  
3. The program will automatically detect and display these functions in the command combo boxes.  
4. If they do not appear, try restarting the program.

---

## How to Create a Plugin
1. Create a `.cpp` file with functions following this signature:
   ```cpp
   extern "C" __declspec(dllexport) void Volume(int Control, int Data, int State)
Control: the command (e.g., a controller button)

Data: the data value (e.g., analog stick value)

State: the state (e.g., button pressed or released)

Implement your desired algorithm inside the function body.

Compile the file into a .dll and place it in /Plugins/. 
es. ```bash
    g++ -shared -o /path/for/dll /path/to/cpp -lole32 -luuid -static-libgcc "-Wl,--out-implib,libmialib.a"

Add the file name and function names to the .json file.
es. ```json
    "Volume.dll": [
        "Volume"
    ]
##Technical Details
- Uses Dear ImGui with OpenGL, ensuring high performance.

- Optimized for minimized window usage: in this mode, rendering is reduced to one frame every 150 ms to lower CPU usage, while keeping commands active via separate threads.

- Device scanning occurs once per second on a separate thread to further minimize CPU load.

- The .json file stores the function names and file references for loading .dll plugins.

Intended for users needing a fast interface to MIDI devices (e.g., Lightroom, OBS, Windows integration), but the community is free to create plugins for any purpose.