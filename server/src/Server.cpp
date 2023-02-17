#include "Server.h"

namespace Habitify
{
    namespace HabitifyServer
    {
        void HabitifyServer::Run()
        {
            std::cout << "[INFO] Starting Server..." << std::endl;

            mgrpc_builder.AddListeningPort(m_server_address, grpc::InsecureServerCredentials());
            mgrpc_builder.RegisterService(this);
            std::unique_ptr<grpc::Server> _server(mgrpc_builder.BuildAndStart());

            std::cout << "Server Running. Waiting on " << m_server_address << std::endl;

            _server->Wait();
            return;
        }

        ::grpc::Status HabitifyServer::Ping(::grpc::ServerContext* context, const ::HabCom::Id* request, ::HabCom::Id* response)
        {
            std::cout << "[INFO] Ping arrives: " << request->id() << std::endl;
            response->set_id(12345);
            std::cout << "[INFO] Ping is returned: " << response->id() << std::endl;
            return grpc::Status::OK;
        }

        
    HabitifyServer* CreateServer()
    {
        return new HabitifyServer;
    }
    }
}