#ifndef Game_hpp
#define Game_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

class Game {
public:
	Game();
	~Game();

	void Init(const char* title, int xPos, int yPos, int width, int height, bool fullScreen);
	void HandleEvents();
	void Update();
	void Render();
	void Clean();

	bool Running() { return isRunning; };

	bool BoxToBox(SDL_Rect box1, SDL_Rect box2);
	bool SideToBox(SDL_Rect box1, SDL_Rect box2);

	void InitGame();

	void EatApple();
	void GenerateApplePos();
	void DeleteSnake();

	void UpdateGameplay();
	void RenderGameplay();
	void RenderMenu();

private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
};

#endif // !Game_hpp