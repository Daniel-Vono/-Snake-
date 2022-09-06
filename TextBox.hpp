#ifndef TextBox_hpp
#define TextBox_hpp

//Includes the libraries used for SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

//Includes the string data type
#include <string>

class TextBox{
    public:

        //Initializes the text box with all null values
        TextBox();

        //Initializes the text box
        void InitializeTextBox(int x, int y, int r, int g, int b, int size, std::string msg, SDL_Renderer* renderer);

        //Text box position
        int xPos;
        int yPos;

        //Text box dest rect
        SDL_Rect destRect;

        //Text font size
        int fontSize;

        TTF_Font* font;
        SDL_Color color;

        //Text box message
        const char* text;

        //Text Box texture
        SDL_Texture* texture;
        
};
#endif