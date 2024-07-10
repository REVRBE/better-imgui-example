#include "app.h"

// To bring back the console, simply go into the project settings -> Linker -> System -> SubSystem and change it from Windows to Console. Make sure to change the main definition to int main and comment out WinMain.

//int main(int, char**)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    app::get_instance().run();
    return 0;
}
