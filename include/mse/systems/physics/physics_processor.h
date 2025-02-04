#ifndef MSE_SYSTEMS_PHYSICS_PROCESSOR_H
#define MSE_SYSTEMS_PHYSICS_PROCESSOR_H

#include <mse/core.h>
//#include <mse/systems/physics/world.h> // World

// implementation-specific
#include <mse/systems/physics/physics_API.h> //PhysicsSystem, PhysicsAPI
// #include <mse/systems/platform.h>

namespace mse
{
	// An entity can have a component PhysicsComponent.
	// PhysicsComponent ties the entity to a specific world.
	// Every world is basically a collection with a set of rules applied to all entities in it.
	// We don't need to check scene registry for entities if we have this collection.
	
	class PhysicsProcessor
	{
	public:
		PhysicsProcessor();
		PhysicsProcessor(const PhysicsSystem& physicsSystem);
		~PhysicsProcessor();
		
		// world management
		World* CreateWorld();
		World* CreateWorld(const WorldProperties& props);
		void DestroyWorld(World* world);
		World* AccessWorld(int id);
		
		void RegisterEntity(Entity* A); // make the entity seen by the physics processor
		void UnregisterEntity(Entity* A); // make it invisible again
		
		// collisions
		void CollisionSelection(Entity* A, Entity* B);
		void CollisionDetection(Entity* A, Entity* B);
		void CollisionReaction(Entity* A, Entity* B);
		
		// reactions
		
		// processors
		void GeneralRoutine(Scene* scene, TimeType sceneTime);
		void ApplyForce(Entity e, glm::vec2 vector, float amplitude);
		void ApplyImpulse(Entity e, glm::vec2 vector, float amplitude);
		
		// general
		void Init(const PhysicsSystem& physicsSystem);
		void Shutdown();
		void Restart(const PhysicsSystem& physicsSystem);
		
		PhysicsSystem GetSystem();
		
	private:
		std::vector<World*> m_Worlds = {};
		PhysicsSystem m_PhysicsSystemSelected = PhysicsSystem::None;
		PhysicsAPI* m_API = nullptr; 
		
		void Move(Entity* A);
		void UnMove(Entity* A);
	};
}

#endif

