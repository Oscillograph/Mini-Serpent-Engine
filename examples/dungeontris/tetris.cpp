#include <dungeontris/game-fwd.h>
// #include <dungeontris/game-data.h>
#include <dungeontris/tetris.h>
// #include "globals.h"

// !!!!!!!!!!!!!!
// GameData - глобальная структура с важными данными для всего игрового приложения
// но есть и переменные, важные только для cTetrisEngine

extern DTetris::TetrisMap tetrisMap;
extern DTetris::TetriminoTemplatesDB tetriminoDB;
extern DTetris::Tetrimino tetrimino;

namespace DTetris
{
	TetrisEngineData tetrisEngineData = {
		0,
		25,
		0,
		2,
		0,
		false,
		false,
		false,
		0,
		false,
		false,
		{},
		TetrisStates::None,
		TetrisMoveDirection::Down
	};

	//TetrisEngine_cTetrisEngine(){
	int TetrisEngine_init(){
		tetrisEngineData.animateLinesRemove = false;

		return 0;
	}

	// look at what we have in the layer 9
	int TetrisEngine_sumLayers()
	{
		return 0;
	}

	// init tetriBlock on its layer
	int TetrisEngine_tetriBlockControl(Tetrimino* block)
	{
		return 0;
	}

	bool TetrisEngine_moveAllowed(Tetrimino* block, TetrisMoveDirection moveDirection)
	{
		bool isAllowed = true;

		int x_backup = block->x;
		int y_backup = block->y;

		TetrisEngine_Move(block, moveDirection);

		for (int x = 0; x < 4; ++x)
		{
			int xIndex = x + block->x;
			for (int y = 0; y < 4; ++y)
			{
				int yIndex = y + block->y;

				if ((block->blocks[x + 4*y].type != BlockType::None) &&
					(tetrisMap.map[xIndex + yIndex*tetrisMap.width].type != BlockType::None))
				{
					isAllowed = false;
					break;
				}
			}
		}

		block->x = x_backup;
		block->y = y_backup;

		return isAllowed;
	}

	int TetrisEngine_Move(Tetrimino* block, TetrisMoveDirection moveDirection)
	{
		switch (moveDirection)
		{
			case TetrisMoveDirection::Left:
			{
				block->x--;
				break;
			}
			case TetrisMoveDirection::Right:
			{
				block->x++;
				break;
			}
			case TetrisMoveDirection::Up:
			{
				block->y--;
			}
			case TetrisMoveDirection::Down:
			default:
			{
				block->y++;
			}
		}

		return 0;
	}

	bool TetrisEngine_rotateAllowed(Tetrimino* block){
		bool isAllowed = true;

		TetrisEngine_Rotate(block, TetrisRotateDirection::Clockwise);

		for (int x = 0; x < 4; ++x)
		{
			int xIndex = x + block->x;
			for (int y = 0; y < 4; ++y)
			{
				int yIndex = y + block->y;

				if ((block->blocks[x + 4*y].type != BlockType::None) &&
					(tetrisMap.map[xIndex + yIndex*tetrisMap.width].type != BlockType::None))
				{
					isAllowed = false;
					break;
				}
			}
		}

		TetrisEngine_Rotate(block, TetrisRotateDirection::CounterClockwise);

		return isAllowed;
	}

	int TetrisEngine_Rotate(Tetrimino* block, TetrisRotateDirection direction)
	{
		std::vector<Block> newBlock;

		if (block->id == 2) // |-block
		{
			newBlock.resize(16);
			for (int i = 0; i < 16; ++i)
			{
				newBlock[i].type = BlockType::None;
			}

			for (int x = 0; x < 4; ++x)
			{
				for (int y = 0; y < 3; ++y)
				{
					switch (direction)
					{
						case TetrisRotateDirection::Clockwise:
						{
							newBlock[x + 4*y].type = block->blocks[(2 - y) + 4*x].type;
							break;
						}
						case TetrisRotateDirection::CounterClockwise:
						{
							newBlock[x + 4*y].type = block->blocks[y + 4*(3 - x)].type;
							break;
						}

						default:
						{}
					}
				}
			}
		} else {
			if (block->id == 3) // square
			{

			} else { // other 3x3 blocks
				newBlock.resize(16);
				for (int i = 0; i < 16; ++i)
				{
					newBlock[i].type = BlockType::None;
				}

				for (int x = 0; x < 3; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						switch (direction)
						{
							case TetrisRotateDirection::Clockwise:
							{
								newBlock[x + 4*y].type = block->blocks[(2 - y) + 4*x].type;
								break;
							}
							case TetrisRotateDirection::CounterClockwise:
							{
								newBlock[x + 4*y].type = block->blocks[y + 4*(2 - x)].type;
								break;
							}

							default:
							{}
						}
					}
				}
			}
		}

		block->blocks.assign(newBlock.begin(), newBlock.end());

		return 0;
	}

	int TetrisEngine_AddBlock(Tetrimino* block)
	{
		for (int x = 0; x < 4; ++x)
		{
			int xIndex = x + block->x;
			for (int y = 0; y < 4; ++y)
			{
				int yIndex = y + block->y;

				if (block->blocks[x + 4*y].type != DTetris::BlockType::None)
				{
					tetrisMap.map[xIndex + yIndex*tetrisMap.width].type = block->blocks[x + 4*y].type;
				}

				block->blocks[x + 4*y].type = DTetris::BlockType::None;
			}
		}

		return 0;
	}

	int TetrisEngine_NewBlock()
	{
		int setSize = 0;
		switch (tetrisEngineData.gameDifficulty)
		{
			case 0:
			{
				// Classic mode
				setSize = 7;
				break;
			}

			case 1:
			{
				// Brick Game mode
				setSize = 16;
				break;
			}

			case 2:
			default:
			{
				// Brick Game mode
				setSize = 16;
				break;
			}
		}

		int tetriminoId = rand() % setSize + 1;
		printf("tetrimino ID: %d of %d;\n", tetriminoId, (int)tetriminoDB.data.size());
		for (size_t i = 0; i < tetriminoDB.data[tetriminoId].block.size(); ++i)
		{
			switch (tetriminoDB.data[tetriminoId].block[i])
			{
				case 0:
				{
					tetrimino.blocks[i].type = DTetris::BlockType::None;
					break;
				}
				case 1:
				{
					tetrimino.blocks[i].type = DTetris::BlockType::Block;
					break;
				}
				default:
				{
					tetrimino.blocks[i].type = DTetris::BlockType::None;
				}
			}
		}

		tetrimino.x = 4;
		tetrimino.y = 0;
		tetrimino.id = tetriminoId;
		tetrimino.fell = false;

		return 0;
	}

	int TetrisEngine_processLines()
	{ // Check if certain lines to be removed - and remove them, probably animating
		// // remove line block
		// static int xCount, yCount;
		// static int colorIndex;
		// static int linesToRemove[4];
		// static boardMatrix gameLayerBackup[2];
		// index = 0;
  //
		// if (!animateLinesRemove){
		// 	yCount = 0; xCount = 0;
  //
		// 	for (y=BOARD_HEIGHT-1; y>=0; y--){
		// 		for (x=0; x<BOARD_WIDTH; x++){
		// 			gameLayerBackup[1][y][x] = GameData.gameLayer[1][y][x];
		// 			gameLayerBackup[2][y][x] = GameData.gameLayer[8][y][x];
		// 		}
		// 	}
  //
		// 	for (y=BOARD_HEIGHT-1; y>=0; y--){ // choose the line
		// 		xCount = 0;
		// 		for (x=0; x<BOARD_WIDTH; x++){ // count blocks in the line
		// 			if (GameData.gameLayer[1][y][x] == 1){
		// 				xCount++;
		// 			}
		// 		}
		// 		for (x=0; x<BOARD_WIDTH; x++){ // save a line in place of this one
		// 			GameData.gameLayer[1][y+yCount][x] = GameData.gameLayer[1][y][x];
		// 			GameData.gameLayer[8][y+yCount][x] = GameData.gameLayer[8][y][x];
		// 		}
		// 		if (xCount == BOARD_WIDTH-2){ // if the line is full - increase yCount
		// 			yCount++;
		// 			linesToRemove[yCount-1] = y;
		// 		}
		// 	}
  //
		// 	for (y=0; y<yCount; y++){
		// 		xCount = y;
		// 		for (x=0; x<BOARD_WIDTH; x++){ // clear what's not cleared
		// 			GameData.gameLayer[1][y][x] = 0;
		// 			GameData.gameLayer[8][y][x] = 0;
		// 		}
		// 		while (xCount >= 0){
		// 			xCount--;
		// 			this->addScore(SCORE_REMOVE_LINE);
		// 		}
		// 	}
		// }
  //
		// if ((yCount > 0) && !animateLinesRemove) {
		// 	animateLinesRemove = true;
		// 	for (y=BOARD_HEIGHT-1; y>=0; y--){
		// 		for (x=0; x<BOARD_WIDTH; x++){
		// 			index = GameData.gameLayer[1][y][x];
		// 			GameData.gameLayer[1][y][x] = gameLayerBackup[1][y][x];
		// 			gameLayerBackup[1][y][x] = index;
		// 			index = GameData.gameLayer[8][y][x];
		// 			GameData.gameLayer[8][y][x] = gameLayerBackup[2][y][x];
		// 			gameLayerBackup[2][y][x] = index;
		// 		}
		// 	}
		// 	colorIndex = 7;
		// }
  //
		// if (animateLinesRemove && colorIndex > 0){
		// 	pauseOn();
		// 	switch(colorIndex){
		// 		case 7: colorIndex = 4; break;
		// 		// case 4: colorIndex = 1; break;
		// 		case 4: colorIndex = 0; animateLinesRemove = false; pauseOff();
		// 		for (y=BOARD_HEIGHT-1; y>=0; y--){
		// 			for (x=0; x<BOARD_WIDTH; x++){
		// 				index = GameData.gameLayer[1][y][x];
		// 				GameData.gameLayer[1][y][x] = gameLayerBackup[1][y][x];
		// 				gameLayerBackup[1][y][x] = index;
		// 				index = GameData.gameLayer[8][y][x];
		// 				GameData.gameLayer[8][y][x] = gameLayerBackup[2][y][x];
		// 				gameLayerBackup[2][y][x] = index;
		// 			}
		// 		}
		// 		yCount = 0;
		// 		break;
		// 	}
		// 	for (y=0; y<yCount; y++){
		// 		for (x=1; x<BOARD_WIDTH-1; x++){
		// 			GameData.gameLayer[8][linesToRemove[y]][x] = colorIndex;
		// 		}
		// 	}
		// }
	}

	int TetrisEngine_addScore(int scoreType)
	{
		// switch (scoreType){
		// 	case SCORE_MOVE_DOWN:	GameData.playerScore+=  1; break;
		// 	case SCORE_ADD_BLOCK:	GameData.playerScore+=  1; break;
		// 	case SCORE_REMOVE_LINE:	GameData.playerScore+= 100; break;
		// }
	}

	int TetrisEngine_gameRestart(Tetrimino* block)
	{
		// this->cleanLayer(1);
		// this->cleanLayer(2);
		// this->cleanLayer(3);
		// this->cleanLayer(4);
		// this->cleanLayer(5);
		// this->cleanLayer(6);
		// this->cleanLayer(7);
		// this->cleanLayer(8);
		// this->cleanLayer(9);
		// block->reset();
		// GameData.gameSpeed = 0;
		// GameData.playerScore = 0;
		// GameData.gameOver = false;
		// GameData.gameStart = true;
		// GameData.gamePause = false;
	}

	int TetrisEngine_processGamePlay(Tetrimino* TBlock)
	{
		// if (GameData.gameStart){
		// 	if (GameData.gameTimer == (GameData.gameSpeedMax - GameData.gameSpeed)){
		// 		tetriBlockControl(TBlock);
		// 		TBlock->canMove = moveAllowed(TBlock, PlayerData.forceDirection);
		// 		if (TBlock->canMove){
		// 			TBlock->Move(PlayerData.forceDirection);
		// 		} else {
		// 			if ((PlayerData.y <= 3) && (PlayerData.x == (BOARD_WIDTH-1)/2-2)){
		// 				addBlock(TBlock);
		// 				PlayerData.canMove = false;
		// 				GameData.gameStart = false;
		// 				GameData.gameOver = true;
		// 			} else {
		// 				addBlock(TBlock);
		// 				TBlock->reset();
		// 			}
		// 		}
		// 		GameData.gameTimer = 0;
		// 	} else {
		// 		GameData.gameTimer++;
		// 	}
		// 	processLines();
		// 	GameData.gameSpeed = GameData.playerScore / 1000;
		// 	if (GameData.gameSpeed > GameData.gameSpeedMax) GameData.gameSpeed = GameData.gameSpeedMax;
  //
		// 	tetriBlockControl(TBlock);
		// 	sumLayers();
		// } else {
		// 	if (animateLinesRemove){
		// 		if (GameData.gameTimer == 5){
		// 			processLines();
		// 			GameData.gameTimer = 0;
		// 		} else {
		// 			GameData.gameTimer++;
		// 		}
		// 	}
		// }
	}

	int TetrisEngine_userInputManagement(char btn, Tetrimino* TBlock)
	{
		// switch (btn){
		// 	case 'w': // w key - rotate TBlock
		// 		PlayerData.canRotate = rotateAllowed(TBlock, 1);
		// 		if (PlayerData.canRotate) TBlock->Rotate(1);
		// 		break;
		// 	case 'a': // a key - move left
		// 		PlayerData.canMove = moveAllowed(TBlock, MOVE_LEFT);
		// 		if (PlayerData.canMove) TBlock->Move(MOVE_LEFT);
		// 		break;
		// 	case 's': // s key - move down
		// 		PlayerData.canMove = moveAllowed(TBlock, MOVE_DOWN);
		// 		if (PlayerData.canMove){
		// 			TBlock->Move(MOVE_DOWN);
		// 			addScore(SCORE_MOVE_DOWN);
		// 		}
		// 		break;
		// 	case 'd': // d key - move right
		// 		PlayerData.canMove = moveAllowed(TBlock, MOVE_RIGHT);
		// 		if (PlayerData.canMove) TBlock->Move(MOVE_RIGHT);
		// 		break;
		// 	case 'p': // p key - gamePause
		// 		if (GameData.gameStart && PlayerData.canMove){ // PAUSE on
		// 			pauseOn();
		// 		} else { // PAUSE off
		// 			pauseOff();
		// 		}
		// 		break;
		// 	case 'r': // r key - Restart
		// 		gameRestart(TBlock);
		// 		break;
		// 	default:
		// 		tetriBlockControl(TBlock);
		// 		break;
		// }
	}

	int TetrisEngine_pauseOn()
	{
		return 0;
	}

	int TetrisEngine_pauseOff()
	{
		return 0;
	}
}

