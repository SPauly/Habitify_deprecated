#include "Node.h"
#include <cstdint>

namespace Habitify
{
    void Node_::OnAttach()
    {
        m_boolean = new int;
        m_render_function = m_render_function = std::bind(render_boolean, this);
    }

    void Node_::OnUIRender()
    {
        if (b_edit_mode.get())
            init();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        window_flags |= ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, color);

        ImGui::Begin(m_id.get().c_str(), NULL, window_flags);
        ImGui::Text(m_id.get().c_str());

        m_render_function();
        
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        ImGui::End();
    }

    void Node_::init()
    {
        ImGui::OpenPopup("Data Node Editor");

        if (ImGui::BeginPopupModal("Data Node Editor", NULL))
        {
            // Explanation Text
            ImGui::Text("This is the Data Node Editor.\nData Nodes basically present datapoints\nthat shall be tracked.");

            // Get ID
            ImGui::InputTextWithHint("Node ID", "f.e. Smartphone Time", _temp_id, IM_ARRAYSIZE(_temp_id));

            // Get Data Type
            ImGui::Combo("Data Type", &m_datatype, "Boolean\0Number\0Text\0\0");
            if (m_datatype == 1)
            {
                ImGui::DragIntRange2("Set Range", &min, &max, 2, -100, 100, "Min: %d units", "Max: %d units");
                if (ImGui::RadioButton("Input", &m_presentation, 0))
                    ;
                ImGui::SameLine();
                if (ImGui::RadioButton("Slider", &m_presentation, 1))
                    ;
            }
            else if (m_datatype == 2)
            {
                if (ImGui::RadioButton("Single Line", &m_presentation, 2))
                    ;
                ImGui::SameLine();
                if (ImGui::RadioButton("Multiple Lines", &m_presentation, 3))
                    ;
            }

            // Get Relevance
            ImGui::Combo("Relevance", &m_relevance, "REQUIRED\0OPTIONAL\0\0");

            // Get Color
            ImGui::ColorEdit4("Color", (float *)&color);

            if (ImGui::Button("Save"))
            {
                // save data
                m_id.set(_temp_id);
                colorHovered = {color.x, color.y, color.z - .1f, color.w - .1f};
                colorNormal = {color.x, color.y, color.z - .2f, color.w - .2f};

                // set render function and reserve space
                switch (m_datatype)
                {
                case NODE_TYPE::BOOLEAN:
                    m_render_function = std::bind(render_boolean, this);
                    m_boolean = new int;
                    break;
                case NODE_TYPE::INT:
                    m_render_function = std::bind(render_number, this);
                    m_number = new float;
                    break;
                case NODE_TYPE::STRING:
                    //m_render_function = std::bind(render_string, this);
                    m_string = new std::string;
                default:
                    m_render_function = std::bind(render_boolean, this);
                    break;
                }

                switch (m_relevance)
                {
                case RELEVANCE::REQUIRED:
                    strcpy(m_crelevance, "REQUIRED");
                    break;
                case RELEVANCE::NOTREQUIRED:
                    strcpy(m_crelevance, "OPTIONAL");
                    break;
                default:
                    strcpy(m_crelevance, "OPTIONAL");
                    break;
                };
                // set flags and close
                b_edit_mode = false;
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }

    void Node_::render_boolean()
    {
        ImGui::RadioButton("Yes", m_boolean, 1);
        ImGui::SameLine();
        ImGui::RadioButton("No", m_boolean, 0);
    }

    void Node_::render_number()
    {
        if(m_presentation == NODE_TYPE_PRESENTATION::SLIDER)
        {
            ImGui::SliderFloat(m_crelevance, m_number, min, max,"%.1f", ImGuiSliderFlags_AlwaysClamp);
        }
        else
        {
            ImGui::InputFloat("input float", m_number, max/20, max/10, "%.3f", ImGuiInputTextFlags_AlwaysInsertMode);
        }
    }

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
                        render_function = std::bind(Node::render_boolean, this);
                        break;
                    }

                    switch (_relevance)
                    {
                    case RELEVANCE::REQUIRED:
                        strcpy(_crelevance, "REQUIRED");
                        break;
                    case RELEVANCE::NOTREQUIRED:
                        strcpy(_crelevance, "OPTIONAL");
                        break;
                    default:
                        strcpy(_crelevance, "OPTIONAL");
                        break;
                    };
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

        if (should_edit.get())
            init();

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

        this->render_function();

        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        ImGui::EndChild();
    }

    void Node::render_int()
    {
        if (min < 0)
        {
            ImGui::InputScalar(_crelevance, ImGuiDataType_S64, INT, &s64);
        }
        else
        {
            ImGui::InputScalar(_crelevance, ImGuiDataType_U64, UINT, &u64);
        }
    }

    void Node::render_boolean()
    {
        ImGui::RadioButton(id.get().c_str(), &_boolean, 1);
        ImGui::SameLine();
        // ImGui::RadioButton("No " + id.get().c_str()), &_boolean, 0);         ImGui::SameLine();
        ImGui::Text(&_crelevance[0]);
    }

    void Node::operate_on_data(NODE_TYPE _type, std::function<void()> _func){};
}