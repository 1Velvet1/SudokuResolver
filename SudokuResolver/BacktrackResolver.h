#ifndef BACKTRACK___RESOLVER
#define BACKTRACK___RESOLVER

#include "defines.h"
#include "gameboard.h"

class BacktrackResolver
{

public:
       
    
    explicit BacktrackResolver(Gameboard& sudoku);

    void backtrack();

    void multiBacktrack();

    ///<summary>
    /// Change how long resolver waits between iterations
    /// </summary>
    /// <param name="period">time in milliseconds</param>
    void changeWaitPeriod(size_t period);

private:
    

    void resolve(const size_t row, const size_t col);
    void nextFreeCell(const size_t currRow, const size_t currCol, size_t& row, size_t& col) const;

private:
    
    bool isFilled;
    size_t waitAmount;
    Gameboard& sBoard;


};


#endif // !BACKTRACK___RESOLVER