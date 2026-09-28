#include <mse/mse.h>

#include <card-game/game-fwd.h>
#include <card-game/game-data.h>
#include <card-game/data-loader.h>

#include <card-game/gamestates.h>
#include <card-game/layers.h>

// ********************************************************************************************** //
//                                    SCENES (do I need one?)
// ********************************************************************************************** //

class GameScene : public mse::Scene
{
public:
	GameScene()
	{
		MSE_LOG("GameScene constructor...");
		// register game states

		MSE_LOG("GameScene: gsm states initiated and stored.");

		// setup initial game state
		MSE_LOG("GameScene constructor...done");
	};

	~GameScene()
	{
		delete player;
		player = nullptr;
		delete npc;
		npc = nullptr;
		MSE_LOG("GameScene destructor...done");
	};

	virtual void OnUpdate(mse::TimeType time)
	{
	}

	mse::Entity* player = nullptr;
	mse::Entity* npc = nullptr;
};

// ********************************************************************************************** //
//                                        Application
// ********************************************************************************************** //

class App : public mse::Application
{
public:
	App() : mse::Application()
	{
		MSE_LOG("Hello, world!");
		MSE_ERROR("Joke");

		std::srand((unsigned int)(time(NULL)));

		MSE_LOG("Commanding to open a window");
		m_window = mse::WindowManager::CreateWindow(u8"Фехтоватор", 50, 50, 320, 240);
		m_window->callbacks[mse::EventTypes::KeyDown] = [&](SDL_Event* event){
			MSE_LOG("Key pressed: ", event->key.key);
			if (event->key.key == SDLK_ESCAPE)
			{
				this->Stop();
			}
			return true;
		};

		// create a custom cursor
		mse::Resource* spriteList = mse::ResourceManager::UseTexture("data/img/screen-images.png", m_window, {0, 0, 0});
		glm::uvec4 spriteListRect = {109, 22, 32, 32};
		m_Cursor = mse::ResourceManager::CreateCursor(m_window, 0, 0, spriteList, spriteListRect, {0, 0, 0});
		mse::Cursor* cursorObj = (mse::Cursor*)(m_Cursor->data);
		SDL_SetCursor(cursorObj->GetNativeCursor());

		mse::Renderer::SetActiveWindow(m_window);

		MSE_LOG("Commanding to create and load scene");
		m_scene = new GameScene();
		mse::SceneManager::Load(m_scene);
		m_scene->Start();
	}

	~App()
	{
		MSE_LOG("Saving config");

		MSE_LOG("Commanding to destroy a window");
		mse::ResourceManager::DropResource(m_Cursor, m_window);
		m_Cursor = nullptr;
		SDL_SetCursor(NULL);
		delete cse_texture;
		cse_texture = nullptr;
		mse::WindowManager::DestroyWindow(m_window);
		m_window = nullptr;
		mse::WindowManager::DestroyWindow(m_window2);
		m_window2 = nullptr;
		MSE_LOG("Goodbye, world!");
	}

public:
	mse::Scene* m_scene = nullptr;
	mse::Window* m_window = nullptr;
	mse::Window* m_window2 = nullptr;
	mse::Texture* cse_texture = nullptr;
	mse::Resource* m_Cursor = nullptr;
};

mse::Application* mse::CreateApplication()
{
	return new App();
}
