#include "LayerStack.h"
#include <algorithm>

namespace Habitify
{
    LayerStack::~LayerStack()
	{
		for (Layer* layer : mvec_layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		mvec_layers.emplace(mvec_layers.begin() + m_layer_insert_index, layer);
		m_layer_insert_index++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		mvec_layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(mvec_layers.begin(), mvec_layers.begin() + m_layer_insert_index, layer);
		if (it != mvec_layers.begin() + m_layer_insert_index)
		{
			layer->OnDetach();
			mvec_layers.erase(it);
			m_layer_insert_index--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(mvec_layers.begin() + m_layer_insert_index, mvec_layers.end(), overlay);
		if (it != mvec_layers.end())
		{
			overlay->OnDetach();
			mvec_layers.erase(it);
		}
	}
}