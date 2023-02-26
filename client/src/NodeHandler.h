#pragma once

#include "NodeWrapper.h"
#include "utils\Node.h"
#include "utils\LayerStack.h"

namespace Habitify
{
    class NodeHandler
    {
    public:
        NodeHandler(const LayerStack*);
        ~NodeHandler() = default;

    private:
        const LayerStack *ptr_layer_stack; 
        std::vector<std::shared_ptr<Habitify::Node>> mvec_nodes;
    };

}
