# Sueca Game
This is a Sueca card game implemented in C++ using SDL2 for the GUI. Sueca is a popular Portuguese card game typically played with four players. This project is developed as a personal challenge and aimed to create a game enjoyable for the developer.

## Dependencies

Before building the game, make sure you have the following dependencies installed:

* SDL2: Simple DirectMedia Layer is a cross-platform development library designed to provide low-level access to audio, keyboard, mouse, joystick, and graphics hardware.
  * To install on Ubuntu, run:
  ```
  sudo apt install libsdl2-dev
  ```

* SDL2 Image: A library for loading images in various formats.
    * To install on Ubuntu, run:
    ```
    sudo apt install libsdl2-image-dev
    ```


## Building the Game
To build the game, follow these steps:

#### Create a build directory:

```
mkdir build
cd build
```

#### Run CMake to generate build files:

```
cmake ..
```

#### Build the project:

```
make
```

#### Run the game:

```
./sueca
```
## References
This project was developed with reference to the SDL tutorials available at [Lazy Foo' Productions](https://lazyfoo.net/tutorials/SDL/).

## Future Development
The project is still ongoing and will receive updates. The primary goal is to complete a version of the game where the player can play against four CPU players at a normal difficulty level. Future plans include implementing more advanced AI using TensorFlow for decision making.