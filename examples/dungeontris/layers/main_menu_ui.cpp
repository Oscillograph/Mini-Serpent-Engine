#include <dungeontris/layers.h>
#include <dungeontris/game-fwd.h>
#include <dungeontris/gamestates.h>
#include <dungeontris/tetris.h>

extern DTetris::GameDB gameDB;
extern DTetris::Game game;
extern DTetris::TetrisMap tetrisMap;
extern GameStateMachine gsm;
extern DTetris::TetrisEngineData DTetris::tetrisEngineData;

MainMenuUILayer::MainMenuUILayer() : mse::Layer()
{
	MSE_LOG("Constructed MainMenuUILayer");
}
MainMenuUILayer::~MainMenuUILayer()
{
	MSE_LOG("Destroyed MainMenuUILayer");
}

void MainMenuUILayer::OnInit()
{
	GameLogo(this);

	mse::gui::Button* playerCreateBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
		this,
		U" Новая  игра",
		{0, 0, 0, 255},
		{110, 40, 100, 13},
		"./data/img/screen-images.png",
		{122, 101, 4, 13},
		{138, 101, 4, 13},
		{154, 101, 4, 13}
	)));
	playerCreateBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
		gsm.ChangeStateTo(DTetris::GamePages::CharacterCreation);
	};

	//    mse::gui::Button* battleBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
	//                                                                                      this,
	//                                                                                      U"  Загрузить",
	//                                                                                      {0, 0, 0, 255},
	//                                                                                      {110, 54, 100, 13},
	//                                                                                      "./data/img/screen-images.png",
	//                                                                                      {122, 101, 4, 13},
	//                                                                                      {138, 101, 4, 13},
	//                                                                                      {154, 101, 4, 13}
	//                                                                                      )));
	//    battleBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
	//        gsm.ChangeStateTo(DTetris::GamePages::CharacterLoad);
	//    };
	//
	//    mse::gui::Button* scoresBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
	//                                                                                      this,
	//                                                                                      U"   Рекорды",
	//                                                                                      {0, 0, 0, 255},
	//                                                                                      {110, 68, 100, 13},
	//                                                                                      "./data/img/screen-images.png",
	//                                                                                      {122, 101, 4, 13},
	//                                                                                      {138, 101, 4, 13},
	//                                                                                      {154, 101, 4, 13}
	//                                                                                      )));
	//    scoresBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
	//        gsm.ChangeStateTo(DTetris::GamePages::Highscores);
	//    };

	mse::gui::Button* settingsBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
		this,
		U"  Настройки",
		{0, 0, 0, 255},
		{110, 54, 100, 13},
		"./data/img/screen-images.png",
		{122, 101, 4, 13},
		{138, 101, 4, 13},
		{154, 101, 4, 13}
	)));
	settingsBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
		gsm.ChangeStateTo(DTetris::GamePages::Settings);
	};

	mse::gui::Button* creditsBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
		this,
		U"    Титры",
		{0, 0, 0, 255},
		{110, 68, 100, 13},
		"./data/img/screen-images.png",
		{122, 101, 4, 13},
		{138, 101, 4, 13},
		{154, 101, 4, 13}
	)));
	creditsBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
		gsm.ChangeStateTo(DTetris::GamePages::Credits);
	};

	mse::gui::Button* exitBtn = (mse::gui::Button*)(AddElement(
		new mse::gui::Button(
			this,
			U"    Выход",
			{0, 0, 0, 255},
			{110, 82, 100, 13},
			"./data/img/screen-images.png",
			{122, 101, 4, 13},
			{138, 101, 4, 13},
			{154, 101, 4, 13}
		)));
	exitBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
		gsm.ChangeStateTo(DTetris::GamePages::Exit);
	};

	AddElement(new mse::gui::Text(
		this,
		U"Перед началом игры характеристики \nперсонажей генерируются случайным образом.\n\nЕсли выпавшие не нравятся, можно вернуться в главное меню и начать игру снова.\n\nБой считается выигранным, если \nперсонаж игрока нокаутировал соперника\nили продержался 30 ходов.\n\nЧтобы выйти из игры в любое время, \nдостаточно нажать клавишу ESC.",
		{10, 110, 300, 110},
		{0, 0, 0, 255},
		{255, 255, 255, 255},
		1));
}

void MainMenuUILayer::OnUpdate(mse::TimeType t)
{}
