#pragma once
#include <vector>
#include "Field.h"

#define LEFT_SHOOT_TACTIC 1
#define RIGHT_SHOOT_TACTIC 2
#define UP_SHOOT_TACTIC 3
#define BOTTOM_SHOOT_TACTIC 4

class Field;

class Enemi
{
	std::vector<Cell*> cells;
	Field* field;

	std::vector<Cell*> hits;
	int currentHit;
	int enemiShipsCount;

	std::vector<int> tactics;
	int currentTactic;

	void RemoveByCell(Cell& cell);
	void RemoveByCoord(Coord coord);

	void Remove(Cell*& cell);

	void ClearHits();
	void ClearVectors();

	void ResetTactitcs();
	void RemoveTactic();
	void RemoveTactic(int tactic);

	void RemoveHorizontalTactics();
	void RemoveVerticalTactics();

	bool IsCellExist(Cell cell);

	Cell* AIHit();
	void Calibration();
	void Miss(Cell*& cell);
public:
	Enemi();
	Enemi(Field* field);

	void Shoot(CDC*& pDC);
};

