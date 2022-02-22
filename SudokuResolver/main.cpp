#include <iostream>
#include <thread>

#include "gameboard.h"
#include "graphics.h"
#include "BMP.h"


void colourTest() {

    SDL& main_ = SDL::GetInstance();

    while (!SDL::isCreated);


    main_.drawGrid(GRID_POSITION::TOP_LEFT);
    main_.drawNumber(GRID_POSITION::TOP_LEFT, 6, 4, 5);
    main_.drawNumber(GRID_POSITION::TOP_LEFT, 9, 1, 6);
    main_.drawNumber(GRID_POSITION::TOP_LEFT, 4, 7, 9);
    main_.drawNumber(GRID_POSITION::TOP_LEFT, 1, 9, 4);
    Sleep(5000);
    main_.drawGrid(GRID_POSITION::TOP_RIGHT);
    main_.drawNumber(GRID_POSITION::TOP_RIGHT, 6, 4, 5);
    main_.drawNumber(GRID_POSITION::TOP_RIGHT, 9, 1, 6);
    main_.drawNumber(GRID_POSITION::TOP_RIGHT, 1, 9, 4);
    main_.drawNumber(GRID_POSITION::TOP_RIGHT, 4, 7, 9);


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