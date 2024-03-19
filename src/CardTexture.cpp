#include "CardTexture.hpp"

CardTexture::CardTexture(SDL_Renderer* renderer, std::string imagePath){
    //Initialize
    screenRenderer = renderer;
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
    
    auto isSuccessFull = loadFromFile(imagePath);
}

CardTexture::~CardTexture()
{
    //Deallocate
    free();
}

bool CardTexture::loadFromFile(std::string path)
{
    // Get rid of preexisting texture
    free();

    // The final texture
    SDL_Texture* newTexture = nullptr;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        // Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(screenRenderer, loadedSurface);
        if (newTexture == nullptr)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        else
        {
            // Get image dimensions
            mWidth = loadedSurface->w / renderFactor;
            mHeight = loadedSurface->h / renderFactor;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    // Assign the loaded texture to member variable
    mTexture = newTexture;

    // Return success only if the texture is loaded successfully
    return mTexture != nullptr;
}


void CardTexture::render( int x, int y)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    //Render to screen
    SDL_RenderCopyEx( screenRenderer, mTexture, NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE );
}

void CardTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}
