#ifndef DUNGEONTRIS_APP
#define DUNGEONTRIS_APP

#include <mse/mse.h>

class GameScene : public mse::Scene
{
public:
	GameScene();

	~GameScene();

	virtual void OnUpdate(mse::TimeType time);

	mse::Entity* player = nullptr;
	mse::Entity* npc = nullptr;
};

class App : public mse::Application
{
public:
	App();
	~App();

public:
	mse::Scene* m_scene = nullptr;
	mse::Window* m_window = nullptr;
	mse::Window* m_window2 = nullptr;
	mse::Texture* cse_texture = nullptr;
	mse::Resource* m_Cursor = nullptr;
};

#endif
