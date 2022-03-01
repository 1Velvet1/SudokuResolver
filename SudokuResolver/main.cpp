#include <iostream>
#include <thread>

#include "gameboard.h"
#include "graphics.h"
#include "BacktrackResolver.h"
#include "defines.h"


void Render(Gameboard& gb) {

    SDL& sdl = SDL::GetInstance();


    while (!SDL::isCreated);

    sdl.changeBackgroundColour(0xFF, 0xFD, 0xD0, 0xff);
    sdl.drawGrid(GRID_POSITION::TOP_LEFT);
    sdl.fillGrid(GRID_POSITION::TOP_LEFT, gb.getBoardArray());

    if (gb.checkValid()) {

        while (SDL::isCreated && !gb.isSolved()) {

            sdl.fillGrid(GRID_POSITION::TOP_LEFT, gb.getBoardArray(), gb.getColourScheme());

        }


    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    if (gb.checkCompletion()) {

        sdl.fillGrid(GRID_POSITION::TOP_LEFT, gb.getBoardArray(), 0x00FF00);
        
    }
    else {

        sdl.fillGrid(GRID_POSITION::TOP_LEFT, gb.getBoardArray(), 0xFF0000);

    }


}

void Solve(Gameboard& gb) {
    
    BacktrackResolver testResolver(gb);
    testResolver.changeWaitPeriod(20);
    while (!SDL::isCreated);


    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    testResolver.backtrack();

}

int main(int argc, char* argv[]) {

    SDL::changeWindowDimensions(800, 600);

    Gameboard test;
    test.writeElement(0, 1, 1);
    test.writeElement(5, 7, 8);
    test.writeElement(1, 8, 3);
    test.writeElement(5, 4, 5);
    test.writeElement(7, 2, 2);
    test.writeElement(2, 0, 7);
    test.writeElement(8, 0, 4);
    test.writeElement(6, 4, 9);
    test.writeElement(0, 5, 3);
    test.writeElement(8, 6, 1);
    test.writeElement(1, 1, 8);
    test.writeElement(1, 8, 5);
    test.writeElement(5, 4, 4);
    test.writeElement(1, 6, 7);
    test.writeElement(2, 7, 2);
    test.writeElement(3, 3, 5);
    test.writeElement(0, 8, 6);
    test.writeElement(7, 4, 8);

    SDL& main_ = SDL::GetInstance();
    std::thread& t1 = main_.getWindowThread();
    std::thread t2(Render, std::ref(test));
    std::thread t3(Solve, std::ref(test));


    t1.join();
    t2.join();
    t3.join();



    return 0;

}