#include <iostream>
#include "Game.hpp"

int main(int argc, char* args[]) {
    // O construtor já inicializa a janela e carrega as texturas
    Game game;

    // O método 'run' contém o loop principal (processEvents, update, render)
    game.run();

    return 0;
}