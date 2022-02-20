#include "graphics.h"
#include <iostream>

//static member initizlization
int SDL::SCREENWIDTH = BASE_WINDOW_WIDTH;
int SDL::SCREENHEIGHT = BASE_WINDOW_HEIGHT;
volatile bool SDL::isCreated = false;

//init functions

SDL_INIT_ERROR SDL::initSDL() {

    winWidth = SDL::SCREENWIDTH;
    winHeight = SDL::SCREENHEIGHT;

    if (SDL_Init(SDL_INIT_EVERYTHING)) { return SDL_INIT_ERROR::SDL_INIT_SUBSYSTEM; }
    
    this->window = SDL_CreateWindow("Sudoku Solver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->winWidth, this->winHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (this->window == nullptr) { return SDL_INIT_ERROR::SDL_WINDOW_CREATE; }

    SDL_SetWindowMinimumSize(this->window, BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT);    

    this->screenMatrice.resize(BASE_WINDOW_HEIGHT);
    for (auto& row : this->screenMatrice) { row = std::vector<uint32_t>(BASE_WINDOW_WIDTH); }

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

        while (SDL_PollEvent(&e)) {

            switch (e.type) {

            case SDL_QUIT:
                SDL::isCreated = false;
                break;
            case SDL_WINDOWEVENT:
                SDL_GetWindowSize(this->window, &this->winWidth, &this->winHeight);
                break;
            default:
                break;

            }

        }
        
        this->UpdateRender();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

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

void SDL::drawNumber(const GRID_POSITION place, const size_t row, const size_t column, const uint16_t number)
{
    


}

void SDL::ClearRender() {

    SDL_SetRenderDrawColor(this->renderer, this->backgroundCol[0], this->backgroundCol[1], this->backgroundCol[2], this->backgroundCol[3]);
    SDL_RenderClear(this->renderer);

}

uint32_t SDL::toRGB(uint8_t R, uint8_t G, uint8_t B) const {

    uint32_t RGB = 0;

    RGB = R;
    RGB = RGB << sizeof(R) * 8;
    RGB += G;
    RGB = RGB << sizeof(G) * 8;
    RGB += B;

    return RGB;

}

void SDL::RGBtoColr(const uint32_t& RGB, uint8_t& R, uint8_t& G, uint8_t& B) const {

    R = (RGB >> 16) & 0xFF;
    G = (RGB >> 8) & 0xFF;
    B = RGB & 0xFF;    

}

matrice SDL::scaleMatrice() const{

    matrice temp;
    matrice base = this->screenMatrice;   
    temp.resize(this->winHeight);
    for (auto& row : temp) { row = std::vector<uint32_t>(this->winWidth); }
    const double scaleY = static_cast<double>(BASE_WINDOW_HEIGHT) / static_cast<double>(this->winHeight);
    const double scaleX = static_cast<double>(BASE_WINDOW_WIDTH) / static_cast<double>(this->winWidth);

    for (size_t y = 0; y < this->winHeight; y++) {

        for (size_t x = 0; x < this->winWidth; x++) {

            temp[y][x] = base[scaleY * y][scaleX * x];

        }

    }

    return temp;

}

void SDL::UpdateRender() {
    
    size_t x = 0;
    size_t y = 0;
    uint8_t R = 0;
    uint8_t G = 0;
    uint8_t B = 0;
    matrice currentScreen;

    if (BASE_WINDOW_WIDTH == this->winWidth && BASE_WINDOW_HEIGHT == this->winHeight) {currentScreen = this->screenMatrice; }
    else { currentScreen = this->scaleMatrice(); }

    this->ClearRender();

    isUpdating = true;
    for (const auto& row : currentScreen) {

        for (const auto& col : row) {

            if(col != 0){  

                this->RGBtoColr(col, R, G, B);
                SDL_SetRenderDrawColor(this->renderer, R, G, B, 0xFF);
                SDL_RenderDrawPoint(this->renderer, x, y);
                
            }

            x++;

        }
      
        x = 0;
        y++;

    }
    isUpdating = false;

    SDL_RenderPresent(this->renderer);

}

void SDL::drawGrid(const GRID_POSITION place) {
    
    matrice temp = this->screenMatrice;
    size_t rows = 0;    

    switch (place) {

    case GRID_POSITION::TOP_LEFT:     

        while (rows < 10) {

            for (size_t i = 0; i < 199; i++) {
                temp[13 + rows * 22][20 + i] = rows % 3 == 0 ? 0x1 : 0xA9A9A9;
            }
            rows++;

        }
        rows = 0;
        while (rows < 10) {
        
            for (size_t i = 0; i < 199; i++) {
                temp[13 + i][20 + rows * 22] = rows % 3 == 0 ? 0x1 : 0xA9A9A9;
        
            }
            rows++;
        
        }
        break;
    case GRID_POSITION::TOP_RIGHT:
        while (rows < 10) {

            for (size_t i = 0; i < 199; i++) {
                temp[13 + rows * 22][320 + i] = rows % 3 == 0 ? 0x1 : 0xA9A9A9;
            }
            rows++;

        }
        rows = 0;
        while (rows < 10) {
        
            for (size_t i = 0; i < 199; i++) {
                temp[13 + i][320 + rows * 22] = rows % 3 == 0 ? 0x1 : 0xA9A9A9;
        
            }
            rows++;
        
        }
        break;
   

    }
    while (isUpdating);

    this->screenMatrice = temp;

}