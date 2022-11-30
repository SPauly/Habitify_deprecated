#include "Board.h"

namespace Habitify
{
    void Board::OnUIRender()  
    {
        ImGui::Begin("Board");
            if(ImGui::IsWindowHovered()){
                ImGui::Text("Board is hovered");
                if(ImGui::IsMouseClicked(1))
                {   
                    ImGui::Text("Right ");
                    mvec_nodes.push_back(std::make_shared<Node_>());
                }
            }
            else
                ImGui::Text("Test");
        ImGui::End();
    }
}