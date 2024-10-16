#include "Core.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
    Core* core;
    bool result;


    core = new Core;
    if (!core)
    {
        return 0;
    }

    result = core->Initalize();
    if (result)
    {
        core->Run();
    }

    core->Destroy();
    delete core;

    return 0;
}