#include "graphics.h"
#include <iostream>
#include <string>

//static member initizlization
int SDL::SCREENWIDTH = BASE_WINDOW_WIDTH;
int SDL::SCREENHEIGHT = BASE_WINDOW_HEIGHT;
volatile bool SDL::isCreated = false;

//init functions

SDL_INIT_ERROR SDL::initSDL() {

    winWidth = SDL::SCREENWIDTH;
    winHeight = SDL::SCREENHEIGHT;
    leftGridDrawn = false;
    rightGridDrawn = false;

    if (SDL_Init(SDL_INIT_EVERYTHING)) { return SDL_INIT_ERROR::SDL_INIT_SUBSYSTEM; }

    this->window = SDL_CreateWindow("Sudoku Solver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->winWidth, this->winHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (this->window == nullptr) { return SDL_INIT_ERROR::SDL_WINDOW_CREATE; }

    SDL_SetWindowMinimumSize(this->window, BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT);

    this->screenMatrice.resize(BASE_WINDOW_HEIGHT);
    for (auto& row : this->screenMatrice) { row = std::vector<RGBcolour>(BASE_WINDOW_WIDTH); }

    BMP tempBMP;

    for (size_t i = 1; i < 10; i++) {

        std::string path = ".\\res\\" + std::to_string(i) + ".bmp";

        try {

            tempBMP = BMP(path.c_str());
            this->numbersMap.insert(std::pair<uint16_t, image>(i, tempBMP.getImage()));

        }
        catch (const std::exception& IOexception) {

            std::cout << "Cannot load resource files: " << IOexception.what() << "\n";

        }

    }

    BMP iconFile;    

    try {

        iconFile = BMP(".\\res\\icon.bmp");

    }
    catch (const std::exception& IOexception) {

        std::cout << "Cannot load resource files: " << IOexception.what() << "\n";

    }

    const image& icon = iconFile.getImage(); 
    const size_t iconSize = iconFile.getWidth() * iconFile.getHeight();
    RGBcolour* pixels = new RGBcolour[iconSize];
    size_t pixel = 0;

    for (const auto& row : icon) {

        for (const auto& col : row) {

            pixels[pixel] = col;
            pixel++;

        }

    }
    SDL_Surface* iconSurf = SDL_CreateRGBSurfaceFrom(pixels, iconFile.getWidth(), iconFile.getHeight(), iconFile.getDepth(), sizeof(RGBcolour) * iconFile.getWidth(), NULL, NULL, NULL, 0xFF000000);

    SDL_SetWindowIcon(this->window, iconSurf);
    SDL_FreeSurface(iconSurf);
    delete[] pixels;

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
    int x, y;

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

    if (SDL::isCreated) {

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


void SDL::drawNumber(const GRID_POSITION place, const size_t row, const size_t column, const uint16_t number, const RGBcolour colour = 0x1)
{

    if (row == 0 || column == 0 || number >= 10 || column > 9 || row > 9) { return; }


    const auto& curNum = this->numbersMap.find(number);
    const image& num = curNum->second;

    this->screenMatriceLock.lock();
    matrice temp = this->screenMatrice;
    size_t xOffset = 0;
    size_t yOffset = 0;

    switch (place) {

    case GRID_POSITION::TOP_LEFT:

        if (!this->leftGridDrawn) { return; }

        yOffset = TOP_GRID_OFFSET + (num.size() + 2) * (row - 1) + 2;
        xOffset = TOP_LEFT_GRID_xOFFSET + (num[0].size() + 2) * (column - 1) + 2;

        break;
    case GRID_POSITION::TOP_RIGHT:

        if (!this->rightGridDrawn) { return; }

        yOffset = TOP_GRID_OFFSET + (num.size() + 2) * (row - 1) + 2;
        xOffset = TOP_RIGHT_GRID_xOFFSET + (num[0].size() + 2) * (column - 1) + 2;

        break;

    case GRID_POSITION::NUMBER_GRID:
        //TODO
        break;

    }

    for (size_t i = 0; i < 20; i++) {

        for (size_t j = 0; j < 20; j++) {

            temp[yOffset + i][xOffset + j] = (num[i][j] == 0xFFFFFF) ? 0 : (num[i][j] == 0) ? colour : colour + (num[i][j] | colour);
           
        }

    }


    this->screenMatrice = temp;
    this->screenMatriceLock.unlock();

}

void SDL::eraseNumber(const GRID_POSITION place, const size_t row, const size_t column) {

    if (row == 0 || column == 0 || row > 9 || column > 9) { return; }

    this->screenMatriceLock.lock();
    matrice temp = this->screenMatrice;
    size_t xOffset = 0;
    size_t yOffset = 0;
    

    switch (place) {

    case GRID_POSITION::TOP_LEFT:

        if (!this->leftGridDrawn) { return; }

        yOffset = TOP_GRID_OFFSET + 22 * (row - 1) + 2;
        xOffset = TOP_LEFT_GRID_xOFFSET + 22 * (column - 1) + 2;

        break;
    case GRID_POSITION::TOP_RIGHT:

        if (!this->rightGridDrawn) { return; }

        yOffset = TOP_GRID_OFFSET + 22 * (row - 1) + 1;
        xOffset = TOP_RIGHT_GRID_xOFFSET + 22 * (column - 1) + 1;


        break;

    case GRID_POSITION::NUMBER_GRID:
        //TODO
        break;

    }


    for (size_t i = 0; i < 20; i++) {

        for (size_t j = 0; j < 20; j++) {

            temp[yOffset + i][xOffset + j] = 0;

        }

    }

    this->screenMatrice = temp;
    this->screenMatriceLock.unlock();

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

void SDL::RGBtoColr(const RGBcolour& RGB, uint8_t& R, uint8_t& G, uint8_t& B) const {

    R = (RGB >> 16) & 0xFF;
    G = (RGB >> 8) & 0xFF;
    B = RGB & 0xFF;    

}

matrice SDL::scaleMatrice(){


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

    this->screenMatriceLock.lock();
    if (BASE_WINDOW_WIDTH == this->winWidth && BASE_WINDOW_HEIGHT == this->winHeight) {currentScreen = this->screenMatrice; }
    else { currentScreen = this->scaleMatrice(); }
    this->screenMatriceLock.unlock();

    this->ClearRender();

    for (const auto& row : currentScreen) {

        for (const auto& col : row) {

            if(col != 0){  

                this->RGBtoColr(col, R, G, B);
                SDL_SetRenderDrawColor(this->renderer, R, G, B, col >> 24);
                SDL_RenderDrawPoint(this->renderer, x, y);
                
            }

            x++;

        }
      
        x = 0;
        y++;

    }

    SDL_RenderPresent(this->renderer);

}

void SDL::drawGrid(const GRID_POSITION place) {
    
    this->screenMatriceLock.lock();
    matrice temp = this->screenMatrice;

    size_t rows = 0;    

    switch (place) {

    case GRID_POSITION::TOP_LEFT:     

        while (rows < 10) {

            for (size_t i = 0; i < 199; i++) {
                temp[TOP_GRID_OFFSET + rows * 22][TOP_LEFT_GRID_xOFFSET + i] = rows % 3 == 0 ? 0x1 : 0xA9A9A9;
            }
            rows++;

        }
        rows = 0;
        while (rows < 10) {
        
            for (size_t i = 0; i < 199; i++) {
                temp[TOP_GRID_OFFSET + i][TOP_LEFT_GRID_xOFFSET + rows * 22] = rows % 3 == 0 ? 0x1 : 0xA9A9A9;
        
            }
            rows++;
        
        }
        this->leftGridDrawn = true;

        break;
    case GRID_POSITION::TOP_RIGHT:
        while (rows < 10) {

            for (size_t i = 0; i < 199; i++) {
                temp[TOP_GRID_OFFSET + rows * 22][TOP_RIGHT_GRID_xOFFSET + i] = rows % 3 == 0 ? 0x1 : 0xA9A9A9;
            }
            rows++;

        }
        rows = 0;
        while (rows < 10) {
        
            for (size_t i = 0; i < 199; i++) {
                temp[TOP_GRID_OFFSET + i][TOP_RIGHT_GRID_xOFFSET + rows * 22] = rows % 3 == 0 ? 0x1 : 0xA9A9A9;
        
            }
            rows++;
        
        }
        this->rightGridDrawn = true;

        break;
   

    }

    this->screenMatrice = temp;
    this->screenMatriceLock.unlock();

}

void SDL::clearGrid(const GRID_POSITION place)
{

    size_t xOffset = 0;
    size_t yOffset = 0;

    this->screenMatriceLock.lock();
    matrice temp = this->screenMatrice;

    switch (place) {

    case GRID_POSITION::TOP_LEFT:

        this->leftGridDrawn = false;

        xOffset = TOP_LEFT_GRID_xOFFSET;
        yOffset = TOP_GRID_OFFSET;

        break;

    case GRID_POSITION::TOP_RIGHT:

        this->rightGridDrawn = false;

        xOffset = TOP_RIGHT_GRID_xOFFSET;
        yOffset = TOP_GRID_OFFSET;

        break;

    case GRID_POSITION::NUMBER_GRID:
        //TODO
        break;
    }

    for (size_t i = 0; i < 199; i++) {

        for (size_t j = 0; j < 199; j++) {

            temp[i + TOP_GRID_OFFSET][j + TOP_LEFT_GRID_xOFFSET] = 0;

        }

    }

    this->screenMatrice = temp;
    this->screenMatriceLock.unlock();



}

void SDL::fillGrid(const GRID_POSITION place, const board& vals, const RGBcolour colour) {

    for (size_t i = 1; i < 10; i++) {

        for (size_t j = 1; j < 10; j++) {

            if (vals[i - 1][j - 1] == 0) { this->eraseNumber(place, i, j); continue; }

            this->drawNumber(place, i, j, vals[i - 1][j - 1], colour);

        }

    }

}

void SDL::fillGrid(const GRID_POSITION place, const board& vals, const matrice& colourScheme) {

    for (size_t i = 1; i < 10; i++) {

        for (size_t j = 1; j < 10; j++) {

            if (vals[i - 1][j - 1] == 0 || colourScheme[i - 1][j - 1] == 0) { this->eraseNumber(place, i, j); continue; }

            this->drawNumber(place, i, j, vals[i - 1][j - 1], colourScheme[i - 1][j - 1]);

        }

    }

}