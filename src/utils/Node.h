#pragma once

#include "imgui.h"
#include "Layer.h"
#include "DataTypes.h"

#include <string>
#include <functional>

namespace Habitify
{
    class Node_ : public Layer
    {
    public:
        Node_(){};
        virtual ~Node_(){};

        virtual void OnAttach() override;
        virtual void OnUIRender() override;
    
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

        //Metadata
        Parameter<std::string> m_id{"New Node"};    
        int m_datatype = NODE_TYPE::INT;
        int m_relevance = RELEVANCE::OPTIONAL;
        int m_presentation = NODE_TYPE_PRESENTATION::INPUT;
        int min = 0, max = 0;
        ImVec4 color = {0.4f, 0.7f, 0.0f, 1.0f};
        ImVec4 colorHovered;
        ImVec4 colorNormal;

        //data
        int *m_boolean = nullptr;
        float *m_number = nullptr;
        std::string *m_string = nullptr;
    };

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