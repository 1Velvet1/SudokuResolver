#include <iostream>

#include "gameboard.h"
#include "graphics.h"



int main(int argc, char* argv[]) {
      
   SDL::changeWindowDimensions(800, 600);
   SDL& main_ = SDL::GetInstance();
   
   main_.changeBackgroundColour(0xFF, 0xF0, 0xF0, 0xFF);
   

    return 0;

}