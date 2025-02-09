#ifndef MSE_SYSTEMS_PRESETS_UNITS_H
#define MSE_SYSTEMS_PRESETS_UNITS_H

#include <mse/core.h>
#include <mse/systems/platform/platform.h>
#include <mse/systems/ecs/entity.h>

namespace mse
{
	typedef std::unordered_map<int, SDL_Keycode> 	KBControls;
	
	namespace Arcade
	{
		// ================== Unit - can be controlled and interacted with
		
		class Unit : public Entity
		{
		public:
			Unit() = default;
			Unit(
				Scene* scene,
				Window* window,
				const std::string& name,
				const std::string& spritePath,
				const KBControls& keyboardControls,
				const glm::vec2& transformPosition,
				const glm::vec2& transformSize,
				const glm::vec2& spriteTilingFactor = {0.0f, 0.0f},
				const glm::uvec3& spriteColorKey = {0, 0, 0},
				const glm::vec2& physicalPosition = {100, 100}
				);
			~Unit();
			
			void InitBackend();
			void InitFrontend();
			
			void SetAnimations(
				std::vector<int> animationStates, 
				glm::uvec2 frameSize, 
				float framesPerSecond = 20.0f,
				bool loopAnimations = true
				);
			void ChangeDirection(int direction);
			void ChangePhysics(const glm::vec2& speedMax);
			
		protected:
			Entity* m_Entity = nullptr;
			Resource* m_SpriteTexture = nullptr;
			Window* m_window = nullptr;
			std::string m_name = "";
			std::string m_spritePath = "";
			KBControls m_keyboardControls = {};
			glm::vec2 m_transformPosition = {0.0f, 0.0f};
			glm::vec2 m_transformSize = {0.0f, 0.0f};
			glm::vec2 m_spriteTilingFactor = {0.0f, 0.0f};
			glm::uvec3 m_spriteColorKey = {0, 0, 0};
			glm::vec2 m_physicalPosition = {100, 100};
			glm::vec2 m_SpeedMax = {25.0f, 25.0f}; // meters per second
		};
		
		// ================== SpriteUnit - can be controlled and interacted with, has no physics
		
		class SpriteUnit : public Entity
		{
		public:
			SpriteUnit() = default;
			SpriteUnit(
				Scene* scene,
				Window* window,
				const std::string& name,
				const std::string& spritePath,
				const KBControls& keyboardControls,
				const glm::vec2& transformPosition,
				const glm::vec2& transformSize,
				const glm::vec2& spriteTilingFactor = {0.0f, 0.0f},
				const glm::uvec3& spriteColorKey = {0, 0, 0}
				);
			~SpriteUnit();
			
			void SetAnimations(
				std::vector<int> animationStates, 
				glm::uvec2 frameSize, 
				float framesPerSecond = 20.0f,
				bool loopAnimations = true
				);
			void ChangeDirection(int direction);
			void ChangePhysics(const glm::vec2& speedMax);
			
		protected:
			Entity* m_Entity = nullptr;
			Resource* m_SpriteTexture = nullptr;
			Window* m_user = nullptr;
			glm::vec2 m_SpeedMax = {25.0f, 25.0f}; // meters per second
		};
		
		// ================== Doodad - can't be controlled, but can be interacted with
		
		class Doodad : public Entity
		{
		public:
			Doodad() = default;
			Doodad(
				Scene* scene,
				Window* window,
				const std::string& name,
				const std::string& spritePath,
				const KBControls& keyboardControls,
				const glm::vec2& transformPosition,
				const glm::vec2& transformSize,
				const glm::vec2& spriteTilingFactor = {0.0f, 0.0f},
				const glm::uvec3& spriteColorKey = {0, 0, 0},
				const glm::vec2& physicalPosition = {100, 100},
				const std::vector<glm::vec2>& hitboxPoints = {{-0.01f, 2.0f}, {0.01f, 2.0f}, {0.01f, 0.0f}, {-0.01f, 0.0f}}
				);
			~Doodad();
			
			void SetAnimations(
				std::vector<int> animationStates, 
				glm::uvec2 frameSize, 
				float framesPerSecond = 20.0f,
				bool loopAnimations = true
				);
			void ChangeDirection(int direction);
			void ChangePhysics(const glm::vec2& speedMax);
			
		protected:
			Entity* m_Entity = nullptr;
			Resource* m_SpriteTexture = nullptr;
			Window* m_user = nullptr;
			glm::vec2 m_SpeedMax = {25.0f, 25.0f}; // meters per second
		};
		
		// ================== Decoration - can't be interacted with, can't be controlled
		
		class Decoration : public Entity
		{
		public:
			Decoration() = default;
			Decoration(
				Scene* scene,
				Window* window,
				const std::string& name,
				const std::string& spritePath, 
				const glm::vec2& place,
				const glm::vec2& size,
				const glm::vec2& tilingFactor = {0.0f, 0.0f},
				const glm::uvec3& colorKey = {0, 0, 0}
				);
			~Decoration();
			
			void ChangeAppearance(
				const std::string& spritePath, 
				const glm::vec2& place,
				const glm::vec2& size,
				const glm::vec2& tilingFactor
				);
			
		protected:
			Entity* m_Entity = nullptr;
			Resource* m_SpriteTexture = nullptr;
			Window* m_user = nullptr;
		};
	}
}

#endif

