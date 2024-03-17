#if !defined(CARD_TEXTURE_HPP)
#define CARD_TEXTURE_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class CardTexture
{
private:
     SDL_Renderer* screenRenderer;
    //The actual hardware texture
    SDL_Texture* mTexture;
    //Image dimensions
    int mWidth;
    int mHeight;
    int renderFactor { 5 };
public:
    CardTexture(SDL_Renderer* renderer, std::string imagePath);
    ~CardTexture();

    bool loadFromFile( std::string path );
    void free();
    void render( int x, int y);
    //Gets image dimensions
    int getWidth() const{ return mWidth; };
    int getHeight() const{ return mHeight; };
};


#endif // CARD_TEXTURE_HPP
