#include "Gameobject.hpp"

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

Gameobject::Gameobject(){
	
}

void Gameobject::UpdatePosition() {
	xPos = destRect.x;
	yPos = destRect.y;
}

void Gameobject::RevertPosition() {
	destRect.x = xPos;
	destRect.y = yPos;
}