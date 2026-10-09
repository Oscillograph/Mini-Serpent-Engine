#include "dungeontris/game-fwd.h"
#include <dungeontris/tetris.h>
// #include "globals.h"

// !!!!!!!!!!!!!!
// GameData - глобальная структура с важными данными для всего игрового приложения
// но есть и переменные, важные только для cTetrisEngine

namespace DTetris
{
	TetrisEngineData TetrisEngineData = {
		0,
		25,
		0,
		2,
		0,
		false,
		false,
		{},
		TetrisStates::None,
		TetrisMoveDirection::Down
	};

	//TetrisEngine_cTetrisEngine(){
	int TetrisEngine_init(){
		TetrisEngineData.animateLinesRemove = false;

		// initialize layers
		for (y = 0; y < BOARD_HEIGHT; y++){
			for (x = 0; x < BOARD_WIDTH; x++){
				for (index = 0; index < 10; index++){
					GameData.gameLayer[index][y][x] = 0;
				}
			}
		}

		// make walls
		for (y = 0; y < BOARD_HEIGHT; y++){
			for (x = 0; x < BOARD_WIDTH; x++){
				if ((y == BOARD_HEIGHT - 1) || (x == 0) || (x == BOARD_WIDTH - 1)){
					GameData.gameLayer[0][y][x] = 1;
				}
			}
		}
		GameData.difficulty = 3; // 1 means "A", 2 means "B", 3 means "C"
		GameData.playerScore = 0;
		// Game speed formulae: 10ms*(gameSpeedMax - gameSpeed)
		GameData.gameSpeed = 0; // initial game speed
		GameData.gameSpeedMax = 25; // maximum game speed.
		GameData.gameTimer = 0;
	}

	// look at what we have in the layer 9
	int TetrisEngine_sumLayers(){
		this->cleanLayer(9);

		for (y = 0; y < BOARD_HEIGHT; y++){
			for (x = 0; x < BOARD_WIDTH; x++){
				// sum up all layers
				for (index = 0; index < 8; index++){
					GameData.gameLayer[9][y][x] = GameData.gameLayer[9][y][x] + GameData.gameLayer[index][y][x];
				}
			}
		}
	}

	int TetrisEngine_cleanLayer(int id){
		for (y = 0; y < BOARD_HEIGHT; y++){
			for (x = 0; x < BOARD_WIDTH; x++){
				// clean the layer
				GameData.gameLayer[id][y][x] = 0;
			}
		}
	}

	// init tetriBlock on its layer
	int TetrisEngine_tetriBlockControl(cPlayerCharacter *block){
		this->cleanLayer(2);

		for (y = 0; y < BLOCK_HEIGHT; y++){
			for (x = 0; x < BLOCK_WIDTH; x++){
				GameData.gameLayer[2][PlayerData.y + y][PlayerData.x + x] = PlayerData.form[y][x];
			}
		}
	}

	bool TetrisEngine_moveAllowed(DTetris::Tetrimino* block, DTetris::TetrisMoveDirection moveDirection){
		bool isAllowed = true;
		const int xbackup = PlayerData.x;
		const int ybackup = PlayerData.y;

		block->Move(moveDirection);
		this->tetriBlockControl(block);

		this->sumLayers();
		for (y = 0; y < BOARD_HEIGHT; y++){
			for (x = 0; x < BOARD_WIDTH; x++){
				if (GameData.gameLayer[9][y][x] > 1){
					isAllowed = false;
				}
			}
		}

		PlayerData.x = xbackup;
		PlayerData.y = ybackup;
		this->tetriBlockControl(block);

		return isAllowed;
	}

	bool TetrisEngine_rotateAllowed(Tetrimino* block){
		bool isAllowed = true;

		if (PlayerData.blockNum != 1){
			this->cleanLayer(2);
			block->Rotate(rotateDirection);
			this->tetriBlockControl(block);

			this->sumLayers();
			for (y = 0; y < BOARD_HEIGHT; y++){
				for (x = 0; x < BOARD_WIDTH; x++){
					if (GameData.gameLayer[9][y][x] > 1){
						isAllowed = false;
					}
				}
			}

			block->Rotate(rotateDirection);
			block->Rotate(rotateDirection);
			block->Rotate(rotateDirection);
			this->cleanLayer(2);
			this->cleanLayer(9);
			this->tetriBlockControl(block);
		} else {
			isAllowed = false;
		}

		return isAllowed;
	}

	int TetrisEngine_Rotate(Tetrimino* block)
	{
		return 0;
	}

	int TetrisEngine_addBlock(cPlayerCharacter *block){
		const int currentBlockColor = PlayerData.color;
		for (y = 0; y < BOARD_HEIGHT; y++){
			for (x = 0; x < BOARD_WIDTH; x++){
				GameData.gameLayer[1][y][x] = GameData.gameLayer[1][y][x] + GameData.gameLayer[2][y][x];
				GameData.gameLayer[8][y][x] = GameData.gameLayer[8][y][x] + GameData.gameLayer[2][y][x]*PlayerData.color;
			}
		}
		this->addScore(SCORE_ADD_BLOCK);
	}

	int TetrisEngine_processLines(){ // Check if certain lines to be removed - and remove them, probably animating
		// remove line block
		static int xCount, yCount;
		static int colorIndex;
		static int linesToRemove[4];
		static boardMatrix gameLayerBackup[2];
		index = 0;

		if (!animateLinesRemove){
			yCount = 0; xCount = 0;

			for (y=BOARD_HEIGHT-1; y>=0; y--){
				for (x=0; x<BOARD_WIDTH; x++){
					gameLayerBackup[1][y][x] = GameData.gameLayer[1][y][x];
					gameLayerBackup[2][y][x] = GameData.gameLayer[8][y][x];
				}
			}

			for (y=BOARD_HEIGHT-1; y>=0; y--){ // choose the line
				xCount = 0;
				for (x=0; x<BOARD_WIDTH; x++){ // count blocks in the line
					if (GameData.gameLayer[1][y][x] == 1){
						xCount++;
					}
				}
				for (x=0; x<BOARD_WIDTH; x++){ // save a line in place of this one
					GameData.gameLayer[1][y+yCount][x] = GameData.gameLayer[1][y][x];
					GameData.gameLayer[8][y+yCount][x] = GameData.gameLayer[8][y][x];
				}
				if (xCount == BOARD_WIDTH-2){ // if the line is full - increase yCount
					yCount++;
					linesToRemove[yCount-1] = y;
				}
			}

			for (y=0; y<yCount; y++){
				xCount = y;
				for (x=0; x<BOARD_WIDTH; x++){ // clear what's not cleared
					GameData.gameLayer[1][y][x] = 0;
					GameData.gameLayer[8][y][x] = 0;
				}
				while (xCount >= 0){
					xCount--;
					this->addScore(SCORE_REMOVE_LINE);
				}
			}
		}

		if ((yCount > 0) && !animateLinesRemove) {
			animateLinesRemove = true;
			for (y=BOARD_HEIGHT-1; y>=0; y--){
				for (x=0; x<BOARD_WIDTH; x++){
					index = GameData.gameLayer[1][y][x];
					GameData.gameLayer[1][y][x] = gameLayerBackup[1][y][x];
					gameLayerBackup[1][y][x] = index;
					index = GameData.gameLayer[8][y][x];
					GameData.gameLayer[8][y][x] = gameLayerBackup[2][y][x];
					gameLayerBackup[2][y][x] = index;
				}
			}
			colorIndex = 7;
		}

		if (animateLinesRemove && colorIndex > 0){
			pauseOn();
			switch(colorIndex){
				case 7: colorIndex = 4; break;
				// case 4: colorIndex = 1; break;
				case 4: colorIndex = 0; animateLinesRemove = false; pauseOff();
				for (y=BOARD_HEIGHT-1; y>=0; y--){
					for (x=0; x<BOARD_WIDTH; x++){
						index = GameData.gameLayer[1][y][x];
						GameData.gameLayer[1][y][x] = gameLayerBackup[1][y][x];
						gameLayerBackup[1][y][x] = index;
						index = GameData.gameLayer[8][y][x];
						GameData.gameLayer[8][y][x] = gameLayerBackup[2][y][x];
						gameLayerBackup[2][y][x] = index;
					}
				}
				yCount = 0;
				break;
			}
			for (y=0; y<yCount; y++){
				for (x=1; x<BOARD_WIDTH-1; x++){
					GameData.gameLayer[8][linesToRemove[y]][x] = colorIndex;
				}
			}
		}
	}

	int TetrisEngine_addScore(int scoreType){
		switch (scoreType){
			case SCORE_MOVE_DOWN:	GameData.playerScore+=  1; break;
			case SCORE_ADD_BLOCK:	GameData.playerScore+=  1; break;
			case SCORE_REMOVE_LINE:	GameData.playerScore+= 100; break;
		}
	}

	int TetrisEngine_gameRestart(cPlayerCharacter *block){
		this->cleanLayer(1);
		this->cleanLayer(2);
		this->cleanLayer(3);
		this->cleanLayer(4);
		this->cleanLayer(5);
		this->cleanLayer(6);
		this->cleanLayer(7);
		this->cleanLayer(8);
		this->cleanLayer(9);
		block->reset();
		GameData.gameSpeed = 0;
		GameData.playerScore = 0;
		GameData.gameOver = false;
		GameData.gameStart = true;
		GameData.gamePause = false;
	}

	int TetrisEngine_processGamePlay(cPlayerCharacter *TBlock){
		if (GameData.gameStart){
			if (GameData.gameTimer == (GameData.gameSpeedMax - GameData.gameSpeed)){
				tetriBlockControl(TBlock);
				TBlock->canMove = moveAllowed(TBlock, PlayerData.forceDirection);
				if (TBlock->canMove){
					TBlock->Move(PlayerData.forceDirection);
				} else {
					if ((PlayerData.y <= 3) && (PlayerData.x == (BOARD_WIDTH-1)/2-2)){
						addBlock(TBlock);
						PlayerData.canMove = false;
						GameData.gameStart = false;
						GameData.gameOver = true;
					} else {
						addBlock(TBlock);
						TBlock->reset();
					}
				}
				GameData.gameTimer = 0;
			} else {
				GameData.gameTimer++;
			}
			processLines();
			GameData.gameSpeed = GameData.playerScore / 1000;
			if (GameData.gameSpeed > GameData.gameSpeedMax) GameData.gameSpeed = GameData.gameSpeedMax;

			tetriBlockControl(TBlock);
			sumLayers();
		} else {
			if (animateLinesRemove){
				if (GameData.gameTimer == 5){
					processLines();
					GameData.gameTimer = 0;
				} else {
					GameData.gameTimer++;
				}
			}
		}
	}

	int TetrisEngine_userInputManagement(char btn, cPlayerCharacter *TBlock){
		switch (btn){
			case 'w': // w key - rotate TBlock
				PlayerData.canRotate = rotateAllowed(TBlock, 1);
				if (PlayerData.canRotate) TBlock->Rotate(1);
				break;
			case 'a': // a key - move left
				PlayerData.canMove = moveAllowed(TBlock, MOVE_LEFT);
				if (PlayerData.canMove) TBlock->Move(MOVE_LEFT);
				break;
			case 's': // s key - move down
				PlayerData.canMove = moveAllowed(TBlock, MOVE_DOWN);
				if (PlayerData.canMove){
					TBlock->Move(MOVE_DOWN);
					addScore(SCORE_MOVE_DOWN);
				}
				break;
			case 'd': // d key - move right
				PlayerData.canMove = moveAllowed(TBlock, MOVE_RIGHT);
				if (PlayerData.canMove) TBlock->Move(MOVE_RIGHT);
				break;
			case 'p': // p key - gamePause
				if (GameData.gameStart && PlayerData.canMove){ // PAUSE on
					pauseOn();
				} else { // PAUSE off
					pauseOff();
				}
				break;
			case 'r': // r key - Restart
				gameRestart(TBlock);
				break;
			default:
				tetriBlockControl(TBlock);
				break;
		}
	}

	int TetrisEngine_pauseOn(){
		GameData.gameStart = false;
	}

	int TetrisEngine_pauseOff(){
		GameData.gameStart = true;
	}

	TetrisEngine_cTetrisEngine(){
		// this->init();
		// empty constructor
		// selfName = "Tetris";
	}
}

