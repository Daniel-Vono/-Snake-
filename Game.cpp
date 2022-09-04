#include "Game.hpp"
#include "Gameobject.hpp"
#include "TextBox.hpp"

#include <list>
#include <iterator>
#include <unistd.h>
#include <ctime>
#include <SDL2/SDL_mixer.h>

enum Direction{
	Up,
	Down,
	Left,
	Right
};
enum GameState{
	Menu,
	Gameplay
};

std::list<Gameobject> snake;
Gameobject apple;

SDL_Rect menuSnakePos[3];

TextBox gameOverText;
TextBox restartText;
TextBox titleText;
TextBox startGameText;
TextBox scoreTextGameOver;
TextBox scoreTextGameOverNum;

GameState currentState = Menu;
Direction snakeDir = Right;
const int SNAKE_SCALE = 32;
bool gameOver = false;
int score;

char snakeFaceRightPath[] = "SnakeFaceRight.png";
char snakeBodyPath[] = "SnakeBody.png";
char appleTexturePath[] = "Apple.png";

SDL_Texture* snakeBodyTex;
SDL_Texture* snakeFaceUpTex;
SDL_Texture* snakeFaceDownTex;
SDL_Texture* snakeFaceLeftTex;
SDL_Texture* snakeFaceRightTex;
SDL_Texture* appleTex;

Mix_Chunk* eatSound;
Mix_Chunk* snakeBreakSound;

Game::Game(){
	srand((unsigned) time(0));
	TTF_Init();
}
Game::~Game()
{
}

void Game::Init(const char *title, int xPos, int yPos, int width, int height, bool fullScreen){

	int flags = 0;
	if (fullScreen){
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
		std::cout << "Subsystems..." << std::endl;

		SDL_Window *window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (window){
			std::cout << "Window Created..." << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer){
			std::cout << "Renderer Created..." << std::endl;
			SDL_SetRenderDrawColor(renderer, 112, 88, 72, 255);
		}

		//Init audio settings
		Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

		isRunning = true;

		titleText.InitializeTextBox(192, 16, 0, 0, 0, 35, "+Snake+", renderer);
		startGameText.InitializeTextBox(192/2, 456, 0, 0, 0, 35, "Press Space To Start", renderer);

		
		SDL_Surface* tempSurfaceBody = IMG_Load(snakeBodyPath);
		snakeBodyTex = SDL_CreateTextureFromSurface(renderer, tempSurfaceBody);
		SDL_FreeSurface(tempSurfaceBody);

		SDL_Surface* tempSurfaceUp = IMG_Load("SnakeFaceUp.png");
		snakeFaceUpTex = SDL_CreateTextureFromSurface(renderer, tempSurfaceUp);
		SDL_FreeSurface(tempSurfaceUp);

		SDL_Surface* tempSurfaceDown = IMG_Load("SnakeFaceDown.png");
		snakeFaceDownTex = SDL_CreateTextureFromSurface(renderer, tempSurfaceDown);
		SDL_FreeSurface(tempSurfaceDown);

		SDL_Surface* tempSurfaceLeft = IMG_Load("SnakeFaceLeft.png");
		snakeFaceLeftTex = SDL_CreateTextureFromSurface(renderer, tempSurfaceLeft);
		SDL_FreeSurface(tempSurfaceLeft);

		SDL_Surface* tempSurfaceRight = IMG_Load(snakeFaceRightPath);
		snakeFaceRightTex = SDL_CreateTextureFromSurface(renderer, tempSurfaceRight);
		SDL_FreeSurface(tempSurfaceRight);

		SDL_Surface* tempSurfaceApple = IMG_Load(appleTexturePath);
		appleTex = SDL_CreateTextureFromSurface(renderer, tempSurfaceApple);
		SDL_FreeSurface(tempSurfaceApple);

		eatSound = Mix_LoadWAV("Eat.wav");
		snakeBreakSound = Mix_LoadWAV("SnakeBreak.wav");

		//InitGame();

		menuSnakePos[0].x = 64;
		menuSnakePos[0].y = 192;
		menuSnakePos[0].w = 64;
		menuSnakePos[0].h = 64;
		menuSnakePos[1].x = 128;
		menuSnakePos[1].y = 192;
		menuSnakePos[1].w = 64;
		menuSnakePos[1].h = 64;
		menuSnakePos[2].x = 192;
		menuSnakePos[2].y = 192;
		menuSnakePos[2].w = 64;
		menuSnakePos[2].h = 64;
	}
	else{
		isRunning = false;
	}
}

void Game::HandleEvents(){
	SDL_Event event;

	while (SDL_PollEvent(&event)){

		switch (event.type){

		case SDL_QUIT:
			isRunning = false;
			break;

		case SDL_KEYDOWN:

			static int temp;
			temp++;

			if ((event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_UP) && snakeDir != 1){
				std::cout << "UP" << temp << std::endl;
				snakeDir = Up;
			}
			if ((event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN) && snakeDir != Up){
				std::cout << "DOWN" << temp<< std::endl;
				snakeDir = Down;
			}
			if ((event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT) && snakeDir != Right){
				std::cout << "LEFT" << temp << std::endl;
				snakeDir = Left;
			}
			if ((event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT) && snakeDir != Left){
				std::cout << "RIGHT" << temp << std::endl;
				snakeDir = Right;
			}
			if(gameOver && event.key.keysym.sym == SDLK_r){
				InitGame();
				std::cout << "Goto menu" << std::endl;
			}
			if(currentState == Menu && event.key.keysym.sym == SDLK_SPACE){
				InitGame();
				std::cout << "Init Game" << std::endl;
			}

			break;

		default:
			break;
		}
	}
}

void Game::Update()
{
	switch(currentState){
		case Menu:
			break;

		case Gameplay:
			UpdateGameplay();
			break;
	}
	
}

void Game::UpdateGameplay(){

	if(!gameOver){
		
		switch (snakeDir){
			case Up:
				snake.front().texture = snakeBodyTex;
				snake.push_front(Gameobject(snake.front().xPos, snake.front().yPos - SNAKE_SCALE, SNAKE_SCALE, SNAKE_SCALE, snakeBodyTex, renderer));
				snake.pop_back();
				snake.front().texture = snakeFaceUpTex;
				break;

			case Down:
				snake.front().texture = snakeBodyTex;
				snake.push_front(Gameobject(snake.front().xPos, snake.front().yPos + SNAKE_SCALE, SNAKE_SCALE, SNAKE_SCALE, snakeBodyTex, renderer));
				snake.pop_back();
				snake.front().texture = snakeFaceDownTex;
				break;

			case Left:
				snake.front().texture = snakeBodyTex;
				snake.push_front(Gameobject(snake.front().xPos - SNAKE_SCALE, snake.front().yPos, SNAKE_SCALE, SNAKE_SCALE, snakeBodyTex, renderer));
				snake.pop_back();
				snake.front().texture = snakeFaceLeftTex;
				break;

			case Right:
				snake.front().texture = snakeBodyTex;
				snake.push_front(Gameobject(snake.front().xPos + SNAKE_SCALE, snake.front().yPos, SNAKE_SCALE, SNAKE_SCALE, snakeBodyTex, renderer));
				snake.pop_back();
				snake.front().texture = snakeFaceRightTex;
				break;
		}

		usleep(100000);

		if (snake.front().xPos < 0 || snake.front().xPos > 511 || snake.front().yPos < 0 || snake.front().yPos > 511){
			gameOver = true;
			scoreTextGameOverNum.InitializeTextBox(280, 80, 0, 0, 0, 20, std::to_string(score), renderer);
			DeleteSnake();
			std::cout << "DEAD bound" << std::endl;
			return;
		}
		else{
			std::list<Gameobject>::iterator it = snake.begin();
			std::advance(it, 2);
			for (it; it != snake.end(); ++it){
				if (BoxToBox(snake.begin()->destRect, it->destRect)){
					gameOver = true;
					scoreTextGameOverNum.InitializeTextBox(280, 80, 0, 0, 0, 20, std::to_string(score), renderer);
					DeleteSnake();
					std::cout << "DEAD self" << std::endl;
					return;
				}
			}
		}

		if(BoxToBox(snake.begin()->destRect, apple.destRect)){
			std::cout << "YUM" << std::endl;
			EatApple();
		}
	}
	else{
		DeleteSnake();
	}
	
}

void Game::Render(){
	switch(currentState){
		case Menu:
			RenderMenu();
			break;

		case Gameplay:
			RenderGameplay();
			break;
	}

}

void Game::RenderGameplay(){
	
	// Clear renderer buffer
	SDL_RenderClear(renderer);

	// Add Backgrounds to Render Here

	// Add Objects to Render Here

	for (std::list<Gameobject>::iterator it = snake.begin(); it != snake.end(); ++it){
		SDL_RenderCopy(renderer, it->texture, NULL, &it->destRect);
	}

	if(!gameOver){
		SDL_RenderCopy(renderer, apple.texture, NULL, &apple.destRect);
	}
	else{
		SDL_RenderCopy(renderer, gameOverText.texture, NULL, &gameOverText.destRect);
		SDL_RenderCopy(renderer, restartText.texture, NULL, &restartText.destRect);
		SDL_RenderCopy(renderer, scoreTextGameOver.texture, NULL, &scoreTextGameOver.destRect);
		SDL_RenderCopy(renderer, scoreTextGameOverNum.texture, NULL, &scoreTextGameOverNum.destRect);
	}
	

	// Displays the things we are rendering
	SDL_RenderPresent(renderer);
}

void Game::RenderMenu(){

	// Clear renderer buffer
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, titleText.texture, NULL, &titleText.destRect);
	SDL_RenderCopy(renderer, startGameText.texture, NULL, &startGameText.destRect);

	SDL_RenderCopy(renderer, snakeBodyTex, NULL, &menuSnakePos[0]);
	SDL_RenderCopy(renderer, snakeBodyTex, NULL, &menuSnakePos[1]);
	SDL_RenderCopy(renderer, snakeFaceRightTex, NULL, &menuSnakePos[2]);

	// Displays the things we are rendering
	SDL_RenderPresent(renderer);
}

void Game::Clean(){
	std::cout << "Cleaning chun..." << std::endl;
	Mix_FreeChunk(eatSound);
	std::cout << "Cleaning chun2..." << std::endl;
	Mix_FreeChunk(snakeBreakSound);
	std::cout << "close Mixer..." << std::endl;
	Mix_CloseAudio();
	std::cout << "quit Mixer..." << std::endl;
	Mix_Quit();

	std::cout << "Mixer Cleaned." << std::endl;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}

void Game::InitGame(){
	
	gameOver = false;
	currentState = Gameplay;

	snakeDir = Right;

	score = 0;

	snake.push_back(Gameobject(96, 128, SNAKE_SCALE, SNAKE_SCALE, snakeFaceRightTex, renderer));
	snake.push_back(Gameobject(80, 128, SNAKE_SCALE, SNAKE_SCALE, snakeBodyTex, renderer));
	snake.push_back(Gameobject(64, 128, SNAKE_SCALE, SNAKE_SCALE, snakeBodyTex, renderer));

	apple = Gameobject(384, 256, SNAKE_SCALE, SNAKE_SCALE, appleTex, renderer);


	gameOverText.InitializeTextBox(170, 32, 0, 0, 0, 35, "Game Over", renderer);
	restartText.InitializeTextBox(120, 442, 0, 0, 0, 30, "Press R to play again", renderer);


	scoreTextGameOver.InitializeTextBox(220, 80, 0, 0, 0, 20, "Score:", renderer);
}

//Eats the existing apple and increases the length of the snake
void Game::EatApple(){

	//Based on the directon of the snake
	switch (snakeDir){

		case Up:

			//Set the old head texture to the body texture
			snake.front().texture = snakeBodyTex;

			//Add a new snake segment to the head and set the texture to the snake head texture
			snake.push_front(Gameobject(snake.front().xPos, snake.front().yPos - SNAKE_SCALE, SNAKE_SCALE, SNAKE_SCALE, snakeBodyTex, renderer));
			snake.front().texture = snakeFaceUpTex;

			break;

		case Down:

			//Set the old head texture to the body texture
			snake.front().texture = snakeBodyTex;

			//Add a new snake segment to the head and set the texture to the snake head texture
			snake.push_front(Gameobject(snake.front().xPos, snake.front().yPos + SNAKE_SCALE, SNAKE_SCALE, SNAKE_SCALE, snakeBodyTex, renderer));
			snake.front().texture = snakeFaceDownTex;

			break;

		case Left:

			//Set the old head texture to the body texture
			snake.front().texture = snakeBodyTex;

			//Add a new snake segment to the head and set the texture to the snake head texture
			snake.push_front(Gameobject(snake.front().xPos - SNAKE_SCALE, snake.front().yPos, SNAKE_SCALE, SNAKE_SCALE, snakeBodyTex, renderer));
			snake.front().texture = snakeFaceLeftTex;

			break;

		case Right:

			//Set the old head texture to the body texture
			snake.front().texture = snakeBodyTex;

			//Add a new snake segment to the head and set the texture to the snake head texture
			snake.push_front(Gameobject(snake.front().xPos + SNAKE_SCALE, snake.front().yPos, SNAKE_SCALE, SNAKE_SCALE, snakeBodyTex, renderer));
			snake.front().texture = snakeFaceRightTex;

			break;
	}

	//Generate a new position for the apple
	GenerateApplePos();

	//Increment the score
	score++;

	//Plays the eat apple sound
	Mix_PlayChannel(-1, eatSound, 0);
}

//Generates a new currently empty position for the apple
void Game::GenerateApplePos(){

	//Generates a random position for the apple
	apple.destRect.x = (rand() % 16) * SNAKE_SCALE;
	apple.destRect.y = (rand() % 16) * SNAKE_SCALE;
	
	//Checks each segment of the snake to see if the apple exists inide the snake
	std::list<Gameobject>::iterator it = snake.begin();
	for (it; it != snake.end(); ++it){

		//If the apple dest rect collides with a snake segment dest rect
		if (BoxToBox(apple.destRect, it->destRect)){

			//Generate a new apple position and break from the loop once done
			GenerateApplePos();
			break;
		}
	}
}

//Removes the head of the snake body
void Game::DeleteSnake(){

	//If the snake is empty
	if(!snake.empty()){
		
		//Remove the head of the snake
		snake.pop_front();

		//Play the snake break sound effect
		std::cout << "Audio Del: " << Mix_PlayChannel(-1, snakeBreakSound, 0) << std::endl;

		//Make the program sleep
		usleep(50000);
	}
}

//Checks collision between 2 SDL rects
bool Game::BoxToBox(SDL_Rect box1, SDL_Rect box2){

	//No collision on the left (-x)
	if ((box1.x + box1.w) < box2.x + 1) { return false; }
	//No collision on the right (+x)
	else if ((box1.x) > (box2.x + box2.w - 1)) { return false; }
	//No collision on the top (-y)
	else if ((box1.y + box1.h) < box2.y + 1) { return false; }
	//No collision on the bottom (+y)
	else if (box1.y > (box2.y + box2.h - 1)) { return false; }
	//Collision between the boxes
	else { return true; }
}