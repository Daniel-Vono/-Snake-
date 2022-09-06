//Includes the header files used to define class in the file
#include "TextBox.hpp"

//Initializes the text box
void TextBox::InitializeTextBox(int x, int y, int r, int g, int b, int size, std::string msg, SDL_Renderer* renderer){
    
    //Initializes the text box position
    xPos = x;
    yPos = y;

    //Sets the font size
    fontSize = size;

    //Sets the font
    font = TTF_OpenFont("OpenSans.ttf", size);

    //Sets the text colour
    SDL_Color clr;
    clr.r = r;
    clr.g = g;
    clr.b = b;
    color = clr;

    //Create the text box's texture
    SDL_Surface* tempSurface = TTF_RenderText_Solid(font, msg.c_str(), color);
    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);

    //Initialize the size of the dest rect
    destRect.w = tempSurface->w;
    destRect.h = tempSurface->h;

    //Free the temporary surface
    SDL_FreeSurface(tempSurface);
}

//Initializes the text box with all null values
TextBox::TextBox(){}