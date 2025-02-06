#ifndef MSE_SYSTEMS_CAMERAS_CAMERA_MANAGER_H
#define MSE_SYSTEMS_CAMERAS_CAMERA_MANAGER_H

#include <mse/core.h>

namespace mse
{
	class CameraManager
	{
	public:
		static void Init();
		static Camera2D* CreateCamera2D(Scene* scene);
		static std::vector<Camera2D*> GetCameras(Scene* scene);
		static void ChangeScene(Camera2D* camera, Scene* scene);
		static void DestroyCamera(Camera2D* camera);
		
		static std::unordered_map<Camera2D*, Scene*> cameras; 
		static std::unordered_multimap<Scene*, Camera2D*> scenesWithCameras;
	};
}

#endif
