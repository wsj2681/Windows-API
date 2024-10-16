#include "Core.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
    Core* core = nullptr;
    bool result;


    core = new Core;
    if (!core)
    {
        return 0;
    }

    if (result = core->Initalize())
    {
        core->Run();
    }

    core->Destroy();
    delete core;

    return 0;
}