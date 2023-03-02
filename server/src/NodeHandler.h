#pragma once

#include <vector>

#include "NodeWrapper.h"

namespace Habitify
{
    class NodeHandler
    {
    public:
        NodeHandler() = default;
        ~NodeHandler() = default;

        HabCom::Status HandleNode(const HabCom::Node*);
        HabCom::Status SaveNode();
        
    private:
        std::vector<std::shared_ptr<NodeWrapper>> mvec_node_wrappers;
    };    
}