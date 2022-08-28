#include "TextBox.hpp"

TextBox::TextBox(){

}

TextBox::TextBox(int x, int y, int r, int g, int b, int size, const char* msg, SDL_Renderer* renderer){

    xPos = x;
    yPos = y;
    fontSize = size;
    text = msg;

    font = TTF_OpenFont("OpenSans.ttf", size);
    SDL_Color clr;
    clr.r = r;
    clr.g = g;
    clr.b = b;
    color = clr;

    SDL_Surface* tempSurface = TTF_RenderText_Solid(font, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);

    destRect.x = xPos;
    destRect.y = yPos;
    destRect.w = tempSurface->w;
    destRect.h = tempSurface->h;

    SDL_FreeSurface(tempSurface);
}

void TextBox::InitializeTextBox(int x, int y, int r, int g, int b, int size, std::string msg, SDL_Renderer* renderer){
    xPos = x;
    yPos = y;
    fontSize = size;
    //text = msg;

    font = TTF_OpenFont("OpenSans.ttf", size);
    SDL_Color clr;
    clr.r = r;
    clr.g = g;
    clr.b = b;
    color = clr;

    SDL_Surface* tempSurface = TTF_RenderText_Solid(font, msg.c_str(), color);
    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);

    destRect.x = xPos;
    destRect.y = yPos;
    destRect.w = tempSurface->w;
    destRect.h = tempSurface->h;

    SDL_FreeSurface(tempSurface);
}