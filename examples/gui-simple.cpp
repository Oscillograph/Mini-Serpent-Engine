#include "mse/systems/ecs/components.h"
#include "mse/systems/ecs/entity.h"
#include "mse/systems/scenes/scene.h"
#include "mse/systems/scenes/scene_manager.h"
#include <mse/mse.h>

/*
 * GUI Simple is an app to develop and test gui capabilities of Mini Serpent Engine
 *
 * Isn't really an example or anything
 */

// Set up layers:
class MainLayer : public mse::Layer
{
public:
	MainLayer()
	: mse::Layer()
	{
		MSE_LOG("GUISimpleApp/MainLayer: Constructed a simple layer");
	};

	~MainLayer()
	{
		MSE_LOG("GUISimpleApp/MainLayer: Deconstructed a simple layer");
	};
};

// Set up scenes
class MainScene : public mse::Scene
{
public:
	MainScene()
	: mse::Scene()
	{
		MSE_LOG("GUISimpleApp/MainScene: Constructed a simple scene");

		testEntity = CreateEntity("testEntity");
		testEntity->AddComponent<mse::AnimationComponent>();
		mse::AnimationComponent& animationComponent = testEntity->GetComponent<mse::AnimationComponent>();
		animationComponent.SetOnStartedCallback([](void* a, void* b, void* c) -> void {
			MSE_LOG("Callback: Animation started");
			return;
		}, nullptr, nullptr, nullptr);
		MSE_LOG("Animation component set up.");
		animationComponent.Start();
	};

	~MainScene()
	{
		DestroyEntity(*testEntity);
		MSE_LOG("GUISimpleApp/MainScene: Deconstructed a simple scene");
	}

public:
	mse::Entity* testEntity = nullptr;

};

// Set up the app class:
class GUISimpleApp : public mse::Application
{
public:
	GUISimpleApp() : mse::Application()
	{
		MSE_LOG("GUISimpleApp: Hello, world!");

		MSE_LOG("GUISimpleApp: Commanding to open a window");
		m_window = mse::WindowManager::CreateWindow(u8"MSE GUISimple window", 50, 50, 320, 240);
		mse::Renderer::SetActiveWindow(m_window);

		m_window->GetLayerManager()->Attach(new MainLayer());

		m_scene = new MainScene();
		mse::SceneManager::Load(m_scene);
		m_scene->Start();
	}
	~GUISimpleApp()
	{
		MSE_LOG("GUISimpleApp: Commanding to close a window");
		mse::WindowManager::DestroyWindow(m_window);
		m_window = nullptr;
		MSE_LOG("GUISimpleApp: Goodbye, world!");
	}

private:
	mse::Window* m_window = nullptr;
	mse::Scene* m_scene = nullptr;
};

// Define a pre-declared function to create the application:
mse::Application* mse::CreateApplication()
{
	return new GUISimpleApp();
}
