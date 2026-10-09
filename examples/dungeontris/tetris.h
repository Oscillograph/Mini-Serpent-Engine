#ifndef DUNGEONTRIS_TETRIS_H
#define DUNGEONTRIS_TETRIS_H

#include <cstdint> // for size_t, uint32_t

namespace DTetris {
	enum class TetrisStates
	{
		None		= 0,
		Processing	= 1,
		Paused		= 2,
		GameOver	= 3,
		Win			= 4,
	}

	struct TetrisData
	{
		int gameSpeed = 0;
		uint32_t gameTimer = 0;
		TetrisStates state = TetrisStates::None;
	};

	static TetrisData tetrisData;

	// TetrisEngine API
	int TetrisEngine_Init();
	int TetrisEngine_Tick();
	int TetrisEngine_Restart();
	int TetrisEngine_Shutdown();

	// TetrisEngine internals
	int TetrisEngine_sumLayers();
	int TetrisEngine_cleanLayer(int id);
	int TetrisEngine_tetriblockControl(void* block);
	bool TetrisEngine_moveAllowed(void* block, int direction);
	bool TetrisEngine_rotateAllowed(void* block, int direction);
	int TetrisEngine_addBlock(void* block);
	int TetrisEngine_processLines();
	int TetrisEngine_addScore(int score);
	int TetrisEngine_pauseOn();
	int TetrisEngine_pauseOff();
}

#endif
