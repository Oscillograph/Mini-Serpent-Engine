#include <mse/systems/physics/world.h>

#include <mse/systems/ecs/entity.h>
#include <mse/systems/ecs/components.h>

namespace mse
{
	WorldProperties::WorldProperties()
	{
	}
	
	WorldProperties::WorldProperties(const glm::vec2& worldSize)
	: size(worldSize)
	{
	}
	
	// World Collection
	World::World()
	{
	}
	
	World::World(const WorldProperties& props)
	: properties(props)
	{
	}
	
	World::~World()
	{
		for (Entity* e : entities)
		{
			delete e;
			e = nullptr;
		}
	}
	
	void World::AddEntity(Entity& e)
	{
		Entity* entity = new Entity(e.GetID(), e.GetScene());
		entities.push_back(entity);
	}
	
	void World::RemoveEntity(Entity* e)
	{
		if (e != nullptr)
		{
			for (auto it = entities.begin(); it != entities.end(); it++)
			{
				if (*it == e)
				{
					delete *it;
					*it = nullptr;
					entities.erase(it);
					break;
				}
			}
		}
	}
}

