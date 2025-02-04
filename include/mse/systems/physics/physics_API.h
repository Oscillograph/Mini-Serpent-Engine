#ifndef MSE_SYSTEMS_PHYSICS_PHYSICS_API_H
#define MSE_SYSTEMS_PHYSICS_PHYSICS_API_H

#include <mse/core.h>
#include <mse/systems/physics/world.h>
#include <mse/systems/platform/platform.h>

namespace mse
{
	// forward declarations
	class Scene;
	class Entity;
	// --------------------
	
	enum class PhysicsSystem 
	{
		None		= 0,
		MSE	 		= 1,
		Box2D		= 2
	};
	
	// This is an API to use any suitable physics engine.
	// The need for it arises from that physics engines are very complex, and their management parts
	// are not to interfere with other game engine systems.
	class PhysicsEngineAPI
	{
	public:
		// virtual ~PhysicsAPI() = 0;
		
		virtual void CreateWorld(const WorldProperties& props) = 0;
		virtual void DestroyWorld(int worldID) = 0;
		
		virtual void RegisterEntity(Entity* A) = 0; // make the entity seen by the physics processor
		virtual void ChangeType(Entity* A) = 0;
		virtual void UnregisterEntity(Entity* A) = 0; // make it invisible again
		
		// collisions
		virtual void CollisionSelection(Entity* A, Entity* B) = 0;
		virtual void CollisionDetection(Entity* A, Entity* B) = 0;
		virtual void CollisionReaction(Entity* A, Entity* B) = 0;
		
		// reactions
		
		// processors
		virtual void GeneralRoutine(Scene* scene, TimeType sceneTime) = 0;
		virtual void ApplyForce(Entity e, glm::vec2 vector, float amplitude) = 0;
		virtual void ApplyImpulse(Entity e, glm::vec2 vector, float amplitude) = 0;
		
		// general
		virtual void Shutdown() = 0;
	};
	
	PhysicsEngineAPI* CreatePhysicsProcessor(PhysicsSystem psID);
}

#endif

