#pragma once

#include <memory>

#include "Layer.h"
#include "Node.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>

#include "Habitify_protocol.grpc.pb.h"

namespace Habitify
{
    class DebugConnection : public Layer
    {
    public:
        DebugConnection() = default;
        DebugConnection(std::shared_ptr<grpc::Channel> _channel, const std::vector<std::shared_ptr<Node>> *_nodes) : mgrpc_channel(_channel), mptr_nodes(_nodes) {
            m_stub = HabCom::Server::NewStub(mgrpc_channel);
            m_clientID.set_id(54321);
        };
        ~DebugConnection() = default;

        virtual void OnUIRender() override{
            ImGui::Begin("DebugConnection");
                if(ImGui::Button("Ping Server"))
                {
                    grpc::ClientContext context;
                    grpc::Status status = m_stub->Ping(&context, m_clientID, &m_serverID);
                    ImGui::Text("[Ping]: %d ", m_serverID.id());
                } ImGui::SameLine();
                if(ImGui::Button("Save Nodes"))
                {
                    grpc::ClientContext context;
                    for(auto i : *mptr_nodes){
                        grpc::Status status = m_stub->SaveNode(&context, *i->get_node(), &m_save_status);
                    }
                    ImGui::Text("Send nodes");
                }
            ImGui::End();
        };
    private:
        std::shared_ptr<grpc::Channel> mgrpc_channel;
        std::unique_ptr<HabCom::Server::Stub> m_stub;

        const std::vector<std::shared_ptr<Node>> *mptr_nodes;

        HabCom::Id m_clientID;
        HabCom::Id m_serverID;
        HabCom::Status m_save_status;
    };
}