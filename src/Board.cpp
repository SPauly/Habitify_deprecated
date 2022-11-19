#include "Board.h"

namespace Habitify
{
    void Board::OnUIRender()  
    {
        ImGui::Begin("Board");
            ImGui::Text("Test");
        ImGui::End();
    }
}