#include "Server.h"

namespace Habitify
{
    namespace HabitifyServer
    {
        Server::~Server()
        {
            Shutdown();
        }

        void Server::Run()
        {
            std::cout << "[INFO] Starting Server..." << std::endl;

            mgrpc_builder.AddListeningPort(m_server_address, grpc::InsecureServerCredentials());
            mgrpc_builder.RegisterService(this);
            mgrpc_server = mgrpc_builder.BuildAndStart();

            std::cout << "Server Running. Waiting on " << m_server_address << std::endl;

            mgrpc_server->Wait();
            return;
        }

        void Server::Shutdown()
        {
            mgrpc_server->Shutdown();
        }

        ::grpc::Status Server::Ping(::grpc::ServerContext *context, const ::HabCom::Id *request, ::HabCom::Id *response)
        {
            std::cout << "[INFO] Ping arrives: " << request->id() << std::endl;
            response->set_id(12345);
            std::cout << "[INFO] Ping is returned: " << response->id() << std::endl;
            return grpc::Status::OK;
        }

        grpc::Status Server::GetNode(::grpc::ServerContext *context, const ::HabCom::Id *request, ::HabCom::Node *response)
        {

        }

        grpc::Status Server::SaveNode(::grpc::ServerContext *context, const ::HabCom::Node *request, ::HabCom::Status *response)
        {
            m_node_handler.HandleNode(request);
            auto temp_id = request->id();
            response->set_flag(m_node_handler.SaveNode().flag());
            return grpc::Status::OK;
        }

        Server *CreateServer()
        {
            return new Server;
        }
    }
}