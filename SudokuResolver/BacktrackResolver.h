#ifndef BACKTRACK___RESOLVER

#include "defines.h"
#include "gameboard.h"

class BacktrackResolver
{

public:
       
    BacktrackResolver(Gameboard& sudoku);

    void backtrack();

    void multiBacktrack();

private:
    

    void resolve(const size_t row, const size_t col);
    void nextFreeCell(const size_t currRow, const size_t currCol, size_t& row, size_t& col) const;
    
private:
    
    bool isFilled;
    Gameboard& sBoard;


};


#endif // !BACKTRACK___RESOLVER