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
        ImGui::Begin("Another Window"); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me")) 
            ;
        ImGui::End();
    }
}