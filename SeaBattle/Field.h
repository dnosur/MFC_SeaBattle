#pragma once

#include "Ship.h"
#include "ColorRefs.h"
#include "Enemi.h"

#define FIELD_SIZE_X 10
#define FIELD_SIZE_Y 10

#define SHIPS_COUNT 10

class Field
{
	Cell field[FIELD_SIZE_X][FIELD_SIZE_Y];
	Ship ships[SHIPS_COUNT];

	CString shipsCountText;

	int left, right, top, bottom;

	int shipsCount;

	const int fieldSize = 500;
	const int cellSize = fieldSize / 10;

	const int padding = 10;

	bool useSmoke;

	Cell currentCell;

	Ship* GetShip(Coord coord);
	Ship* GetShip(CRect& rect);

	void ResetPrevRect(CDC*& pDC);
	void ResetFields(CDC*& pDC);
public:
	Field();
	Field(const int X, const int Y, bool useSmoke = false);
	Field(const int left, const int right, const int top, const int bottom, bool useSmoke = false);

	Field(const Field& other);
	Field& operator=(const Field& other); 

	~Field();

	void Draw(CDC*& pDC, CRect& clientRect);
	void DrawShips(CDC* pDC);

	void UpdateShipsCount(CDC*& pDC);

	void AutoSet(CDC*& pDC);
	void SetShip(CDC*& pDC, Ship& ship);

	void ShootToCell(CDC*& pDC, Cell& cell, bool& isHitShip);

	int GetShipsCount();

	void ClearShips(CDC*& pDC);

	bool IsShip(Coord coord);
	bool IsShip(CRect& rect);

	bool IsCanSetShip(Coord coord);

	bool IsShootedField(Coord coord);
	bool IsShootedField(CRect& rect);

	void ShipHover(CDC*& pDC, Ship& ship, bool canSet = true);
	void MouseHover(CDC*& pDC, CPoint& point);

	Cell& operator[](const Coord& coord);
	Cell* operator[](const CPoint& point);
	Cell& operator[](const CRect& rect);
};
