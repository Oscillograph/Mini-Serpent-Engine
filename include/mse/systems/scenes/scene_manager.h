#ifndef MSE_SYSTEMS_SCENES_SCENE_MANAGER_H
#define MSE_SYSTEMS_SCENES_SCENE_MANAGER_H

#include <mse/core.h>

namespace mse
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();
		
		void Load(Scene* scene);
		void Unload(Scene* scene);
		void Remove(Scene* scene);
		
		inline const std::vector<Scene*>& GetContents() const { return m_Scenes; }
		
		std::vector<Scene*>::iterator begin() { return m_Scenes.begin(); }
		std::vector<Scene*>::iterator end()   { return m_Scenes.end();   }
		
	protected:
		std::vector<Scene*> m_Scenes;
		uint32_t m_SceneInsertIndex = 0;
	};
}

#endif
