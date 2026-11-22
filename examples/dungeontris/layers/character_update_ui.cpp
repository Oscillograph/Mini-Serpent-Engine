#include "mse/systems/platform/events/events.h"
#include "mse/systems/windows/layers/gui/items/button.h"
#include "mse/systems/windows/layers/gui/items/image.h"
#include "utf8/cpp17.h"
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
	mse::gui::Text* text2 = (mse::gui::Text*)(AddElement(
		new mse::gui::Text(
			this,
			U"Развить класс:",
			{20, 35, 100, 10},
			{0, 0, 0, 255},
			{255, 255, 255, 255})));

	levelUpBtn = (mse::gui::Button*)(AddElement(
		new mse::gui::Button(
			this,
			U"10",
			{32, 32, 32, 255},
			{130, 35, 35, 12},
			"./data/img/screen-images.png",
			{327, 19, 11, 12},
			{327, 33, 4, 12},
			{343, 33, 4, 12}
		)));

	levelUpBtn->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
		game.playerCharacter.level += 1;
		game.playerCharacterUpdated = true;
	};

	classStatsTextBox = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{110, 60, 100, 50},
		{0, 0, 0, 255},
		{255, 255, 255, 255})));
	classStatsValuesBox = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{210, 60, 30, 50},
		{0, 0, 0, 255},
		{255, 255, 255, 255})));
	AddElement(new mse::gui::Image(
		this,
		{245, 60, 10, 10},
		"./data/img/screen-images.png",
		{14, 11, 10, 10},
		{0, 0, 0, 0}
	));
	treasuresValueTextBox = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{260, 60, 30, 40},
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
	int level = 0;
	float str, agi, end, health = 0.0f; // stats for characters
	std::u32string stats; // stats text gui content
	std::stringstream strstream; // buffer to add numbers
	strstream <<
	"Уровень:" <<
	"\nЗдоровье:  " <<
	"\nСила:      " <<
	"\nЛовкость:  " <<
	"\nВыносливость: ";

	stats = utf8::utf8to32(strstream.str());
	classStatsTextBox->ChangeText(stats);
	strstream.str("");
	strstream.clear();

	level = game.playerCharacter.level;
	health = game.playerCharacter.stats.health;
	str = game.playerCharacter.stats.strength;
	agi = game.playerCharacter.stats.agility;
	end = game.playerCharacter.stats.endurance;
	strstream <<
	level << "\n" <<
	health << "\n" <<
	str << "\n" <<
	agi << "\n" <<
	end;

	stats = utf8::utf8to32(strstream.str());

	classStatsValuesBox->ChangeText(stats);
	strstream.str("");
	strstream.clear();

	// treasures
	strstream << game.playerCharacter.blocks[DTetris::BlockType::Treasure];
	stats = utf8::utf8to32(strstream.str());
	treasuresValueTextBox->ChangeText(stats);
	strstream.str("");
	strstream.clear();

	// weapon and traits shops
	if (game.playerCharacter.main_class.type != DTetris::Classes::Monk)
	{
		slotAImage = (mse::gui::Image*)AddElement(new mse::gui::Image(
			this,
			{50, 120, 40, 27},
			"./data/img/screen-images.png",
			gameDB.weapons[game.playerCharacter.weapon.id].sourceButtonArea,
			{0, 0, 0, 255}
		));
	} else {
		slotAImage = (mse::gui::Image*)AddElement(new mse::gui::Image(
			this,
			{50, 120, 40, 27},
			"./data/img/screen-images.png",
			gameDB.traits[game.playerCharacter.traits[(size_t)DTetris::TraitSlot::A]].sprite_area,
			{0, 0, 0, 255}
		));
	}

	slotABtn = (mse::gui::Button*)AddElement(new mse::gui::Button(
		this,
		slotAImage->layerArea,
		"./data/img/screen-images.png",
		{0, 0, 0},
		{600, 0, 40, 27},
		{560, 0, 40, 27},
		{560, 0, 40, 27},
		{600, 0, 40, 27}
	)); // weapon or trait if Monk
	slotABtn->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
		if (slotABtn->isPushed)
		{
			if (game.playerCharacter.main_class.type == DTetris::Classes::Monk)
			{
				game.shopType = DTetris::ShopTypes::Traits;
			} else {
				game.shopType = DTetris::ShopTypes::Weapons;
			}

			game.traitSlot = DTetris::TraitSlot::A;
			gsm.ChangeStateTo(DTetris::GamePages::Shop, this);
		}
	};


	slotBImage = (mse::gui::Image*)AddElement(new mse::gui::Image(
		this,
		{90, 120, 40, 27},
		"./data/img/screen-images.png",
		gameDB.traits[game.playerCharacter.traits[(size_t)DTetris::TraitSlot::B]].sprite_area,
		{0, 0, 0, 255}
	));
	slotBBtn = (mse::gui::Button*)AddElement(new mse::gui::Button(
		this,
		slotBImage->layerArea,
		"./data/img/screen-images.png",
		{0, 0, 0},
		{600, 0, 40, 27},
		{560, 0, 40, 27},
		{560, 0, 40, 27},
		{600, 0, 40, 27}
	));
	slotBBtn->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
		if (slotBBtn->isPushed)
		{
			game.shopType = DTetris::ShopTypes::Traits;
			game.traitSlot = DTetris::TraitSlot::B;
			gsm.ChangeStateTo(DTetris::GamePages::Shop, this);
		}
	};

	slotCImage = (mse::gui::Image*)AddElement(new mse::gui::Image(
		this,
		{130, 120, 40, 27},
		"./data/img/screen-images.png",
		gameDB.traits[game.playerCharacter.traits[(size_t)DTetris::TraitSlot::C]].sprite_area,
		{0, 0, 0, 255}
	));
	slotCBtn = (mse::gui::Button*)AddElement(new mse::gui::Button(
		this,
		slotCImage->layerArea,
		"./data/img/screen-images.png",
		{0, 0, 0},
		{600, 0, 40, 27},
		{560, 0, 40, 27},
		{560, 0, 40, 27},
		{600, 0, 40, 27}
	));
	slotCBtn->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
		if (slotCBtn->isPushed)
		{
			game.shopType = DTetris::ShopTypes::Traits;
			game.traitSlot = DTetris::TraitSlot::C;
			gsm.ChangeStateTo(DTetris::GamePages::Shop, this);
		}
	};

	// confirm changes
	mse::gui::Button* PlayerCharacterUpdateBtn = (mse::gui::Button*)(AddElement(
		new mse::gui::Button(
			this,
			U"Подтвердить",
			{32, 32, 32, 255},
			{110, 215, 100, 13},
			"./data/img/screen-images.png",
			{122, 101, 4, 13},
			{138, 101, 4, 13},
			{154, 101, 4, 13}
		)));
	PlayerCharacterUpdateBtn->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
		// RecalculateCharacter();
		gsm.ChangeStateTo(DTetris::GamePages::ArenaSetup);
		// game.playerCharacterUpdated = true;
	};


	// hints
	slotAHint = (mse::gui::Hint*)AddElement(
		new mse::gui::Hint(
			this,
			slotABtn,
			U"",
			{0, 0, 200, 30},
			{32, 32, 32, 196},
			{218, 218, 255, 255},
			nullptr,
			1,
			true,
			{255, 255, 255, 255}
		));
	if (game.playerCharacter.main_class.type == DTetris::Classes::Monk)
	{
		slotAHint->isVisible = true;
	} else {
		slotAHint->isVisible = false;
	}
	slotBHint = (mse::gui::Hint*)AddElement(
		new mse::gui::Hint(
			this,
			slotBBtn,
			U"",
			{0, 0, 200, 30},
			{32, 32, 32, 196},
			{218, 218, 255, 255},
			nullptr,
			1,
			true,
			{255, 255, 255, 255}
		));
	slotCHint = (mse::gui::Hint*)AddElement(
		new mse::gui::Hint(
			this,
			slotCBtn,
			U"",
			{0, 0, 200, 30},
			{32, 32, 32, 196},
			{218, 218, 255, 255},
			nullptr,
			1,
			true,
			{255, 255, 255, 255}
		));

	game.playerCharacterUpdated = true;
}

void CharacterUpdateUILayer::OnUpdate(mse::TimeType t)
{
	if (game.playerCharacterUpdated)
	{
		RecalculateCharacter();

		if (game.playerCharacter.main_class.type == DTetris::Classes::Monk)
		{
			slotAImage->ChangeSourceArea(gameDB.traits[game.playerCharacter.traits[(size_t)DTetris::TraitSlot::A]].sprite_area);
			slotAHint->ChangeText(gameDB.traits[game.playerCharacter.traits[(size_t)DTetris::TraitSlot::A]].description);
			slotAHint->isVisible = true;
		} else {
			slotAImage->ChangeSourceArea(gameDB.weapons[game.playerCharacter.weapon.id].sourceButtonArea);
			slotAHint->ChangeText(game.playerCharacter.weapon.name);
			slotAHint->isVisible = false;
		}

		slotBImage->ChangeSourceArea(gameDB.traits[game.playerCharacter.traits[(size_t)DTetris::TraitSlot::B]].sprite_area);
		slotBHint->ChangeText(gameDB.traits[game.playerCharacter.traits[(size_t)DTetris::TraitSlot::B]].description);

		slotCImage->ChangeSourceArea(gameDB.traits[game.playerCharacter.traits[(size_t)DTetris::TraitSlot::C]].sprite_area);
		slotCHint->ChangeText(gameDB.traits[game.playerCharacter.traits[(size_t)DTetris::TraitSlot::C]].description);

		UpdateCharacterStatsBox();

		game.playerCharacterUpdated = false;
	}
}

void CharacterUpdateUILayer::RecalculateCharacter()
{
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

void CharacterUpdateUILayer::UpdateCharacterStatsBox()
{
	std::stringstream strstream; // buffer to add numbers

	strstream <<
	game.playerCharacter.level << "\n" <<
	game.playerCharacter.stats_max.health << "\n" <<
	game.playerCharacter.stats_max.strength << "\n" <<
	game.playerCharacter.stats_max.agility << "\n" <<
	game.playerCharacter.stats_max.endurance << "\n";

	classStatsValuesBox->ChangeText(utf8::utf8to32(strstream.str()));
}
