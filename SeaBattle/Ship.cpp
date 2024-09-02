#include "pch.h"
#include "Ship.h"

void Ship::PushDeck(Deck deck)
{
	if (decks.size() + 1 >= size) {
		return;
	}

	decks.push_back(deck);
}

void Ship::Shooted(Deck& deck)
{
	deck.shooted = true;
	livingDecksCount--;

	if (livingDecksCount <= 0) {
		dead = true;
	}
}

Deck* Ship::GetDeck(Coord coord)
{
	for (Deck& deck : decks) {
		if (coord == deck.cell.coord) {
			return &deck;
		}
	}
	return nullptr;
}

Deck* Ship::GetDeck(CRect& rect)
{
	for (Deck& deck : decks) {
		if (rect == deck.cell.rect) {
			return &deck;
		}
	}
	return nullptr;
}

Ship::Ship() : size(0), dead(true), livingDecksCount(0) {}

Ship::Ship(int size)
{
	if (size >= 5) {
		throw "Deck size error!";
	}

	this->size = size;
	dead = false;
	livingDecksCount = size;

	decks.resize(size);
}

Ship::Ship(int size, std::vector<Cell> cells)
{
	if (size >= 5) {
		throw "Deck size error!";
	}

	this->size = size;
	dead = false;
	livingDecksCount = size;

	for (Cell& cell : cells) {
		PushDeck(Deck(cell));
	}
}


int Ship::GetSize()
{
	return size;
}

int Ship::GetLivingDecksCount()
{
	return livingDecksCount;
}

void Ship::Draw(CDC*& pDC, bool hide)
{
	for (Deck& deck : decks) {
		pDC->FillSolidRect(deck.cell.rect, deck.shooted || deck.cell.shooted 
			? SHOOTED_DECK_COLOR 
			: !hide 
				? USER_SHIP_COLOR
				: BASE_FIELD_COLOR
		);
	}
}

bool Ship::IsDead()
{
	return dead;
}

bool Ship::IsShooted(Cell& cell)
{
	for (Deck& deck : decks) {
		if (cell.coord == deck.cell.coord) {
			Shooted(deck);
			return true;
		}
	}
	return false;
}

bool Ship::IsShooted(CRect& rect)
{
	for (Deck& deck : decks) {
		if (rect == deck.cell.rect) {
			Shooted(deck);
			return true;
		}
	}
	return false;
}

bool Ship::IsShooted(Coord coord)
{
	for (Deck& deck : decks) {
		if (coord == deck.cell.coord) {
			Shooted(deck);
			return true;
		}
	}
	return false;
}

bool Ship::IsDeck(Coord coord)
{
	for (Deck& deck : decks) {
		if (deck.cell.coord == coord) {
			return true;
		}
	}
	return false;
}

bool Ship::IsDeck(CRect& rect)
{
	for (Deck& deck : decks) {
		if (deck.cell.rect == rect) {
			return true;
		}
	}
	return false;
}

bool Ship::IsShootedDeck(Coord coord)
{
	if (!IsDeck(coord)) {
		return false;
	}

	Deck* deck = GetDeck(coord);

	if (deck == nullptr) {
		return false;
	}

	return deck->shooted;
}

bool Ship::IsShootedDeck(CRect& rect)
{
	if (!IsDeck(rect)) {
		return false;
	}

	Deck* deck = GetDeck(rect);

	if (deck == nullptr) {
		return false;
	}

	return deck->shooted;
}

void Ship::ClearDeck()
{
	for (Deck& deck : decks) {
		deck.shooted = false;
		deck.cell = Cell();
	}
}


Deck& Ship::operator[](const int index)
{
	if (index < 0 || index > GetSize()) {
		throw "Ship index error!";
	}
	return decks[index];
}
