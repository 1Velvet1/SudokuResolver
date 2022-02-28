#include "BacktrackResolver.h"
#include <thread>
#include <iostream>

BacktrackResolver::BacktrackResolver(Gameboard& sudoku):sBoard(sudoku), isFilled(false)
{
}

void BacktrackResolver::backtrack() {

    size_t sRow = 0;
    size_t sCol = 0;

    this->resolve(sRow, sCol);


    if (!this->sBoard.checkCompletion()) { std::cout << "I'm a big dumbass!\n"; }
    else { std::cout << "YEAH!!!!!\nI'm cool!\n"; }

}

void BacktrackResolver::resolve(const size_t row, const size_t col)
{

    for (size_t i = 1; i < 10; i++) {

        this->sBoard.writeElement(row, col, i);

        if (this->sBoard.checkConflicts(row, col)) {

            size_t nRow = 0;
            size_t nCol = 0;

            this->nextFreeCell(0, 0, nRow, nCol);

            if (row == 8 && col == 8) { this->isFilled = true; return; }

            std::this_thread::sleep_for(std::chrono::milliseconds(140));
            this->resolve(nRow, nCol);

            if (this->isFilled) { 
                return; }

        }
        else if (i == 9) {

            this->sBoard.writeElement(row, col, 0);

            return;

        }

    }

}

void BacktrackResolver::nextFreeCell(const size_t currRow, const size_t currCol, size_t& row, size_t& col) const
{

    for (size_t i = currRow; i < 9; i++) {

        for (size_t j = currCol; j < 9; j++) {

            if (this->sBoard.getElement(i, j) == 0) { 
                
                row = i;
                col = j;

                return;

            }

        }

    }
}
