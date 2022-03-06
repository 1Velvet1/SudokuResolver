#include <iostream>
#include <thread>

#include "gameboard.h"
#include "graphics.h"
#include "BacktrackResolver.h"
#include "defines.h"



void Render(const Gameboard& gb, const GRID_POSITION gPos) {

    SDL& sdl = SDL::GetInstance();

    while (!SDL::isCreated);

    sdl.changeBackgroundColour(0xFF, 0xFD, 0xD0, 0xff);
    sdl.drawGrid(gPos);
    sdl.fillGrid(gPos, gb.getBoardArray());

    if (gb.checkValid()) {

        while (SDL::isCreated && !gb.isSolved()) {

            sdl.fillGrid(gPos, gb.getBoardArray(), gb.getColourScheme());

        }

    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    if (gb.isSolved()) {

        sdl.fillGrid(gPos, gb.getBoardArray(), 0x00FF00);
        
    }
    else {

        sdl.fillGrid(gPos, gb.getBoardArray(), 0xFF0000);

    }

}


void Solve(Gameboard& gb, const size_t waitTime = 0) {
    
    BacktrackResolver testResolver(gb);
    testResolver.changeWaitPeriod(waitTime);
    while (!SDL::isCreated);

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    testResolver.backtrack();

}

int main(int argc, char* argv[]) {

    SDL::changeWindowDimensions(800, 600);
    

    Gameboard test; 
    test.readFromFile("C:\\Users\\Laptop\\Desktop\\123.txt");     
    Gameboard test1;
    test1.readFromFile("C:\\Users\\Laptop\\Desktop\\1234.txt");

    SDL& main_ = SDL::GetInstance();
    std::thread& t1 = main_.getWindowThread();
    std::thread t2(Render, std::ref(test), GRID_POSITION::TOP_LEFT);
    std::thread t22(Render, std::ref(test1), GRID_POSITION::TOP_RIGHT);
    std::thread t3(Solve, std::ref(test), 0);
    std::thread t33(Solve, std::ref(test1), 0);


    t1.join();
    t2.join();
    t22.join();
    t3.join();  
    t33.join();

    return 0;

}