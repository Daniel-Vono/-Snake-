#ifndef Game_hpp
#define Game_hpp

//Includes the libraries used for SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

//Includes libraries for time and sleeping
#include <ctime>
#include <unistd.h>

//Includes libraries for the list and iterator data types
#include <list>
#include <iterator>

//All of the games main procceses are contained in this class
class Game {

	public:
		//Game class constructor and deconstructor
		Game();
		~Game();

		//Initializes the Game class
		void Init(const char* title, int xPos, int yPos, int width, int height, bool fullScreen);

		//Handles input for the game
		void HandleEvents();

		//Switches between different sub update functions based on the game state
		void Update();

		//Switches between different sub rendering functions based on the game state
		void Render();
		
		//Closes and removes any existing processes in the game
		void Clean();

		//Returns if the game is running
		bool IsRunning() { return isRunning; };

		//Checks collision between 2 SDL rects
		bool BoxToBox(SDL_Rect box1, SDL_Rect box2);


		//Called to initialize/restart the game
		void InitGame();


		//Eats the existing apple and increases the length of the snake
		void EatApple();

		//Generates a new currently empty position for the apple
		void GenerateApplePos();

		//Removes the head of the snake body
		void DeleteSnake();


		//Renders everything durring gameplay
		void RenderGameplay();

		//Renders everything in the menu screen
		void RenderMenu();

	private:
		
		//States if the game is running or not
		bool isRunning;

		//Contains the SDL window and renderer
		SDL_Window* window;
		SDL_Renderer* renderer;
};

#endif