#include "mse/systems/windows/layers/gui/items/button.h"
#include "mse/systems/windows/layers/gui/items/field.h"
#include "mse/utils/logger.h"
#include <mse/systems/platform/events/events.h>
#include <dungeontris/layers.h>
#include <dungeontris/gui.h>
#include <dungeontris/game-fwd.h>
#include <dungeontris/gamestates.h>
#include <dungeontris/tetris.h>

extern DTetris::GameDB gameDB;
extern DTetris::Game game;
extern DTetris::TetrisMap tetrisMap;
extern GameStateMachine gsm;
extern DTetris::TetrisEngineData DTetris::tetrisEngineData;


ShopUILayer::ShopUILayer() : mse::Layer()
{
	MSE_LOG("Constructed a ShopUI layer");
}

ShopUILayer::~ShopUILayer()
{
	MSE_LOG("Destroyed a ShopUI layer");
}

void ShopUILayer::OnInit()
{
	mse::gui::Image* backgroundImage = (mse::gui::Image*)(AddElement(new mse::gui::Image(
		this,
		{0, 0, 320, 240},
		"./data/img/background.png",
		{0, 0, 320, 240},
		{0, 0, 0, 255})));
}

void ShopUILayer::OnUpdate(mse::TimeType t)
{}


ShopWeaponsUILayer::ShopWeaponsUILayer()
{
	MSE_LOG("Constructed a ShopWeaponsUI layer");
}
ShopWeaponsUILayer::~ShopWeaponsUILayer()
{
	MSE_LOG("Destroyed a ShopWeaponsUI layer");
}

void ShopWeaponsUILayer::OnInit()
{
	weaponsGUIElements.reserve(6);

	// weapons
	AddElement(new mse::gui::Text(
		this,
		U"Оружие",
		{40, 70, 60, 10},
		{0, 0, 0, 0},
		{255, 255, 255, 255}
	));

	size_t offset = 2; // because 0 and 1 are not collectible

	for (size_t i = offset; i < 3 + offset; ++i)
	{
		weaponsGUIElements.push_back(
			(mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
				this,
				{ 40 + 40*(i - offset), 90, 40, 27},
				gameDB.weapons[i].sourceButtonArea,
				gameDB.weapons[i].cost
			)))
		);

		weaponsGUIElements[weaponsGUIElements.size()-1]->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
			PickWeapon(i);
		};
	}

	for (size_t i = 3 + offset; i < 6 + offset; ++i)
	{
		weaponsGUIElements.push_back(
			(mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
				this,
				{ 40 + 40*(i - 3 - offset), 120, 40, 27},
				gameDB.weapons[i].sourceButtonArea,
				gameDB.weapons[i].cost
			)))
		);

		weaponsGUIElements[weaponsGUIElements.size()-1]->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
			PickWeapon(i);
		};
	}

	closeBtn = (mse::gui::Button*)AddElement(new mse::gui::Button(
		this,
		{176, 60, 14, 13},
		"./data/img/screen-images.png",
		{0, 0, 0},
		{24, 102, 14, 13},
		{38, 102, 14, 13},
		{52, 102, 14, 13},
		{66, 102, 14, 13}
	));
	closeBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
		gsm.ChangeStateTo(DTetris::GamePages::CharacterUpdate);
	};

	// general UI layout
	AddElement(new mse::gui::Field(
		this,
		{30, 60, 160, 140},
		{64, 32, 32, 255},
		true,
		1,
		{255, 255, 255, 255}
	));
}

void ShopWeaponsUILayer::OnUpdate(mse::TimeType t)
{}

ShopTraitsUILayer::ShopTraitsUILayer()
{
	MSE_LOG("Constructed a ShopTraitsUI layer");
}
ShopTraitsUILayer::~ShopTraitsUILayer()
{
	MSE_LOG("Destroyed a ShopTraitsUI layer");
}


void ShopTraitsUILayer::OnInit()
{
	traitsGUIElements.reserve(18);

	// general UI layout
	AddElement(new mse::gui::Field(
		this,
		{30, 60, 260, 140},
		{128, 64, 128, 255},
		true,
		1,
		{255, 255, 255, 255}
	));

	closeBtn = (mse::gui::Button*)AddElement(new mse::gui::Button(
		this,
		{290, 60, 14, 13},
		"./data/img/screen-images.png",
		{0, 0, 0},
		{24, 102, 14, 13},
		{38, 102, 14, 13},
		{52, 102, 14, 13},
		{66, 102, 14, 13}
	));

	closeBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
		gsm.ChangeStateTo(DTetris::GamePages::CharacterUpdate);
	};

	// traits
	AddElement(new mse::gui::Text(
		this,
		U"Особенности",
		{30, 80, 100, 10},
		{0, 0, 0, 0},
		{255, 255, 255, 255}
	));

	size_t offset = 1; // because 0 and 1 are not collectible
	size_t offset_y = 0;
	size_t offset_x = 0;

	for (size_t i = offset; i < 6 + offset; ++i)
	{
		traitsGUIElements.push_back(
			(mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
				this,
				{ 30 + 40*(i - offset), 30, 40, 27},
				gameDB.traits[(DTetris::Traits)i].sprite_area,
				gameDB.traits[(DTetris::Traits)i].cost
			)))
		);

		traitsGUIElements[traitsGUIElements.size()-1]->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
			PickTrait((DTetris::Traits)i);
		};
	}

	for (size_t i = 5 + offset; i < 12 + offset; ++i)
	{
		traitsGUIElements.push_back(
			(mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
				this,
				{ 30 + 40*(i - 6 - offset), 60, 40, 27},
				gameDB.traits[(DTetris::Traits)i].sprite_area,
				gameDB.traits[(DTetris::Traits)i].cost
			)))
		);

		traitsGUIElements[traitsGUIElements.size()-1]->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
			PickTrait((DTetris::Traits)i);
		};
	}

	for (size_t i = 10 + offset; i < 18 + offset; ++i)
	{
		traitsGUIElements.push_back(
			(mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
				this,
				{ 30 + 40*(i - 12 - offset), 90, 40, 27},
				gameDB.traits[(DTetris::Traits)i].sprite_area,
				gameDB.traits[(DTetris::Traits)i].cost
			)))
		);

		traitsGUIElements[traitsGUIElements.size()-1]->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
			PickTrait((DTetris::Traits)i);
		};
	}
}
void ShopTraitsUILayer::OnUpdate(mse::TimeType t)
{}


void PickTrait(DTetris::Traits trait)
{
	if (!game.shop_traits[trait])
	{
		// not owned
		if (game.playerCharacter.blocks[DTetris::BlockType::Treasure] - gameDB.traits[trait].cost >= 0)
		{
			game.playerCharacter.blocks[DTetris::BlockType::Treasure] = game.playerCharacter.blocks[DTetris::BlockType::Treasure] - gameDB.traits[trait].cost;
			game.shop_traits[trait] = true;
		}
	} else {
		// owned
		if (game.playerCharacter.traits[(size_t)game.traitSlot] == trait)
		{
			// unpick
			game.playerCharacter.traits[(size_t)game.traitSlot] = DTetris::Traits::None;

			game.playerCharacterUpdated = true;
		} else {
			// pick
			game.playerCharacter.traits[(size_t)game.traitSlot] = trait;

			game.playerCharacterUpdated = true;

			gsm.ChangeStateTo(DTetris::GamePages::CharacterUpdate);
		}
	}
}

void PickWeapon(size_t weapon)
{
	if (!game.shop_weapons[(DTetris::WeaponSpriteType)weapon])
	{
		// not owned
		if (game.playerCharacter.blocks[DTetris::BlockType::Treasure] - gameDB.weapons[weapon].cost >= 0)
		{
			game.playerCharacter.blocks[DTetris::BlockType::Treasure] = game.playerCharacter.blocks[DTetris::BlockType::Treasure] - gameDB.weapons[weapon].cost;
			game.shop_weapons[(DTetris::WeaponSpriteType)weapon] = true;
		}
	} else {
		// owned
		if (game.playerCharacter.weapon.sprite == (DTetris::WeaponSpriteType)weapon)
		{
			// unpick
			game.inputWeapon = gameDB.weapons[0]; // fists

			game.playerCharacterUpdated = true;
		} else {
			// pick
			game.inputWeapon = gameDB.weapons[(size_t)weapon];

			if (game.inputWeapon.sprite == DTetris::WeaponSpriteType::LegendarySword)
			{
				game.inputWeapon.sourceArea.x = game.inputWeapon.sourceArea.x + (game.inputWeapon.sourceArea.z) * ((int)game.playerCharacter.main_class.type - 1);
				game.inputWeapon.sourceHandleArea.x = game.inputWeapon.sourceHandleArea.x + (game.inputWeapon.sourceArea.z) * ((int)game.playerCharacter.main_class.type - 1);
			}

			game.playerCharacterUpdated = true;

			gsm.ChangeStateTo(DTetris::GamePages::CharacterUpdate);
		}
	}
}
