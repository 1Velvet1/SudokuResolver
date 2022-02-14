#ifndef GRAPHICS___H
#define GRAPHICS___H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <vector>
#include <iostream>
#include <thread>
#include "lib/SDL2/include/SDL.h"



enum class SDL_INIT_ERROR {

    SDL_INIT_SUBSYSTEM  = 1 << 0,
    SDL_WINDOW_CREATE   = 1 << 1,
    SDL_RENDERER_CREATE = 1 << 2,
    SDL_RENDER_CLEAR    = 1 << 3,
    SDL_BACKGROUND      = 1 << 4,
    SDL_INIT_OK         = 0

};

enum class GRID_POSITION {

    TOP_LEFT = 1,
    TOP_RIGHT = 2,
    BOTTOM_LEFT = 3,
    BOTTOM_RIGHT = 4

};

class SDL {      

public: //static
       

    static SDL& GetInstance() {
        
        static SDL instance;
        static bool isCreated;
        
        if (!isCreated) {

            isCreated = true;

        }
        
        return instance;

    }

    //must be called before class creation
    static void changeWindowDimensions(size_t width, size_t height) {

        if (!isCreated) {

            SCREENWIDTH = width;
            SCREENHEIGHT = height;

        }

    }

private:

    static size_t SCREENWIDTH;
    static size_t SCREENHEIGHT;
    static bool isCreated;

public:

    ~SDL();

    void ClearRender();
    void changeBackgroundColour(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha);    
    void drawNumber(const GRID_POSITION place, const size_t row, const size_t column, const uint16_t number);    
    void drawGrid(const GRID_POSITION place);
    void fillGrid(const GRID_POSITION place, const uint16_t vals[81]);
    void clearGrid(const GRID_POSITION place);
    
private:

    SDL_INIT_ERROR initSDL();
    void eventHandler();
    void invoke();
    SDL();
    SDL(SDL&);

    std::thread thisThread;
    SDL_Window* window;
    SDL_Renderer* renderer;
    size_t winWidth;
    size_t winHeight;
    std::vector<uint8_t> backgroundCol;

};


#endif // !GRAPHICS___H