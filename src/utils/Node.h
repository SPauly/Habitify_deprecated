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
        Node();
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
        void render_boolean();
        void render_int();
        void render_float(){};
        void render_string(){};

        void operate_on_data(NODE_TYPE, std::function<void()>);

    private:
        //internal use
        FLAG<bool> b_is_initialized = false;
        FLAG<bool> should_edit = false;        

        char temp_id[64] = "";
        int _relevance = 0;    
        char _crelevance[9] = "XXX";   
        
        int _datatype = 0;           
        int _presentation = 0;
        int _boolean = 0;
        std::function<void()> render_function;

        //ImGui related
        const ImS64 s64 = 1ULL;
        const ImU64 u64 = 1ULL;

        //Metadata
        Parameter<std::string> id{""};
    	ImVec4 color = {0.4f, 0.7f, 0.0f, 0.5f};
        ImVec4 colorHovered;
        ImVec4 colorNormal;

        RELEVANCE relevance = RELEVANCE::OPTIONAL;
        NODE_TYPE datatype = NODE_TYPE::INT;
        NODE_TYPE_PRESENTATION presentation = NODE_TYPE_PRESENTATION::INPUT;
        int min = 0, max = 10;

        //data
        bool *BOOLEAN = nullptr;
        int64_t *INT = nullptr;
        uint64_t *UINT = nullptr;
        double *FLOAT = nullptr;
        std::string *STRING = nullptr;
    };
}