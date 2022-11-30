#pragma once

#include "Layer.h"

#include <vector>

namespace Habitify
{
    class LayerStack
    {
    public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return mvec_layers.begin(); }
		std::vector<Layer*>::iterator end() { return mvec_layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return mvec_layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return mvec_layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return mvec_layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return mvec_layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return mvec_layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return mvec_layers.rend(); }
	private:
		std::vector<Layer*> mvec_layers;
		unsigned int m_layer_insert_index = 0;
    };
}