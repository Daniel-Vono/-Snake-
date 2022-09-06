//Includes SDL libraries and the game class
#include <SDL2/SDL.h>
#include"Game.hpp"

//The game class
Game* game = nullptr;

int main(int argc, char* argv[]) {

    //Stores the target frames per second and delay between updates
    const int FPS = 60;
    const int FRAME_DELAY = 1000 / FPS;

    //Numbers that controll the rate that updates happen
    Uint32 frameStart;
    int frameTime;

    //Initialize the game
    game = new Game();
    game->Init("+Snake+", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, false);

    //While the game is still running
    while (game->IsRunning()) {

        //Note which frame the update starts on
        frameStart = SDL_GetTicks();

        //Handel events, update the game and render to the screen
        game->HandleEvents();
        game->Update();
        game->Render();

        //Note how many frames it took to update the game
        frameTime = SDL_GetTicks() - frameStart;

        //If the target frame time is still too short, add a delay
        if (FRAME_DELAY > frameTime) SDL_Delay(FRAME_DELAY - frameTime);

    }

    //Clean the game
    game->Clean();

    //Exit the program
    return 0;
}