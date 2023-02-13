#include "Application.h"


int main(){
    Habitify::Application *app = Habitify::CreateApplication();
    app->Run();
    delete app;
    return 0;
}