#include "pch.h"
#include "Enemi.h"

void Enemi::RemoveByCell(Cell& cell)
{
	for (Cell*& item : cells) {
		if (item->coord == cell.coord) {
			Remove(item);
			return;
		}
	}
}

void Enemi::RemoveByCoord(Coord coord)
{
	if (coord.X < 0 || coord.Y < 0 ||
		coord.X >= FIELD_SIZE_X || coord.Y >= FIELD_SIZE_Y) {
		return;
	}

	for (Cell*& item : cells) {
		if (item->coord == coord) {
			Remove(item);
			return;
		}
	}
}

void Enemi::Remove(Cell*& cell)
{
	try {
		_cwprintf(L"Remove from cells X %i Y %i-é\n", cell->coord.X, cell->coord.Y);
		cells.erase(std::remove(cells.begin(), cells.end(), cell), cells.end());
		return;
	}
	catch (std::exception ex) {
		return;
	}
}

void Enemi::ClearHits()
{
	currentHit = 0;
	if (!hits.size()) {
		return;
	}
	hits.clear();
}

void Enemi::ClearVectors()
{
	if (cells.size()) {
		cells.clear();
	}
	ClearHits();
}

void Enemi::ResetTactitcs()
{
	currentTactic = 0;
	if (tactics.size()) {
		tactics.clear();
	}

	for (int i = 1; i <= 4; i++) {
		tactics.push_back(i);
	}
}

void Enemi::RemoveTactic()
{
	RemoveTactic(currentTactic);
	currentTactic = 0;
}

void Enemi::RemoveTactic(int tactic)
{
	if (tactic < 1) {
		return;
	}

	for (int& item : tactics) {
		if (item == tactic) {
			tactics.erase(std::remove(tactics.begin(), tactics.end(), item), tactics.end());
		}
	}
}

void Enemi::RemoveHorizontalTactics()
{
	if (!tactics.size()) {
		return;
	}

	RemoveTactic(LEFT_SHOOT_TACTIC);
	RemoveTactic(RIGHT_SHOOT_TACTIC);
}

void Enemi::RemoveVerticalTactics()
{
	if (!tactics.size()) {
		return;
	}

	RemoveTactic(BOTTOM_SHOOT_TACTIC);
	RemoveTactic(UP_SHOOT_TACTIC);
}

bool Enemi::IsCellExist(Cell cell)
{
	for (Cell*& item : cells) {
		if (item->coord == cell.coord) {
			return true;
		}
	}
	return false;
}

Cell* Enemi::AIHit()
{
	if (!hits.size()) {
		return cells[rand() % cells.size()];
	}

	bool change = false;

	if (hits[currentHit]->coord.X + 1 >= FIELD_SIZE_X) {
		if (currentTactic == RIGHT_SHOOT_TACTIC) {
			change = true;
		}
		RemoveTactic(RIGHT_SHOOT_TACTIC);
	}

	if (hits[currentHit]->coord.Y + 1 >= FIELD_SIZE_Y) {
		if (currentTactic == BOTTOM_SHOOT_TACTIC) {
			change = true;
		}
		RemoveTactic(BOTTOM_SHOOT_TACTIC);
	}

	if (hits[currentHit]->coord.X - 1 < 0) {
		if (currentTactic == LEFT_SHOOT_TACTIC) {
			change = true;
		}
		RemoveTactic(LEFT_SHOOT_TACTIC);
	}

	if (hits[currentHit]->coord.Y - 1 < 0) {
		if (currentTactic == UP_SHOOT_TACTIC) {
			change = true;
		}
		RemoveTactic(UP_SHOOT_TACTIC);
	}

	if (!tactics.size()) {
		return cells[rand() % cells.size()];
	}

	currentTactic = change || currentTactic <= 0
		? tactics[rand() % tactics.size()]
		: currentTactic;

	Coord coord = hits[currentHit]->coord;

	if (currentTactic == LEFT_SHOOT_TACTIC) {
		coord.X -= 1;
		return &(*field)[coord];
	}

	if (currentTactic == RIGHT_SHOOT_TACTIC) {
		coord.X += 1;
		return &(*field)[coord];
	}

	if (currentTactic == UP_SHOOT_TACTIC) {
		coord.Y -= 1;
		return &(*field)[coord];
	}

	if (currentTactic == BOTTOM_SHOOT_TACTIC) {
		coord.Y += 1;
		return &(*field)[coord];
	}


	return cells[rand() % cells.size()];
}

void Enemi::Calibration()
{
	for (Cell*& hit : hits) {
		RemoveByCell(*hit);

		RemoveByCoord(Coord(hit->coord.X + 1, hit->coord.Y));
		RemoveByCoord(Coord(hit->coord.X - 1, hit->coord.Y));

		RemoveByCoord(Coord(hit->coord.X, hit->coord.Y + 1));
		RemoveByCoord(Coord(hit->coord.X, hit->coord.Y - 1));

		RemoveByCoord(Coord(hit->coord.X + 1, hit->coord.Y + 1));
		RemoveByCoord(Coord(hit->coord.X - 1, hit->coord.Y - 1));

		RemoveByCoord(Coord(hit->coord.X - 1, hit->coord.Y + 1));
		RemoveByCoord(Coord(hit->coord.X + 1, hit->coord.Y - 1));
	}
}

void Enemi::Miss(Cell*& cell)
{
	currentHit = 0;
	RemoveTactic();
	RemoveByCell(*cell);
}

Enemi::Enemi()
{
	field = nullptr;
	currentHit = currentTactic = enemiShipsCount = 0;
}

Enemi::Enemi(Field* field)
{
	this->field = field;
	enemiShipsCount = SHIPS_COUNT;
	currentHit = currentTactic = 0;
	for (int i = 0; i < FIELD_SIZE_X; i++) {
		for (int j = 0; j < FIELD_SIZE_Y; j++) {
			Coord coord = Coord(j, i);
			cells.push_back(&(*this->field)[coord]);
		}
	}
	ResetTactitcs();
}

void Enemi::Shoot(CDC*& pDC)
{
	bool horizontal = rand() % 2;

	bool isHit = false;

	while (true) {
		Cell* cell = AIHit();

		if (field->IsShootedField(cell->coord)) {
			Miss(cell);
			continue;
		}

		field->ShootToCell(pDC, *cell, isHit);
		_cwprintf(L"Enemi shoot X: %i Y: %i Shooted %i\n", cell->coord.X, cell->coord.Y, isHit);

		if (!isHit) {
			Miss(cell);
			return;
		}

		hits.push_back(cell);
		currentHit = hits.size() - 1;

		if (field->GetShipsCount() <= 0) {
			ClearVectors();
			ResetTactitcs();
			return;
		}

		if (field->GetShipsCount() < enemiShipsCount) {
			Calibration();
			ClearHits();
			ResetTactitcs();
			enemiShipsCount = field->GetShipsCount();
			continue;
		}

		if (currentTactic == LEFT_SHOOT_TACTIC ||
			currentTactic == RIGHT_SHOOT_TACTIC) {
			RemoveVerticalTactics();
		}

		if (currentTactic == UP_SHOOT_TACTIC ||
			currentTactic == BOTTOM_SHOOT_TACTIC) {
			RemoveHorizontalTactics();
		}
	}
}
