#pragma once

#include "imgui.h"

#include "utils/DataTypes.h"

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
        FLAG<bool> is_initialized = false;
    };
}