#pragma once

#include "imgui.h"
#include "utils/Layer.h"

namespace Habitify
{
    class Board : public Habitify::Layer
    {
    public: 
        virtual void OnUIRender() override;
    };
} // namespace Habitify
