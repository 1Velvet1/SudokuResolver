#include "gameboard.h"

Gameboard::Gameboard() {

    for (size_t i = 0; i < 10; i++) {

        for (size_t j = 0; j < 10; j++) {

            this->vals_[i][j] = 0;

        }

    }


}

Gameboard::Gameboard(const board& vals) {

    this->vals_ = vals;

}

