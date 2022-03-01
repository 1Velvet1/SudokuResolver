#include "gameboard.h"
#include <unordered_set>
#include <string>



Gameboard::Gameboard():solved(false)
{

    this->vals_.resize(9);
    this->colourScheme.resize(9);

    for (size_t i = 0; i < 9; i++) {

        this->vals_[i].resize(9);
        this->colourScheme[i].resize(9);

        for (size_t j = 0; j < 9; j++) {

            this->vals_[i][j] = 0;
            this->colourScheme[i][j] = 0;

        }

    }

}

Gameboard::Gameboard(const board& vals) {

    if (vals.size() == 9 && vals[0].size() == 9){

        this->vals_ = vals;

        for (size_t i = 0; i < 9; i++) {

            for (size_t j = 0; j < 9; j++) {

                this->colourScheme[i][j] = vals[i][j] == 0 ? 0 : 0x1;

            }

        }

    }
    else {

        *this = Gameboard();

    }

}

void Gameboard::writeElement(const size_t row, const size_t column, const uint16_t val)
{

    if (row < 9 && column < 9 && val < 10) { 
        
        this->vals_[row][column] = val; 
        this->colourScheme[row][column] = 0x1;
        this->solved = false;

    }

}

void Gameboard::writeColour(const size_t row, const size_t column, const RGBcolour colour)
{

    if (row < 9 && column < 9) {

        this->colourScheme[row][column] = colour;

    }

}

uint16_t Gameboard::getElement(const size_t row, const size_t column) const
{

    if (row < 9 && column < 9 ) { return this->vals_[row][column]; }
    else { return 10; }

}

board Gameboard::getBoardArray() const
{
    return this->vals_;
}

matrice Gameboard::getColourScheme() const
{

    return this->colourScheme;

}

cell Gameboard::getCell(const size_t row, const size_t column) const
{

    size_t startRow;
    size_t startColumn;

    switch (row) {

    case 0:
    case 1:
    case 2:

        startRow = 0;

        break;

    case 3:
    case 4:
    case 5:

        startRow = 3;

        break;

    case 6:
    case 7:
    case 8:

        startRow = 6;

        break;

    default:

        startRow = 10;

        break;


    }
    switch (column) {

    case 0:
    case 1:
    case 2:

        startColumn = 0;

        break;

    case 3:
    case 4:
    case 5:

        startColumn = 3;

        break;

    case 6:
    case 7:
    case 8:

        startColumn = 6;

        break;

    default:

        startColumn = 10;

        break;


    }
    
    if (startRow == 10 || startColumn == 10) { return cell(); }

    cell temp(9);
    size_t counter = 0;
    
    for (size_t i = startRow; i < startRow + 3; i++) {               

        for (size_t j = startColumn; j < startColumn + 3; j++) {

            temp[counter] = this->vals_[i][j];
            counter++;

        }

    }

    return temp;

}

bool Gameboard::checkConflicts(const size_t row, const size_t column) const
{

    const uint32_t current = this->vals_[row][column];

    for (size_t i = 0; i < 9; i++) {
                
        if ((i != column && current == this->vals_[row][i]) || (i != row && current == this->vals_[i][column])) { return false; }
        
    }

    cell temp = this->getCell(row, column);

    bool check = false;

    for (size_t i = 0; i < 9; i++) {

        if (temp[i] == current) {

            if (!check) {

                check = true;

            }
            else {

                return false;

            }

        }


    }

    return true;
}

bool Gameboard::checkCompletion()
{
    std::unordered_set<std::string> sudoku;

    for (size_t i = 0; i < 9; i++) {

        for (size_t j = 0; j < 9; j++) {

            std::string row = "r" + std::to_string(i) + "v" + std::to_string(this->vals_[i][j]);
            std::string col = "c" + std::to_string(j) + "v" + std::to_string(this->vals_[i][j]);
            std::string cell = "cl" + std::to_string(i / 3) + std::to_string(j / 3) + "v" + std::to_string(this->vals_[i][j]);

            if (sudoku.count(row) || sudoku.count(col) || sudoku.count(cell)) { 
                return false; 
            }

            sudoku.insert(row);
            sudoku.insert(col);
            sudoku.insert(cell);


        }


    }

    this->solved = true;

    return true;

}

bool Gameboard::checkValid() const
{
    std::unordered_set<std::string> sudoku;

    for (size_t i = 0; i < 9; i++) {

        for (size_t j = 0; j < 9; j++) {

            if (this->vals_[i][j] == 0) { continue; }

            std::string row = "r" + std::to_string(i) + "v" + std::to_string(this->vals_[i][j]);
            std::string col = "c" + std::to_string(j) + "v" + std::to_string(this->vals_[i][j]);
            std::string cell = "cl" + std::to_string(i / 3) + std::to_string(j / 3) + "v" + std::to_string(this->vals_[i][j]);

            if (sudoku.count(row) || sudoku.count(col) || sudoku.count(cell)) {
                return false;
            }

            sudoku.insert(row);
            sudoku.insert(col);
            sudoku.insert(cell);


        }


    }

    return true;

}