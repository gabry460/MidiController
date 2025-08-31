#include <iostream>
#include <windows.h>
#include <audioclient.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>


extern "C" __declspec(dllexport) void Volume(int Control, int Data, int State)
{
    // Inizializza COM
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        std::cerr << "CoInitialize fallita: " << std::hex << hr << std::endl;
        return;
    }

    IMMDeviceEnumerator* pEnumerator = NULL;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
                        __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) {
        std::cerr << "CoCreateInstance fallita: " << std::hex << hr << std::endl;
        CoUninitialize();
        return;
    }

    IMMDevice* pDevice = NULL;
    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
    if (FAILED(hr)) {
        std::cerr << "GetDefaultAudioEndpoint fallita: " << std::hex << hr << std::endl;
        pEnumerator->Release();
        CoUninitialize();
        return;
    }

    IAudioEndpointVolume* pEndpointVolume = NULL;
    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pEndpointVolume);
    if (FAILED(hr)) {
        std::cerr << "Activate fallita: " << std::hex << hr << std::endl;
        pDevice->Release();
        pEnumerator->Release();
        CoUninitialize();
        return;
    }

    hr = pEndpointVolume->SetMasterVolumeLevelScalar(static_cast<float>(Data)/127, NULL);
    if (FAILED(hr)) {
        std::cerr << "SetMasterVolumeLevelScalar fallita: " << std::hex << hr << std::endl;
    } else {
        std::cout << "[+] Volume impostato a " << (int)((float)(Data)) << "%" << std::endl;
    }

    // Cleanup
    pEndpointVolume->Release();
    pDevice->Release();
    pEnumerator->Release();
    CoUninitialize();
}

extern "C" __declspec(dllexport) void StopAudio(int Control, int Data, int State)
{
    if(State == 137)
    {
        keybd_event(VK_MEDIA_PLAY_PAUSE, 0, 0, 0);  
        keybd_event(VK_MEDIA_PLAY_PAUSE, 0, KEYEVENTF_KEYUP, 0);
    }
}

extern "C" __declspec(dllexport) void NextTrack(int Control, int Data, int State)
{
    if(State == 137)
    {
        keybd_event(VK_MEDIA_NEXT_TRACK, 0, 0, 0);  
        keybd_event(VK_MEDIA_NEXT_TRACK, 0, KEYEVENTF_KEYUP, 0);
    }
}

extern "C" __declspec(dllexport) void PrevTrack(int Control, int Data, int State)
{
    if(State == 137)
    {
        keybd_event(VK_MEDIA_PREV_TRACK, 0, 0, 0);  
        keybd_event(VK_MEDIA_PREV_TRACK, 0, KEYEVENTF_KEYUP, 0);
    }
}

extern "C" __declspec(dllexport) void VolumeUp(int Control, int Data, int State)
{
    if(State == 137)
    {
        keybd_event(VK_VOLUME_UP, 0, 0, 0);  
        keybd_event(VK_VOLUME_UP, 0, KEYEVENTF_KEYUP, 0);
    }
}

extern "C" __declspec(dllexport) void VolumeDown(int Control, int Data, int State)
{
    if(State == 137)
    {
        keybd_event(VK_VOLUME_DOWN, 0, 0, 0);  
        keybd_event(VK_VOLUME_DOWN, 0, KEYEVENTF_KEYUP, 0);
    }
}

extern "C" __declspec(dllexport) void Zoom(int Control, int Data, int State)
{
    if(State == 137)
    {
        keybd_event(VK_ZOOM, 0, 0, 0);  
        keybd_event(VK_ZOOM, 0, KEYEVENTF_KEYUP, 0);
    }
}

extern "C" __declspec(dllexport) void Mute(int Control, int Data, int State)
{
    if(State == 137)
    {
        keybd_event(VK_VOLUME_MUTE, 0, 0, 0);  
        keybd_event(VK_VOLUME_MUTE, 0, KEYEVENTF_KEYUP, 0);
    }
}
