#include <mse/systems/windows/layers/layer.h>
#include <mse/systems/windows/layers/layer_manager.h>
#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/events/events.h>
#include <mse/systems/windows/window.h>
#include <mse/systems/windows/layers/gui/gui.h>

namespace mse
{
	Layer::Layer()
	{
		m_elements.reserve(32);
	}

	Layer::~Layer()
	{
		m_window = nullptr;
		for (std::pair<int, GUIItem*> element : m_elements)
		{
			delete element.second;
			m_elements[element.first] = nullptr;
		}
		MSE_CORE_LOG("Layer destroyed, along with ", m_elements.size(), " gui elements");
	}

	void Layer::OnAttach()
	{
	}

	void Layer::Attach(Window* window)
	{
		if (window != nullptr)
		{
			OnAttach();
			
			m_window = window;
			
			// fill the screen with ids of elements
			m_screenWidth = m_window->GetPrefs().width;
			m_screenHeight = m_window->GetPrefs().height;
//			m_screen.resize(m_screenWidth * m_screenHeight);
//			m_screen.assign(m_screenWidth * m_screenHeight, -1); // make a "blank" screen
//			for (std::pair<int, GUIItem*> element : m_elements)
//			{
//				int xmin = element.second->x;
//				int xmax = xmin + element.second->width;
//				for (int x = xmin; (x < xmax) && (x < m_screenWidth); ++x)
//				{
//					int ymin = element.second->y;
//					int ymax = ymin + element.second->height;
//					for (int y = ymin; (y < ymax) && (y < m_screenHeight); ++y)
//					{
//						m_screen[x + y*m_screenWidth] = element.first;
//					}
//				}
//			}
			
			MSE_CORE_LOG("Layer: attached to window ", m_window->GetPrefs().title);
		} else {
			MSE_CORE_ERROR("Layer: cannot attach a layer to nullptr");
		}
	}
	
	void Layer::OnInit()
	{}
	
	void Layer::Init()
	{
		OnInit();
		MSE_CORE_LOG("Layer: Initialized");
	}

	void Layer::OnDetach()
	{}

	void Layer::Detach()
	{
		OnDetach();
		
		MSE_CORE_LOG("Layer: detached from window ", m_window->GetPrefs().title);
		
		m_window = nullptr;
		
		// clear the screen
		m_screenWidth = 0;
		m_screenHeight = 0;
//		m_screen.resize(m_screenWidth * m_screenHeight);
	}

	void Layer::OnUpdate()
	{}

	void Layer::Update()
	{
		OnUpdate();
	}

	void Layer::OnDisplay()
	{}

	void Layer::Display()
	{
		OnDisplay();
		
		for(const std::pair<int, GUIItem*>& element : m_elements)
		{
			if (element.second != nullptr)
			{
				element.second->Display();
			}
		}
	}

	bool Layer::HandleEvent(EventTypes eventType, SDL_Event* event, int elementId)
	{
//		MSE_CORE_LOG("Layer: event received for elementId=", elementId);
		// layer-specific events
		if (elementId == -1)
		{
			switch (eventType)
			{
				case EventTypes::MouseMoved:
				{
					// process moving mouse away from the previous element area
					if (m_window->GetLayerManager()->m_mouseOverElementID != -1)
					{
						m_elements[m_window->GetLayerManager()->m_mouseOverElementID]->HandleEvent(EventTypes::GUIItemMouseOut, event);
						m_window->GetLayerManager()->m_mouseOverElementID = elementId;
					}
					break;
				}
//				case EventTypes::MouseButtonDown:
//				{
//					if (m_window->GetLayerManager()->m_mouseOverElementID != -1)
//					{
//						m_elements[m_window->GetLayerManager()->m_mouseOverElementID]->HandleEvent(EventTypes::GUIItemMouseButtonDown, event);
//						m_window->GetLayerManager()->m_mouseOverElementID = elementId;
//					}
//					break;
//				}
//				case EventTypes::MouseButtonUp:
//				{
//					if (m_window->GetLayerManager()->m_mouseOverElementID != -1)
//					{
//						m_elements[m_window->GetLayerManager()->m_mouseOverElementID]->HandleEvent(EventTypes::GUIItemMouseButtonUp, event);
//						m_window->GetLayerManager()->m_mouseOverElementID = elementId;
//					}
//					break;
//				}
			}
		} else {
			if (m_elements.find(elementId) != m_elements.end())
			{
//				MSE_CORE_LOG("Layer: found element with id=", elementId);
				// gui elements events
				switch (eventType)
				{
					// mouse can either move to the element, move out of it or do something over it
				case EventTypes::MouseMoved:
					{
						if (elementId != m_window->GetLayerManager()->m_mouseOverElementID)
						{
							// process moving mouse away from the previous element area
							if (m_window->GetLayerManager()->m_mouseOverElementID != -1)
							{
								m_elements[m_window->GetLayerManager()->m_mouseOverElementID]->HandleEvent(EventTypes::GUIItemMouseOut, event);
							}
							
							// handle the event of moving in
							m_window->GetLayerManager()->m_mouseOverElementID = elementId;
							return m_elements[elementId]->HandleEvent(EventTypes::GUIItemMouseOver, event);
						} else {
							// handle the event of just moving the cursor inside the element area
							return m_elements[elementId]->HandleEvent(EventTypes::GUIItemMouseMove, event);
						}
						break;
					}
				case EventTypes::MouseButtonDown:
					{
						return m_elements[elementId]->HandleEvent(EventTypes::GUIItemMouseButtonDown, event);
						break;
					}
				case EventTypes::MouseButtonUp:
					{
						return m_elements[elementId]->HandleEvent(EventTypes::GUIItemMouseButtonUp, event);
						break;
					}
				case EventTypes::MouseWheel:
					{
						return m_elements[elementId]->HandleEvent(EventTypes::GUIItemMouseWheel, event);
						break;
					}
				case EventTypes::KeyDown:
					{
						return m_elements[elementId]->HandleEvent(EventTypes::KeyDown, event);
						break;
					}
				case EventTypes::KeyUp:
					{
						return m_elements[elementId]->HandleEvent(EventTypes::KeyUp, event);
						break;
					}
				}
			}
		}
		
		return false;
	}

	Window* Layer::GetWindow()
	{
		return m_window;
	}
	
	GUIItem* Layer::AddElement(GUIItem* gui)
	{
		if (gui != nullptr)
		{
			m_elements[gui->id] = gui;
			MSE_CORE_LOG("Layer: added a gui element with id=", gui->id);
			return m_elements[gui->id];
		} else {
			MSE_CORE_ERROR("Layer: cannot add a gui element that is nullptr");
		}
		return nullptr;
	}
}
