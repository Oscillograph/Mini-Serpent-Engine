#include <dungeontris/layers.h>
#include <dungeontris/game-fwd.h>
#include <dungeontris/gamestates.h>
#include <dungeontris/tetris.h>

extern DTetris::GameDB gameDB;
extern DTetris::Game game;
extern DTetris::TetrisMap tetrisMap;
extern GameStateMachine gsm;
extern DTetris::TetrisEngineData DTetris::tetrisEngineData;

void CharacterUpdateUILayer::OnInit()
{
	mse::gui::Text* text2 = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"Развить класс:", {20, 35, 100, 10}, {0, 0, 0, 255}, {255, 255, 255, 255})));

	// new interface
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

	// character avatar preview
	glm::uvec4 character_src = game.playerCharacter.sprite_whole_area;

	characterImage = (mse::gui::Image*)AddElement(new mse::gui::Image(
		this,
		{50, 60, character_src.z, character_src.w},
		"./data/img/screen-images.png",
		character_src,
		{0, 0, 0, 255}));

	glm::uvec4 handle_src = game.playerCharacter.sprite_handle_area;
	glm::uvec4 head_src = game.playerCharacter.sprite_head_area;
	glm::uvec4 head_special_src = game.playerCharacter.sprite_head_special_area;
	glm::uvec4 weapon_src = game.playerCharacter.weapon.sourceArea;
	glm::uvec4 weapon_handle_src = game.playerCharacter.weapon.sourceHandleArea;

	glm::ivec2 handle_delta_src = {
		((int)weapon_src.x - (int)weapon_handle_src.x) - ((int)character_src.x - (int)handle_src.x),
		((int)weapon_src.y - (int)weapon_handle_src.y) - ((int)character_src.y - (int)handle_src.y)
	};

	weaponImage = (mse::gui::Image*)AddElement(new mse::gui::Image(
		this,
		{
			characterImage->layerArea.x + handle_delta_src.x,
			characterImage->layerArea.y + handle_delta_src.y,
			weapon_src.z,
			weapon_src.w
		},
		"./data/img/screen-images.png",
		game.playerCharacter.weapon.sourceArea,
		{0, 0, 0, 255}));

	if (game.playerCharacter.weapon.sprite == DTetris::WeaponSpriteType::LegendarySword)
	{
		headSpecialImage = (mse::gui::Image*)AddElement(new mse::gui::Image(
			this,
			{
				characterImage->layerArea.x + (head_special_src.x - character_src.x),
				characterImage->layerArea.y + (head_special_src.y - character_src.y),
				head_special_src.z,
				head_special_src.w
			},
			"./data/img/screen-images.png",
			game.playerCharacter.sprite_head_special_area,
			{0, 0, 0, 255}));
	} else {
		headSpecialImage = (mse::gui::Image*)AddElement(new mse::gui::Image(
			this,
			{
				characterImage->layerArea.x + (head_src.x - character_src.x),
				characterImage->layerArea.y + (head_src.y - character_src.y),
				head_src.z,
				head_src.w
			},
			"./data/img/screen-images.png",
			game.playerCharacter.sprite_head_area,
			{0, 0, 0, 255}));
	}

	// show stats

	// treasures

	// weapon and traits shops

	// confirm changes
	mse::gui::Button* PlayerCharacterUpdateBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Подтвердить", {32, 32, 32, 255}, {110, 215, 100, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
	PlayerCharacterUpdateBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
		game.playerCharacterUpdated = true;
	};
}

void CharacterUpdateUILayer::OnUpdate(mse::TimeType t)
{
	if (changeMade)
	{
		RecalculateCharacter();
		changeMade = false;
	}
}

void CharacterUpdateUILayer::RecalculateCharacter()
{
	std::u32string stats = U"";
	std::u32string main_class = U"";
	std::u32string sub_class = U"";

	main_class = gameDB.classes[game.playerCharacter.main_class.type].name;

	if (game.playerCharacter.sub_class.type != DTetris::Classes::None)
	{
		sub_class = gameDB.classes[game.playerCharacter.sub_class.type].name;
	} else {
		if (game.inputClass != game.playerCharacter.main_class.type)
		{
			sub_class = gameDB.classes[game.inputClass].name;
		}
	}

	// calculate stats for display
	game.playerCharacter.stats =
	{
		game.playerCharacter.level * (game.playerCharacter.stats_max.health + game.playerCharacter.stats_max.endurance),
		game.playerCharacter.stats_max.strength,
		game.playerCharacter.stats_max.agility,
		game.playerCharacter.stats_max.endurance,
	};

	for (DTetris::Traits trait : game.playerCharacter.traits)
	{
		if (trait == DTetris::Traits::Strong)
		{
			game.playerCharacter.stats.strength++;
		}
		if (trait == DTetris::Traits::Agile)
		{
			game.playerCharacter.stats.agility++;
		}
		if (trait == DTetris::Traits::Survivor)
		{
			game.playerCharacter.stats.endurance++;
			game.playerCharacter.stats.health = game.playerCharacter.level * (game.playerCharacter.stats_max.health + game.playerCharacter.stats.endurance);
		}
	}
}
