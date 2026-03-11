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
    
    // Criar os 4 jogadores
    players.push_back(std::make_unique<Player>(0, "Voce", false)); // Humano
    players.push_back(std::make_unique<Player>(1, "CPU 1", true));
    players.push_back(std::make_unique<Player>(2, "CPU 2", true));
    players.push_back(std::make_unique<Player>(3, "CPU 3", true));

    // Distribuir 10 cartas para cada
    for (int i = 0; i < players.size(); ++i) {
        // Usamos players[i] para acessar o jogador atual do loop
        players[i]->setCards(deck->drawCards(10));
        
        // Se não for o Player 0 (Humano), vira as cartas para baixo
        if (i != 0) {
            for (auto& card : players[i]->getHand()) {
                card->setFaceUp(false);
            }
        }
    }
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
    window.draw(backgroundSprite);

    for (int i = 0; i < players.size(); ++i) {
        const auto& hand = players[i]->getHand();
        float x, y, rotation;
        float spacing = 40.0f; // Espaçamento entre as cartas

        for (size_t j = 0; j < hand.size(); ++j) {
            if (i == 0) { // Baixo (Humano)
                x = 400.0f + (j * 50.0f);
                y = 600.0f;
                rotation = 0.0f;
            } 
            else if (i == 1) { // Direita
                x = 1100.0f;
                y = 200.0f + (j * spacing);
                rotation = 270.0f; // Girada para a esquerda
            } 
            else if (i == 2) { // Cima
                x = 800.0f - (j * 50.0f);
                y = 120.0f;
                rotation = 180.0f; // De cabeça para baixo
            } 
            else if (i == 3) { // Esquerda
                x = 180.0f;
                y = 500.0f - (j * spacing);
                rotation = 90.0f; // Girada para a direita
            }

            hand[j]->setRotation(rotation);
            hand[j]->setPosition(x, y);
            hand[j]->render(window);
        }
    }

    window.display();
}