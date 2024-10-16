#include "Core.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
    Core* Core;
    bool result;


    Core = new Core;
    if (!System)
    {
        return 0;
    }

    result = Core->Initialize();
    if (result)
    {
        Core->Run();
    }

    System->Shutdown();
    delete System;
    System = 0;

    return 0;
}