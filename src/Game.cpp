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
    sf::Vector2f deckPos{640.0f, 360.0f}; // Centro da mesa

    // Inicializa os jogadores e distribui cartas
    for (int i = 0; i < 4; i++) {
        players.push_back(std::make_unique<Player>(i, "Nome", i != 0));
        auto dealtCards = deck->drawCards(10);
        
        for (auto& card : dealtCards) {
            // TODAS as cartas nascem no centro da mesa
            card->setPosition(deckPos, true); 
            card->setRotation(0.0f, true);
        }
        players[i]->setCards(dealtCards);
    }
}

Game::~Game() {}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        processEvents();
        update(deltaTime); // Passamos o tempo decorrido
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

void Game::update(float deltaTime) {
    for (int i = 0; i < players.size(); ++i) {
        const auto& hand = players[i]->getHand();
        float spacing = 40.0f; // Espaço entre cartas

        for (size_t j = 0; j < hand.size(); ++j) {
            sf::Vector2f target;
            float rot = 0.0f;

            if (i == 0) { // BAIXO (Humano)
                target.x = 400.0f + (j * 50.0f);
                target.y = 600.0f;
                rot = 0.0f;
            } 
            else if (i == 1) { // DIREITA (CPU 1)
                target.x = 1100.0f;
                target.y = 200.0f + (j * spacing);
                rot = 270.0f;
            } 
            else if (i == 2) { // CIMA (CPU 2)
                // Inverti o cálculo para elas espalharem da esquerda para a direita
                target.x = 800.0f - (j * 50.0f); 
                target.y = 120.0f;
                rot = 180.0f;
            } 
            else if (i == 3) { // ESQUERDA (CPU 3)
                target.x = 180.0f;
                target.y = 500.0f - (j * spacing);
                rot = 90.0f;
            }

            // Define os alvos para a animação
            hand[j]->setPosition(target);
            hand[j]->setRotation(rot);

            // Processa o movimento suave (Lerp)
            hand[j]->update(deltaTime);
        }
    }
    
    // Não esqueça de atualizar as cartas que estiverem na mesa também!
    for (auto& card : tableCards) {
        card->update(deltaTime);
    }
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