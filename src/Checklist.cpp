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
        if(!is_initialized.get()){
            create_list();
        }
        
        ImGui::Begin(name.get().c_str());

        ImGui::End();

    }

    void Checklist::create_list()
    {
        static char _list_name[64]; 
        static int pos = -1;
        nodes.resize(10);
        ImGui::Begin("Create New Checklist"); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImGui::InputTextWithHint("REQUIRED", "Enter Name", _list_name, IM_ARRAYSIZE(_list_name));
                for(int i = 0; i <= pos; i++)
                {
                    nodes.at(i).init();
                }
                if(ImGui::Button("Add Node"))
                {
                    ++pos;
                    nodes.at(pos).set_edit_mode();
                }
                if(ImGui::Button("Save Checklist"))
                {
                    name.set(_list_name);
                    is_initialized.set(true);
                }
        ImGui::End();
    }
}