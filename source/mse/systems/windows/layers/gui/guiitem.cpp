#include <mse/systems/windows/layers/gui/guiitem.h>

namespace mse
{
	int GUIItem::m_idNext = 0;
	
	GUIItem::GUIItem()
	{
		id = m_idNext;
		m_idNext++;
		
		layerArea = {0, 0, 0, 0};
		layerMask.clear();
		callbacks.clear();
	}
	
	GUIItem::~GUIItem()
	{
		layerArea = {0, 0, 0, 0};
		layerMask.clear();
		callbacks.clear();
	}
	
	bool GUIItem::HandleEvent(EventTypes eventType, SDL_Event* event)
	{
//		MSE_CORE_LOG("GUIItem: event received by ", m_elementName);
		
		// only one callback per event type
		if (callbacks.find(eventType) != callbacks.end())
		{
//			MSE_CORE_LOG("GUIItem: callback found");
			callbacks[eventType](event);
			return true;
		}
		return false;
	}
	
	void GUIItem::OnDisplay()
	{
		HandleEvent(EventTypes::GUIItemDisplay, nullptr);
	}
	
	void GUIItem::Display()
	{
		// renderer commands 
	}
}
