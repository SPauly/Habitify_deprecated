#include "Node.h"
#include <cstdint>

namespace Habitify
{
    Node::~Node()
    {
        if(locked_node_ptr)
            delete locked_node_ptr;
    }

    void Node::OnAttach()
    {
        node.set_name(_temp_id);
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
        ImGui::PushStyleColor(ImGuiCol_WindowBg, 0);

        ImGui::Begin(node.get_name().c_str(), NULL, window_flags);
        ImGui::Text(node.get_name().c_str());

        m_render_function();
        
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        ImGui::End();
    }

    void Node::init()
    {
        if(!locked_node_ptr)
            locked_node_ptr = new MutableNodeLock(node);
        
        #define MUTABLE_NODE_ACCESS locked_node_ptr->operator->()

        ImGui::OpenPopup("Data Node Editor");

        if (ImGui::BeginPopupModal("Data Node Editor", NULL))
        {
            // Explanation Text
            ImGui::Text("This is the Data Node Editor.\nData Nodes basically present datapoints\nthat shall be tracked.");

            // Get ID
            ImGui::InputTextWithHint("Node ID", "f.e. Smartphone Time", _temp_id, IM_ARRAYSIZE(_temp_id));

            // Get Data Type
            ImGui::Combo("Data Type", (int)MUTABLE_NODE_ACCESS->type(), "Boolean\0Number\0Text\0\0");
            if (MUTABLE_NODE_ACCESS->type() == 1)
            {
                ImGui::DragIntRange2("Set Range", &MUTABLE_NODE_ACCESS->min(), &MUTABLE_NODE_ACCESS->max(), 2, -100, 100, "Min: %d units", "Max: %d units");
                if (ImGui::RadioButton("Input", &MUTABLE_NODE_ACCESS->_type_presentation, 0))
                    ;
                ImGui::SameLine();
                if (ImGui::RadioButton("Slider", &MUTABLE_NODE_ACCESS->_type_presentation, 1))
                    ;
            }
            else if (MUTABLE_NODE_ACCESS->type() == 2)
            {
                if (ImGui::RadioButton("Single Line", &MUTABLE_NODE_ACCESS->_type_presentation, 2))
                    ;
                ImGui::SameLine();
                if (ImGui::RadioButton("Multiple Lines", &MUTABLE_NODE_ACCESS->_type_presentation, 3))
                    ;
            }

            // Get Relevance
            ImGui::Combo("Relevance", &MUTABLE_NODE_ACCESS->mutable(), "REQUIRED\0OPTIONAL\0\0");

            // Get Color
            //ImGui::ColorEdit4("Color", (float *)&color);

            if (ImGui::Button("Save"))
            {
                // save data
                MUTABLE_NODE_ACCESS->_id = _temp_id;
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

                delete locked_node_ptr; //free lock and merge with original
                
                // set flags and close
                b_edit_mode = false;
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }

    void Node::render_boolean()
    {
        ImGui::RadioButton("Yes", (int*)MUTABLE_NODE_ACCESS->_boolean, 1);
        ImGui::SameLine();
        ImGui::RadioButton("No", (int*)MUTABLE_NODE_ACCESS->_boolean, 0);
    }

    void Node::render_number()
    {
        if(node->type_representation() == HabCom::NodeTypePresentation::SLIDER)
        {
            ImGui::SliderFloat(m_crelevance, MUTABLE_NODE_ACCESS->_number, (float)node->min(), (float)node->max() ,"%.1f", ImGuiSliderFlags_AlwaysClamp);
        }
        else
        {
            ImGui::InputFloat("input float", MUTABLE_NODE_ACCESS->_number, node->max()/20, node->max()/10, "%.3f", ImGuiInputTextFlags_AlwaysInsertMode);
        }
    }

}