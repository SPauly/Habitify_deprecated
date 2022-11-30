#pragma once

#include <vector>
#include <memory>

#include "imgui.h"
#include "utils/Layer.h"
#include "utils/Node.h"

namespace Habitify
{
    class Board : public Habitify::Layer
    {
    public: 
        virtual void OnUIRender() override;
    private:
        std::vector<std::shared_ptr<Node_>> mvec_nodes;
    };
} // namespace Habitify
