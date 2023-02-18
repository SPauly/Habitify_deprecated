#include "Board.h"

namespace Habitify
{
    void Board::OnUIRender()
    {
        ImGui::Begin("Board");
        for (auto layer : m_layer_stack)
        {
            layer->OnUpdate(0);
        };

        if (ImGui::IsWindowHovered())
        {
            ImGui::Text("Board is hovered");
            if (ImGui::IsMouseClicked(1))
            {
                ImGui::Text("Right ");
                mvec_nodes.push_back(std::make_shared<Node_>());
                m_layer_stack.PushLayer(mvec_nodes.back());
            }
        }
        else
            ImGui::Text("Test");

        for (auto layer : m_layer_stack)
        {
            layer->OnUIRender();
        };

        ImGui::End();
    }
}