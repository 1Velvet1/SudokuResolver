#ifndef GAMEBOARD___H

#define WIN32_LEAN_AND_MEAN

#include <stdint.h>
#include <Windows.h>
#include <vector>

using board = std::vector<std::vector<uint32_t>>;

class Gameboard
{

public:
    
    Gameboard();  
    explicit Gameboard(const board& vals);

    uint16_t getElement(const size_t row, const size_t column) const;
    uint16_t* getValArray() const;
    void writeElement(const size_t row, const size_t column, const uint16_t val);
    
    bool checkCompletion() const;

private:

    board vals_;    


};

#endif // !GAMEBOARD___H
