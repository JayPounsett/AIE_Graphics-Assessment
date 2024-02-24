#include <iostream>

#include "Application.h"

int main()
{
    Application* app = new Application();

    if (app->startup() == true) {
        while (app->update()) {
            app->draw();
        }
        app->shutdown();
    }
    delete app;
    return 0;
}
