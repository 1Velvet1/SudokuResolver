#include "graphics.h"
#include <iostream>


//static member initizlization
size_t SDL::SCREENWIDTH = 1600;
size_t SDL::SCREENHEIGHT = 1200;
bool SDL::isCreated = false;

//init functions

SDL_INIT_ERROR SDL::initSDL() {

    winWidth = SDL::SCREENWIDTH;
    winHeight = SDL::SCREENHEIGHT;

    if (SDL_Init(SDL_INIT_EVERYTHING)) { return SDL_INIT_ERROR::SDL_INIT_SUBSYSTEM; }
    
    this->window = SDL_CreateWindow("Sudoku Solver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->winWidth, this->winHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (this->window == nullptr) { return SDL_INIT_ERROR::SDL_WINDOW_CREATE; }

    SDL_SetWindowMinimumSize(this->window, 600, 400);

    this->renderer = SDL_CreateRenderer(this->window, -1, NULL);
    if (this->renderer == nullptr) { return SDL_INIT_ERROR::SDL_RENDERER_CREATE; }


    if (SDL_SetRenderDrawColor(this->renderer, this->backgroundCol[0], this->backgroundCol[1], this->backgroundCol[2], this->backgroundCol[3])) {

        return SDL_INIT_ERROR::SDL_BACKGROUND;

    }

    if (SDL_RenderClear(this->renderer)) { return SDL_INIT_ERROR::SDL_RENDER_CLEAR; }

    SDL_RenderPresent(this->renderer);

    return SDL_INIT_ERROR::SDL_INIT_OK;

}

void SDL::eventHandler() {

    SDL_Event e;

    while (SDL::isCreated) {

        SDL_WaitEvent(&e);

        if (e.type == SDL_QUIT) { SDL::isCreated = false; }
        else { this->ClearRender(); }


    }


}

void SDL::invoke() {       

    switch (this->initSDL()) {

    case SDL_INIT_ERROR::SDL_INIT_SUBSYSTEM:

        std::cout << "SDL subsystem initialization error\n" << SDL_GetError();

        break;

    case SDL_INIT_ERROR::SDL_WINDOW_CREATE:

        std::cout << "SDL window create error\n" << SDL_GetError();

        break;

    case SDL_INIT_ERROR::SDL_RENDERER_CREATE:

        std::cout << "SDL renderer create error\n" << SDL_GetError();

        break;

    case SDL_INIT_ERROR::SDL_BACKGROUND:

        std::cout << "SDL background create error\n" << SDL_GetError();

        break;

    case SDL_INIT_ERROR::SDL_RENDER_CLEAR:

        std::cout << "SDL renderer clear error\n" << SDL_GetError();

        break;

    case SDL_INIT_ERROR::SDL_INIT_OK:

        SDL::isCreated = true;
        std::cout << "SDL successfully initialized\n";

        break;

    default:

        std::cout << "Undefined return value\n";
        abort();

    }

    eventHandler();

}

SDL::SDL() :window(nullptr), renderer(nullptr), backgroundCol({0x0, 0x0 ,0x0, 0xFF}), winWidth(SDL::SCREENWIDTH), winHeight(SDL::SCREENHEIGHT)
{      
    this->thisThread = std::thread(&SDL::invoke, this);

}

SDL::~SDL() {

    if (this->isCreated) {

        SDL_RenderClear(this->renderer);
        SDL_DestroyRenderer(this->renderer);
        SDL_DestroyWindow(this->window);
        isCreated = false;

    }

}

std::thread& SDL::getWindowThread() {

    return this->thisThread;

}

//-------------------------------------------------------//

void SDL::changeBackgroundColour(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha) {

    this->backgroundCol[0] = red;
    this->backgroundCol[1] = green;
    this->backgroundCol[2] = blue;
    this->backgroundCol[3] = alpha;

}

void SDL::ClearRender() {

    SDL_SetRenderDrawColor(this->renderer, this->backgroundCol[0], this->backgroundCol[1], this->backgroundCol[2], this->backgroundCol[3]);
    SDL_RenderClear(this->renderer);
    SDL_RenderPresent(this->renderer);

}

void SDL::drawGrid(const GRID_POSITION place) {



}