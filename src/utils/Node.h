#pragma once

#include "imgui.h"
#include "DataTypes.h"

#include <string>
#include <functional>

namespace Habitify
{

    class Node
    {
    public:
        Node(){};
        virtual ~Node(){};

        void render();

        void init();

        inline bool is_initialized()
        {
            return b_is_initialized.get();
        }

        inline void set_edit_mode()
        {
            should_edit.set(true);
        }

    protected:
        void render_int();
        void render_uint();
        void render_string();

    private:
        FLAG<bool> b_is_initialized = false;
        FLAG<bool> should_edit = false;        
        
        char temp_id[64] = "";
        Parameter<std::string> id{""};

    	float color[4] = {0.4f, 0.7f, 0.0f, 0.5f};

        int _relevance = 0;
        RELEVANCE relevance = RELEVANCE::OPTIONAL;
        int _datatype = 0;
        NODE_TYPE type = NODE_TYPE::INT;

        std::function<void()> render_function;

        //data
        int64_t *INT = nullptr;
        uint64_t *UINT = nullptr;
        std::string *STRING = nullptr;
    };
}