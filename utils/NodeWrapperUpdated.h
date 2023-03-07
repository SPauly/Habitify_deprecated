#pragma once

#include <memory>

#include "Habitify_protocol.pb.h"

namespace Habitify
{
    class InterfaceBase
    {
    public:
        virtual ~InterfaceBase()
        {
        }

        virtual int32_t get_Id();
        virtual void set_Id(HabCom::Node*);
        virtual void sync_nodes();
    };

    template <typename T>
    class NodeWrapper : public InterfaceBase
    {
    public:
        template <typename T>
        NodeWrapper(T &&obj) : interface(std::make_shared<T>(std::forward<T>(obj)))
        {
        }

        HabCom::Node *get_node() const
        {
            sync_nodes();
            return interface->get_node();
        }

        int32_t get_Id() override
        {
            return interface->get_Id();
        }

        void set_node(HabCom::Node* _node = nullptr) override
        {
            interface->set_Id(node.get());
        }
    
        void sync_nodes() override {
            interface->sync_nodes();
        }

    private:
        std::shared_ptr<InterfaceBase> interface;
        std::shared_ptr<HabCom::Node> node;
    };

    // usage
    class ImGuiNode : public InterfaceBase
    {
    private:
        std::unique_ptr<HabCom::Node> node;
    };
}