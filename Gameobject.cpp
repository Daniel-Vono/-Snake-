#include "Gameobject.hpp"

void Gameobject::InitializeGameobject(int x, int y, int w, int h, char texturePath[], SDL_Renderer* renderer) {
	xPos = x;
	yPos = y;
	width = w;
	height = h;

	destRect.x = xPos;
	destRect.y = yPos;
	destRect.w = width;
	destRect.h = height;

	//center = {width / 2, height / 2};

	SDL_Surface* tempSurface = IMG_Load(texturePath);
	texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
}

Gameobject::Gameobject(int x, int y, int w, int h, char texturePath[], SDL_Renderer* renderer) {
	xPos = x;
	yPos = y;
	width = w;
	height = h;

	destRect.x = xPos;
	destRect.y = yPos;
	destRect.w = width;
	destRect.h = height;

	//center = {width / 2, height / 2};

	SDL_Surface* tempSurface = IMG_Load(texturePath);
	texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
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