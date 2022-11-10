#include "Application.h"


int main(){
    Habitify::Application *app = new Habitify::Application();
    app->Run();
    delete app;
    return 0;
}