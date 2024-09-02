#pragma once
#include "Cell.h"

struct Deck {
	Deck(): shooted(false){}
	Deck(Cell cell, bool shooted = false): cell(cell), shooted(shooted){}

	Cell cell;
	bool shooted;
};