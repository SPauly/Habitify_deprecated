#pragma once

#include <memory>

#include "Habitify_protocol.pb.h"

namespace Habitify
{
    class NodeWrapper
    {
    public:
        NodeWrapper(){
            mptr_node = std::make_unique<HabCom::Node>();
        };
        virtual ~NodeWrapper() = default;

        HabCom::Node* operator->()
        {
            return mptr_node.get();
        }

    private:
        std::unique_ptr<HabCom::Node> mptr_node;
    };

}