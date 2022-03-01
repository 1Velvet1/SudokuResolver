#include "BacktrackResolver.h"
#include "graphics.h"
#include <thread>
#include <wchar.h>
#include <string>

BacktrackResolver::BacktrackResolver(Gameboard& sudoku): isFilled(false), waitAmount(0), sBoard(sudoku)
{
}

void BacktrackResolver::backtrack() {

    size_t sRow = 0;
    size_t sCol = 0;
    this->isFilled = false;

    if (!this->sBoard.checkCompletion()) { MessageBox(NULL, L"The entered sudoku is incorrect!", L"Sudoku Resolver", MB_OK); return; }

    const auto start = std::chrono::high_resolution_clock::now();

    this->resolve(sRow, sCol);

    const auto execTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

    std::wstring msg = L"execution time: " + std::to_wstring(static_cast<size_t>(execTime.count())) + L" ms.";

    if (SDL::isCreated) { MessageBox(NULL, msg.c_str(), L"Sudoku Resolver", MB_OK); }
    
    this->sBoard.checkCompletion();

}

void BacktrackResolver::resolve(const size_t row, const size_t col)
{

    if (!SDL::isCreated) { this->isFilled = true; return; }

    for (size_t i = 1; i < 10; i++) {

        this->sBoard.writeElement(row, col, i);
        this->sBoard.writeColour(row, col, 0xFF0000);

        std::this_thread::sleep_for(std::chrono::milliseconds(this->waitAmount));

        if (this->sBoard.checkConflicts(row, col)) {

            size_t nRow = 0;
            size_t nCol = 0;

            this->nextFreeCell(0, 0, nRow, nCol);

            if (row == 8 && col == 8) { this->isFilled = true; return; }

            this->sBoard.writeColour(row, col, 0x0000FF);

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


void BacktrackResolver::changeWaitPeriod(size_t period) {

    this->waitAmount = period;

}