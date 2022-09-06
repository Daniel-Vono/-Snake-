//Includes the header files used to define class in the file
#include "Gameobject.hpp"

//Initializes the gameobject
Gameobject::Gameobject(int x, int y, int w, int h, SDL_Texture* tex, SDL_Renderer* renderer) {
	
	//Initializes the positions and dimensions
	xPos = x;
	yPos = y;
	width = w;
	height = h;

	//Initialize the dest rect
	destRect.x = xPos;
	destRect.y = yPos;
	destRect.w = width;
	destRect.h = height;

	//Initializes the texture
	texture = tex;
}

//Initializes the gameobject with all null values
Gameobject::Gameobject(){}