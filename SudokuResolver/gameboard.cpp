#include "gameboard.h"

Gameboard::Gameboard() {

    for (size_t i = 0; i < 81; i++) { values[i] = 0; }


}

Gameboard::Gameboard(const uint16_t val[81]) {

    if (val != nullptr) {

        memcpy(values, val, 81 * sizeof(uint16_t));

    }
    else {

        *this = Gameboard();

    }

}

