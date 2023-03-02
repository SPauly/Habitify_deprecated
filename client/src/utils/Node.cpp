#include "Node.h"
#include <cstdint>

namespace Habitify
{
    void Node::OnAttach()
    {
        node->set_name(_temp_id);
        node->set_boolean(false);
        node.set_temp_copy(true);
        m_node_name = node->name();
        m_render_function = std::bind(render_boolean, this);
    }

    void Node::OnUIRender()
    {
        if (b_edit_mode.get())
            init();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        window_flags |= ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, node->color().x());

        ImGui::Begin(m_node_name.c_str(), NULL, window_flags);
        ImGui::Text(m_node_name.c_str());

        m_render_function();
        
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        ImGui::End();
    }

    void Node::init()
    {
        ImGui::OpenPopup("Data Node Editor");

        if (ImGui::BeginPopupModal("Data Node Editor", NULL))
        {
            // Explanation Text
            ImGui::Text("This is the Data Node Editor.\nData Nodes basically present datapoints\nthat shall be tracked.");

            // Get ID
            ImGui::InputTextWithHint("Node ID", "f.e. Smartphone Time", _temp_id, IM_ARRAYSIZE(_temp_id));

            // Get Data Type
            ImGui::Combo("Data Type", &node.get_mutable()->_type, "Boolean\0Number\0Text\0\0");
            if (node.get_mutable()->_type == 1)
            {
                ImGui::DragIntRange2("Set Range", &node.get_mutable()->_min, &node.get_mutable()->_max, 2, -100, 100, "Min: %d units", "Max: %d units");
                if (ImGui::RadioButton("Input", &node.get_mutable()->_type_presentation, 0))
                    ;
                ImGui::SameLine();
                if (ImGui::RadioButton("Slider", &node.get_mutable()->_type_presentation, 1))
                    ;
            }
            else if (node.get_mutable()->_type == 2)
            {
                if (ImGui::RadioButton("Single Line", &node.get_mutable()->_type_presentation, 2))
                    ;
                ImGui::SameLine();
                if (ImGui::RadioButton("Multiple Lines", &node.get_mutable()->_type_presentation, 3))
                    ;
            }

            // Get Relevance
            ImGui::Combo("Relevance", &node.get_mutable()->_relevance, "REQUIRED\0OPTIONAL\0\0");

            // Get Color
            //ImGui::ColorEdit4("Color", (float *)&color);

            if (ImGui::Button("Save"))
            {
                // save data
                node.get_mutable()->_id = _temp_id;
                // copy from temp to node
                node.merge_temp(); 

                //colorHovered = {color.x, color.y, color.z - .1f, color.w - .1f};
                //colorNormal = {color.x, color.y, color.z - .2f, color.w - .2f};

                // set render function and reserve space
                switch (node->type())
                {
                case HabCom::NodeType::BOOLEAN:
                    node->set_boolean(true);
                    m_render_function = std::bind(render_boolean, this);
                    break;
                case HabCom::NodeType::INT:
                    node->set_number(0.0f);
                    m_render_function = std::bind(render_number, this);
                    break;
                case HabCom::NodeType::STRING:
                    node->set_text("");
                    //m_render_function = std::bind(render_string, this);
                default:
                    m_render_function = std::bind(render_boolean, this);
                    break;
                }

                switch (node->relevance())
                {
                case HabCom::Relevance::REQUIRED:
                    strcpy(m_crelevance, "REQUIRED");
                    break;
                case HabCom::Relevance::NOTREQUIRED:
                    strcpy(m_crelevance, "OPTIONAL");
                    break;
                default:
                    strcpy(m_crelevance, "OPTIONAL");
                    break;
                };

                node.merge_temp(); //necessary to ensure temp is also up to date
                // set flags and close
                b_edit_mode = false;
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }

    void Node::render_boolean()
    {
        ImGui::RadioButton("Yes", (int*)node.get_mutable()->_boolean, 1);
        ImGui::SameLine();
        ImGui::RadioButton("No", (int*)node.get_mutable()->_boolean, 0);
    }

    void Node::render_number()
    {
        if(node->type_representation() == HabCom::NodeTypePresentation::SLIDER)
        {
            ImGui::SliderFloat(m_crelevance, node.get_mutable()->_number, (float)node->min(), (float)node->max() ,"%.1f", ImGuiSliderFlags_AlwaysClamp);
        }
        else
        {
            ImGui::InputFloat("input float", node.get_mutable()->_number, node->max()/20, node->max()/10, "%.3f", ImGuiInputTextFlags_AlwaysInsertMode);
        }
    }

}