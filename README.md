# Project Spaceship

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/z9dGlHc2tec/0.jpg)](https://www.youtube.com/watch?v=z9dGlHc2tec)

https://www.youtube.com/watch?v=z9dGlHc2tec

## Requirements
- SFML 2.4
- gcc (7.1 or greater) or clang (4.0 or greater)
### Linux

```
sudo apt-get install libsfml-dev
```
### Windows
1. you need to compile sfml 2.4 from source as static library you can check https://www.sfml-dev.org/tutorials/2.4/compile-with-cmake.php for instructions
2. change src/Makefile-windows `INCLUDE_PATHS` to point at path\to\sfml\include and `LIBRARY_PATHS` to point at path\to\sfml\lib

### Build
#### Linux
```
cd src
make clean
make -f Makefile-unix
```
### windows
```
cd src
make clean
make -f Makefile-windows
```

## Running
to run you just need to run `game.exe` or `./game`

## gameplay
your objective is to survive and defeat the boss
- use arrow keys for movement
- use x for shooting
- use z for boost
