#include <mse/systems/windows/layers/gui/items/button.h>
#include <mse/systems/windows/layers/gui/items/field.h>
#include <mse/utils/logger.h>
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

	// printf("gameDB.weapons.size: %d\n", (int)gameDB.weapons.size());
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

		if (game.shop_weapons[gameDB.weapons[i].sprite])
		{
			weaponsGUIElements[i - offset]->state += mse::gui::StoreItemStates::Bought;
		}

		if (gameDB.weapons[i].id == game.playerCharacter.weapon.id)
		{
			weaponsGUIElements[i - offset]->state += mse::gui::StoreItemStates::Equipped;
		}

		// printf("weaponsGUIElements[%d - 2]->callback MBtDn.\n", (int)i);
		weaponsGUIElements[i - offset]->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&, i](SDL_Event* event){
			// printf("weaponsGUIElements[%d - 2]->callback MBtDn.\n", (int)i);
			PickWeapon(i, this);
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

		if (game.shop_weapons[gameDB.weapons[i].sprite])
		{
			weaponsGUIElements[i - offset]->state += mse::gui::StoreItemStates::Bought;
		}

		if (gameDB.weapons[i].id == game.playerCharacter.weapon.id)
		{
			weaponsGUIElements[i - offset]->state += mse::gui::StoreItemStates::Equipped;
		}

		// printf("weaponsGUIElements[%d - 2]->callback MBtDn.\n", (int)i);
		weaponsGUIElements[i - offset]->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&, i](SDL_Event* event){
			PickWeapon(i, this);
		};
	}

	// general UI layout
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

	// traits
	AddElement(new mse::gui::Text(
		this,
		U"Особенности",
		{30, 80, 100, 10},
		{0, 0, 0, 0},
		{255, 255, 255, 255}
	));

	size_t offset = 1; // because 0 is not collectible

	for (size_t i = offset; i < 6 + offset; ++i)
	{
		traitsGUIElements.push_back(
			(mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
				this,
				{ 35 + 40*(i - offset), 100, 40, 27},
				gameDB.traits[(DTetris::Traits)i].sprite_area,
				gameDB.traits[(DTetris::Traits)i].cost
			)))
		);

		if (game.shop_traits[(DTetris::Traits)i])
		{
			traitsGUIElements[traitsGUIElements.size()-1]->state += mse::gui::StoreItemStates::Bought;
		}

		if (((DTetris::Traits)i == game.playerCharacter.traits[(size_t)DTetris::TraitSlot::A]) ||
			((DTetris::Traits)i == game.playerCharacter.traits[(size_t)DTetris::TraitSlot::B]) ||
			((DTetris::Traits)i == game.playerCharacter.traits[(size_t)DTetris::TraitSlot::C]))
		{
			traitsGUIElements[traitsGUIElements.size()-1]->state += mse::gui::StoreItemStates::Equipped;
		}

		traitsGUIElements[traitsGUIElements.size()-1]->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&, i](SDL_Event* event){
			PickTrait((DTetris::Traits)i, this);
		};
	}

	for (size_t i = 6 + offset; i < 12 + offset; ++i)
	{
		traitsGUIElements.push_back(
			(mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
				this,
				{ 35 + 40*(i - 6 - offset), 130, 40, 27},
				gameDB.traits[(DTetris::Traits)i].sprite_area,
				gameDB.traits[(DTetris::Traits)i].cost
			)))
		);

		if (game.shop_traits[(DTetris::Traits)i])
		{
			traitsGUIElements[traitsGUIElements.size()-1]->state += mse::gui::StoreItemStates::Bought;
		}

		if (((DTetris::Traits)i == game.playerCharacter.traits[(size_t)DTetris::TraitSlot::A]) ||
			((DTetris::Traits)i == game.playerCharacter.traits[(size_t)DTetris::TraitSlot::B]) ||
			((DTetris::Traits)i == game.playerCharacter.traits[(size_t)DTetris::TraitSlot::C]))
		{
			traitsGUIElements[traitsGUIElements.size()-1]->state += mse::gui::StoreItemStates::Equipped;
		}

		traitsGUIElements[traitsGUIElements.size()-1]->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&, i](SDL_Event* event){
			PickTrait((DTetris::Traits)i, this);
		};
	}

	for (size_t i = 12 + offset; i < 18 + offset; ++i)
	{
		traitsGUIElements.push_back(
			(mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
				this,
				{ 35 + 40*(i - 12 - offset), 160, 40, 27},
				gameDB.traits[(DTetris::Traits)i].sprite_area,
				gameDB.traits[(DTetris::Traits)i].cost
			)))
		);

		if (game.shop_traits[(DTetris::Traits)i])
		{
			traitsGUIElements[traitsGUIElements.size()-1]->state += mse::gui::StoreItemStates::Bought;
		}

		if (((DTetris::Traits)i == game.playerCharacter.traits[(size_t)DTetris::TraitSlot::A]) ||
			((DTetris::Traits)i == game.playerCharacter.traits[(size_t)DTetris::TraitSlot::B]) ||
			((DTetris::Traits)i == game.playerCharacter.traits[(size_t)DTetris::TraitSlot::C]))
		{
			traitsGUIElements[traitsGUIElements.size()-1]->state += mse::gui::StoreItemStates::Equipped;
		}

		traitsGUIElements[traitsGUIElements.size()-1]->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&, i](SDL_Event* event){
			PickTrait((DTetris::Traits)i, this);
		};
	}

	// general UI layout
	closeBtn = (mse::gui::Button*)AddElement(new mse::gui::Button(
		this,
		{276, 60, 14, 13},
		"./data/img/screen-images.png",
		{0, 0, 0},
		{24, 102, 14, 13},
		{38, 102, 14, 13},
		{52, 102, 14, 13},
		{66, 102, 14, 13}
	));

	closeBtn->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
		if (closeBtn->isPushed)
		{
			gsm.ChangeStateTo(DTetris::GamePages::CharacterUpdate);
		}
	};

	AddElement(new mse::gui::Field(
		this,
		{30, 60, 260, 140},
		{64, 32, 32, 255},
		true,
		1,
		{255, 255, 255, 255}
	));
}
void ShopTraitsUILayer::OnUpdate(mse::TimeType t)
{}


void PickTrait(DTetris::Traits trait, mse::Layer* layer)
{
	MSE_TRACE("ShopUI: PickTrait");
	if ((trait != DTetris::Traits::None) && layer)
	{
		size_t offset = 1; // 0 is Traits::None, so (size_t)trait is always >= 1

		if (!game.shop_traits[trait])
		{
			// not owned
			if (game.playerCharacter.blocks[DTetris::BlockType::Treasure] - gameDB.traits[trait].cost >= 0)
			{
				game.playerCharacter.blocks[DTetris::BlockType::Treasure] = game.playerCharacter.blocks[DTetris::BlockType::Treasure] - gameDB.traits[trait].cost;

				if (!(((ShopTraitsUILayer*)layer)->traitsGUIElements[(size_t)trait - offset]->state & mse::gui::StoreItemStates::Bought))
				{
					((ShopTraitsUILayer*)layer)->traitsGUIElements[(size_t)trait - offset]->state += mse::gui::StoreItemStates::Bought;
				}

				game.shop_traits[trait] = true;
				MSE_LOG("ShopUI: PickTrait: bought trait ", utf8::utf32to8(gameDB.traits[trait].name));
			} else {
				MSE_LOG("ShopUI: PickTrait: cannot buy trait ", utf8::utf32to8(gameDB.traits[trait].name), " - not enough treasures");
			}
		} else {
			// owned
			if (game.playerCharacter.traits[(size_t)game.traitSlot] == trait)
			{
				// unpick
				game.playerCharacter.traits[(size_t)game.traitSlot] = DTetris::Traits::None;

				if (((ShopTraitsUILayer*)layer)->traitsGUIElements[(size_t)trait - offset]->state & mse::gui::StoreItemStates::Equipped)
				{
					((ShopTraitsUILayer*)layer)->traitsGUIElements[(size_t)trait - offset]->state -= mse::gui::StoreItemStates::Equipped;
				}

				game.playerCharacterUpdated = true;
				MSE_LOG("ShopUI: PickTrait: unpicked trait ", utf8::utf32to8(gameDB.traits[trait].name));
			} else {
				// pick
				game.playerCharacter.traits[(size_t)game.traitSlot] = trait;

				if (!(((ShopTraitsUILayer*)layer)->traitsGUIElements[(size_t)trait - offset]->state & mse::gui::StoreItemStates::Equipped))
				{
					((ShopTraitsUILayer*)layer)->traitsGUIElements[(size_t)trait - offset]->state += mse::gui::StoreItemStates::Equipped;
				}

				// make sure we don't pick the same trait twice
				for (size_t i = 0; i <= (size_t)DTetris::TraitSlot::C; ++i)
				{
					if ((size_t)game.traitSlot != i)
					{
						if (game.playerCharacter.traits[i] == trait)
						{
							game.playerCharacter.traits[i] = DTetris::Traits::None;
							break;
						}
					}
				}

				game.traitSlot = DTetris::TraitSlot::None;
				game.playerCharacterUpdated = true;
				MSE_LOG("ShopUI: PickTrait: picked trait ", utf8::utf32to8(gameDB.traits[trait].name));

				gsm.ChangeStateTo(DTetris::GamePages::CharacterUpdate);
			}
		}
	} else {
		MSE_LOG("ShopUI: PickTrait: trait ", (int)trait, " is None or layer is nullptr");
	}
}

// weapon is an index in gameDB.weapons
void PickWeapon(size_t weaponID, mse::Layer* layer)
{
	MSE_TRACE("ShopUI: PickWeapon");
	if ((gameDB.weapons[weaponID].sprite != DTetris::WeaponSpriteType::None) && layer)
	{
		size_t offset = 2; // 0 is bare fists, 1 is knuckles, so weaponID is always >= 2

		if (!game.shop_weapons[gameDB.weapons[weaponID].sprite])
		{
			// not owned
			if (game.playerCharacter.blocks[DTetris::BlockType::Treasure] - gameDB.weapons[weaponID].cost >= 0)
			{
				game.playerCharacter.blocks[DTetris::BlockType::Treasure] = game.playerCharacter.blocks[DTetris::BlockType::Treasure] - gameDB.weapons[weaponID].cost;

				if (!(((ShopWeaponsUILayer*)layer)->weaponsGUIElements[weaponID - offset]->state & mse::gui::StoreItemStates::Bought))
				{
					((ShopWeaponsUILayer*)layer)->weaponsGUIElements[weaponID - offset]->state += mse::gui::StoreItemStates::Bought;
				}

				game.shop_weapons[gameDB.weapons[weaponID].sprite] = true;
				MSE_LOG("ShopUI: PickWeapon: bought weapon ", utf8::utf32to8(gameDB.weapons[weaponID].name));
			} else {
				MSE_LOG("ShopUI: PickWeapon: cannot buy weapon ", utf8::utf32to8(gameDB.weapons[weaponID].name), " - not enough treasures");
			}
		} else {
			// owned
			if (game.playerCharacter.weapon.id == weaponID)
			{
				// unpick
				if (((ShopWeaponsUILayer*)layer)->weaponsGUIElements[weaponID - offset]->state & mse::gui::StoreItemStates::Equipped)
				{
					((ShopWeaponsUILayer*)layer)->weaponsGUIElements[weaponID - offset]->state -= mse::gui::StoreItemStates::Equipped;
				}

				game.playerCharacter.weapon = gameDB.weapons[0]; // bare fists

				game.playerCharacterUpdated = true;
				MSE_LOG("ShopUI: PickWeapon: unpicked weapon ", utf8::utf32to8(gameDB.weapons[weaponID].name));
			} else {
				// pick
				if (game.inputWeapon.sprite == DTetris::WeaponSpriteType::LegendarySword)
				{
					game.inputWeapon.sourceArea.x = game.inputWeapon.sourceArea.x + (game.inputWeapon.sourceArea.z) * ((int)game.playerCharacter.main_class.type - 1);
					game.inputWeapon.sourceHandleArea.x = game.inputWeapon.sourceHandleArea.x + (game.inputWeapon.sourceArea.z) * ((int)game.playerCharacter.main_class.type - 1);
				}

				if (!(((ShopWeaponsUILayer*)layer)->weaponsGUIElements[weaponID - offset]->state & mse::gui::StoreItemStates::Equipped))
				{
					((ShopWeaponsUILayer*)layer)->weaponsGUIElements[weaponID - offset]->state += mse::gui::StoreItemStates::Equipped;
				}

				game.playerCharacter.weapon = gameDB.weapons[weaponID];

				game.traitSlot = DTetris::TraitSlot::None;
				game.playerCharacterUpdated = true;
				MSE_LOG("ShopUI: PickWeapon: picked weapon ", utf8::utf32to8(gameDB.weapons[weaponID].name));

				gsm.ChangeStateTo(DTetris::GamePages::CharacterUpdate);
			}
		}
	} else {
		MSE_LOG("ShopUI: PickWeapon: weapon ", (int)weaponID, " corresponds to WeaponSpriteType::None or layer is nullptr");
	}
}
