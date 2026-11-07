#include <dungeontris/layers.h>
#include <dungeontris/game-fwd.h>
#include <dungeontris/gamestates.h>
#include <dungeontris/tetris.h>

extern DTetris::GameDB gameDB;
extern DTetris::Game game;
extern DTetris::TetrisMap tetrisMap;
extern GameStateMachine gsm;
extern DTetris::TetrisEngineData DTetris::tetrisEngineData;

GameOverUILayer::GameOverUILayer() : mse::Layer()
{
	MSE_LOG("Constructed GameOverUILayer");
}
GameOverUILayer::~GameOverUILayer()
{
	MSE_LOG("Destroyed GameOverUILayer");
}

void GameOverUILayer::OnInit()
{
	GameLogo(this);

	AddElement(new mse::gui::Text(
		this,
		U"Проигрыш",
		{105, 80, 120, 40},
		{0, 0, 0, 255},
		{255, 64, 64, 255},
		2));
	AddElement(new mse::gui::Text(
		this,
		U"Герой старался изо всех сил,\nно противник превзошёл его.\nУдачи в следующий раз!",
		{50, 160, 220, 40},
		{0, 0, 0, 255},
		{196, 196, 196, 255},
		1));

	mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"К началу", {32, 32, 32, 255}, {120, 215, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
	mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
		gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
	};
}

void GameOverUILayer::OnUpdate(mse::TimeType t)
{}
