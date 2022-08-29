#include <iostream>
#include <SDL2/SDL.h>

#include"Game.hpp"

Game* game = nullptr;

int main(int argc, char* argv[]) {

    const int FPS = 9;
    const int FRAME_DELAY = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();
    game->Init("+Snake+", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, false);

    while (game->Running()) {

        frameStart = SDL_GetTicks();

        game->HandleEvents();
        game->Update();
        game->Render();

        frameTime = SDL_GetTicks() - frameStart;

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    game->Clean();

    return 0;
}