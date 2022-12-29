#pragma once

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "Habitify_protocol.grpc.pb.h"

namespace HabitifyServer
{
    class HabitifyServer final : public Habitify::Communication::Service
    {
        
    };
}