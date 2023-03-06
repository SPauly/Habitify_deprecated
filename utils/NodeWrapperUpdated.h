#pragma once

#include <memory>

#include "Habitify_protocol.pb.h"

namespace Habitify
{
    class NodeWrapper
    {
    public:
        template <typename T>
        NodeWrapper(T &&obj) : wrapped_node(std::make_shared<Wrapper<T>>(std::forward<T>(obj)))
        {
        }

    //this is the interface
        class InternalBase
        {
        public:
            virtual ~InternalBase()
            {
            }

            virtual int32_t get_Id();
            virtual void sync_node();
            virtual HabCom::Node* get_node();  
        };

        template <typename T>
        class Wrapper : public InternalBase
        {
        public:
            int32_t get_Id() override {
                return wrapped_object.get_Id();
            }
            void sync_node() override
            {
                wrapped_object.sync_node();
            }

        private:
            T wrapped_object;
        };

        int32_t get_Id(){
            return wrapped_node->get_Id();
        }

        HabCom::Node* get_node() const{
            return wrapped_node->get_node();
        }

    private:
        std::shared_ptr<InternalBase> wrapped_node;
    };


 //usage
    class ImGuiNode : public NodeWrapper::InternalBase
    {
    private:
        std::unique_ptr<HabCom::Node> node;
    };
}