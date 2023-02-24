#pragma once

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "Habitify_protocol.grpc.pb.h"

#include <iostream>
#include <string>
#include <memory>

namespace Habitify
{
    namespace HabitifyServer
    {
        
        class Server final : public HabCom::Server::Service
        {
        public:
            Server() = default;
            ~Server();

            grpc::Status Ping(::grpc::ServerContext*, const ::HabCom::Id*, ::HabCom::Id*) override;
            grpc::Status SaveNode(::grpc::ServerContext*, const ::HabCom::Node*, ::HabCom::Status*) override;
            grpc::Status GetNode(::grpc::ServerContext*, const ::HabCom::Id*, ::HabCom::Node*) override;

            void Run();
            void Shutdown();

        private:
            std::string m_server_address = "0.0.0.0:50051";
            grpc::ServerBuilder mgrpc_builder;
            std::unique_ptr<grpc::Server> mgrpc_server;
        };

        Server* CreateServer();
    }
}