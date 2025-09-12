#include <mse/mse.h>

class SimpleUILayer : public mse::Layer
{
public:
	enum class MovementDirection
	{
		None,
		Left,
		Right,
		Down,
		Up
	};
	
	SimpleUILayer()
	: mse::Layer()
	{
		MSE_LOG("Constructed a simpleUI layer");
		
		gameBoard.resize(10 * 20, 0);
		gameBoard[12] = 1;
	}
	
	virtual void OnInit() override
	{
		bmpFont = mse::ResourceManager::UseResource(mse::ResourceType::FontBitmap, "./data/fonts/my8bit3.bmp", this->GetWindow());
		
		// Game GUI
		gameCanvas = (mse::gui::Canvas*)(AddElement(new mse::gui::Canvas(this, {10, 10, gameCanvasGrid.x * gameCanvasGrid.z, gameCanvasGrid.y * gameCanvasGrid.w}, {32, 64, 48, 255})));
		elementInFocus = GetElementId(gameCanvas);
		MSE_LOG(elementInFocus);
		
		gameCanvas->callbacks[mse::EventTypes::KeyDown] = [&](SDL_Event* event){
			MSE_LOG("Callback!");
			switch (event->key.keysym.sym)
			{
				case SDLK_LEFT:
				{
					direction = MovementDirection::Left;
					break;
				}
				case SDLK_RIGHT:
				{
					direction = MovementDirection::Right;
					break;
				}
				case SDLK_DOWN:
				{
					direction = MovementDirection::Down;
					break;
				}
				case SDLK_UP:
				{
					direction = MovementDirection::Up;
					break;
				}
			}
			MSE_LOG("Player: ", player.x, " : ", player.y);
		};

		mse::gui::Button* playBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Играть", {100, 10, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		playBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mode = 1;
		};
		mse::gui::Button* settingsBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Настройки", {100, 20, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		settingsBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mode = 2;
		};
		mse::gui::Button* scoresBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Рекорды", {100, 30, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		scoresBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mode = 3;
		};
		mse::gui::Button* aboutBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Титры", {100, 40, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		aboutBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mode = 4;
		};
		mse::gui::Button* exitBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Выйти", {100, 50, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		exitBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mode = 5;
		};
		
		scoreCanvas = (mse::gui::Canvas*)(AddElement(new mse::gui::Canvas(this, {100, 70, 80, 30}, {32, 64, 48, 255})));
		
		itemCanvas = (mse::gui::Canvas*)(AddElement(new mse::gui::Canvas(this, {100, 110, 80, 60}, {32, 64, 48, 255})));
		
		mse::Application::GetApplication()->SetFPS(MSE_FPS60);
	}
	
	virtual void OnUpdate() override
	{
		switch (mode)
		{
			// idle
			case 0:
			{
				break;
			}
			// game
			case 1:
			{
				if (!gameOver)
				{
					gameCanvas->Clear();
					scoreCanvas->Clear();
					itemCanvas->Clear();
					
					// correct player position
					player.x = (player.x < 0) ? 0 : player.x;
					player.x = (player.x > gameCanvasGrid.z - 1) ? gameCanvasGrid.z - 1 : player.x;
					player.y = (player.y > gameCanvasGrid.w - 1) ? gameCanvasGrid.w - 1 : player.y;
					player.y = (player.y < 0) ? 0 : player.y;
					
					// update player
					MovePlayer(direction);
					
					// update tail
					for (int i = tail.size() - 1; i > 0; --i)
					{
						tail[i] = tail[i - 1];
						
						// check if the player collides with his tail 
						if ((tail[i].x == player.x) && (tail[i].y == player.y))
						{
							gameOver = true;
						}
					}
					
					if ((player.x == apple.x) && (player.y == apple.y))
					{
						// increase score
						player.z++;
						
						// increase tail
						tail.push_back({player.x, player.y});
						
						apple.x = rand() % gameCanvasGrid.z;
						apple.y = rand() % gameCanvasGrid.w;
					}
					
					// draw tail
					for (int i = tail.size() - 1; i > 0; --i)
					{
						gameCanvas->DrawRect(
							tail[i].x*gameCanvasGrid.x, 
							tail[i].y*gameCanvasGrid.y, 
							(tail[i].x + 1)*gameCanvasGrid.x, 
							(tail[i].y + 1)*gameCanvasGrid.y, 
							{255, 255, 0, 255}
							);
					}
					
					// draw player
					gameCanvas->DrawRect(
						player.x*gameCanvasGrid.x, 
						player.y*gameCanvasGrid.y, 
						(player.x + 1)*gameCanvasGrid.x, 
						(player.y + 1)*gameCanvasGrid.y, 
						{255, 0, 255, 255}
						);
					
					// draw apple
					gameCanvas->DrawRect(
						apple.x*gameCanvasGrid.x, 
						apple.y*gameCanvasGrid.y, 
						(apple.x + 1)*gameCanvasGrid.x, 
						(apple.y + 1)*gameCanvasGrid.y, 
						{255, 0, 0, 255}
						);
					
					// draw scores
					std::stringstream score;
					score << player.z;
					mse::Renderer::SurfaceDrawText(
						(mse::Texture*)(scoreCanvas->GetTexture()->data),
						{5, 5, 70, 20},
						1,
//						U"Hello!",
						U"Счёт: " + utf8::utf8to32(score.str()),
						bmpFont,
						{255, 255, 0, 255},
						0
						);
					score.str("");
					score << player.w;
					mse::Renderer::SurfaceDrawText(
						(mse::Texture*)(scoreCanvas->GetTexture()->data),
						{5, 15, 70, 20},
						1,
//						U"Hello!",
						U"Жизни: " + utf8::utf8to32(score.str()),
						bmpFont,
						{255, 255, 0, 255},
						0
						);
					((mse::Texture*)(scoreCanvas->GetTexture()->data))->Update();
				}
				
				break;
			}
			// settings
			case 2:
			{
				break;
			}
			// scores
			case 3:
			{
				break;
			}
			// about
			case 4:
			{
				break;
			}
			// exit
			case 5:
			{
				MSE_LOG("Commanding to stop the application");
				mse::Application::GetApplication()->Stop();
				break;
			}
		}
	}
	
	~SimpleUILayer()
	{
		MSE_LOG("Destroyed a simpleUI layer");
	}
	
	int mode = 0; // 0 - nothing, 1 - game, 2 - settings, 3 - scores, 4 - about, 5 - exit
	std::vector<int> gameBoard = {};
	mse::gui::Canvas* gameCanvas = nullptr;
	mse::gui::Canvas* scoreCanvas = nullptr;
	mse::gui::Canvas* itemCanvas = nullptr;
	glm::ivec4 gameCanvasGrid = {8, 8, 10, 20};
	glm::ivec4 player = {5, 10, 1, 3}; // x, y, length, lives
	MovementDirection direction = MovementDirection::None; // 1 - left, 2 - right, 3 - down, 4 - up;
	std::vector<glm::ivec2> tail = {{-1, -1}};
	glm::ivec2 apple = {0, 0};
	bool gameOver = false;
	mse::Resource* bmpFont = nullptr;
	
	void MovePlayer(MovementDirection dir)
	{
		tail[0] = {player.x, player.y};
		switch (dir)
		{
			case MovementDirection::Left:
			{
				--player.x;
				break;
			}
			case MovementDirection::Right:
			{
				++player.x;
				break;
			}
			case MovementDirection::Up:
			{
				--player.y;
				break;
			}
			case MovementDirection::Down:
			{
				++player.y;
				break;
			}
		}
	}
};

class App : public mse::Application
{
public:
	App() : mse::Application()
	{
		MSE_LOG("Hello, world!");
		MSE_CORE_ERROR("Joke");
		
		MSE_LOG("Commanding to open a window");
		m_window = mse::WindowManager::CreateWindow(u8"Test window", 50, 50, 200, 200);

		mse::Renderer::SetActiveWindow(m_window);
		m_window->GetLayerManager()->Attach(new SimpleUILayer());
	}
	
	~App()
	{
		MSE_LOG("Commanding to destroy a window");
		delete cse_texture;
		cse_texture = nullptr;
		mse::WindowManager::DestroyWindow(m_window);
		m_window = nullptr;
		MSE_LOG("Goodbye, world!");
	}
	
private:
	mse::Window* m_window = nullptr;
	mse::Texture* cse_texture = nullptr;
};

mse::Application* mse::CreateApplication()
{
	return new App();
}
