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