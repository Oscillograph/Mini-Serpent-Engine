#include <dungeontris/layers.h>
#include <dungeontris/game-fwd.h>
#include <dungeontris/gamestates.h>
#include <dungeontris/tetris.h>

extern DTetris::GameDB gameDB;
extern DTetris::Game game;
extern DTetris::TetrisMap tetrisMap;
extern GameStateMachine gsm;
extern DTetris::TetrisEngineData DTetris::tetrisEngineData;

WinnerUILayer::WinnerUILayer() : mse::Layer()
{
	MSE_LOG("Constructed WinnerUILayer");
}
WinnerUILayer::~WinnerUILayer()
{
	MSE_LOG("Destroyed WinnerUILayer");
}

void WinnerUILayer::OnInit()
{
	GameLogo(this);

	AddElement(new mse::gui::Text(
		this,
		U"Победа!",
		{110, 80, 120, 40},
		{0, 0, 0, 255},
		{64, 255, 64, 255},
		2));
	if (game.battleCounter < 5)
	{
		AddElement(new mse::gui::Text(
			this,
			U"В битве было нелегко, но герой\nсправился и одолел противника.\nВпереди новое испытание!",
			{50, 160, 220, 40},
			{0, 0, 0, 255},
			{196, 196, 196, 255},
			1));

		mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"К наградам", {32, 32, 32, 255}, {120, 215, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
		mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			gsm.ChangeStateTo(DTetris::GamePages::CharacterUpdate);
		};
	} else {
		AddElement(new mse::gui::Text(
			this,
			U"Герой покорил арену, преодолев\nвсе трудности и опасности. \nЕго удали нет равных, и теперь,по праву чемпиона, он войдёт \nв Зал Славы как живая легенда.\nЭто было великолепно!",
			{50, 140, 220, 60},
			{0, 0, 0, 255},
			{196, 196, 196, 255},
			1));

		mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"К началу", {32, 32, 32, 255}, {120, 215, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
		mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
		};
		//        mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"В Зал Славы", {120, 220, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		//        mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
		//            gsm.ChangeStateTo(DTetris::GamePages::Highscores);
		//        };
	}
}

void WinnerUILayer::OnUpdate(mse::TimeType t)
{}
