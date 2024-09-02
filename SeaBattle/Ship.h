#pragma once
#include <vector>

#include "Deck.h"
#include "ColorRefs.h"

class Ship
{
	int size;
	std::vector<Deck> decks;

	bool dead;
	int livingDecksCount;

	void PushDeck(Deck deck);

	void Shooted(Deck& deck);

	Deck* GetDeck(Coord coord);
	Deck* GetDeck(CRect& rect);
public:
	Ship();
	Ship(int size);
	Ship(int size, std::vector<Cell> cells);

	int GetSize();
	int GetLivingDecksCount();
	
	void Draw(CDC*& pDC, bool hide = false);

	bool IsDead();

	bool IsShooted(Cell& cell);
	bool IsShooted(CRect& rect);
	bool IsShooted(Coord coord);

	bool IsDeck(Coord coord);
	bool IsDeck(CRect& rect);

	bool IsShootedDeck(Coord coord);
	bool IsShootedDeck(CRect& rect);

	void ClearDeck();

	Deck& operator[](const int index);
};