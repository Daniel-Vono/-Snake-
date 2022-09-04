#ifndef Gameobject_hpp
#define Gameobject_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Gameobject {
	public:
		void UpdatePosition();
		void RevertPosition();
		Gameobject(int x, int y, int w, int h, SDL_Texture* tex, SDL_Renderer* renderer);
		Gameobject();

		int xPos;
		int yPos;
		int width;
		int height;

		SDL_Rect destRect;

		SDL_Texture* texture;
};


#endif