#ifndef GAMEBOARD___H

#define WIN32_LEAN_AND_MEAN

#include <stdint.h>
#include <Windows.h>


class Gameboard
{

public:
    
    Gameboard();  
    explicit Gameboard(const uint16_t val[81]);

    uint16_t getElement(const size_t row, const size_t column) const;
    uint16_t* getValArray() const;
    void writeElement(const size_t row, const size_t column, const uint16_t val);
    
    bool checkCompletion() const;

private:

    uint16_t values[81];
    


};

#endif // !GAMEBOARD___H
