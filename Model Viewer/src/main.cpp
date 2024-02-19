#include "Application.h"
#include <iostream>

int main() {

	Application *app = new Application();

	if (app->Startup() == true) {
        while (app->Update()) {
            app->Draw();
		}
        app->Shutdown();
	}
    delete app;
	return 0;
}

