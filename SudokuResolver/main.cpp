#include <iostream>
#include <thread>

#include "gameboard.h"
#include "graphics.h"


void colourTest() {

    SDL& main_ = SDL::GetInstance();

    while (true) {

        main_.changeBackgroundColour(0x00, 0x00, 0xFF, 0x0F);
        Sleep(400);
        main_.changeBackgroundColour(0xFF, 0x00, 0x00, 0x0F);
        Sleep(400);

    }

}


int main(int argc, char* argv[]) {
      
   SDL::changeWindowDimensions(800, 600);
   SDL& main_ = SDL::GetInstance();
   std::thread& t1 = main_.getWindowThread();
   std::thread t2(colourTest);

   t1.join();
   t2.join();

   

    return 0;

}