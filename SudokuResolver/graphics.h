#ifndef GRAPHICS___H
#define GRAPHICS___H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <thread>
#include <vector>
#include <map>
#include <mutex>

#include "lib/SDL2/include/SDL.h"
#include "BMP.h"
#include "defines.h"

constexpr int BASE_WINDOW_WIDTH = 600;
constexpr int BASE_WINDOW_HEIGHT = 400;
constexpr int TOP_GRID_OFFSET = 13;
constexpr int TOP_LEFT_GRID_xOFFSET = 20;
constexpr int TOP_RIGHT_GRID_xOFFSET = 320;


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
    TOP_RIGHT,
    NUMBER_GRID

};

class SDL {      

public: //static
       
    //after class has been initialized window thread should be joined via GetWindowThread()
    static SDL& GetInstance() {
        
        static SDL instance;
        static bool isCreated;
        
        if (!isCreated) {

            isCreated = true;

        }
        
        return instance;

    }

    //must be called before class creation
    static void changeWindowDimensions(int width, int height) {

        if (!isCreated) {

            SCREENWIDTH = width;
            SCREENHEIGHT = height;
                
        }

    }

    volatile static bool isCreated;

private:

    static int SCREENWIDTH;
    static int SCREENHEIGHT;

public: //non-static

    ~SDL();

    void UpdateRender();
    void changeBackgroundColour(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha);    

    /// <summary>
/// call this function to draw a number in specific place
/// </summary>
/// <param name="place">The grid in which the number should be drawn</param>
/// <param name="row">row of the number: 1..9</param>
/// <param name="column">column of the number 1..9</param>
/// <param name="number">number to be drawn: 1..9</param>
/// <param name="colour">the colour of the number (black is 0x1, 0x0 is background)</param>
    void drawNumber(const GRID_POSITION place, const size_t row, const size_t column, const uint16_t number, const RGBcolour colour);
    void eraseNumber(const GRID_POSITION place, const size_t row, const size_t column);
    void drawGrid(const GRID_POSITION place);

    void fillGrid(const GRID_POSITION place, const board& vals, const matrice& colourScheme);
    void fillGrid(const GRID_POSITION place, const board& vals, const RGBcolour colour = 0x1);
    void clearGrid(const GRID_POSITION place);

    std::thread& getWindowThread();

private:

    matrice scaleMatrice();
    uint32_t toRGB(uint8_t R, uint8_t G, uint8_t B) const;
    void RGBtoColr(const RGBcolour& RGB, uint8_t& R, uint8_t& G, uint8_t& B) const;
    void ClearRender();
    SDL_INIT_ERROR initSDL();
    void eventHandler();
    void invoke();
    SDL();
    SDL(SDL&);

    //thread management
    std::thread thisThread;
    std::mutex screenMatriceLock;
    //local variables
    bool leftGridDrawn;
    bool rightGridDrawn;
    matrice screenMatrice;
    SDL_Window* window;
    SDL_Renderer* renderer;
    int winWidth;
    int winHeight;
    std::vector<uint8_t> backgroundCol;
    std::map<uint16_t, image> numbersMap;

};


#endif // !GRAPHICS___H