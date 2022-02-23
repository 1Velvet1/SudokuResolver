#include <iostream>
#include <thread>

#include "gameboard.h"
#include "graphics.h"
#include "BMP.h"


void colourTest() {

    SDL& main_ = SDL::GetInstance();

    while (!SDL::isCreated);


    main_.drawGrid(GRID_POSITION::TOP_LEFT);
    
    std::vector<std::vector<uint16_t>> arr(9);

    for (size_t i = 0; i < 9; i++) {

        arr[i].resize(9);

        for (size_t j = 0; j < 9; j++) {

            arr[i][j] = 1 + rand() % 9;

        }

    }

    main_.fillGrid(GRID_POSITION::TOP_LEFT, arr);

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