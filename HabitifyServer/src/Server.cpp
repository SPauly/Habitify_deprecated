#include "Server.h"

namespace Habitify
{
    namespace HabitifyServer
    {
        Status HabServer::Ping(ServerContext* context, const Id* request, Id* response)
        {
            std::cout << "[PING]: " << request->get_id() << std::endl;
            std::cout << "      [ANSWER]: 1234" << std::endl;
            response->set_id(1234);
            return Status::OK;
        }

        void HabServer::RunServer()
        {
            std::string server_address("0.0.0.0:50051");

            ServerBuilder builder;
            builder.AddListeningPort(server_address, grpc::InsecureServerCredentials);
            builder.RegisterService(this);
            std::unique_ptr<Server> server(builder.BuildAndStart());
            std::cout<<"[INFO] Server listening on " << server_address <<std::endl;
            server->Wait();
        }
    }
}