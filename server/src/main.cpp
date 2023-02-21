#include<iostream>
#include "Server.h"

using Habitify::HabitifyServer::Server;
using Habitify::HabitifyServer::CreateServer;

int main(){
    std::cout<<"Starting Server..."<<std::endl;
    Server *server = CreateServer();
    server->Run();
    delete server;
    std::cout<<"Server stopped. Press ENTER to close"<<std::endl;
    std::cin.get();
    return 0;
}