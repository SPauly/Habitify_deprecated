#pragma once

#include <vector>
#include <memory>

#include "imgui.h"
#include "utils/LayerStack.h"
#include "utils/Node.h"

namespace Habitify
{
    class Board : public Habitify::Layer
    {
    public: 
        virtual void OnUIRender() override;
        inline const std::vector<std::shared_ptr<Node>> *get_node_vec()
        {
            return &mvec_nodes;
        }
    private:
        LayerStack m_layer_stack;
        std::vector<std::shared_ptr<Node>> mvec_nodes;
    };
} // namespace Habitify
