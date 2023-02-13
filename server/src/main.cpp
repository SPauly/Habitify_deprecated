#include <iostream>
#include "Server.h"

using Habitify::HabitifyServer;

int main()
{
    HabServer* _server = new HabServer;
    _server->RunServer();
    
    return 0;
}
