#include <mse/systems/scenes/scene_manager.h>
#include <mse/systems/scenes/scene.h>

namespace mse
{
	std::vector<Scene*> SceneManager::m_Scenes = {};
	
	void SceneManager::Init()
	{
		MSE_CORE_LOG("Scene Manager: Initialization");
	}
	
	void SceneManager::Shutdown()
	{
		MSE_CORE_LOG("Scene Manager: Shutdown");
		for (Scene* scene : m_Scenes)
		{
			delete scene;
		}
	}
	
	void SceneManager::Load(Scene* scene)
	{
		if (scene != nullptr)
		{
			m_Scenes.push_back(scene);
			if (!scene->IsInitialized())
			{
				scene->Init();
			}
			scene->Load();
		} else {
			MSE_CORE_LOG("Scene Manager: Cannot load a scene that is nullptr");
		}
	}
	
	void SceneManager::Unload(Scene* scene)
	{
		if (scene != nullptr)
		{
			scene->Unload();
		} else {
			MSE_CORE_LOG("Scene Manager: Cannot unload a scene that is nullptr");
		}
	}
	
	void SceneManager::Remove(Scene* scene)
	{
		Unload(scene);
		
		auto it = std::find(m_Scenes.begin(), m_Scenes.end(), scene);
		if (it != m_Scenes.end())
		{
			m_Scenes.erase(it);
			delete scene;
		}
	}
	
	void SceneManager::Update(TimeType time)
	{
		for (Scene* scene : m_Scenes)
		{
			if (scene->IsInitialized() && scene->IsRunning())
			{
				scene->Update(time);
			}
		}
	}
}
