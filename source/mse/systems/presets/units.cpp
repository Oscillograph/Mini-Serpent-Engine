#include <mse/systems/presets/units.h>

#include <mse/systems/scenes/scene.h>
#include <mse/systems/windows/layers/layer.h>
#include <mse/systems/windows/window.h>
#include <mse/systems/ecs/components.h>
#include <mse/systems/platform/renderer/texture.h>
#include <mse/systems/platform/input/input.h>
#include <mse/systems/resources/resource_manager.h>

// TODO: fix Resource users (Scene* -> Window*)

namespace mse
{
	namespace Arcade
	{
		// ================== Unit
		
		Unit::Unit(
			Scene* scene,
			Window* window,
			const std::string& name,
			const std::string& spritePath,
			const KBControls& keyboardControls,
			const glm::vec2& transformPosition,
			const glm::vec2& transformSize,
			const glm::vec2& spriteTilingFactor,
			const glm::uvec3& spriteColorKey,
			const glm::vec2& physicalPosition
			)
		{
			m_Entity = scene->CreateEntity(name);
			m_EntityID = m_Entity->GetID();
			m_Scene = scene;
			m_user = window;
			
			// physical position
			PositionComponent& position = m_Entity->AddComponent<PositionComponent>(10, 0);
			position.direction = 1; // 1 means right, -1 means left
			MSE_CORE_LOG("Entity ID: ", (uint32_t)(m_Entity->GetID()), "\; Position: ", m_Entity->GetComponent<PositionComponent>().x, ", ", m_Entity->GetComponent<PositionComponent>().y);
			
			// screen representation
			TransformComponent& transform = m_Entity->AddComponent<TransformComponent>();
			transform.position = transformPosition;
			transform.size = transformSize;
			transform.Normalize({
				window->GetPrefs().width,
				window->GetPrefs().height,
			});
			
			// state machine setup
			// TODO: Allow set up states manually
			StateMachineComponent& stateMachine = m_Entity->AddComponent<StateMachineComponent>();
			State* walkState = stateMachine.AddState(EntityStates::WALK);
			State* standState = stateMachine.AddState(EntityStates::STAND);
			State* jumpState = stateMachine.AddState(EntityStates::JUMP);
			State* flyState = stateMachine.AddState(EntityStates::FLY);
			State* fallState = stateMachine.AddState(EntityStates::FALL);
			
			walkState->AllowEntryFrom(standState->data);
			walkState->AllowEntryFrom(fallState->data);
			walkState->AllowExitTo(standState->data);
			walkState->AllowExitTo(jumpState->data);
			
			standState->AllowEntryFrom(walkState->data);
			standState->AllowEntryFrom(fallState->data);
			standState->AllowExitTo(walkState->data);
			standState->AllowExitTo(jumpState->data);
			
			jumpState->AllowEntryFrom(standState->data);
			jumpState->AllowEntryFrom(walkState->data);
			jumpState->AllowExitTo(flyState->data);
			
			flyState->AllowEntryFrom(jumpState->data);
			flyState->AllowExitTo(fallState->data);
			
			fallState->AllowEntryFrom(flyState->data);
			fallState->AllowExitTo(standState->data);
			fallState->AllowExitTo(walkState->data);
			
			stateMachine.SetState(EntityStates::STAND);
			
			// controls setup
			KeyBoardComponent& keyboard = m_Entity->AddComponent<KeyBoardComponent>(keyboardControls);
			
			// sprite control
			if (spritePath.length() > 0){
				m_SpriteTexture = ResourceManager::UseTexture(
					spritePath, 
					window,
					{spriteColorKey.x, spriteColorKey.y, spriteColorKey.z}
					);
				
				SpriteComponent& spriteComponent = m_Entity->AddComponent<SpriteComponent>((Texture*)(m_SpriteTexture->data));
				spriteComponent.tilingFactor = {spriteTilingFactor.x, spriteTilingFactor.y};
			}
			
			// physics setup
			PhysicsComponent& physicsComponent = m_Entity->AddComponent<PhysicsComponent>();
			physicsComponent.position = {physicalPosition.x, physicalPosition.y, 0};
			
			PhysicsHitBox hitbox;
			hitbox.hitBoxType = PhysicsDefines::HitBoxType::Circle;
			hitbox.points = {{0.0f, -5.0f}};
			hitbox.pointsNormalized = {{0.0f, 0.0f}};
			hitbox.radius = 10.0f;
			
			physicsComponent.entity = *m_Entity;
			physicsComponent.hitBoxes.push_back(hitbox);
			physicsComponent.velocity = {0.0f, 0.0f};
			physicsComponent.acceleration = {0.0f, 0.0f};
			physicsComponent.mass = 1.0f;
			physicsComponent.friction = 0.1f;
			physicsComponent.bodyType = mse::PhysicsDefines::BodyType::Dynamic;
			scene->GetPhysicsProcessor()->RegisterEntity(m_Entity);
		}
		
		Unit::~Unit()
		{
			ResourceManager::DropResource(m_SpriteTexture, m_user);
			m_SpriteTexture = nullptr;
			m_user = nullptr;
			
			MSE_CORE_ASSERT(m_Entity, "Unit destructor: m_Entity was a null pointer.");
			delete m_Entity;
			m_Entity = nullptr;
		}
		
		// animation addon to sprite control
		void Unit::SetAnimations(
			std::vector<int> animationStates, 
			glm::uvec2 frameSize, 
			float framesPerSecond,
			bool loopAnimations
			)
		{
			// MSE_CORE_LOG("Setup animations for ", m_Entity->GetComponent<NameComponent>().value);
			// MSE_CORE_LOG("m_SpriteTexture w-h: ", m_SpriteTexture->GetWidth(), "; ", m_SpriteTexture->GetHeight());
			// MSE_CORE_LOG("frameSize: ", frameSize.x, "; ", frameSize.y);
			
			AnimationComponent& animationComponent = m_Entity->AddComponent<AnimationComponent>();
			
			glm::uvec2 startPoint = {0 , 0};
			glm::uvec2 endPoint = {0, 0};
			
			for (int i = 0; i < animationStates.size(); i++)
			{
				startPoint = {0, i * frameSize.y};
				endPoint = {((Texture*)(m_SpriteTexture->data))->GetWidth(), (i * frameSize.y + frameSize.y)};
				// MSE_CORE_LOG("Animation #", animationStates[i]);
				// MSE_CORE_LOG("Start at: ", startPoint.x, "; ", startPoint.y);
				// MSE_CORE_LOG("End at: ", endPoint.x, "; ", endPoint.y);
				animationComponent.Add(
					animationStates[i], 
					new AnimationFrames( 
						{startPoint.x, startPoint.y}, 
						{endPoint.x, endPoint.y},
						frameSize.x, 
						frameSize.y, 
						framesPerSecond, 
						loopAnimations
						)
					);
			}
			
			animationComponent.Set(EntityStates::STAND1);
			animationComponent.Start();
		}
		
		void Unit::ChangeDirection(int direction)
		{
			// 1 means right, -1 means left
			m_Entity->GetComponent<PositionComponent>().direction = direction;
		}
		
		void Unit::ChangePhysics(const glm::vec2& speedMax)
		{
		}
		
		// ================== SpriteUnit
		
		SpriteUnit::SpriteUnit(
			Scene* scene,
			Window* window,
			const std::string& name,
			const std::string& spritePath,
			const KBControls& keyboardControls,
			const glm::vec2& transformPosition,
			const glm::vec2& transformSize,
			const glm::vec2& spriteTilingFactor,
			const glm::uvec3& spriteColorKey
			)
		{
			m_Entity = scene->CreateEntity(name);
			m_EntityID = m_Entity->GetID();
			m_Scene = scene;
			m_user = window;
			
			// physical position
			PositionComponent& position = m_Entity->AddComponent<PositionComponent>(10, 0);
			position.direction = 1; // 1 means right, -1 means left
			MSE_CORE_LOG("Entity ID: ", (uint32_t)(m_Entity->GetID()), "\; Position: ", m_Entity->GetComponent<PositionComponent>().x, ", ", m_Entity->GetComponent<PositionComponent>().y);
			
			// screen representation
			TransformComponent& transform = m_Entity->AddComponent<TransformComponent>();
			transform.position = transformPosition;
			transform.size = transformSize;
			transform.Normalize({
				window->GetPrefs().width,
				window->GetPrefs().height,
			});
			
			// state machine setup
			// TODO: Allow set up states manually
			StateMachineComponent& stateMachine = m_Entity->AddComponent<StateMachineComponent>();
			State* walkState = stateMachine.AddState(EntityStates::WALK);
			State* standState = stateMachine.AddState(EntityStates::STAND);
			State* jumpState = stateMachine.AddState(EntityStates::JUMP);
			State* flyState = stateMachine.AddState(EntityStates::FLY);
			State* fallState = stateMachine.AddState(EntityStates::FALL);
			
			walkState->AllowEntryFrom(standState->data);
			walkState->AllowEntryFrom(fallState->data);
			walkState->AllowExitTo(standState->data);
			walkState->AllowExitTo(jumpState->data);
			
			standState->AllowEntryFrom(walkState->data);
			standState->AllowEntryFrom(fallState->data);
			standState->AllowExitTo(walkState->data);
			standState->AllowExitTo(jumpState->data);
			
			jumpState->AllowEntryFrom(standState->data);
			jumpState->AllowEntryFrom(walkState->data);
			jumpState->AllowExitTo(flyState->data);
			
			flyState->AllowEntryFrom(jumpState->data);
			flyState->AllowExitTo(fallState->data);
			
			fallState->AllowEntryFrom(flyState->data);
			fallState->AllowExitTo(standState->data);
			fallState->AllowExitTo(walkState->data);
			
			stateMachine.SetState(EntityStates::STAND);
			
			// controls setup
			KeyBoardComponent& keyboard = m_Entity->AddComponent<KeyBoardComponent>(keyboardControls);
			
			// sprite control
			if (spritePath.length() > 0){
				m_SpriteTexture = ResourceManager::UseTexture(
					spritePath, 
					window,
					{spriteColorKey.x, spriteColorKey.y, spriteColorKey.z}
					);
				
				SpriteComponent& spriteComponent = m_Entity->AddComponent<SpriteComponent>((Texture*)(m_SpriteTexture->data));
				spriteComponent.tilingFactor = {spriteTilingFactor.x, spriteTilingFactor.y};
			}
		}
		
		SpriteUnit::~SpriteUnit()
		{
			ResourceManager::DropResource(m_SpriteTexture, m_user);
			m_SpriteTexture = nullptr;
			m_user = nullptr;
			
			MSE_CORE_ASSERT(m_Entity, "Unit destructor: m_Entity was a null pointer.");
			delete m_Entity;
			m_Entity = nullptr;
		}
		
		// animation addon to sprite control
		void SpriteUnit::SetAnimations(
			std::vector<int> animationStates, 
			glm::uvec2 frameSize, 
			float framesPerSecond,
			bool loopAnimations
			)
		{
			// MSE_CORE_LOG("Setup animations for ", m_Entity->GetComponent<NameComponent>().value);
			// MSE_CORE_LOG("m_SpriteTexture w-h: ", m_SpriteTexture->GetWidth(), "; ", m_SpriteTexture->GetHeight());
			// MSE_CORE_LOG("frameSize: ", frameSize.x, "; ", frameSize.y);
			
			AnimationComponent& animationComponent = m_Entity->AddComponent<AnimationComponent>();
			
			glm::uvec2 startPoint = {0 , 0};
			glm::uvec2 endPoint = {0, 0};
			
			for (int i = 0; i < animationStates.size(); i++)
			{
				startPoint = {0, i * frameSize.y};
				endPoint = {((Texture*)(m_SpriteTexture->data))->GetWidth(), (i * frameSize.y + frameSize.y)};
				// MSE_CORE_LOG("Animation #", animationStates[i]);
				// MSE_CORE_LOG("Start at: ", startPoint.x, "; ", startPoint.y);
				// MSE_CORE_LOG("End at: ", endPoint.x, "; ", endPoint.y);
				animationComponent.Add(
					animationStates[i], 
					new AnimationFrames( 
						{startPoint.x, startPoint.y}, 
						{endPoint.x, endPoint.y},
						frameSize.x, 
						frameSize.y, 
						framesPerSecond, 
						loopAnimations
						)
					);
			}
			
			animationComponent.Set(EntityStates::STAND1);
			animationComponent.Start();
		}
		
		void SpriteUnit::ChangeDirection(int direction)
		{
			// 1 means right, -1 means left
			m_Entity->GetComponent<PositionComponent>().direction = direction;
		}
		
		void SpriteUnit::ChangePhysics(const glm::vec2& speedMax)
		{
		}
		
		// ================== Doodad
		
		Doodad::Doodad(
			Scene* scene,
			Window* window,
			const std::string& name,
			const std::string& spritePath,
			const KBControls& keyboardControls,
			const glm::vec2& transformPosition,
			const glm::vec2& transformSize,
			const glm::vec2& spriteTilingFactor,
			const glm::uvec3& spriteColorKey,
			const glm::vec2& physicalPosition,
			const std::vector<glm::vec2>& hitboxPoints
			)
		{
			m_Entity = scene->CreateEntity(name);
			m_user = window;
			
			// physical position
			PositionComponent& position = m_Entity->AddComponent<PositionComponent>(10, 0);
			position.direction = 1; // 1 means right, -1 means left
			
			// screen representation
			TransformComponent& transform = m_Entity->AddComponent<TransformComponent>();
			transform.position = transformPosition;
			transform.size = transformSize;
			transform.Normalize({
				window->GetPrefs().width,
				window->GetPrefs().height,
			});
			
			// state machine setup
			// TODO: Allow set up states manually
			StateMachineComponent& stateMachine = m_Entity->AddComponent<StateMachineComponent>();
			State* walkState = stateMachine.AddState(EntityStates::WALK);
			State* standState = stateMachine.AddState(EntityStates::STAND);
			State* jumpState = stateMachine.AddState(EntityStates::JUMP);
			State* flyState = stateMachine.AddState(EntityStates::FLY);
			State* fallState = stateMachine.AddState(EntityStates::FALL);
			
			walkState->AllowEntryFrom(standState->data);
			walkState->AllowEntryFrom(fallState->data);
			walkState->AllowExitTo(standState->data);
			walkState->AllowExitTo(jumpState->data);
			
			standState->AllowEntryFrom(walkState->data);
			standState->AllowEntryFrom(fallState->data);
			standState->AllowExitTo(walkState->data);
			standState->AllowExitTo(jumpState->data);
			
			jumpState->AllowEntryFrom(standState->data);
			jumpState->AllowEntryFrom(walkState->data);
			jumpState->AllowExitTo(flyState->data);
			
			flyState->AllowEntryFrom(jumpState->data);
			flyState->AllowExitTo(fallState->data);
			
			fallState->AllowEntryFrom(flyState->data);
			fallState->AllowExitTo(standState->data);
			fallState->AllowExitTo(walkState->data);
			
			stateMachine.SetState(EntityStates::STAND);
			
			// controls setup
			KeyBoardComponent& keyboard = m_Entity->AddComponent<KeyBoardComponent>(keyboardControls);
			
			// sprite control
			if (spritePath.length() > 1){
				m_SpriteTexture = ResourceManager::UseTexture(
					spritePath, 
					window, 
					{spriteColorKey.x, spriteColorKey.y, spriteColorKey.z}
					);
				SpriteComponent& spriteComponent = m_Entity->AddComponent<SpriteComponent>((Texture*)(m_SpriteTexture->data));
				spriteComponent.tilingFactor = {spriteTilingFactor.x, spriteTilingFactor.y};
				// MSE_CORE_LOG("Sprite added");
			} else {
				// MSE_CORE_LOG("Sprite was not added");
			}
			// MSE_CORE_LOG("Sprite component section passed");
			
			// physics setup
			PhysicsComponent& physicsComponent = m_Entity->AddComponent<PhysicsComponent>();
			physicsComponent.position = {physicalPosition.x, physicalPosition.y, 0};
			
			PhysicsHitBox hitbox;
			hitbox.hitBoxType = PhysicsDefines::HitBoxType::Rectangle;
			hitbox.points = hitboxPoints;
			hitbox.pointsNormalized = hitboxPoints;
			
			physicsComponent.entity = *m_Entity;
			physicsComponent.hitBoxes.push_back(hitbox);
			physicsComponent.velocity = {0.0f, 0.0f};
			physicsComponent.acceleration = {0.0f, 0.0f};
			physicsComponent.mass = 1.0f;
			physicsComponent.friction = 0.1f;
			physicsComponent.bodyType = mse::PhysicsDefines::BodyType::Static;
			scene->GetPhysicsProcessor()->RegisterEntity(m_Entity);
		}
		
		Doodad::~Doodad()
		{
			ResourceManager::DropResource(m_SpriteTexture, m_user);
			m_SpriteTexture = nullptr;
			m_user = nullptr;
			
			MSE_CORE_ASSERT(m_Entity, "Doodad destructor: m_Entity was a null pointer.");
			delete m_Entity;
			m_Entity = nullptr;
		}
		
		// animation addon to sprite control
		void Doodad::SetAnimations(
			std::vector<int> animationStates, 
			glm::uvec2 frameSize, 
			float framesPerSecond,
			bool loopAnimations
			)
		{
			MSE_CORE_LOG("Setup animations for ", m_Entity->GetComponent<NameComponent>().value);
			// MSE_CORE_LOG("m_SpriteTexture w-h: ", m_SpriteTexture->GetWidth(), "; ", m_SpriteTexture->GetHeight());
			// MSE_CORE_LOG("frameSize: ", frameSize.x, "; ", frameSize.y);
			
			AnimationComponent& animationComponent = m_Entity->AddComponent<AnimationComponent>();
			
			glm::uvec2 startPoint = {0 , 0};
			glm::uvec2 endPoint = {0, 0};
			
			for (int i = 0; i < animationStates.size(); i++)
			{
				if (m_SpriteTexture != nullptr)
				{
					startPoint = {0, i * frameSize.y};
					endPoint = {((Texture*)(m_SpriteTexture->data))->GetWidth(), (i * frameSize.y + frameSize.y)};
				}
				
				// MSE_CORE_LOG("Animation #", animationStates[i]);
				// MSE_CORE_LOG("Start at: ", startPoint.x, "; ", startPoint.y);
				// MSE_CORE_LOG("End at: ", endPoint.x, "; ", endPoint.y);
				animationComponent.Add(
					animationStates[i], 
					new AnimationFrames( 
						{startPoint.x, startPoint.y}, 
						{endPoint.x, endPoint.y},
						frameSize.x, 
						frameSize.y, 
						framesPerSecond, 
						loopAnimations
						)
					);
			}
			
			animationComponent.Set(EntityStates::STAND1);
			animationComponent.Start();
		}
		
		void Doodad::ChangeDirection(int direction)
		{
			// 1 means right, -1 means left
			m_Entity->GetComponent<PositionComponent>().direction = direction;
		}
		
		void Doodad::ChangePhysics(const glm::vec2& speedMax)
		{
		}
		
		// ================== Decoration
		
		Decoration::Decoration(
			Scene* scene,
			Window* window,
			const std::string& name,
			const std::string& spritePath, 
			const glm::vec2& place,
			const glm::vec2& size,
			const glm::vec2& tilingFactor,
			const glm::uvec3& colorKey
			)
		{
			m_Entity = scene->CreateEntity(name);
			
			// screen position
			TransformComponent& transform = m_Entity->AddComponent<TransformComponent>();
			transform.position = {place.x, place.y};
			transform.size = {size.x, size.y};
			transform.Normalize({
				window->GetPrefs().width,
				window->GetPrefs().height,
			});
			
			// sprite control
			m_SpriteTexture = ResourceManager::UseTexture(
				spritePath, 
				window, 
				{colorKey.x, colorKey.y, colorKey.z}
				);
			SpriteComponent& sprite = m_Entity->AddComponent<SpriteComponent>((Texture*)(m_SpriteTexture->data));
			sprite.tilingFactor = {tilingFactor.x, tilingFactor.y};
		}
		
		Decoration::~Decoration()
		{
			ResourceManager::DropResource(m_SpriteTexture, m_user);
			m_SpriteTexture = nullptr;
			m_user = nullptr;
			
			MSE_CORE_ASSERT(m_Entity, "Decoration destructor: m_Entity was a null pointer.");
			delete m_Entity;
			m_Entity = nullptr;
		}
		
		void Decoration::ChangeAppearance(
			const std::string& spritePath, 
			const glm::vec2& place,
			const glm::vec2& size,
			const glm::vec2& tilingFactor
			)
		{
		}
	}
}

