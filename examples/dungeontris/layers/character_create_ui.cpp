#include "mse/systems/platform/events/events.h"
#include "mse/systems/windows/layers/gui/items/checkbox.h"
#include <dungeontris/layers.h>
#include <dungeontris/game-fwd.h>
#include <dungeontris/gamestates.h>
#include <dungeontris/tetris.h>

extern DTetris::GameDB gameDB;
extern DTetris::Game game;
extern DTetris::TetrisMap tetrisMap;
extern GameStateMachine gsm;
extern DTetris::TetrisEngineData DTetris::tetrisEngineData;

CharacterCreateUILayer::CharacterCreateUILayer() : mse::Layer()
{
	MSE_LOG("Constructed CharacterCreateUILayer");
}

CharacterCreateUILayer::~CharacterCreateUILayer()
{
	MSE_LOG("Destroyed CharacterCreateUILayer");
}

void CharacterCreateUILayer::OnInit()
{
	GameLogo(this);

	mse::gui::Image* classSelectorBtn = nullptr;

	classTraitHint = (mse::gui::Hint*)(AddElement(
		new mse::gui::Hint(
			this,
			nullptr,
			U"",
			{0, 0, 200, 48},
			{64, 32, 32, 255},
			{218, 218, 255, 255},
			nullptr,
			1,
			true,
			{255, 255, 255, 255})));
	mse::gui::Hint* hintText1 = (mse::gui::Hint*)(AddElement(
		new mse::gui::Hint(
			this,
			nullptr,
			gameDB.classes[DTetris::Classes::Rogue].name,
			{0, 0, 110, 12},
			{64, 32, 32, 255},
			{218, 218, 255, 255},
			nullptr,
			1,
			true,
			{255, 255, 255, 255})));
	mse::gui::Hint* hintText2 = (mse::gui::Hint*)(AddElement(
		new mse::gui::Hint(
			this,
			nullptr,
			gameDB.classes[DTetris::Classes::Warrior].name,
			{0, 0, 110, 12},
			{64, 32, 32, 255},
			{218, 218, 255, 255},
			nullptr,
			1,
			true,
			{255, 255, 255, 255})));
	mse::gui::Hint* hintText3 = (mse::gui::Hint*)(AddElement(
		new mse::gui::Hint(
			this,
			nullptr,
			gameDB.classes[DTetris::Classes::Barbarian].name,
			{0, 0, 110, 12},
			{64, 32, 32, 255},
			{218, 218, 255, 255},
			nullptr,
			1,
			true,
			{255, 255, 255, 255})));
	mse::gui::Hint* hintText4 = (mse::gui::Hint*)(AddElement(
		new mse::gui::Hint(
			this,
			nullptr,
			gameDB.classes[DTetris::Classes::Monk].name,
			{0, 0, 110, 12},
			{64, 32, 32, 255},
			{218, 218, 255, 255},
			nullptr,
			1,
			true,
			{255, 255, 255, 255})));
	mse::gui::Hint* hintText5 = (mse::gui::Hint*)(AddElement(
		new mse::gui::Hint(
			this,
			nullptr,
			gameDB.classes[DTetris::Classes::Cultist].name,
			{0, 0, 110, 12},
			{64, 32, 32, 255},
			{218, 218, 255, 255},
			nullptr,
			1,
			true,
			{255, 255, 255, 255})));
	mse::gui::Hint* hintText6 = (mse::gui::Hint*)(AddElement(
		new mse::gui::Hint(
			this,
			nullptr,
			gameDB.classes[DTetris::Classes::Witcher].name,
			{0, 0, 110, 12},
			{64, 32, 32, 255},
			{218, 218, 255, 255},
			nullptr,
			1,
			true,
			{255, 255, 255, 255})));

	mse::gui::Hint* hintText7 = (mse::gui::Hint*)(AddElement(
		new mse::gui::Hint(
			this,
			nullptr,
			U"Добавляет крестовину\nв набор выпадающих\nтетримино.",
			{0, 0, 160, 36},
			{64, 32, 32, 255},
			{218, 218, 255, 255},
			nullptr,
			1,
			true,
			{255, 255, 255, 255})));

	// rogue selector
	classSelectorBtn = (mse::gui::Image*)AddElement(new mse::gui::Image(
		this,
		{10, 70, 14, 13},
		"./data/img/screen-images.png",
		{326, 2, 14, 13},
		{0, 0, 0, 255}));
	hintText1->BindTo(classSelectorBtn);
	classSelectorBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event)
	{
		classAvatarPreviewImage->ChangeSourceArea(
			{100, 173, 33, 50}
		);

		game.inputClass = DTetris::Classes::Rogue;
		game.inputStats = {
			gameDB.classes[DTetris::Classes::Rogue].health,
			gameDB.classes[DTetris::Classes::Rogue].strength,
			gameDB.classes[DTetris::Classes::Rogue].agility,
			gameDB.classes[DTetris::Classes::Rogue].endurance
		};

		UpdateClassStats();
		UpdateClassTips();
	};

	// warrior selector
	classSelectorBtn = (mse::gui::Image*)AddElement(new mse::gui::Image(
		this,
		{25, 70, 14, 13},
		"./data/img/screen-images.png",
		{341, 2, 14, 13},
		{0, 0, 0, 255}));
	classSelectorBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event)
	{
		classAvatarPreviewImage->ChangeSourceArea(
			{133, 173, 33, 50}
		);

		game.inputClass = DTetris::Classes::Warrior;
		game.inputStats = {
			gameDB.classes[DTetris::Classes::Warrior].health,
			gameDB.classes[DTetris::Classes::Warrior].strength,
			gameDB.classes[DTetris::Classes::Warrior].agility,
			gameDB.classes[DTetris::Classes::Warrior].endurance
		};

		UpdateClassStats();
		UpdateClassTips();
	};
	hintText2->BindTo(classSelectorBtn);

	// barbarian selector
	classSelectorBtn = (mse::gui::Image*)AddElement(new mse::gui::Image(
		this,
		{10, 85, 14, 13},
		"./data/img/screen-images.png",
		{356, 2, 14, 13},
		{0, 0, 0, 255}));
	classSelectorBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event)
	{
		classAvatarPreviewImage->ChangeSourceArea(
			{166, 171, 33, 52}
		);

		game.inputClass = DTetris::Classes::Barbarian;
		game.inputStats = {
			gameDB.classes[DTetris::Classes::Barbarian].health,
			gameDB.classes[DTetris::Classes::Barbarian].strength,
			gameDB.classes[DTetris::Classes::Barbarian].agility,
			gameDB.classes[DTetris::Classes::Barbarian].endurance
		};

		UpdateClassStats();
		UpdateClassTips();
	};
	hintText3->BindTo(classSelectorBtn);

	// monk selector
	classSelectorBtn = (mse::gui::Image*)AddElement(new mse::gui::Image(
		this,
		{25, 85, 14, 13},
		"./data/img/screen-images.png",
		{371, 2, 14, 13},
		{0, 0, 0, 255}));
	classSelectorBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event)
	{
		classAvatarPreviewImage->ChangeSourceArea(
			{368, 172, 22, 50}
		);

		game.inputClass = DTetris::Classes::Monk;
		game.inputStats = {
			gameDB.classes[DTetris::Classes::Monk].health,
			gameDB.classes[DTetris::Classes::Monk].strength,
			gameDB.classes[DTetris::Classes::Monk].agility,
			gameDB.classes[DTetris::Classes::Monk].endurance
		};

		UpdateClassStats();
		UpdateClassTips();
	};
	hintText4->BindTo(classSelectorBtn);

	// cultist selector
	classSelectorBtn = (mse::gui::Image*)AddElement(new mse::gui::Image(
		this,
		{10, 100, 14, 13},
		"./data/img/screen-images.png",
		{386, 2, 14, 13},
		{0, 0, 0, 255}));
	classSelectorBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event)
	{
		classAvatarPreviewImage->ChangeSourceArea(
			{337, 172, 30, 50}
		);

		game.inputClass = DTetris::Classes::Cultist;
		game.inputStats = {
			gameDB.classes[DTetris::Classes::Cultist].health,
			gameDB.classes[DTetris::Classes::Cultist].strength,
			gameDB.classes[DTetris::Classes::Cultist].agility,
			gameDB.classes[DTetris::Classes::Cultist].endurance
		};

		UpdateClassStats();
		UpdateClassTips();
	};
	hintText5->BindTo(classSelectorBtn);

	// witcher selector
	classSelectorBtn = (mse::gui::Image*)AddElement(new mse::gui::Image(
		this,
		{25, 100, 14, 13},
		"./data/img/screen-images.png",
		{401, 2, 14, 13},
		{0, 0, 0, 255}));
	classSelectorBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event)
	{
		classAvatarPreviewImage->ChangeSourceArea(
			{390, 170, 26, 50}
		);

		game.inputClass = DTetris::Classes::Witcher;
		game.inputStats = {
			gameDB.classes[DTetris::Classes::Witcher].health,
			gameDB.classes[DTetris::Classes::Witcher].strength,
			gameDB.classes[DTetris::Classes::Witcher].agility,
			gameDB.classes[DTetris::Classes::Witcher].endurance
		};

		UpdateClassStats();
		UpdateClassTips();
	};
	hintText6->BindTo(classSelectorBtn);


	// character avatar preview
	classAvatarPreviewImage = (mse::gui::Image*)AddElement(new mse::gui::Image(
		this,
		{50, 60, 33, 52},
		"./data/img/screen-images.png",
		{0, 0, 0, 0},
		{0, 0, 0, 255}));


	// stats and starting trait preview
	classNameTextBox = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{110, 60, 100, 10},
		{0, 0, 0, 255},
		{255, 196, 0, 255})));
	classStatsTextBox = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{110, 70, 100, 50},
		{0, 0, 0, 255},
		{255, 255, 255, 255})));
	classStatsValuesBox = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{210, 70, 100, 40},
		{0, 0, 0, 255},
		{255, 255, 255, 255})));
	classTraitTextBox = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{110, 120, 100, 12},
		{0, 0, 0, 255},
		{255, 128, 255, 255})));
	classTraitHint->BindTo(classTraitTextBox);

	// class tips
	classTipsBox = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{10, 140, 200, 80},
		{0, 0, 0, 255},
		{128, 255, 128, 255})));

	hardcoreTetrisMod = (mse::gui::Checkbox*)(AddElement(new mse::gui::Checkbox(
		this,
		{220, 140, 14, 13},
		"data/img/screen-images.png",
		{0, 0, 0},
		&(game.hardcoreTetrisMod),
		{24, 88, 14, 13},
		{38, 88, 14, 13},
		{52, 88, 14, 13},
		{66, 88, 14, 13})));
	mse::gui::Text* hardcoreTetrisModText = (mse::gui::Text*)AddElement(new mse::gui::Text(
		this,
		U"Хардкор",
		{240, 140, 70, 10},
		{0, 0, 0, 0},
		{128, 255, 128, 255}));
	hardcoreTetrisModText->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
		hardcoreTetrisMod->Toggle();
	};
	hintText7->BindTo(hardcoreTetrisModText);

	mse::gui::Button* proceedBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
		this, U"На подвиги!",
		{32, 32, 32, 255},
		{220, 200, 92, 13},
		"./data/img/screen-images.png",
		{122, 101, 4, 13},
		{138, 101, 4, 13},
		{154, 101, 4, 13}
	)));
	proceedBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [=](SDL_Event* event){
		gsm.ChangeStateTo(DTetris::GamePages::ArenaSetup);
	};

	mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
		this,
		U"Главное меню",
		{32, 32, 32, 255},
		{220, 215, 92, 13},
		"./data/img/screen-images.png",
		{122, 101, 4, 13},
		{138, 101, 4, 13},
		{154, 101, 4, 13})));
	mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
		gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
	};

	// Set default character
	classAvatarPreviewImage->ChangeSourceArea(
		{100, 173, 33, 50}
	);

	game.inputClass = DTetris::Classes::Rogue;
	game.inputStats = {
		gameDB.classes[DTetris::Classes::Rogue].health,
		gameDB.classes[DTetris::Classes::Rogue].strength,
		gameDB.classes[DTetris::Classes::Rogue].agility,
		gameDB.classes[DTetris::Classes::Rogue].endurance
	};

	UpdateClassStats();
	UpdateClassTips();
}

void CharacterCreateUILayer::OnUpdate(mse::TimeType t)
{}

void CharacterCreateUILayer::UpdateClassStats()
{

	float str, agi, end, health = 0.0f; // stats for characters
	std::u32string stats; // stats text gui content
	std::stringstream strstream; // buffer to add numbers
	DTetris::Classes characterClass = game.inputClass;

	classNameTextBox->ChangeText(gameDB.classes[characterClass].name);
	// classTraitTextBox->ChangeText(gameDB.classes[characterClass].);

	health = gameDB.classes[characterClass].health;
	str = gameDB.classes[characterClass].strength;
	agi = gameDB.classes[characterClass].agility;
	end = gameDB.classes[characterClass].endurance;

	strstream <<
	"Здоровье:  " <<
	"\nСила:      " <<
	"\nЛовкость:  " <<
	"\nВыносливость: " <<
	"\nПерк:\n";

		stats = utf8::utf8to32(strstream.str());
		classStatsTextBox->ChangeText(stats);

		strstream.str("");
		strstream.clear();

		strstream <<
		health << "\n" <<
		str << "\n" <<
		agi << "\n" <<
		end;

		stats = utf8::utf8to32(strstream.str());

		classStatsValuesBox->ChangeText(stats);
		strstream.str("");
		strstream.clear();
}

void CharacterCreateUILayer::UpdateClassTips()
{
	std::u32string tips; // stats text gui content
	std::stringstream strstream; // buffer to add numbers
	DTetris::Classes characterClass = game.inputClass;

	strstream <<
	"Подсказки:  \n";

		strstream << utf8::utf32to8(gameDB.classes[characterClass].tips);

		tips = utf8::utf8to32(strstream.str());
		classTipsBox->ChangeText(tips);
		strstream.str("");
		strstream.clear();

		classTraitTextBox->ChangeText(gameDB.traits[gameDB.classes[characterClass].startingTrait].name);

		classTraitHint->ChangeText(gameDB.traits[gameDB.classes[characterClass].startingTrait].description);
}
