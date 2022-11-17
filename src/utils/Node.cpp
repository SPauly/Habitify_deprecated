#include "Node.h"
#include <cstdint>

namespace Habitify
{
    Node::Node()
    {
    }

    void Node::init()
    {
        if (should_edit.get())
        {
            ImGui::OpenPopup("Data Node Editor");

            if (ImGui::BeginPopupModal("Data Node Editor", NULL))
            {
                // Explanation Text
                ImGui::Text("This is the Data Node Editor.\nData Nodes basically present datapoints\nthat shall be tracked in the Checklist.");

                // Get ID
                ImGui::InputTextWithHint("Node ID", "f.e. Smartphone Time", temp_id, IM_ARRAYSIZE(temp_id));

                // Get Data Type
                ImGui::Combo("Data Type", &_datatype, "Boolean\0Integer\0Float\0Text\0\0");
                if (_datatype == 1 || _datatype == 2)
                {
                    ImGui::DragIntRange2("Set Range", &min, &max, 2, -100, 100, "Min: %d units", "Max: %d units");
                    if (_datatype == 2 && min < 0)
                        min = 0;
                    if (ImGui::RadioButton("Input", &_presentation, 0))
                        ;
                    ImGui::SameLine();
                    if (ImGui::RadioButton("Slider", &_presentation, 1))
                        ;
                }
                else if (_datatype == 3)
                {
                    if (ImGui::RadioButton("Single Line", &_presentation, 2))
                        ;
                    ImGui::SameLine();
                    if (ImGui::RadioButton("Multiple Lines", &_presentation, 3))
                        ;
                }

                // Get Relevance
                ImGui::Combo("Relevance", &_relevance, "REQUIRED\0OPTIONAL\0\0");

                // Get Color
                ImGui::ColorEdit4("Color", (float *)&color);

                if (ImGui::Button("Save"))
                {
                    // save data
                    id.set(temp_id);
                    datatype = static_cast<NODE_TYPE>(_datatype);
                    relevance = static_cast<RELEVANCE>(_relevance);
                    presentation = static_cast<NODE_TYPE_PRESENTATION>(_presentation);
                    colorHovered = {color.x, color.y, color.z - .1f, color.w - .1f};
                    colorNormal = {color.x, color.y, color.z - .2f, color.w - .2f};

                    // set render function and reserve space
                    switch (datatype)
                    {
                    case NODE_TYPE::BOOLEAN:
                        render_function = std::bind(Node::render_boolean, this);
                        BOOLEAN = new bool;
                        break;
                    case NODE_TYPE::INT:
                        render_function = std::bind(Node::render_int, this);
                        if (min < 0)
                            INT = new int64_t;
                        else
                            UINT = new uint64_t;
                        break;
                    case NODE_TYPE::FLOAT:
                        render_function = std::bind(Node::render_float, this);
                        FLOAT = new double;
                    case NODE_TYPE::STRING:
                        render_function = std::bind(Node::render_string, this);
                        STRING = new std::string;
                    default:
                        break;
                    }

                    // set flags and close
                    b_is_initialized.set(true);
                    should_edit.set(false);
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }
        // render temp
        else
        {
            ImGui::PushID(id.get().c_str());
            ImGui::PushStyleColor(ImGuiCol_Button, colorNormal);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorHovered);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);

            std::string _button = "Edit: \'" + id.get() + "\'";
            if (ImGui::Button(_button.c_str()))
            {
                // set render function and reserve space
                switch (datatype)
                {
                case NODE_TYPE::BOOLEAN:
                    delete BOOLEAN;
                    break;
                case NODE_TYPE::INT:
                    if (min < 0)
                        delete INT;
                    else
                        delete UINT;
                    break;
                case NODE_TYPE::FLOAT:
                    delete FLOAT;
                case NODE_TYPE::STRING:
                    delete STRING;
                default:
                    break;
                }
                should_edit.set(true);
            }
            ImGui::PopStyleColor(3);
            ImGui::PopID();

            ImGui::Separator();
        }
    }

    void Node::render()
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        window_flags |= ImGuiWindowFlags_MenuBar;
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, color);

        ImGui::BeginChild(id.get().c_str(), ImVec2(0, 60), true, window_flags);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu(id.get().c_str()))
            {
                if (ImGui::MenuItem("Edit Data Node"))
                {
                    should_edit.set(true);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        render_function();

        ImGui::EndChild();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }

    void Node::render_int()
    {
        if (min < 0)
        {
          //  ImGui::InputScalar(_crelevance, ImGuiDataType_S64, INT, &s64);
          ImGui::Text("hi");
        }
        else
        {
            ImGui::Text("No");
            //ImGui::InputScalar(_crelevance, ImGuiDataType_U64, UINT, &u64);
        }
    }

    void Node::render_boolean()
    {
        if (ImGui::RadioButton("Yes", &_boolean, 1))
            ;
        ImGui::SameLine();
        if (ImGui::RadioButton("No", &_boolean, 0))
            ;
        ImGui::SameLine();
        ImGui::Text(_crelevance);
    }

    void Node::operate_on_data(NODE_TYPE _type, std::function<void()> _func){};
}