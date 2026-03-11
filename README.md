# Sueca Game
A modern implementation of the classic Portuguese card game Sueca, built with C++. This project has transitioned from SDL2 to SFML 3, focusing on a robust object-oriented architecture and preparing for future integration with Machine Learning models.

## Refactor Highlights

* Graphics Engine: Fully migrated to SFML 3, utilizing modern C++17 standards.

* Resource Management: Implemented a texture caching system in CardDeck to optimize memory usage and prevent redundant disk I/O.

* Native Portability: Enhanced support for macOS (App Bundles) and Linux, including automated asset deployment via CMake.

* AI-Ready Architecture: Clear decoupling between game logic (Game), data structures (CardDeck), and decision-making (Player).

## Dependencies

Ensure you have a C++17 compatible compiler and CMake installed.

* macOS (Recommended via Homebrew)
  ```
  brew install sfml
  ```

* Ubuntu / Linux
    ```
    sudo apt install libsfml-dev
    ```


## Building the Game
To build the game, follow these steps:

#### Create a build directory:

```
mkdir build
cd build
```

(Note: Ensure the installed version is 3.0 or higher for compatibility with the provided CMake targets).

## Building and Running

### Create a build directory:

```
mkdir build
cd build
```

### Configure and Build::

```
cmake ..
make
```

### Run the game:

* macOS

```
open sueca.app
```

* Linux

```
./sueca
```

## Machine Learning Readiness

The game is designed to facilitate the collection of match data:

* State Vectorization: Player hands and table states can be easily converted into numerical formats (tensors/vectors).

* Decision Interface: The playCard method allows for transparent interaction, whether the action comes from a Human (input) or an AI (model inference).

## Roadmap

[x] Migration from SDL2 to SFML 3.

[x] Sprite-based card rendering system.

[x] Deck shuffling and distribution logic.

[x] macOS Bundle pathing and asset automation.

## Credits
Developed by João Gouveia as a personal software engineering and artificial intelligence challenge.