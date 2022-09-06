//Includes the header files used to define classes in the file
#include "Game.hpp"
#include "Gameobject.hpp"
#include "TextBox.hpp"

//Represents the directon of the snake
enum Direction{
	Up,
	Down,
	Left,
	Right
};

//Represents the current state of the game
enum GameState{
	Menu,
	Gameplay
};

//Current state of the game
GameState currentState = Menu;

//Represents if the game has been lost
bool gameOver = false;

//The number that represents the scale of objects, used to scale object size and collision detection
const int SNAKE_SCALE = 32;

//Stores the players score
int score;

//Represents the snake's current direction
Direction snakeDir = Right;

//Snake and apple game objects
std::list<Gameobject> snake;
Gameobject apple;

//Snake and apple positions
SDL_Rect menuSnakeRect[3];
SDL_Rect menuAppleRect;

//Text boxes
TextBox gameOverText;
TextBox restartText;
TextBox titleText;
TextBox startGameText;
TextBox scoreTextGameOver;
TextBox scoreTextGameOverNum;

//Stores the textures used by the game
SDL_Texture* snakeBodyTex;
SDL_Texture* snakeFaceUpTex;
SDL_Texture* snakeFaceDownTex;
SDL_Texture* snakeFaceLeftTex;
SDL_Texture* snakeFaceRightTex;
SDL_Texture* appleTex;

//Stores the sound effects
Mix_Chunk* eatSound;
Mix_Chunk* snakeBreakSound;

//Game class constructor and deconstructor
Game::Game(){}
Game::~Game(){}

//Initializes the Game class
void Game::Init(const char *title, int xPos, int yPos, int width, int height, bool fullScreen){

	//Sets the flags used in initialization
	int flags = 0;
	if (fullScreen) flags = SDL_WINDOW_FULLSCREEN;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0){

		//Create the SDL Window
		SDL_Window *window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);

		//Create the SDL renderer and set the background colour
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) SDL_SetRenderDrawColor(renderer, 112, 88, 72, 255);

		//Initializes SDL TTF
		TTF_Init();

		//Init audio settings
		Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

		//Initialize the random number generator
		srand((unsigned) time(0));

		//Set the game running boolean to true
		isRunning = true;

		//Initializes the text boxes used in the gameplay
		gameOverText.InitializeTextBox(170, 32, 0, 0, 0, 35, "Game Over", renderer);
		restartText.InitializeTextBox(120, 442, 0, 0, 0, 30, "Press R to play again", renderer);
		scoreTextGameOver.InitializeTextBox(220, 80, 0, 0, 0, 20, "Score:", renderer);

		//Initializes the text boxes used in the menu
		titleText.InitializeTextBox(192, 16, 0, 0, 0, 35, "+Snake+", renderer);
		startGameText.InitializeTextBox(192/2, 456, 0, 0, 0, 35, "Press Space To Start", renderer);
		
		//Creates the snake body texture
		SDL_Surface* tempSurfaceBody = IMG_Load("SnakeBody.png");
		snakeBodyTex = SDL_CreateTextureFromSurface(renderer, tempSurfaceBody);
		SDL_FreeSurface(tempSurfaceBody);

		//Ceates the snake face up texture
		SDL_Surface* tempSurfaceUp = IMG_Load("SnakeFaceUp.png");
		snakeFaceUpTex = SDL_CreateTextureFromSurface(renderer, tempSurfaceUp);
		SDL_FreeSurface(tempSurfaceUp);

		//Ceates the snake face down texture
		SDL_Surface* tempSurfaceDown = IMG_Load("SnakeFaceDown.png");
		snakeFaceDownTex = SDL_CreateTextureFromSurface(renderer, tempSurfaceDown);
		SDL_FreeSurface(tempSurfaceDown);

		//Ceates the snake face left texture
		SDL_Surface* tempSurfaceLeft = IMG_Load("SnakeFaceLeft.png");
		snakeFaceLeftTex = SDL_CreateTextureFromSurface(renderer, tempSurfaceLeft);
		SDL_FreeSurface(tempSurfaceLeft);

		//Ceates the snake face right texture
		SDL_Surface* tempSurfaceRight = IMG_Load("SnakeFaceRight.png");
		snakeFaceRightTex = SDL_CreateTextureFromSurface(renderer, tempSurfaceRight);
		SDL_FreeSurface(tempSurfaceRight);

		//Ceates the apple texture
		SDL_Surface* tempSurfaceApple = IMG_Load("Apple.png");
		appleTex = SDL_CreateTextureFromSurface(renderer, tempSurfaceApple);
		SDL_FreeSurface(tempSurfaceApple);

		//Loads the sound effects
		eatSound = Mix_LoadWAV("Eat.wav");
		snakeBreakSound = Mix_LoadWAV("SnakeBreak.wav");

		//Sets the positions used for textures in the menu
		menuSnakeRect[0].x = 64;
		menuSnakeRect[0].y = 192;
		menuSnakeRect[0].w = 64;
		menuSnakeRect[0].h = 64;
		menuSnakeRect[1].x = 128;
		menuSnakeRect[1].y = 192;
		menuSnakeRect[1].w = 64;
		menuSnakeRect[1].h = 64;
		menuSnakeRect[2].x = 192;
		menuSnakeRect[2].y = 192;
		menuSnakeRect[2].w = 64;
		menuSnakeRect[2].h = 64;
		menuAppleRect.x = 384;
		menuAppleRect.y = 192;
		menuAppleRect.w = 64;
		menuAppleRect.h = 64;
	}
	//Else something didn't initialize property so set the game running boolean to false
	else{
		isRunning = false;
	}
}

//Handles input for the game
void Game::HandleEvents(){

	//A variable that holds an SDL event
	SDL_Event event;

	//While there are still events to handel
	while (SDL_PollEvent(&event)){

		//Check which event is currently being handled
		switch (event.type){

			//Quit the game
			case SDL_QUIT:
				isRunning = false;
				break;

			//If a key is pressed
			case SDL_KEYDOWN:


				//If the W or Up Arrow key is pressed and the snake can move up, set the snake direction to up
				if ((event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_UP) && snakeDir != Down) snakeDir = Up;
				
				//If the S or Down Arrow key is pressed and the snake can move down, set the snake direction to down
				if ((event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN) && snakeDir != Up) snakeDir = Down;
				
				//If the A or Left Arrow key is pressed and the snake can move left, set the snake direction to left
				if ((event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT) && snakeDir != Right) snakeDir = Left;
				
				//If the D or Right Arrow key is pressed and the snake can move up, set the snake direction to right
				if ((event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT) && snakeDir != Left) snakeDir = Right;
				
				//If the R Key is pressed and the game is over, start the game
				if(event.key.keysym.sym == SDLK_r && gameOver) InitGame();
				
				//If the Space Key is pressed and the user is in the menu, start the game
				if(currentState == Menu && event.key.keysym.sym == SDLK_SPACE) InitGame();
				
				break;

			default:
				break;
		}
	}
}

//Switches between different sub update functions based on the game state
void Game::Update()
{
	//If the game is not over
	if(!gameOver){
		
		//Move the snake based on it's direction
		switch (snakeDir){

			case Up:
				
				//Set the old head texture to the body texture
				snake.front().texture = snakeBodyTex;

				//Add a new snake segment to the head and set the texture to the snake head texture
				snake.push_front(Gameobject(snake.front().xPos, snake.front().yPos - SNAKE_SCALE, SNAKE_SCALE, SNAKE_SCALE, snakeBodyTex, renderer));
				snake.front().texture = snakeFaceUpTex;

				//Removes the end segment of the snake
				snake.pop_back();

				break;

			case Down:

				//Set the old head texture to the body texture
				snake.front().texture = snakeBodyTex;

				//Add a new snake segment to the head and set the texture to the snake head texture
				snake.push_front(Gameobject(snake.front().xPos, snake.front().yPos + SNAKE_SCALE, SNAKE_SCALE, SNAKE_SCALE, snakeBodyTex, renderer));
				snake.front().texture = snakeFaceDownTex;

				//Removes the end segment of the snake
				snake.pop_back();

				break;

			case Left:

				//Set the old head texture to the body texture
				snake.front().texture = snakeBodyTex;

				//Add a new snake segment to the head and set the texture to the snake head texture
				snake.push_front(Gameobject(snake.front().xPos - SNAKE_SCALE, snake.front().yPos, SNAKE_SCALE, SNAKE_SCALE, snakeBodyTex, renderer));
				snake.front().texture = snakeFaceLeftTex;

				//Removes the end segment of the snake
				snake.pop_back();

				break;

			case Right:

				//Set the old head texture to the body texture
				snake.front().texture = snakeBodyTex;

				//Add a new snake segment to the head and set the texture to the snake head texture
				snake.push_front(Gameobject(snake.front().xPos + SNAKE_SCALE, snake.front().yPos, SNAKE_SCALE, SNAKE_SCALE, snakeBodyTex, renderer));
				snake.front().texture = snakeFaceRightTex;

				//Removes the end segment of the snake
				snake.pop_back();

				break;
		}

		//Pause the whole application for 0.1 seconds
		usleep(100000);

		//If the snake head collides with the edge of the screen
		if (snake.front().xPos < 0 || snake.front().xPos > 511 || snake.front().yPos < 0 || snake.front().yPos > 511){

			//Set the game to over boolean to true
			gameOver = true;

			//Sets the value of the score text
			scoreTextGameOverNum.InitializeTextBox(280, 80, 0, 0, 0, 20, std::to_string(score), renderer);

			//Delete a snake segment
			DeleteSnake();

			return;
		}
		//Else check if the snake head collides with segment 3 and beyond
		else{

			//Create an itorator that starts at the 3rd segment of the snake
			std::list<Gameobject>::iterator it = snake.begin();
			std::advance(it, 2);

			//Loop through the snake segments
			for (it; it != snake.end(); ++it){

				//If the snake head collides with the current segment
				if (BoxToBox(snake.begin()->destRect, it->destRect)){

					//Set the game over boolean to true
					gameOver = true;

					//Sets the value of the score text
					scoreTextGameOverNum.InitializeTextBox(280, 80, 0, 0, 0, 20, std::to_string(score), renderer);

					//Delete a snake segment
					DeleteSnake();

					return;
				}
			}
		}

		//If the snake head collides with the apple
		if(BoxToBox(snake.begin()->destRect, apple.destRect)) EatApple();
	}
	//Else, the game is over so delete a snake segment if any remain
	else DeleteSnake();
}

//Switches between different sub rendering functions based on the game state 
void Game::Render(){

	switch(currentState){

		//Render the menu graphics
		case Menu:
			RenderMenu();
			break;

		//Render the gameplay graphics
		case Gameplay:
			RenderGameplay();
			break;
	}

}

//Renders everything durring gameplay
void Game::RenderGameplay(){
	
	// Clear renderer buffer
	SDL_RenderClear(renderer);

	//Renders each segment of the snake
	for (std::list<Gameobject>::iterator it = snake.begin(); it != snake.end(); ++it){
		SDL_RenderCopy(renderer, it->texture, NULL, &it->destRect);
	}

	//If the game is not over, render the apple
	if(!gameOver) SDL_RenderCopy(renderer, apple.texture, NULL, &apple.destRect);

	//Else render the text on the game over screen
	else{

		SDL_RenderCopy(renderer, gameOverText.texture, NULL, &gameOverText.destRect);
		SDL_RenderCopy(renderer, restartText.texture, NULL, &restartText.destRect);
		SDL_RenderCopy(renderer, scoreTextGameOver.texture, NULL, &scoreTextGameOver.destRect);
		SDL_RenderCopy(renderer, scoreTextGameOverNum.texture, NULL, &scoreTextGameOverNum.destRect);
	}
	
	// Displays the things being rendered
	SDL_RenderPresent(renderer);
}

//Renders everything in the menu screen
void Game::RenderMenu(){

	// Clear renderer buffer
	SDL_RenderClear(renderer);

	//Renders the text in the menu
	SDL_RenderCopy(renderer, titleText.texture, NULL, &titleText.destRect);
	SDL_RenderCopy(renderer, startGameText.texture, NULL, &startGameText.destRect);

	//Renders the snake
	SDL_RenderCopy(renderer, snakeBodyTex, NULL, &menuSnakeRect[0]);
	SDL_RenderCopy(renderer, snakeBodyTex, NULL, &menuSnakeRect[1]);
	SDL_RenderCopy(renderer, snakeFaceRightTex, NULL, &menuSnakeRect[2]);

	//Renders the apple
	SDL_RenderCopy(renderer, appleTex, NULL, &menuAppleRect);

	// Displays the things we are rendering
	SDL_RenderPresent(renderer);
}

//Closes and removes any existing processes in the game
void Game::Clean(){

	//Frees the 2 audio chunks
	Mix_FreeChunk(eatSound);
	Mix_FreeChunk(snakeBreakSound);

	//Close and quit SDL Mixer
	Mix_CloseAudio();
	Mix_Quit();

	//Close the window and quit SDL
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

//Called to initialize/restart the game
void Game::InitGame(){
	
	//Set the game over and game state
	gameOver = false;
	currentState = Gameplay;

	//Sets the snake's direction to right
	snakeDir = Right;

	//Set the score to 0
	score = 0;

	//Initializes the first 3 segments of the snake
	snake.push_back(Gameobject(96, 128, SNAKE_SCALE, SNAKE_SCALE, snakeFaceRightTex, renderer));
	snake.push_back(Gameobject(80, 128, SNAKE_SCALE, SNAKE_SCALE, snakeBodyTex, renderer));
	snake.push_back(Gameobject(64, 128, SNAKE_SCALE, SNAKE_SCALE, snakeBodyTex, renderer));

	//Initializes the apple
	apple = Gameobject(384, 256, SNAKE_SCALE, SNAKE_SCALE, appleTex, renderer);
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

	//Plays the eat apple sound effect on the next available channel
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

		//Play the snake break sound effect on the next available channel
		Mix_PlayChannel(-1, snakeBreakSound, 0);

		//Make the program sleep
		usleep(50000);
	}
}

//Checks collision between 2 SDL rects
bool Game::BoxToBox(SDL_Rect box1, SDL_Rect box2){

	//No collision on the left (-x)
	if ((box1.x + box1.w) < box2.x + 1) return false;

	//No collision on the right (+x)
	else if ((box1.x) > (box2.x + box2.w - 1)) return false;

	//No collision on the top (-y)
	else if ((box1.y + box1.h) < box2.y + 1) return false;

	//No collision on the bottom (+y)
	else if (box1.y > (box2.y + box2.h - 1)) return false;

	//Collision between the boxes
	else return true;
}