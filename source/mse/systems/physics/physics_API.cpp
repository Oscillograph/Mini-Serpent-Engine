#include <mse/systems/physics/physics_API.h>

//#include <mse/systems/physics/msephysics.h>
//#include <mse/systems/physics/box2dphysics.h>

namespace mse
{
	PhysicsAPI* CreatePhysicsProcessor(PhysicsSystem psID)
	{	
		switch (psID)
		{
			case PhysicsSystem::None:
			{
				return nullptr;
			}
//			case PhysicsSystem::mse: 
//			{
//				return new msePhysics(); 
//				break;
//			}
//			case PhysicsSystem::Box2D: 
//			{
//				return new Box2DPhysics(); 
//				break;
//			}
		default:
			MSE_CORE_LOG("Physics: Couldn't guess the physics system to initialize");
		}
		return nullptr;
	}
}

