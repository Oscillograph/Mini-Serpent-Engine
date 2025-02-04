#ifndef MSE_SYSTEMS_SCENES_SCENE_MANAGER_H
#define MSE_SYSTEMS_SCENES_SCENE_MANAGER_H

#include <mse/core.h>

namespace mse
{
	class SceneManager
	{
	public:
//		SceneManager();
//		~SceneManager();
		
		static void Init();
		static void Shutdown();
		
		static void Load(Scene* scene);
		static void Unload(Scene* scene);
		static void Remove(Scene* scene);
		
		static void Update(TimeType time);
		
		inline static std::vector<Scene*>& GetScenes() { return m_Scenes; }
		
	protected:
		static std::vector<Scene*> m_Scenes;
	};
}

#endif
