#include <iostream>
#include <thread>

#include "gameboard.h"
#include "graphics.h"
#include "BacktrackResolver.h"
#include "defines.h"


void Render(const Gameboard& gb) {

    SDL& main_ = SDL::GetInstance();

    while (!SDL::isCreated);

    main_.changeBackgroundColour(0xFF, 0xFD, 0xD0, 0xff);

    main_.drawGrid(GRID_POSITION::TOP_LEFT); 
        
    while (SDL::isCreated) {

        main_.fillGrid(GRID_POSITION::TOP_LEFT, gb.getBoardArray(), 0x1);

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    }

}

void Resolve(Gameboard& gb) {


    while (!SDL::isCreated);

    gb.writeElement(1, 5, 7);
    gb.writeElement(7, 7, 4);
    gb.writeElement(2, 8, 1);
    gb.writeElement(5, 1, 5);
    gb.writeElement(6, 4, 1);

    BacktrackResolver testResolver(gb);

    testResolver.backtrack();

}

int main(int argc, char* argv[]) {
      
   SDL::changeWindowDimensions(800, 600); 

   Gameboard test;


   SDL& main_ = SDL::GetInstance();
   std::thread& t1 = main_.getWindowThread();
   std::thread t2(Render, std::ref(test));
   std::thread t3(Resolve, std::ref(test));

   
   t1.join();
   t2.join();
   t3.join();
   
   

    return 0;

}