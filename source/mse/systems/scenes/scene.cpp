#include <mse/systems/scenes/scene.h>
#include <mse/systems/ecs/entity.h>
#include <mse/systems/ecs/components.h>
#include <mse/systems/physics/physics_processor.h>

#include <mse/systems/application/application.h>
#include <mse/systems/platform/renderer/renderer.h>
//#include <mse/systems/platform/renderer/camera2d.h>
#include <mse/systems/windows/window.h>
#include <mse/systems/windows/layers/layer.h>

namespace mse
{
	Scene::Scene()
	{
//		m_SceneCamera = new Camera2D();
		
		PhysicsInit(PhysicsSystem::None);
	}
	
	Scene::Scene(const PhysicsSystem& physicsSystem)
	{
//		m_SceneCamera = new Camera2D();
		
		PhysicsInit(physicsSystem);
	}
	
	Scene::~Scene()
	{
		m_Layer = nullptr;
		
		MSE_CORE_ASSERT(m_SceneCamera, "Scene destructor: m_SceneCamera was a null pointer.");
		delete m_SceneCamera;
		m_SceneCamera = nullptr;
	}
	
	void Scene::OnInit()
	{
	}
	
	void Scene::Init()
	{
		MSE_CORE_LOG("Scene: Init...");
		MSE_CORE_LOG("- initialized? Answer: ", m_Initialized);
		MSE_CORE_LOG("- user-defined OnInit");
		OnInit();
		
		// ...
		m_Initialized = true;
		MSE_CORE_LOG("- initialized");
		MSE_CORE_LOG("- initialized? Answer: ", m_Initialized);
		
		MSE_CORE_LOG("- user-defined OnInitialized");
		OnInitialized();
	}
	
	void Scene::OnInitialized()
	{
	}
	
	void Scene::Load()
	{
		// ...
		MSE_CORE_LOG("Scene: Load...");
		if (m_PhysicsProcessor == nullptr)
		{
			MSE_CORE_LOG("- initialize physics processor and access its world");
			PhysicsInit(m_PhysicsSystem, false);
		}
		MSE_CORE_LOG("- loaded");
		
		MSE_CORE_LOG("- user-defined OnLoaded");
		OnLoaded();
	}
	
	void Scene::OnLoaded()
	{
	}
	
	void Scene::Unload() // should be called before OnUnloaded
	{
		MSE_CORE_LOG("Scene: Unload...");
		// ...
		PhysicsShutdown();
		
		if (m_CurrentWorld != nullptr)
		{
			// delete m_CurrentWorld;
			m_CurrentWorld = nullptr;
			MSE_CORE_LOG("- current world is set to nullptr");
		}
		
		MSE_CORE_LOG("- unloaded");
		
		MSE_CORE_LOG("- user-defined OnUnloaded");
		OnUnloaded();
	}
	
	void Scene::OnUnloaded()
	{
	}
	
	void Scene::Update(TimeType sceneTime)
	{
		// . . .
		
		OnUpdate(sceneTime); // user-defined scene update function
		
		if (GetPhysicsProcessor() != nullptr)
			UpdatePhysics(sceneTime); // engine-defined physics update mechanic
		
		// . . .
	}
	
	void Scene::OnUpdate(TimeType sceneTime)
	{
	}
	
	void Scene::Start()
	{
	}
	
	void Scene::Stop()
	{
	}
	
	void Scene::Pause()
	{
	}
	
	void Scene::Resume()
	{
	}
	
	void Scene::PhysicsInit(const PhysicsSystem& physicsSystem, bool reconfigureCamera)
	{
		MSE_CORE_LOG("Scene: Initialize physics system of type ", (int)physicsSystem);
		if (physicsSystem != PhysicsSystem::None)
		{
			m_PhysicsSystem = physicsSystem;
			m_PhysicsProcessor = new PhysicsProcessor(physicsSystem);
			
			m_CurrentWorld = m_PhysicsProcessor->AccessWorld(0);
			if (reconfigureCamera)
			{
//				m_SceneCamera->SetWorldConstraints({m_CurrentWorld->properties.size.x, m_CurrentWorld->properties.size.y});
//				m_SceneCamera->Retarget({
//					-m_CurrentWorld->properties.size.x/10,
//					-m_CurrentWorld->properties.size.y/10,
//					m_CurrentWorld->properties.size.x/10,
//					m_CurrentWorld->properties.size.y/10
//				});
			}
			PhysicsOn();
		} else {
			m_PhysicsSystem = PhysicsSystem::None;
			m_PhysicsProcessor = new PhysicsProcessor(PhysicsSystem::None);
			
			m_CurrentWorld = nullptr;
			
			if (reconfigureCamera)
			{
//				m_SceneCamera->SetWorldConstraints({0.0f, 0.0f});
//				m_SceneCamera->Retarget({
//					0,
//					0,
//					0,
//					0,
//				});
			}
			PhysicsOff();
		}
	}
	
	void Scene::PhysicsOn()
	{
		MSE_CORE_LOG("Scene: physics turn ON");
		m_PhysicsOn = true;
	}
	
	void Scene::PhysicsOff()
	{
		MSE_CORE_LOG("Scene: physics turn OFF");
		m_PhysicsOn = false;
	}
	
	void Scene::PhysicsShutdown()
	{
		MSE_CORE_LOG("Scene: shutdown physics");
		if (m_PhysicsProcessor != nullptr)
			delete m_PhysicsProcessor;
		m_PhysicsProcessor = nullptr;
		PhysicsOff();
		MSE_CORE_LOG("- physics processor unloaded");
	}
	
	void Scene::UpdatePhysics(TimeType sceneTime) // calls physics processor general routine
	{
		if ((m_PhysicsProcessor != nullptr) && m_PhysicsOn)
		{
			m_PhysicsProcessor->GeneralRoutine(this, sceneTime);
			// MSE_CORE_LOG("Physics processor general routine called and processed");
		}
	}
	
	Entity* Scene::CreateEntity(const std::string& name)
	{
		Entity* entity = new Entity( m_Registry.create(), this );
		
		// TODO: create default components
		auto& nameComponent = entity->AddComponent<NameComponent>();
		nameComponent.value = name.empty() ? "Nameless entity" : name;
		// entity->AddComponent<TransformComponent>();
		return entity; 
	}
	
	Entity* Scene::CreateEntityWithUUID(const std::string& name, uint64_t uuid)
	{
		Entity* entity = CreateEntity(name);
		
		if (uuid == 0) // assume that 0 is not a valid uuid
			uuid = utils::UUID::Generate();
		auto& uuidComponent = entity->AddComponent<UUIDComponent>(uuid);
		
		return entity; 
	}
	
	Entity* Scene::CreatePhysicalEntity(const std::string& name)
	{
		Entity* entity = CreateEntity(name);
		entity->AddComponent<PhysicsComponent>();
		m_PhysicsProcessor->RegisterEntity(entity);
		
		return entity;
	}
	
	void Scene::DestroyEntity(Entity& entity)
	{
		if (entity.HasComponent<PhysicsComponent>())
			m_PhysicsProcessor->UnregisterEntity(&entity);
		
		m_Registry.destroy(entity.GetID());
	}
}



