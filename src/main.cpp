#include <iostream>
#include "windowManager.h"

int main()
{
    WindowManager window(1280, 720, "Main Window");
    window.Tick();
    return 0;
}
