#pragma once

#include <vector>
#include <string>
#include <functional>

#include "imgui.h"

#include "utils/DataTypes.h"
#include "utils/Node.h"

namespace Habitify
{
    class Checklist
    {
    public:
        Checklist();
        virtual ~Checklist();

        void render();
    
    protected:
        void create_list();
        void add_element();
        
    private:
        Parameter<std::string> name{"Your New Checklist"};
        FLAG<bool> is_initialized = false;
        Node temp;
        std::vector<Node> nodes;
        std::vector<std::function<void()>> node_render_functions;
    };
}