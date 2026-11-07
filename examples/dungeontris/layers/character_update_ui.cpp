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

	// pick a subclass

	// disable if level limit reached

	// disable if level limit reached

	// pick a weapon
	glm::uvec4 srcKeep = {0, 0, 34, 64};
	glm::uvec4 srcNew = {0, 0, 34, 64};


	mse::gui::Text* text3 = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"Выбрать оружие:", {155, 20, 120, 10}, {0, 0, 0, 255}, {255, 255, 255, 255})));
	AddElement(new mse::gui::Image(this, {155, 95 - srcKeep.w, srcKeep.z, srcKeep.w}, "./data/img/heroes.png", srcKeep, {0, 0, 0, 255}));
	keepSameWeaponBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Оставить", {32, 32, 32, 255}, {135, 95, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
	keepSameWeaponBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
		game.inputWeapon = game.playerCharacter.weapon;
		changeMade = true;

		keepSameWeaponBtn->Disable();
		pickDroppedWeaponBtn->Enable();
	};
	keepSameWeaponBtn->Disable();

	AddElement(new mse::gui::Image(this, {240, 95 - srcNew.w, srcNew.z, srcNew.w}, "./data/img/heroes.png", srcNew, {0, 0, 0, 255}));
	pickDroppedWeaponBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Подобрать", {32, 32, 32, 255}, {220, 95, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
	pickDroppedWeaponBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
		DTetris::Weapon* weapon = &(game.npcCharacter.drop_list[0]);

		if (weapon->sprite == DTetris::WeaponSpriteType::LegendarySword)
		{
			weapon->sourceArea.x = weapon->sourceArea.x + (weapon->sourceArea.z) * ((int)game.playerCharacter.main_class.type - 1);
			weapon->sourceHandleArea.x = weapon->sourceHandleArea.x + (weapon->sourceArea.z) * ((int)game.playerCharacter.main_class.type - 1);
		}

		game.inputWeapon.name = weapon->name;
		game.inputWeapon.type = weapon->type;
		game.inputWeapon.damage = weapon->damage;
		game.inputWeapon.sprite = weapon->sprite;
		game.inputWeapon.cost = weapon->cost;
		game.inputWeapon.sourceArea = weapon->sourceArea;
		game.inputWeapon.sourceHandleArea = weapon->sourceHandleArea;
		changeMade = true;

		keepSameWeaponBtn->Enable();
		pickDroppedWeaponBtn->Disable();
	};

	// change to shop
	// gsm.ChangeStateTo(DTetris::GamePages::Shop, this);

	// defaults
	game.inputClass = game.playerCharacter.main_class.type;
	game.inputWeapon = game.playerCharacter.weapon;

	// show stats
	statsBox = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"",
		{10, 109, 287, 100},
		{0, 64, 0, 255},
		{255, 255, 128, 255},
		1,
		true,
		{255, 196, 0, 255})));
	changeMade = true;

	AddElement(new mse::gui::VScrollbar(
		this,
		{297, 109, 13, 100},
		statsBox,
		"./data/img/screen-images.png",
		{0, 0, 0},
		{24, 49, 52, 12},
		{24, 74, 44, 10},
		{24, 61, 52, 12},
		{63, 61, 13, 4},
		{63, 65, 13, 4},
		{63, 69, 13, 4}));

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

		std::stringstream strstream;
		strstream <<
		"Уровень:    " << game.playerCharacter.level <<
		"\nКласс:      " << utf8::utf32to8(main_class.c_str()) << " (" << game.playerCharacter.main_class.level << ")" <<
		"\nПодкласс: " << utf8::utf32to8(sub_class.c_str()) << " (" << game.playerCharacter.sub_class.level << ")" <<
		"\nЗдоровье: " << game.playerCharacter.stats.health <<
		"\nСила:     " << game.playerCharacter.stats.strength <<
		"\nЛовкость: " << game.playerCharacter.stats.agility <<
		"\nВыносливость: " << game.playerCharacter.stats.endurance <<
		"\nОружие: " << utf8::utf32to8(game.inputWeapon.name.c_str()) <<
		"\n          (урон " << (int)(roundf(game.inputWeapon.damage));
		strstream << ", " << utf8::utf32to8(gameDB.damageTypes[game.inputWeapon.type].name);
		strstream << ")" <<
		"\nСпособности: ";
		for (size_t i = 0; i < game.playerCharacter.traits.size(); ++i)
		{
			if (i > 0)
			{
				strstream << ", ";
			}
			strstream << "\n  " << utf8::utf32to8(gameDB.traits[game.playerCharacter.traits[i]].name)
			<< "\n    " << utf8::utf32to8(gameDB.traits[game.playerCharacter.traits[i]].description);
		}
		strstream << "\n\nБитв пройдено: " << game.battleCounter << " из 5";
		stats = utf8::utf8to32(strstream.str());
		statsBox->ChangeText(stats);

		changeMade = false;
	}
}
