#pragma once
#include <Windows.h>
#include <atltypes.h>

#include "Coord.h"

struct Cell {
	Cell() {
		letter = _T('A');
		number = -1;
        shooted = false;
	}

	Cell(TCHAR letter, int number, Coord coord, CRect rect)
		: letter(letter), number(number), coord(coord), rect(rect), shooted(false) {}

	TCHAR letter;
	int number;

	Coord coord;
	CRect rect;

    bool shooted;

    bool operator==(const Cell& other) const {
        return (letter == other.letter) &&
            (number == other.number) &&
            (coord == other.coord) &&
            (rect == other.rect) &&
            (shooted == other.shooted);
    }

    bool operator!=(const Cell& other) const {
        return !(*this == other);
    }

    Cell& operator=(const Cell& other) {
        if (this != &other) {
            letter = other.letter;
            number = other.number;
            coord = other.coord;
            rect = other.rect;
            shooted = other.shooted;
        }
        return *this;
    }
};