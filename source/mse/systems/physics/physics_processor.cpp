#include <mse/systems/physics/physics_processor.h>
#include <mse/systems/scenes/scene.h>
#include <mse/systems/ecs/entity.h>
#include <mse/systems/ecs/components.h>
#include <mse/systems/canban.h>

namespace mse
{
	// Physics Processor
	// std::vector<World*> PhysicsProcessor::m_Worlds;
	// PhysicsSystem PhysicsProcessor::m_PhysicsSystemSelected = PhysicsSystem::None;
	// PhysicsAPI* PhysicsProcessor::m_API = nullptr;
	
	PhysicsProcessor::PhysicsProcessor()
	{
		Init(PhysicsSystem::MSE);
	}
	
	PhysicsProcessor::PhysicsProcessor(const PhysicsSystem& physicsSystem)
	{
		Init(physicsSystem);
	}
	
	PhysicsProcessor::~PhysicsProcessor()
	{
		Shutdown();
	}
	
	
	World* PhysicsProcessor::CreateWorld()
	{
		if (m_API != nullptr)
		{
			// default world properties
			WorldProperties props = WorldProperties();
			
			// create a new world
			World* world = new World(props);
			m_Worlds.push_back(world);
			
			m_API->CreateWorld(props);
			
			return world;
		}
		
		return nullptr;
	}
	
	World* PhysicsProcessor::CreateWorld(const WorldProperties& props)
	{
		if (m_API != nullptr)
		{
			// create a new world
			World* world = new World(props);
			m_Worlds.push_back(world);
			
			// b2Vec2 box2DGravity = {props.gravity.x, props.gravity.y};
			// m_Box2DWorld(box2DGravity);
			
			m_API->CreateWorld(props);
			return world;
		}
		
		return nullptr;
	}
	
	void PhysicsProcessor::DestroyWorld(World* world)
	{
		MSE_CORE_LOG("Physics Processor: world destruction routine...");
		if (world != nullptr)
		{
			for (auto it = m_Worlds.begin(); it != m_Worlds.end(); it++)
			{
				if (*it == world)
				{
					delete *it;
					m_Worlds.erase(it);
					world = nullptr;
				}
			}
		}
	}
	
	World* PhysicsProcessor::AccessWorld(int id)
	{
		if (id < m_Worlds.size())
			return m_Worlds[id];
		return nullptr;
	}
	
	void PhysicsProcessor::RegisterEntity(Entity* A) // make the entity seen by the physics processor
	{
		if (m_API != nullptr)
			m_API->RegisterEntity(A);
	}
	
	void PhysicsProcessor::UnregisterEntity(Entity* A) // make it invisible again
	{
		if (m_API != nullptr)
			m_API->UnregisterEntity(A);
	}
	
	void PhysicsProcessor::CollisionSelection(Entity* A, Entity* B)
	{
	}
	
	void PhysicsProcessor::CollisionDetection(Entity* A, Entity* B)
	{
	}
	
	void PhysicsProcessor::CollisionReaction(Entity* A, Entity* B)
	{
	}
	
	void PhysicsProcessor::GeneralRoutine(Scene* scene, TimeType sceneTime)
	{
		if (m_API != nullptr)
		{
			entt::registry& entityRegistry = scene->GetRegistry();
			auto physicsCollection = entityRegistry.view<PhysicsComponent>();
			
			// read canban board
			CanbanEvent canbanEvent;
			if (Canban::GetTask(CanbanEvents::Physics_ChangeType, canbanEvent))
			{
				// The reason to transfer type change to physics implementation is that
				// each implementation has its own way of dealing with world processing,
				// and the general engine abstraction can't know it beforehand.
				m_API->ChangeType(((Entity*)(canbanEvent.data))); 
			}
			// MSE_CORE_LOG("Canban processed");
			m_API->GeneralRoutine(scene, sceneTime);
			
			/*
			// check for groups of objects that could possibly collide
			for (auto e : physicsCollection)
		{
			Entity entity(e, scene);
			PhysicsComponent& physicsComponent = entity.GetComponent<PhysicsComponent>();
			
			// TODO: develop a satisfying collision and pre-collision check algorithm
			
			// is there a possible collision?
			if (physicsComponent.bodyType != PhysicsDefines::BodyType::Banned)
		{
			// if it is possible, then make sure if it happened
			}
			
			
			// others react on this one
			// if ((physicsComponent.bodyType == PhysicsDefines::BodyType::Static)
			//	|| (physicsComponent.bodyType == PhysicsDefines::BodyType::Dynamic))
			// {
			//	
			// }
			
			
			// this one reacts to others
			if ((physicsComponent.bodyType == PhysicsDefines::BodyType::Astral)
			|| (physicsComponent.bodyType == PhysicsDefines::BodyType::Dynamic))
		{
			
			}
			}
			*/
		}
	}
	
	void PhysicsProcessor::ApplyForce(Entity e, glm::vec2 vector, float amplitude)
	{
		m_API->ApplyForce(e, vector, amplitude);
	}
	
	void PhysicsProcessor::ApplyImpulse(Entity e, glm::vec2 vector, float amplitude)
	{
		m_API->ApplyImpulse(e, vector, amplitude);
	}
	
	void PhysicsProcessor::Move(Entity* A)
	{
		PhysicsComponent& physics = A->GetComponent<PhysicsComponent>();
		
		// translational movement
		physics.velocity.x += physics.acceleration.x * physics.time;
		physics.velocity.y += physics.acceleration.y * physics.time;
		
		float moveX = physics.velocity.x * physics.time;
		float moveY = physics.velocity.y * physics.time;
		
		// move center of mass if it's that easy
		// += moveX
		// += moveY
		
		// affect hitboxes
	}
	
	void PhysicsProcessor::UnMove(Entity* A)
	{
		PhysicsComponent& physics = A->GetComponent<PhysicsComponent>();
		
		// translational movement
		// physics.velocity.x += physics.acceleration.x * physics.time;
		// physics.velocity.y += physics.acceleration.y * physics.time;
		
		float moveX = physics.velocity.x * physics.time;
		float moveY = physics.velocity.y * physics.time;
		
		// move center of mass if it's that easy
		// -= moveX
		// -= moveY
		
		// affect hitboxes
	}
	
	void PhysicsProcessor::Init(const PhysicsSystem& physicsSystem)
	{
		MSE_CORE_LOG("Physics Processor: Init...");
		m_API = CreatePhysicsProcessor(physicsSystem);
		CreateWorld();
	}
	
	void PhysicsProcessor::Shutdown()
	{
		MSE_CORE_LOG("Physics Processor: Shutdown...");
		MSE_CORE_LOG("- delete worlds...");
		for (uint32_t i = 0; i < m_Worlds.size(); i++)
		{
			// delete box2DWorld
			MSE_CORE_LOG("- deleting a world #", i);
			// delete MSE physics world
			MSE_CORE_ASSERT(m_Worlds[i], "PhysicsProcessor Shutdown: world #", i, " was a null pointer.");
			delete m_Worlds[i];
			m_Worlds[i] = nullptr;
		}
		MSE_CORE_LOG("- delete physics API...");
		// MSE_CORE_ASSERT(m_API, "PhysicsProcessor Shutdown: m_API was a null pointer.");
		delete m_API;
		m_API = nullptr;
	}
	
	void PhysicsProcessor::Restart(const PhysicsSystem& physicsSystem)
	{
		Shutdown();
		
		Init(physicsSystem);
	}
	
	PhysicsSystem PhysicsProcessor::GetSystem()
	{
		return m_PhysicsSystemSelected;
	}
}

