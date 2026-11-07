#include <dungeontris/layers.h>
#include <dungeontris/game-fwd.h>
#include <dungeontris/gamestates.h>
#include <dungeontris/tetris.h>

extern DTetris::GameDB gameDB;
extern DTetris::Game game;
extern DTetris::TetrisMap tetrisMap;
extern GameStateMachine gsm;
extern DTetris::TetrisEngineData DTetris::tetrisEngineData;


IntroUILayer::IntroUILayer() : mse::Layer()
{
	MSE_LOG("Constructed IntroUILayer");
}

IntroUILayer::~IntroUILayer()
{
	MSE_LOG("Destroyed IntroUILayer");
	text = nullptr;
}

void IntroUILayer::OnInit()
{
	GameLogo(this);

	text = (mse::gui::Text*)(AddElement(new mse::gui::Text(
		this,
		U"Загрузится через: ",
		{120, 50, 200, 10},
		{0, 0, 0, 255},
		{0, 255, 255, 255})));

	//    mse::SoundMan::PlayTrack("data/audio/tracks/01_towards_neon_shadows.mp3");
}

void IntroUILayer::OnUpdate(mse::TimeType t)
{
	//    if (counterBackup != ((IntroPageState*)gsm.current)->counter)
	//    {
	//        int counterBackup = ((IntroPageState*)gsm.current)->counter;
	//
	//        std::stringstream strstream;
	//        strstream << "Загрузится через: " << counterBackup;
	//        std::u32string newText = utf8::utf8to32(strstream.str());
	//
	//        text->ChangeText(newText);
	//
	//        if (counterBackup < 0)
	//        {
	//            gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
	//        }
	//    }
	gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
}
