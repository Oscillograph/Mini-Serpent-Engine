#include <dungeontris/layers.h>
#include <dungeontris/game-fwd.h>
#include <dungeontris/gamestates.h>
#include <dungeontris/tetris.h>

extern DTetris::GameDB gameDB;
extern DTetris::Game game;
extern DTetris::TetrisMap tetrisMap;
extern GameStateMachine gsm;
extern DTetris::TetrisEngineData DTetris::tetrisEngineData;

ArenaUILayer::ArenaUILayer() : mse::Layer()
{
	MSE_LOG("Constructed ArenaUILayer");
}
ArenaUILayer::~ArenaUILayer()
{
	//    messageLog = nullptr;
	tetrisMapGUI = nullptr;

	int i = 0;
	game.UILogger.Clear();

	MSE_LOG("Destroyed ArenaUILayer");
}

void ArenaUILayer::OnInit()
{
	// npc life
	AddElement(new mse::gui::Image(this, {20, 15, 13, 13}, "./data/img/screen-images.png", {168, 119, 13, 13}, {0, 0, 0, 255}));
	npcLife = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{35, 15, 40, 10},
		{0, 0, 0, 255},
		{255, 196, 0, 255})));
	// npc character
	{
		glm::uvec4 src = {0, 0, 34, 64};

		src = game.npcCharacter.sprite_whole_area;

		AddElement(new mse::gui::Image(this, {40, 90 - src.w, src.z, src.w}, "./data/img/screen-images.png", src, {0, 0, 0, 255}));
	}

	AddElement(new mse::gui::Text(
		this,
		game.npcCharacter.name,
		{10, 100, 80, 10},
		{0, 0, 0, 255},
		{255, 255, 255, 255}));

	AddElement(new mse::gui::Image(this, {260, 15, 13, 13}, "./data/img/screen-images.png", {168, 119, 13, 13}, {0, 0, 0, 255}));
	AddElement(new mse::gui::Text(
		this,
		U"Ловкость",
		{10, 120, 60, 10},
		{0, 0, 0, 255},
		{255, 196, 0, 255}));
	npcStatsAgi = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{70, 120, 20, 10},
		{0, 0, 0, 255},
		{255, 255, 255, 255})));
	AddElement(new mse::gui::Text(
		this,
		U"Вынос-ть",
		{10, 130, 60, 10},
		{0, 0, 0, 255},
		{255, 196, 0, 255}));
	npcStatsEnd = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{70, 130, 20, 10},
		{0, 0, 0, 255},
		{255, 255, 255, 255})));
	AddElement(new mse::gui::Text(
		this,
		U"Сила",
		{10, 140, 60, 10},
		{0, 0, 0, 255},
		{255, 196, 0, 255}));
	npcStatsStr = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{70, 140, 20, 10},
		{0, 0, 0, 255},
		{255, 255, 255, 255})));

	std::stringstream statsContents;
	statsContents << game.npcCharacter.stats.agility;
	npcStatsAgi->ChangeText(utf8::utf8to32(statsContents.str()));
	statsContents.str("");
	statsContents.clear();
	statsContents << game.npcCharacter.stats.strength;
	npcStatsStr->ChangeText(utf8::utf8to32(statsContents.str()));
	statsContents.str("");
	statsContents.clear();
	statsContents << game.npcCharacter.stats.endurance;
	npcStatsEnd->ChangeText(utf8::utf8to32(statsContents.str()));
	statsContents.str("");
	statsContents.clear();

	AddElement(new mse::gui::Text(
		this,
		U"Особенности:",
		{10, 150, 90, 10},
		{0, 0, 0, 0},
		{255, 196, 0, 255}));
	for (size_t i = 0; i < game.npcCharacter.traits.size(); ++i)
	{
		AddElement(new mse::gui::Text(
			this,
			gameDB.traits[game.npcCharacter.traits[i]].name,
			{10, 160 + i*10, 90, 10},
			{0, 0, 0, 0},
			{255, 255, 255, 255}));
	}

	// player life
	AddElement(new mse::gui::Image(this, {260, 15, 13, 13}, "./data/img/screen-images.png", {168, 119, 13, 13}, {0, 0, 0, 255}));
	playerLife = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{275, 15, 40, 10},
		{0, 0, 0, 255},
		{255, 196, 0, 255})));
	// player character
	{
		glm::uvec4 character_src = game.playerCharacter.sprite_whole_area;
		glm::uvec4 handle_src = game.playerCharacter.sprite_handle_area;
		glm::uvec4 head_src = game.playerCharacter.sprite_head_area;
		glm::uvec4 head_special_src = game.playerCharacter.sprite_head_special_area;
		glm::uvec4 weapon_src = game.playerCharacter.weapon.sourceArea;
		glm::uvec4 weapon_handle_src = game.playerCharacter.weapon.sourceHandleArea;

		glm::ivec2 handle_delta_src = {
			((int)weapon_src.x - (int)weapon_handle_src.x) - ((int)character_src.x - (int)handle_src.x),
			((int)weapon_src.y - (int)weapon_handle_src.y) - ((int)character_src.y - (int)handle_src.y)
		};

		// head accordingly to the lightsaber
		if (game.playerCharacter.weapon.sprite == DTetris::WeaponSpriteType::LegendarySword)
		{
			// monk and witcher can't have their own heads: monk doesn't turn the sword on, and witcher is just too Geralt
			if ((game.playerCharacter.main_class.type != DTetris::Classes::Monk) &&
				(game.playerCharacter.main_class.type != DTetris::Classes::Witcher))
			{
				AddElement(new mse::gui::Image(
					this,
					{
						250 + head_src.x - character_src.x,
						90 + (head_src.y - character_src.y) - character_src.w,
						head_src.z,
						head_src.w
					},
					"./data/img/screen-images.png",
					head_special_src,
					{0, 0, 0, 255}
				));
			}
		}

		// weapon
		AddElement(new mse::gui::Image(
			this,
			{250 + handle_delta_src.x, 90 - character_src.w + handle_delta_src.y, weapon_src.z, weapon_src.w},
			"./data/img/screen-images.png",
			weapon_src,
			{0, 0, 0, 255}));

		// character
		AddElement(new mse::gui::Image(
			this,
			{250, 90 - character_src.w, character_src.z, character_src.w},
			"./data/img/screen-images.png",
			character_src,
			{0, 0, 0, 255}));
	}

	AddElement(new mse::gui::Image(this, {250, 120, 10, 10}, "./data/img/screen-images.png", {64, 11, 10, 10}, {0, 0, 0, 255}));
	blocksAttack = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{270, 120, 40, 10},
		{0, 0, 0, 255},
		{255, 255, 255, 255})));
	AddElement(new mse::gui::Image(this, {250, 130, 10, 10}, "./data/img/screen-images.png", {84, 11, 10, 10}, {0, 0, 0, 255}));
	blocksDefense = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{270, 130, 40, 10},
		{0, 0, 0, 255},
		{255, 255, 255, 255})));
	AddElement(new mse::gui::Image(this, {250, 140, 10, 10}, "./data/img/screen-images.png", {74, 11, 10, 10}, {0, 0, 0, 255}));
	blocksHealing = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{270, 140, 40, 10},
		{0, 0, 0, 255},
		{255, 255, 255, 255})));
	AddElement(new mse::gui::Image(this, {250, 150, 10, 10}, "./data/img/screen-images.png", {14, 11, 10, 10}, {0, 0, 0, 255}));
	blocksTreasure = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{270, 150, 40, 10},
		{0, 0, 0, 255},
		{255, 255, 255, 255})));

	winMessage = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"ПОБЕДА",
		{115, 30, 100, 20},
		{0, 0, 0, 0},
		{196, 255, 0, 255},
		2)));
	winMessage->isVisible = false;
	gameOverMessage = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"ПРОВАЛ",
		{115, 30, 100, 20},
		{0, 0, 0, 0},
		{255, 96, 96, 255},
		2)));
	gameOverMessage->isVisible = false;
	pauseMessage = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"ПАУЗА",
		{120, 30, 100, 20},
		{0, 0, 0, 0},
		{255, 196, 255, 255},
		2)));
	pauseMessage->isVisible = false;

	// tetris map
	tetrisMapGUI = (mse::gui::TetrisMapGUI*)(AddElement(new mse::gui::TetrisMapGUI(
		this,
		{100, 10, 150, 200},
		"data/img/screen-images.png",
		&tetrisMap,
		tetrisMap.width,
		tetrisMap.height
	)));

	int elementId = this->GetElementId(tetrisMapGUI);
	this->elementInFocus = elementId;

	// bottom line
	AddElement(new mse::gui::Text(
		this,
		U"Ход:",
		{30, 215, 35, 10},
		{0, 0, 0, 255},
		{255, 255, 255, 255},
		1
	));
	turnNumber = (mse::gui::Text*)AddElement(new mse::gui::Text(
		this,
		U"",
		{65, 215, 40, 10},
		{0, 0, 0, 255},
		{255, 255, 255, 255},
		1
	));

	nextBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
		this, U"Продолжить", {32, 32, 32, 255}, {120, 215, 85, 13},
		"./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
	nextBtn->Disable();
	nextBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
		game.gamePageHasToChange = true;
	};

	mse::gui::Text* battleNumber = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{220, 215, 90, 10},
		{0, 0, 0, 255},
		{255, 255, 255, 255})));
	std::stringstream battleNumberContents;
	battleNumberContents << "Битва " << (game.battleCounter + 1) << " из 5";
	battleNumber->ChangeText(utf8::utf8to32(battleNumberContents.str()));
	battleNumberContents.str("");
	battleNumberContents.clear();
}

void ArenaUILayer::OnUpdate(mse::TimeType t)
{
	//    std::u32string npcLifeTextContent = U"";
	std::stringstream tempStream;
	tempStream << game.npcCharacter.stats.health;
	npcLife->ChangeText(utf8::utf8to32(tempStream.str()));
	tempStream.str("");
	tempStream.clear();

	tempStream << game.playerCharacter.stats.health;
	playerLife->ChangeText(utf8::utf8to32(tempStream.str()));
	tempStream.str("");
	tempStream.clear();

	tempStream << game.turn;
	turnNumber->ChangeText(utf8::utf8to32(tempStream.str()));
	tempStream.str("");
	tempStream.clear();

	tempStream << game.playerCharacter.blocks[DTetris::BlockType::Attack];
	blocksAttack->ChangeText(utf8::utf8to32(tempStream.str()));
	tempStream.str("");
	tempStream.clear();
	tempStream << game.playerCharacter.blocks[DTetris::BlockType::Defense];
	blocksDefense->ChangeText(utf8::utf8to32(tempStream.str()));
	tempStream.str("");
	tempStream.clear();
	tempStream << game.playerCharacter.blocks[DTetris::BlockType::Healing];
	blocksHealing->ChangeText(utf8::utf8to32(tempStream.str()));
	tempStream.str("");
	tempStream.clear();
	tempStream << game.playerCharacter.blocks[DTetris::BlockType::Treasure];
	blocksTreasure->ChangeText(utf8::utf8to32(tempStream.str()));
	tempStream.str("");
	tempStream.clear();

	if ((game.battleFinished) && !(nextBtn->isEnabled))
	{
		nextBtn->Enable();
	}

	if (DTetris::tetrisEngineData.state == DTetris::TetrisStates::Win)
	{
		if (!winMessage->isVisible)
		{
			winMessage->isVisible = true;
		}
	} else {
		if (winMessage->isVisible)
		{
			winMessage->isVisible = false;
		}
	}

	if (DTetris::tetrisEngineData.state == DTetris::TetrisStates::GameOver)
	{
		if (!gameOverMessage->isVisible)
		{
			gameOverMessage->isVisible = true;
		}
	} else {
		if (gameOverMessage->isVisible)
		{
			gameOverMessage->isVisible = false;
		}
	}

	if (DTetris::tetrisEngineData.state == DTetris::TetrisStates::Paused)
	{
		if (!pauseMessage->isVisible)
		{
			pauseMessage->isVisible = true;
		}
	} else {
		if (pauseMessage->isVisible)
		{
			pauseMessage->isVisible = false;
		}
	}
}
