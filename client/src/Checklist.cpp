#include "Checklist.h"

namespace Habitify
{
    Checklist::Checklist()
    {
    }

    Checklist::~Checklist()
    {
    }

    void Checklist::render()
    {
        if (!is_initialized.get())
        {
            create_list();
        }
        else
        {
            ImGui::Begin(name.get().c_str());

            for (Node &current : nodes)
            {
                current.OnUIRender();
            }

            ImGui::End();
        }
    }

    void Checklist::create_list()
    {
        static char _list_name[64] = "Checklist Name";
        ImGui::Begin("Create New Checklist"); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::InputTextWithHint("REQUIRED", "Enter Name", _list_name, IM_ARRAYSIZE(_list_name));
        for (Node &current : nodes)
        {
            current.OnAttach();
        }
        if (ImGui::Button("Add Node"))
        {
            nodes.push_back(temp);
        }
        if (ImGui::Button("Save Checklist"))
        {
            name.set(_list_name);
            is_initialized.set(true);
        }
        ImGui::End();
    }
}