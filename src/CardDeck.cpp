#include "CardDeck.hpp"
#include <iostream>

CardDeck::CardDeck() {
    char symbols[] = {'2', '3', '4', '5', '6', '7', 'J', 'Q', 'K', 'A'};
    Suit suits[] = {Suit::SPADES, Suit::HEARTS, Suit::DIAMONDS, Suit::CLUBS};

    for (auto s : suits) {

        if (!textureCache["back"].loadFromFile("data/back.png")) {
            std::cerr << "Erro ao carregar back.png" << std::endl;
        }


        for (auto sym : symbols) {
            std::string name = getFileName(s, sym);
            // No SFML 3, loadFromFile retorna um bool que não deve ser ignorado
            if (textureCache.find(name) == textureCache.end()) {
                // REMOVA o "../" do início do caminho
                if (!textureCache[name].loadFromFile("data/" + name + ".png")) {
                    std::cerr << "Falha ao carregar: " << name << std::endl;
                }
            }
            deck.push_back(std::make_shared<Card>(s, sym, textureCache[name], textureCache["back"]));
        }
    }
}

std::string CardDeck::getFileName(Suit s, char sym) {
    std::string n;
    if(sym == 'J') n = "jack";
    else if(sym == 'Q') n = "queen";
    else if(sym == 'K') n = "king";
    else if(sym == 'A') n = "ace";
    else n = std::string(1, sym);

    n += "_of_";
    if(s == Suit::SPADES) n += "spades";
    else if(s == Suit::HEARTS) n += "hearts";
    else if(s == Suit::DIAMONDS) n += "diamonds";
    else n += "clubs";
    return n;
}

void CardDeck::shuffle() {
    std::shuffle(deck.begin(), deck.end(), std::default_random_engine(std::random_device{}()));
}

// Implementação que faltava para o Linker encontrar
std::vector<std::shared_ptr<Card>> CardDeck::drawCards(int count) {
    std::vector<std::shared_ptr<Card>> hand;
    
    // Garante que não tentamos tirar mais cartas do que existem
    int actualCount = std::min(count, (int)deck.size());

    for (int i = 0; i < actualCount; ++i) {
        // Pegamos do final do vetor (back) pois é O(1), mais eficiente
        hand.push_back(deck.back());
        deck.pop_back();
    }
    
    return hand;
}