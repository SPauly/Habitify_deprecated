#pragma once

#include <memory>

#include "Layer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>

#include "Habitify_protocol.grpc.pb.h"

namespace Habitify
{
    class PingDemo : public Layer
    {
    public:
        PingDemo() = default;
        PingDemo(std::shared_ptr<grpc::Channel> _channel) : msprt_channel(_channel) {
            m_stub = HabCom::Server::NewStub(msprt_channel);
            m_clientID.set_id(54321);
        };
        ~PingDemo() = default;

        virtual void OnUIRender() override{
            ImGui::Begin("PingDemo");
                if(ImGui::Button("Ping Server"))
                {
                    grpc::ClientContext context;
                    grpc::Status status = m_stub->Ping(&context, m_clientID, &m_serverID);
                    ImGui::Text("[Ping]: %d ", m_serverID.id());
                }
            ImGui::End();
        };
    private:
        std::shared_ptr<grpc::Channel> msprt_channel;
        std::unique_ptr<HabCom::Server::Stub> m_stub;

        HabCom::Id m_clientID;
        HabCom::Id m_serverID;
    };
}