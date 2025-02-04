#include <mse/systems/scenes/scene_manager.h>
#include <mse/systems/scenes/scene.h>

namespace mse
{
	SceneManager::SceneManager()
	{
		
	}
	
	SceneManager::~SceneManager()
	{
		for (Scene* scene : m_Scenes)
		{
			delete scene;
		}
	}
	
	void SceneManager::Load(Scene* scene)
	{
		m_Scenes.emplace(m_Scenes.begin() + m_SceneInsertIndex, scene);
		if (!scene->IsInitialized())
		{
			scene->Init();
		}
		scene->Load();
		
		m_SceneInsertIndex++;
	}
	
	void SceneManager::Unload(Scene* scene)
	{
		scene->Unload();
	}
	
	void SceneManager::Remove(Scene* scene)
	{
		Unload(scene);
		
		auto it = std::find(m_Scenes.begin(), m_Scenes.end(), scene);
		if (it != m_Scenes.end())
		{
			m_Scenes.erase(it);
			delete scene;
			m_SceneInsertIndex--;
		}
	}
}
