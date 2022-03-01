#ifndef GAMEBOARD___H
#define GAMEBOARD___H

#define WIN32_LEAN_AND_MEAN

#include <stdint.h>
#include <Windows.h>
#include <vector>

#include "defines.h"

class Gameboard
{

public:
    
    Gameboard();  
    explicit Gameboard(const board& vals);

    void writeElement(const size_t row, const size_t column, const uint16_t val);
    void writeColour(const size_t row, const size_t column, const RGBcolour colour);

    uint16_t getElement(const size_t row, const size_t column) const;
    board getBoardArray() const;
    matrice getColourScheme() const;  
    cell getCell(const size_t row, const size_t column) const;
    //returns true if the digit is suitable
    bool checkConflicts(const size_t row, const size_t column) const;
    bool checkCompletion();
    constexpr bool isSolved() const { return this->solved; }

private:

    bool solved;
    board vals_;    
    matrice colourScheme;


};

#endif // !GAMEBOARD___H
