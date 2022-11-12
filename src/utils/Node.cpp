#include "Node.h"
#include <cstdint>

namespace Habitify
{
    void Node::init()
    {
        if (should_edit.get())
        {
            ImGui::OpenPopup("Data Node Editor");

            if (ImGui::BeginPopupModal("Data Node Editor", NULL))
            {
                //Explanation Text
                ImGui::Text("Is initialized: %d Should edit: %d, this: %llu", b_is_initialized.get(), should_edit.get(), reinterpret_cast<std::uintptr_t>(this));
                ImGui::Text("This is the Data Node Editor.\nData Nodes basically present datapoints\nthat shall be tracked in the Checklist.");

                //Get ID
                ImGui::InputTextWithHint("Node ID", "f.e. Smartphone Time", temp_id, IM_ARRAYSIZE(temp_id));

                //Get Data Type
                ImGui::Combo("Data Type", &_datatype, "Integer\0Unsigned Integer\0Float\0Unsigned Float\0Text\0\0");

                //Get Relevance
                ImGui::Combo("Relevance", &_relevance, "REQUIRED\0OPTIONAL\0\0");

                //Get Color
                ImGui::ColorEdit4("Color", color);

                if (ImGui::Button("Save"))
                {
                    id.set(temp_id);
                    type = static_cast<NODE_TYPE> (_datatype);
                    relevance = static_cast<RELEVANCE> (_relevance);

                    b_is_initialized.set(true);
                    should_edit.set(false);
                    ImGui::Text("Is initialized: %d Should edit: %d, this: %llu", b_is_initialized.get(), should_edit.get(), reinterpret_cast<std::uintptr_t>(this));
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }
        // render temp
        else
        {
            if (ImGui::Button("Edit Node"))
                should_edit.set(true);
        }
    }

    void Node::render()
    {
    }
}