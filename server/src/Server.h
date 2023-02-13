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
        using grpc::Status;
        
        class HabServer final : public Communication::HabServer::Service
        {
            Status Ping(ServerContext*, const Id*, Id*) override;
            void RunServer();
        };
    }
}