#ifndef DUNGEONTRIS_TETRIS_H
#define DUNGEONTRIS_TETRIS_H

#include "mse/systems/platform/input/input.h"
#include <dungeontris/game-fwd.h>
#include <cstdint> // for size_t, uint32_t

namespace DTetris
{
	static TetrisEngineData tetrisEngineData;

	// TetrisEngine API
	int TetrisEngine_Init();
	int TetrisEngine_Tick();
	int TetrisEngine_Restart();
	int TetrisEngine_Shutdown();

	// TetrisEngine internals
	int TetrisEngine_sumLayers();
	int TetrisEngine_cleanLayer(int id);
	int TetrisEngine_tetriblockControl(Tetrimino* block);
	bool TetrisEngine_moveAllowed(Tetrimino* block, TetrisMoveDirection moveDirection);
	bool TetrisEngine_rotateAllowed(Tetrimino* block);
	int TetrisEngine_Rotate(Tetrimino* block);
	int TetrisEngine_addBlock(Tetrimino* block);
	int TetrisEngine_processLines();
	int TetrisEngine_addScore(int score);
	int TetrisEngine_pauseOn();
	int TetrisEngine_pauseOff();
}

#endif
