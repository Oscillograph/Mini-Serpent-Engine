#ifndef MSE_SYSTEMS_PHYSICS_WORLD_H
#define MSE_SYSTEMS_PHYSICS_WORLD_H

#include <mse/core.h>

namespace mse
{
	// forward declarations
	class Entity;
	// --------------------
	
	struct WorldProperties
	{
		glm::vec2 gravity = {0.0f, 9.8f}; // up or down directions
		glm::vec2 size = {1000.0f, 1000.0f}; // in meters
		
		WorldProperties();
		WorldProperties(const glm::vec2& worldSize);
	};
	
	struct World
	{
		int id;
		WorldProperties properties;
		std::vector<Entity*> entities;
		
		World();
		World(const WorldProperties& props);
		~World();
		
		void AddEntity(Entity& e);
		void RemoveEntity(Entity* e);
	};
}

#endif

