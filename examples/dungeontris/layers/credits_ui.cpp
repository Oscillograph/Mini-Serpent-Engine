#include <dungeontris/layers.h>
#include <dungeontris/game-fwd.h>
#include <dungeontris/gamestates.h>
#include <dungeontris/tetris.h>

extern DTetris::GameDB gameDB;
extern DTetris::Game game;
extern DTetris::TetrisMap tetrisMap;
extern GameStateMachine gsm;
extern DTetris::TetrisEngineData DTetris::tetrisEngineData;

CreditsUILayer::CreditsUILayer() : mse::Layer()
{
	MSE_LOG("Constructed CreditsUILayer");
}
CreditsUILayer::~CreditsUILayer()
{
	MSE_LOG("Destroyed CreditsUILayer");
}

void CreditsUILayer::OnInit()
{
	GameLogo(this);

	AddElement(new mse::gui::Text(
		this,
		U"Программирование:",
		{20, 30, 220, 10},
		{0, 0, 0, 255},
		{196, 196, 0, 255},
		1));
	AddElement(new mse::gui::Text(
		this,
		U"Broscillograph",
		{60, 40, 220, 10},
		{0, 0, 0, 255},
		{196, 196, 196, 255},
		1));
	AddElement(new mse::gui::Text(
		this,
		U"Графика:",
		{20, 50, 220, 10},
		{0, 0, 0, 255},
		{196, 196, 0, 255},
		1));
	AddElement(new mse::gui::Text(
		this,
		U"Broscillograph",
		{60, 60, 220, 10},
		{0, 0, 0, 255},
		{196, 196, 196, 255},
		1));
	AddElement(new mse::gui::Text(
		this,
		U"Музыка:",
		{20, 70, 220, 10},
		{0, 0, 0, 255},
		{196, 196, 0, 255},
		1));
	AddElement(new mse::gui::Text(
		this,
		U"Трио из Удио\n\"Наша Таня громко плачет\" (2025)",
								  {60, 80, 240, 20},
							   {0, 0, 0, 255},
							   {196, 196, 196, 255},
							   1));
	AddElement(new mse::gui::Text(
		this,
		U"Библиотеки:",
		{20, 100, 220, 10},
		{0, 0, 0, 255},
		{196, 196, 0, 255},
		1));
	AddElement(new mse::gui::Text(
		this,
		U"SDL3, utf8-cpp, I use Arch btw,\nYAML-cpp, GLM, EnTT, SDL-Mixer",
		{60, 110, 240, 20},
		{0, 0, 0, 255},
		{196, 196, 196, 255},
		1));
	AddElement(new mse::gui::Text(
		this,
		U"Моральная поддержка:",
		{20, 130, 220, 10},
		{0, 0, 0, 255},
		{196, 196, 0, 255},
		1));
	AddElement(new mse::gui::Text(
		this,
		U"Сообщество Nau Engine",
		{60, 140, 220, 10},
		{0, 0, 0, 255},
		{196, 196, 196, 255},
		1));
	AddElement(new mse::gui::Text(
		this,
		U"Место для вашей рекламы:",
		{20, 150, 220, 10},
		{0, 0, 0, 255},
		{196, 196, 0, 255},
		1));
	AddElement(new mse::gui::Text(
		this,
		U"",
		{60, 160, 220, 10},
		{0, 0, 0, 255},
		{196, 196, 196, 255},
		1));
	AddElement(new mse::gui::Text(
		this,
		U"Оригинальный диздок:",
		{20, 170, 220, 10},
		{0, 0, 0, 255},
		{196, 196, 0, 255},
		1));
	AddElement(new mse::gui::Text(
		this,
		U"Леста Игры",
		{60, 180, 220, 10},
		{0, 0, 0, 255},
		{196, 196, 196, 255},
		1));

	mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"<< Назад", {32, 32, 32, 255}, {120, 220, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
	mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
		gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
	};
}

void CreditsUILayer::OnUpdate(mse::TimeType t)
{}
