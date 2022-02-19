#include <iostream>
#include <thread>

#include "gameboard.h"
#include "graphics.h"


void colourTest() {

    SDL& main_ = SDL::GetInstance();

    while (true) {

        if (SDL::isCreated) { break; }

    }       

    main_.drawGrid(GRID_POSITION::TOP_LEFT);
    Sleep(5000);
    main_.drawGrid(GRID_POSITION::TOP_RIGHT);
}


int main(int argc, char* argv[]) {
      
   SDL::changeWindowDimensions(800, 600);
   SDL& main_ = SDL::GetInstance();
   std::thread& t1 = main_.getWindowThread();
   std::thread t2(colourTest);

   main_.changeBackgroundColour(0xFF, 0xFD, 0xD0, 0xff);

   t1.join();
   t2.join();

   

    return 0;

}