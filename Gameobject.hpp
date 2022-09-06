#ifndef Gameobject_hpp
#define Gameobject_hpp

//Includes the libraries used for SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Gameobject {
	public:
		//Initializes the gameobject
		Gameobject(int x, int y, int w, int h, SDL_Texture* tex, SDL_Renderer* renderer);

		//Initializes the gameobject with all null values
		Gameobject();

		//Positions and dimensions
		int xPos;
		int yPos;
		int width;
		int height;

		//The gameobject's dest rect
		SDL_Rect destRect;
		
		//The Gameobject's Texture
		SDL_Texture* texture;
};


#endif