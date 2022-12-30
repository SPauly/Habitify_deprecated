#pragma once

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "Habitify_protocol.grpc.pb.h"

namespace Habitify
{
    namespace HabitifyServer
    {
        using grpc::Status;
        
        class Server final : public Communication::HabServer::Service
        {
            Status Ping(ServerContext*, const Id*, Id*) override;
        };
    }
}