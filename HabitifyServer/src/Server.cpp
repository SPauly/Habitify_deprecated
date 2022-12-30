#include "Server.h"

namespace Habitify
{
    namespace HabitifyServer
    {
        Status Server::Ping(ServerContext* context, const Id* request, Id* response)
        {
            std::cout << "[PING]: " << request->get_id() << std::endl;
            std::cout << "      [ANSWER]: 1234" << std::endl;
            response->set_id(1234);
            return Status::OK;
        }
    }
}