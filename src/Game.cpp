#include "Game.hpp"


bool Game::init(){
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( GAME_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }else{
                    background = IMG_LoadTexture(gRenderer, BACKGROUND_PATH);
                    
                    if(background == NULL)
                    {
                        SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), gWindow);
                    }
                    
                }
            }
        }
    }

    return success;
}

void Game::close(){
    //Free loaded images

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();

}

void Game::gameLoop(){
    deck = std::make_unique<CardDeck>(gRenderer);

    p0 = std::make_unique<Player>(0, "Joao");
    p1 = std::make_unique<Player>(1, "Thomas");
    p2 = std::make_unique<Player>(2, "Cecilia");
    p3 = std::make_unique<Player>(3, "Rosa");

    deck->Shuffle();

    p0->SetCards(std::move(deck->Get10Cards()));
    p1->SetCards(std::move(deck->Get10Cards()));
    p2->SetCards(std::move(deck->Get10Cards()));
    p3->SetCards(std::move(deck->Get10Cards()));
   

    //distributing cards
    

    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
        }
        
        
        //Clear screen
        SDL_RenderClear( gRenderer );

        //Update screen
        SDL_RenderCopy(gRenderer, background, NULL, NULL);
        // render cards here
        //deck->GetCard(35)->render();
        //card.render();
        
        SDL_SetRenderDrawColor( gRenderer, 0x0, 0x0, 0x0, 0x0 );
        
        SDL_RenderPresent( gRenderer );
    }
    
    //Free resources and close SDL
    close();
}