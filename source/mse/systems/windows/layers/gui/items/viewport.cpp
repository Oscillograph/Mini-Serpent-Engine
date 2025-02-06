#include <mse/systems/windows/layers/gui/items/viewport.h>
#include <mse/systems/windows/layers/layer.h>
#include <mse/systems/scenes/scene.h>
#include <mse/systems/cameras/camera2D.h>
#include <mse/systems/ecs/entity.h>
#include <mse/systems/ecs/components.h>
#include <mse/systems/ecs/components/states.h>
#include <mse/systems/physics/physics_processor.h>
#include <mse/systems/physics/world.h>

namespace mse
{
	namespace gui
	{
		// general initialization
		Viewport::Viewport()
		: GUIItem()
		{
			Init(nullptr, nullptr, nullptr, U"", {0, 0, 0, 0});
		}
		
		Viewport::Viewport(Layer* layer, Scene* toScene, Camera2D* withCamera, const std::u32string& text, const glm::uvec4& area)
		: GUIItem()
		{
			Init(layer, toScene, withCamera, text, area);
		}
		
		void Viewport::Init(Layer* layer, Scene* toScene, Camera2D* withCamera, const std::u32string& text, const glm::uvec4& area)
		{
			// model
			parentLayer = layer;
			scene = toScene;
			camera = withCamera;
			m_elementName = "Viewport " + utf8::utf32to8(text);
			m_text = text;
			layerArea = area;
			
			layerMask.resize(area.z * area.w);
			for (int x = 0; x < area.z; ++x)
			{
				for (int y = 0; y < area.w; ++y)
				{
					layerMask[x + y*area.z] = id;
				}
			}
			
			//view
			if ((layer != nullptr) && (scene != nullptr) && (camera != nullptr))
			{
				glm::ivec2 sceneConstraints = {0, 0};
				glm::ivec2 cameraConstraints = {0, 0};
//				glm::vec2 scaling = {0.0f, 0.0f};
				
				glm::ivec2 cameraPosition = {0, 0};
				
				if (scene->GetPhysicsProcessor()->AccessWorld(0) != nullptr)
				{
					sceneConstraints = scene->GetPhysicsProcessor()->AccessWorld(0)->properties.size;
					cameraConstraints = {
						camera->properties.z, 
						camera->properties.w
					};
					cameraPosition = {
						camera->properties.x, 
						camera->properties.y
					};
//					scaling = {
//						sceneConstraints.x / cameraConstraints.x, 
//						sceneConstraints.y / cameraConstraints.y
//					};
				}
			}
			
			// controller
			// setup interaction
			callbacks[EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
//				MSE_CORE_LOG(m_elementName, ": ...");
			};
			
			callbacks[EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
//				MSE_CORE_LOG(m_elementName, ": Yay, you clicked me!");
			};
			
			callbacks[EventTypes::GUIItemMouseOver] = [&](SDL_Event* event){
//				MSE_CORE_LOG(m_elementName, ": Hello, Mouse!");
			};
			
			callbacks[EventTypes::GUIItemMouseOut] = [&](SDL_Event* event){
//				MSE_CORE_LOG(m_elementName, ": Goodbye, Mouse!");
			};
		}
		
		Viewport::~Viewport()
		{}
		
		// general GUIItem interface
		void Viewport::Display()
		{}
		
		bool Viewport::HandleEvent(EventTypes eventType, SDL_Event* event)
		{}
		
		// unique Viewport interface
	}
}
