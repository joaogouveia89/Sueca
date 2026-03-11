#include <iostream>
#include "Game.hpp"

int main(int argc, char* args[]) {
    // Game initialization and resource loading
    Game game;

    // Start main game loop (Events, Update, Render)
    game.run();

    return 0;
}