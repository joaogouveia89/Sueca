
#include <iostream>
#include "Game.hpp"

int main( int argc, char* args[] )
{
    Game game;
    //Start up the game
    if( !game.init() )
    {
        printf( "Failed to initialize!\n" );
    }else{
        game.gameLoop();
    }
    return 0;
}