#include <mse/systems/cameras/camera_manager.h>
#include <mse/systems/cameras/camera2D.h>
#include <mse/systems/scenes/scene.h>

namespace mse
{
	std::unordered_map<Camera2D*, Scene*> CameraManager::cameras = {};
	std::unordered_multimap<Scene*, Camera2D*> CameraManager::scenesWithCameras = {};
	
	void CameraManager::Init()
	{}
	
	Camera2D* CameraManager::CreateCamera2D(Scene* scene)
	{
		Camera2D* newCamera = new Camera2D(scene);
		cameras[newCamera] = scene;
		scenesWithCameras.insert({scene, newCamera});
		return newCamera;
	}
	
	std::vector<Camera2D*> CameraManager::GetCameras(Scene* scene)
	{
		std::vector<Camera2D*> fewCameras = {};
		
		for (auto it = scenesWithCameras.find(scene); it != scenesWithCameras.end(); it = scenesWithCameras.find(scene))
		{
			if (it->second != nullptr)
			{
				fewCameras.push_back(it->second);
			}
		}
		
		return fewCameras;
	}
	
	void CameraManager::ChangeScene(Camera2D* camera, Scene* scene)
	{
		// remove old camera registry note
		for (auto it = scenesWithCameras.find(camera->scene); it != scenesWithCameras.end(); it = scenesWithCameras.find(camera->scene))
		{
			if (it->second == camera)
			{
				scenesWithCameras.erase(it);
				break;
			}
		}
		
		// update camera
		camera->scene = scene;
		
		// update camera registry
		cameras[camera] = scene;
		scenesWithCameras.insert({scene, camera});
	}
	
	void CameraManager::DestroyCamera(Camera2D* camera)
	{
		for (auto it = scenesWithCameras.find(camera->scene); it != scenesWithCameras.end(); it = scenesWithCameras.find(camera->scene))
		{
			if (it->second == camera)
			{
				scenesWithCameras.erase(it);
				break;
			}
		}
		
		auto it = cameras.find(camera);
		cameras.erase(it);
		
		delete camera;
		camera = nullptr;
	}
}
