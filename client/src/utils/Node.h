#pragma once

#include "imgui.h"
#include "Layer.h"
#include "DataTypes.h"

#include <string>
#include <functional>

#include "NodeWrapper.h"

namespace Habitify
{
    class Node : public Layer
    {
    public:
        Node(){};
        virtual ~Node(){};

        virtual void OnAttach() override;
        virtual void OnUIRender() override;

        inline const HabCom::Node *get_node(){
            return node.get_sendable();
        };
    
    protected:
        void init();

        void render_boolean();
        void render_number();

    private:
        //Internal use
        FLAG<bool> b_edit_mode = true;
        char _temp_id[64] = "New Node";
        char m_crelevance[9] = "OPTIONAL";

        std::function<void()> m_render_function;

        //ImGui related
        const ImS64 m_s64 = 1ULL;
        const ImU64 m_u64 = 1ULL;
        ImVec4 colorHovered;
        ImVec4 colorNormal;

        //Metadata
        NodeWrapper node;
    };

}