#include "Game.hpp"
#include <iostream>
#ifdef __APPLE__
    #include <mach-o/dyld.h> // Para _NSGetExecutablePath
    #include <libgen.h>      // Para dirname
    #include <unistd.h>      // Para chdir
#endif

// Construtor inicializando a janela no padrão SFML 3
Game::Game() 
    : window(sf::VideoMode(windowSize), gameTitle),
      backgroundSprite(backgroundTexture) 
{
#ifdef __APPLE__
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        // Resolve o caminho real (remove links simbólicos)
        char realPath[1024];
        if (realpath(path, realPath) != NULL) {
            // O caminho atual é: .../sueca.app/Contents/MacOS/sueca
            // Queremos ir para: .../sueca.app/Contents/Resources/
            char* dir = dirname(realPath); // Contents/MacOS
            chdir(dir);
            chdir("../Resources"); // Agora o diretório de trabalho é Resources
        }
    }
#endif

    // Agora o caminho "data/..." funcionará independente de como você abrir o app
    if (!backgroundTexture.loadFromFile("data/bg.png")) {
        std::cerr << "Erro crítico: Nao foi possivel carregar data/bg.png" << std::endl;
    }
    
    deck = std::make_unique<CardDeck>();
    deck->shuffle();
    
    players.push_back(std::make_unique<Player>(0, "Voce", false));
    players[0]->setCards(deck->drawCards(10));
}

Game::~Game() {}

void Game::run() {
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    // PADRÃO SFML 3: pollEvent agora retorna std::optional
    while (const std::optional event = window.pollEvent()) {
        
        // Verifica se o evento é do tipo Closed
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        
        // Exemplo de como tratar tecla pressionada no SFML 3
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                window.close();
            }
        }
    }
}

void Game::update() {
    // Lógica de jogo (IA, turnos, etc)
}

void Game::render() {
    window.clear();
    
    // 1. Desenha o fundo
    window.draw(backgroundSprite);
    
    // 2. Desenha a mão do jogador 0 (Humano)
    // No Game.cpp, dentro do render()
    if (!players.empty() && !players[0]->getHand().empty()) {
        float xPos = 150.0f; 
        float yPos = 550.0f; 
        float spacing = 60.0f; // Diminuindo o espaço elas se sobrepõem elegantemente

        for (auto& card : players[0]->getHand()) {
            card->setPosition(xPos, yPos);
            card->render(window);
            xPos += spacing;
        }
    }

    window.display();
}