#ifndef GRAPHICS___H
#define GRAPHICS___H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "lib/SDL2/include/SDL.h"


enum class SDL_INIT_ERROR{};

enum class GRID_POSITION{};

static class SDL {

public:

    SDL(SDL&) = delete;
    SDL_INIT_ERROR initSDL();
    SDL& GetInstance();


    void changeBackgroundColour(const size_t red, const size_t green, const size_t blue, const size_t alpha);
    void drawNumber(const GRID_POSITION place, const size_t row, const size_t column, const uint16_t number);    
    void drawGrid(const GRID_POSITION place);
    void fillGrid(const GRID_POSITION place, const uint16_t vals[81]);
    void clearGrid(const GRID_POSITION place);

    ~SDL();

private:

    SDL();

    static SDL instance;
    static bool isCreated;


};


#endif // !GRAPHICS___H