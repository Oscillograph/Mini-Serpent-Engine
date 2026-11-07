#include <dungeontris/layers.h>
#include <dungeontris/game-fwd.h>
#include <dungeontris/gamestates.h>
#include <dungeontris/tetris.h>
#include <mse/core.h>
#include <mse/systems/platform/audio/soundman.h>

extern DTetris::GameDB gameDB;
extern DTetris::Game game;
extern DTetris::TetrisMap tetrisMap;
extern GameStateMachine gsm;
extern DTetris::TetrisEngineData DTetris::tetrisEngineData;

SettingsUILayer::SettingsUILayer() : mse::Layer()
{
	MSE_LOG("Constructed a SettingsUI layer");
}

SettingsUILayer::~SettingsUILayer()
{
	MSE_LOG("Destroyed a SettingsUI layer");
}

void SettingsUILayer::OnInit()
{


	// Settings to control from here:
	// --Audio--
	// master volume
	// music volume
	// sounds volume
	// --Graphics--
	// fullscreen toggle on/off



	AddElement(new mse::gui::Text(
		this,
		U"Звук:",
		{20, 40, 140, 10},
		{0, 0, 0, 255},
		{196, 196, 0, 255},
		1));
	AddElement(new mse::gui::Text(
		this,
		U"Музыка фоном",
		{60, 54, 140, 10},
		{0, 0, 0, 255},
		{196, 196, 196, 255},
		1));
	mse::gui::Button* playInBattlegroundBtn = (mse::gui::Button*)AddElement(new mse::gui::Checkbox(
		this,
		{210, 54, 14, 13},
		"data/img/screen-images.png",
		{0, 0, 0},
		&(game.config.playInBackground),
		{24, 88, 14, 13},
		{38, 88, 14, 13},
		{52, 88, 14, 13},
		{66, 88, 14, 13}));
	playInBattlegroundBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [=] (SDL_Event* event)
	{
		mse::Application::config.playInBackground = game.config.playInBackground;
	};

	AddElement(new mse::gui::Text(
		this,
		U"Громкость музыки",
		{60, 68, 140, 10},
		{0, 0, 0, 255},
		{196, 196, 196, 255},
		1));
	musicVolumeSlider = (mse::gui::HSlider*)AddElement(new mse::gui::HSlider(
		this,
		{210, 68, 80, 13},
		&(game.config.musicVolume),
		0,
		1.0f,
		1.0f / 16,
		"data/img/screen-images.png",
		{0, 0, 0},
		{24, 23, 52, 13},
		{24, 11, 40, 11},
		{24, 36, 52, 13},
		{63, 36, 4, 14},
		{67, 36, 5, 14},
		{72, 36, 4, 14}));

	AddElement(new mse::gui::Text(
		this,
		U"Громкость эффектов",
		{60, 82, 140, 10},
		{0, 0, 0, 255},
		{196, 196, 196, 255},
		1));
	soundsVolumeSlider = (mse::gui::HSlider*)AddElement(new mse::gui::HSlider(
		this,
		{210, 82, 80, 13},
		&(game.config.soundsVolume),
		0,
		1.0f,
		1.0f / 16,
		"data/img/screen-images.png",
		{0, 0, 0},
		{24, 23, 52, 13},
		{24, 11, 40, 11},
		{24, 36, 52, 13},
		{63, 36, 4, 14},
		{67, 36, 5, 14},
		{72, 36, 4, 14}));

	AddElement(new mse::gui::Text(
		this,
		U"Графика:",
		{20, 96, 140, 10},
		{0, 0, 0, 255},
		{196, 196, 0, 255},
		1));
	AddElement(new mse::gui::Text(
		this,
		U"Окно во весь экран",
		{60, 110, 140, 10},
		{0, 0, 0, 255},
		{196, 196, 196, 255},
		1));
	mse::gui::Checkbox* fullscreenBtn = (mse::gui::Checkbox*)(AddElement(new mse::gui::Checkbox(
		this,
		{210, 110, 14, 13},
		"data/img/screen-images.png",
		{0, 0, 0},
		&(game.config.fullscreen),
		{24, 88, 14, 13},
		{38, 88, 14, 13},
		{52, 88, 14, 13},
		{66, 88, 14, 13})));
	fullscreenBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [=](SDL_Event* event)
	{
		mse::Application::config.fullscreen = game.config.fullscreen;
		//        fullscreenBtn->state = mse::gui::CheckboxStates::Checked;
		m_window->ToggleFullscreen();
	};

	AddElement(new mse::gui::Text(
		this,
		U"Игровой процесс:",
		{20, 124, 140, 10},
		{0, 0, 0, 255},
		{196, 196, 0, 255},
		1));
	AddElement(new mse::gui::Text(
		this,
		U"Быстрый бой",
		{60, 138, 140, 10},
		{0, 0, 0, 255},
		{196, 196, 196, 255},
		1
	));
	mse::gui::Checkbox* fastBattleBtn = (mse::gui::Checkbox*)AddElement(new mse::gui::Checkbox(
		this,
		{210, 138, 14, 13},
		"data/img/screen-images.png",
		{0, 0, 0},
		&(game.config.fastBattle),
		{24, 88, 14, 13},
		{38, 88, 14, 13},
		{52, 88, 14, 13},
		{66, 88, 14, 13}));
	fastBattleBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [=] (SDL_Event* event)
	{
		game.config.turnLength = (game.config.fastBattle) ? 0 : 500;
	};

	mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"<< Назад", {32, 32, 32, 255}, {120, 220, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
	mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
		gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
	};
}

void SettingsUILayer::OnUpdate(mse::TimeType t)
{
	if (musicVolumeSlider->valueChanged)
	{
		MSE_CORE_LOG("musicVolumeSlider");
		musicVolumeSlider->valueChanged = false;
		mse::Application::config.musicVolume = game.config.musicVolume;
		mse::SoundMan::AdjustTrackVolume(mse::Application::config.musicVolume);
	}
	if (soundsVolumeSlider->valueChanged)
	{
		MSE_CORE_LOG("soundsVolumeSlider");
		soundsVolumeSlider->valueChanged = false;
		mse::Application::config.soundsVolume = game.config.soundsVolume;
		mse::SoundMan::AdjustSoundsVolume(mse::Application::config.soundsVolume);
	}
}
