//
//  Game.hpp
//  Santa Claus Run
//
//  Created by Joao Gouveia on 05/05/2023.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <memory>
#include <SDL.h>
#include <SDL2/SDL_image.h>
#include "CardDeck.hpp"

class Game{
public:
    bool init();
    void close();
    void gameLoop();
    
private:
    //Game constants
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;

    const std::string GAME_TITLE = "Sueca";
    const char* BACKGROUND_PATH = "../data/bg.png";
    
    bool success { true };
    bool quit { false };

    std::unique_ptr<CardDeck> deck;
    
    SDL_Event e;
    SDL_Window* gWindow { NULL };
    SDL_Renderer* gRenderer { NULL };
    
    SDL_Texture* background { NULL };
    
    std::unique_ptr<CardTexture> clubs2;
};

#endif /* Game_hpp */