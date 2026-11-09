#include <algorithm> // std::find()

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

	// general UI layout

	// weapons
	AddElement(new mse::gui::Text(
		this,
		U"Оружие",
		{60, 20, 60, 10},
		{0, 0, 0, 0},
		{255, 255, 255, 255}
	));

	size_t offset = 2; // because 0 and 1 are not collectible

	for (size_t i = offset; i < 3 + offset; ++i)
	{
		weaponsGUIElements.push_back(
			(mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
				this,
				{ 60 + 40*(i - offset), 30, 40, 27},
				{400 + 40*(i - offset), 41, 40, 27},
				gameDB.weapons[i].cost
			)))
		);

		weaponsGUIElements[i]->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
			PickWeapon(i);
		};
	}

	for (size_t i = 3 + offset; i < 6 + offset; ++i)
	{
		weaponsGUIElements.push_back(
			(mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
				this,
				{ 60 + 40*(i - offset), 60, 40, 27},
				{400 + 40*(i - offset), 41, 40, 27},
				gameDB.weapons[i].cost
			)))
		);
	}
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

	// traits
	AddElement(new mse::gui::Text(
		this,
		U"Особенности",
		{60, 80, 100, 10},
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
				{ 60 + 40*(i - offset), 30, 40, 27},
				{400 + 40*(i - offset), 68, 40, 27},
				gameDB.traits[(DTetris::Traits)i].cost
			)))
		);

		traitsGUIElements[i]->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
			PickTrait((DTetris::Traits)i);
		};
	}

	for (size_t i = 5 + offset; i < 12 + offset; ++i)
	{
		traitsGUIElements.push_back(
			(mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
				this,
				{ 60 + 40*(i - offset), 60, 40, 27},
				{400 + 40*(i - offset), 95, 40, 27},
				gameDB.traits[(DTetris::Traits)i].cost
			)))
		);

		traitsGUIElements[i]->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
			PickTrait((DTetris::Traits)i);
		};
	}

	for (size_t i = 10 + offset; i < 18 + offset; ++i)
	{
		traitsGUIElements.push_back(
			(mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
				this,
				{ 60 + 40*(i - offset), 90, 40, 27},
				{400 + 40*(i - offset), 122, 40, 27},
				gameDB.traits[(DTetris::Traits)i].cost
			)))
		);

		traitsGUIElements[i]->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
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
