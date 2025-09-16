#include <mse/systems/windows/layers/layer_manager.h>
#include <mse/systems/windows/window.h>
#include <mse/systems/windows/layers/layer.h>
#include <mse/systems/windows/layers/gui/gui.h>
#include <mse/systems/platform/platform.h>

namespace mse
{
	LayerManager::LayerManager(Window* window)
	: m_window(window)
	{
		Init();
		MSE_CORE_LOG("Layer manager constructed");
	}
	
	LayerManager::~LayerManager()
	{
		Shutdown();
		
		MSE_CORE_LOG("Layer manager destroyed for window \"", m_window->m_basePrefs.title, "\"");
	}
	
	void LayerManager::Init()
	{
		MSE_CORE_LOG("Layer manager initialization for window \"", m_window->m_title, "\"");
		m_screen.resize(m_window->GetPrefs().width * m_window->GetPrefs().height);
		m_layerInsertIndex = 0;
	}
	
	void LayerManager::Attach(Layer* layer)
	{
		if (layer != nullptr)
		{
			auto it = std::find(m_layers.begin(), m_layers.end(), layer);
			if (it == m_layers.end())
			{
				m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer);
				layer->Attach(m_window);
				layer->Init();
				m_layerInsertIndex++;
				MSE_CORE_LOG("Layer manager: attached a layer");
				return;
			} else {
				MSE_CORE_ERROR("Layer manager: layer duplication is forbidden");
			}
		} else {
			MSE_CORE_ERROR("Layer manager: cannot attach a nullptr to window \"", m_window->m_title, "\"");
		}
	}
	
	void LayerManager::Detach(Layer* layer)
	{
		if (layer != nullptr)
		{
			auto it = std::find(m_layers.begin(), m_layers.end(), layer);
			if (it != m_layers.end())
			{
				m_layers.erase(it);
				layer->Detach();
				delete layer;
				m_layerInsertIndex--;
				MSE_CORE_LOG("Layer manager: detached a layer");
			} else {
				MSE_CORE_ERROR("Layer manager: cannot find the layer to detach");
			}
		} else {
			MSE_CORE_ERROR("Layer manager: cannot detach a nullptr from window \"", m_window->m_title, "\"");
		}
	}
	
	bool LayerManager::HandleEvent(EventTypes eventType, SDL_Event* event)
	{
		bool handled = false;
		
		int elementId = -1;
		
		if ((event->type == SDL_MOUSEMOTION) || (event->type == SDL_MOUSEBUTTONDOWN) || (event->type == SDL_MOUSEBUTTONUP) || (event->type == SDL_MOUSEWHEEL))
		{
			// map real mouse coordinates to window basePrefs
			int mouseX = 0;
			int mouseY = 0;
			
			mouseX = (int)floorf(event->motion.x / m_window->GetScale().x);
			mouseY = (int)floorf(event->motion.y / m_window->GetScale().y);
			
			elementId = m_screen[mouseX + mouseY * m_window->GetPrefs().width];
		}
        
//		MSE_CORE_LOG("Layer manager: handling an event for elemnetId=", elementId);
		
		for (int i = m_layers.size() - 1; (i >= 0) && !handled; --i)
		{
			if (m_layers[i] != nullptr)
			{
//                printf("%d -- event sent...", elementId);
				handled = m_layers[i]->HandleEvent(eventType, event, (elementId == -1) ? m_layers[i]->elementInFocus : elementId);
//                printf("... event handled.\n");
			}
		}
		
		return handled;
	}
	
	void LayerManager::UpdateScreen()
	{
//		MSE_CORE_LOG("Layer manager: updating screen");
		for (int i = 0; i < m_screen.size(); ++i)
		{
			m_screen[i] = -1;
		}
		
		for (Layer* layer : m_layers)
		{
			if (layer != nullptr)
			{
				if (layer->enabled)
				{
					layer->Update();
					
					int winWidth = m_window->GetPrefs().width;
					int winHeight = m_window->GetPrefs().height;
					int x_screen, x_mask = 0;
					int y_screen, y_mask = 0;
					for (std::pair<int, GUIItem*> item : layer->GetElements())
					{
						for (x_screen = item.second->layerArea.x, x_mask = 0;
							(x_screen < item.second->layerArea.x + item.second->layerArea.z) && (x_screen < winWidth);
							++x_screen, ++x_mask)
						{
							for (y_screen = item.second->layerArea.y, y_mask = 0;
								(y_screen < item.second->layerArea.y + item.second->layerArea.w) && (y_screen < winHeight);
								++y_screen, ++y_mask)
							{
//								MSE_CORE_LOG("x_screen=", x_screen, "; y_screen=", y_screen, "; x_mask=", x_mask, "; y_mask=", y_mask);
								m_screen[x_screen + y_screen * winWidth] = item.second->layerMask[x_mask + y_mask];
							}
						}
					}
				}
			}
		}
//		MSE_CORE_LOG("Layer manager: screen updated");
	}
	
	void LayerManager::Display()
	{
		for (int i = 0; i < m_layers.size(); ++i)
		{
			if (m_layers[i] != nullptr)
			{
				m_layers[i]->Display();
			}
		} 
	}
	
	void LayerManager::Shutdown()
	{
		for (int i = 0; i < m_layers.size(); ++i)
		{
			delete m_layers[i];
			m_layers[i] = nullptr;
		}
		m_layerInsertIndex = 0;
	}
}
