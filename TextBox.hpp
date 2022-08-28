#ifndef TextBox_hpp
#define TextBox_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <string>

class TextBox{
    public:

        TextBox();
        TextBox(int x, int y, int r, int g, int b, int size, const char* msg, SDL_Renderer* renderer);
        void InitializeTextBox(int x, int y, int r, int g, int b, int size, std::string msg, SDL_Renderer* renderer);

        int xPos;
        int yPos;
        int fontSize;
        const char* text;

        SDL_Rect destRect;
        SDL_Texture* texture;
        TTF_Font* font;
        SDL_Color color;

};
#endif