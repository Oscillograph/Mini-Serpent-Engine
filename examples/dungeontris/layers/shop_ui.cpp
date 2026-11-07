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

	// weapons
	AddElement(new mse::gui::Text(
		this,
		U"Оружие",
		{60, 20, 60, 10},
		{0, 0, 0, 0},
		{255, 255, 255, 255}
	));

	mse::gui::StoreItem* weapon01 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{60, 30, 40, 27},
		{400, 41, 40, 27},
		10
	))); // sword
	weapon01->callbacks[mse::EventTypes::MouseButtonUp] = [&](SDL_Event* event){
		// if (gameDB.weapons[0])
	};
	mse::gui::StoreItem* weapon02 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{100, 30, 40, 27},
		{440, 41, 40, 27},
		10
	))); // spear
	mse::gui::StoreItem* weapon03 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{140, 30, 40, 27},
		{480, 41, 40, 27},
		10
	))); // knife
	mse::gui::StoreItem* weapon04 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{180, 30, 40, 27},
		{520, 41, 40, 27},
		10
	))); // club
	mse::gui::StoreItem* weapon05 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{220, 30, 40, 27},
		{560, 41, 40, 27},
		10
	))); // axe
	mse::gui::StoreItem* weapon06 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{260, 30, 40, 27},
		{600, 41, 40, 27},
		10
	))); // legendary sword


	// traits
	AddElement(new mse::gui::Text(
		this,
		U"Особенности",
		{60, 80, 100, 10},
		{0, 0, 0, 0},
		{255, 255, 255, 255}
	));

	mse::gui::StoreItem* trait01 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{60, 90, 40, 27},
		{400, 68, 40, 27},
		10
	)));
	mse::gui::StoreItem* trait02 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{100, 90, 40, 27},
		{440, 68, 40, 27},
		10
	)));
	mse::gui::StoreItem* trait03 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{140, 90, 40, 27},
		{480, 68, 40, 27},
		10
	)));
	mse::gui::StoreItem* trait04 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{180, 90, 40, 27},
		{520, 68, 40, 27},
		10
	)));
	mse::gui::StoreItem* trait05 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{220, 90, 40, 27},
		{560, 68, 40, 27},
		10
	)));
	mse::gui::StoreItem* trait06 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{260, 90, 40, 27},
		{600, 68, 40, 27},
		10
	)));

	mse::gui::StoreItem* trait07 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{60, 120, 40, 27},
		{400, 68, 40, 27},
		10
	)));
	mse::gui::StoreItem* trait08 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{100, 120, 40, 27},
		{440, 68, 40, 27},
		10
	)));
	mse::gui::StoreItem* trait09 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{140, 120, 40, 27},
		{480, 68, 40, 27},
		10
	)));
	mse::gui::StoreItem* trait10 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{180, 120, 40, 27},
		{520, 68, 40, 27},
		10
	)));
	mse::gui::StoreItem* trait11 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{220, 120, 40, 27},
		{560, 68, 40, 27},
		10
	)));
	mse::gui::StoreItem* trait12 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{260, 120, 40, 27},
		{600, 68, 40, 27},
		10
	)));

	mse::gui::StoreItem* trait13 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{60, 150, 40, 27},
		{400, 68, 40, 27},
		10
	)));
	mse::gui::StoreItem* trait14 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{100, 150, 40, 27},
		{440, 68, 40, 27},
		10
	)));
	mse::gui::StoreItem* trait15 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{140, 150, 40, 27},
		{480, 68, 40, 27},
		10
	)));
	mse::gui::StoreItem* trait16 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{180, 150, 40, 27},
		{520, 68, 40, 27},
		10
	)));
	mse::gui::StoreItem* trait17 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{220, 150, 40, 27},
		{560, 68, 40, 27},
		10
	)));
	mse::gui::StoreItem* trait18 = (mse::gui::StoreItem*)(AddElement(new mse::gui::StoreItem(
		this,
		{260, 150, 40, 27},
		{600, 68, 40, 27},
		10
	)));
}

void ShopUILayer::OnUpdate(mse::TimeType t)
{}
