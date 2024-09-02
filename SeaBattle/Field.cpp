#include "pch.h"
#include "Field.h"

Ship* Field::GetShip(Coord coord) {
	for (int i = 0; i < SHIPS_COUNT; i++) {
		if (!ships[i].IsDeck(coord)) {
			continue;
		}
		return &ships[i];
	}

	return nullptr;
}

Ship* Field::GetShip(CRect& rect) {
	for (int i = 0; i < SHIPS_COUNT; i++) {
		if (!ships[i].IsDeck(rect)) {
			continue;
		}
		return &ships[i];
	}

	return nullptr;
}

Field::Field()
{
	left = right = top = bottom = 10;
	shipsCount = 0;

	useSmoke = false;
}

Field::Field(const int X, const int Y, bool useSmoke)
{
	left = X;
	right = X;

	top = Y;
	bottom = Y;

	shipsCount = 0;

	this->useSmoke = useSmoke;

}

Field::Field(const int left, const int right, const int top, const int bottom, bool useSmoke):
	left(left), right(right), top(top), bottom(bottom), shipsCount(0), useSmoke(useSmoke)
{

}

void Field::ClearShips(CDC*& pDC)
{
	if (!shipsCount) {
		return;
	}

	for (int i = 0; i < SHIPS_COUNT; i++) {
		for (int j = 0; j < ships[i].GetSize(); j++) {
			pDC->FillSolidRect(ships[i][j].cell.rect, BASE_FIELD_COLOR);
		}
		ships[i].ClearDeck();
	}
	shipsCount = 0;
}

void Field::ResetFields(CDC*& pDC)
{
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			Coord coord(j, i);

			if (IsShip(field[i][j].rect) || IsShip(coord)) {
				pDC->FillSolidRect(field[i][j].rect, GetShip(coord)->IsShootedDeck(coord) 
					? SHOOTED_DECK_COLOR 
					: !useSmoke 
						? USER_SHIP_COLOR 
						: BASE_FIELD_COLOR
				);
				continue;
			}

			pDC->FillSolidRect(field[i][j].rect, field[i][j].shooted ? SHOOTED_FIELD_COLOR : BASE_FIELD_COLOR);
		}
	}
}

bool Field::IsShip(Coord coord)
{
	for (int i = 0; i < SHIPS_COUNT; i++) {
		if (ships[i].IsDeck(coord)) {
			return true;
		}
		
	}
	return false;
}

void Field::ResetPrevRect(CDC*& pDC)
{
	if (!currentCell.rect) {
		return;
	}

	if (IsShip(currentCell.rect) || IsShip(currentCell.rect)) {
		pDC->FillSolidRect(currentCell.rect, GetShip(currentCell.coord)->IsShootedDeck(currentCell.coord) 
			? SHOOTED_DECK_COLOR
			: !useSmoke
				? USER_SHIP_COLOR
				: BASE_FIELD_COLOR
		);
		return;
	}


	pDC->FillSolidRect(currentCell.rect, IsShootedField(currentCell.coord) ? SHOOTED_FIELD_COLOR : BASE_FIELD_COLOR);
}

void Field::SetShip(CDC*& pDC, Ship& ship)
{
	if (shipsCount >= SHIPS_COUNT) {
		return;
	}

	ships[shipsCount] = ship;
	shipsCount++;

	ship.Draw(pDC, useSmoke);
}

void Field::AutoSet(CDC*& pDC)
{
	for (int i = 0; i < SHIPS_COUNT; i++) {
		ships[i].ClearDeck();
	}
	shipsCount = 0;
	ResetFields(pDC);

	auto removeCell = [](std::vector<Cell>& cells, Cell& cell) {
		for(Cell& item: cells) {
			if (item.coord == cell.coord) {
				try {
					cells.erase(std::remove(cells.begin(), cells.end(), item), cells.end());
					return;
				}
				catch (std::exception ex) {
					return;
				}
			}
		}
	};

	std::vector<Cell> cells;
	for (int i = 0; i < FIELD_SIZE_Y; i++) {
		for (int j = 0; j < FIELD_SIZE_X; j++) {
			cells.push_back(field[i][j]);
		}
	}


	bool isHorizontal = true;
	const int max_ship_size = 4;
	for (int i = 0; i < max_ship_size; i++) {
		const int max_ship_count = max_ship_size - i;
		const int current_ship_size = i + 1;

		Ship ship(current_ship_size);
		int setted_ships_count = 0;
		while (setted_ships_count != max_ship_count) {
			Cell head = cells[rand() % cells.size()];

			isHorizontal = true;
			bool isCanSet = true;
			for (int j = 0; j < current_ship_size;) {
				Cell temp = head;
				temp.coord = j
					? isHorizontal
						? Coord(temp.coord.X + j, temp.coord.Y)
						: Coord(temp.coord.X, temp.coord.Y + j)
					: temp.coord;

				if (!IsCanSetShip(temp.coord)) {
					if (!isHorizontal) {
						isCanSet = false;
						break;
					}

					j = 0;
					ship.ClearDeck();
					isHorizontal = false;
					continue;
				};


				temp.number += j ? isHorizontal : 0;
				temp.rect = (*this)[temp.coord].rect;
				temp.shooted = false;

				ship[j] = temp;
				ship[j].cell.rect = temp.rect;
				j++;

				//pDC->FillSolidRect(temp.rect, RGB(255, 0, 0));
			}

			if (!isCanSet) {
				removeCell(cells, head);
				ship.ClearDeck();
				continue;
			}

			for (int j = 0; j < current_ship_size; j++) {
				Cell& cell = ship[j].cell;
				removeCell(cells, cell);
			}

			SetShip(pDC, ship);
			setted_ships_count++;

			_cwprintf(L"%i Палубный %i-й\n", current_ship_size, setted_ships_count);
			for (int j = 0; j < ship.GetSize(); j++) {
				_cwprintf(L"X %i Y %i-й\n", ship[j].cell.coord.X, ship[j].cell.coord.Y);
			}
			_cwprintf(L"\n\n");
		}
	}

	//DrawShips(pDC);
}

void Field::DrawShips(CDC* pDC)
{
	for (int i = 0; i < SHIPS_COUNT; i++) {
		for (int j = 0; j < ships[i].GetSize(); j++) {
			pDC->FillSolidRect(ships[i][j].cell.rect, ships[i][j].shooted || ships[i][j].cell.shooted 
				? SHOOTED_DECK_COLOR 
				: !useSmoke 
					? USER_SHIP_COLOR
					: BASE_FIELD_COLOR
			);
		}
	}
}

Field::Field(const Field& other)
	: left(other.left), right(other.right), top(other.top), bottom(other.bottom),
	shipsCount(other.shipsCount),currentCell(other.currentCell), useSmoke(other.useSmoke)
{
	for (int i = 0; i < FIELD_SIZE_X; ++i) {
		for (int j = 0; j < FIELD_SIZE_Y; ++j) {
			field[i][j] = other.field[i][j];
		}
	}

	for (int i = 0; i < SHIPS_COUNT; ++i) {
		ships[i] = other.ships[i];
	}
}

Field& Field::operator=(const Field& other)
{
	if (this == &other) {
		return *this; // самоприсваивание
	}

	left = other.left;
	right = other.right;
	top = other.top;
	bottom = other.bottom;
	shipsCount = other.shipsCount;
	currentCell = other.currentCell;
	useSmoke = other.useSmoke;

	for (int i = 0; i < FIELD_SIZE_X; ++i) {
		for (int j = 0; j < FIELD_SIZE_Y; ++j) {
			field[i][j] = other.field[i][j];
		}
	}

	for (int i = 0; i < SHIPS_COUNT; ++i) {
		ships[i] = other.ships[i];
	}

	return *this;
}
bool Field::IsShootedField(Coord coord)
{
	return field[coord.Y][coord.X].shooted;
}

Field::~Field()
{
}

bool Field::IsCanSetShip(Coord coord)
{
	if (coord.X >= FIELD_SIZE_X || coord.Y >= FIELD_SIZE_Y ||
		coord.X < 0 || coord.Y < 0) {
		return false;
	}

	return !(IsShip(Coord(coord.X + 1, coord.Y)) || IsShip(Coord(coord.X - 1, coord.Y)) ||
		   IsShip(Coord(coord.X, coord.Y + 1)) || IsShip(Coord(coord.X, coord.Y - 1)) ||
		   IsShip(Coord(coord.X + 1, coord.Y + 1)) || IsShip(Coord(coord.X - 1, coord.Y - 1)) || 
		   IsShip(Coord(coord.X - 1, coord.Y + 1)) || IsShip(Coord(coord.X + 1, coord.Y - 1)));
}

void Field::ShootToCell(CDC*& pDC, Cell& cell, bool& isHitShip) {
	for (int i = 0; i < SHIPS_COUNT; i++) {
		if (!ships[i].IsShooted(cell)) {
			continue;
		}

		pDC->FillSolidRect(cell.rect, SHOOTED_DECK_COLOR);
		shipsCount -= ships[i].IsDead();
		isHitShip = true;
		cell.shooted = true;
		return;
	}

	isHitShip = false;
	cell.shooted = true;
	currentCell = cell;
	pDC->FillSolidRect(cell.rect, SHOOTED_FIELD_COLOR);
	return;
}

void Field::ShipHover(CDC*& pDC, Ship& ship, bool canSet)
{
	ResetFields(pDC);
	for (int i = 0; i < ship.GetSize(); i++) {
		if (ship[i].cell.number <= -1) {
			return;
		}

		pDC->FillSolidRect(ship[i].cell.rect, canSet ? USER_SHIP_COLOR : USER_UNSET_SHIP_COLOR);
	}
}

int Field::GetShipsCount()
{
	return shipsCount;
}

Cell& Field::operator[](const Coord& coord)
{
	if (coord.X >= FIELD_SIZE_X || coord.Y >= FIELD_SIZE_Y) {
		throw "Field coord excaption";
	}

	return field[coord.Y][coord.X];
}

Cell* Field::operator[](const CPoint& point)
{
	for (int i = 0; i < FIELD_SIZE_Y; i++) {
		for (int j = 0; j < FIELD_SIZE_X; j++) {
			if (field[i][j].rect.PtInRect(point)) {
				return &field[i][j];
			}
		}
	}
	return nullptr;
}

Cell& Field::operator[](const CRect& rect)
{
	for (int i = 0; i < FIELD_SIZE_Y; i++) {
		for (int j = 0; j < FIELD_SIZE_X; j++) {
			if (field[i][j].rect == rect) {
				return field[i][j];
			}
		}
	}
	throw "Get by rect exception";
}

void Field::UpdateShipsCount(CDC*& pDC) {
    shipsCountText.Format(_T("Живых кораблей: %d"), shipsCount);

    pDC->SetTextColor(RGB(0, 0, 0));
    pDC->SetBkColor(RGB(255, 255, 255));

    CRect textRect(left, top - cellSize - 80, left + 300, top - cellSize - 20); 

    pDC->FillSolidRect(&textRect, RGB(255, 255, 255));

    CFont font;
    font.CreatePointFont(200, _T("Arial"));
    CFont* pOldFont = pDC->SelectObject(&font);

    pDC->TextOut(left + 20, top - cellSize - 60, shipsCountText);

    pDC->SelectObject(pOldFont);
}

bool Field::IsShootedField(CRect& rect)
{
	return false;
}

bool Field::IsShip(CRect& rect)
{
	for (int i = 0; i < SHIPS_COUNT; i++) {
		if (ships[i].IsDeck(rect)) {
			return true;
		}

	}
	return false;
}

void Field::MouseHover(CDC*& pDC, CPoint& point)
{
	ResetPrevRect(pDC);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (!field[i][j].rect.PtInRect(point)) {
				continue;
			}

			pDC->FillSolidRect(field[i][j].rect, CURSOR_COLOR);

			currentCell = field[i][j];
			_cwprintf(L"X %i, Y %i Shooted %i\n", currentCell.coord.X, currentCell.coord.Y, currentCell.shooted);
			return;
		}
	}
}

void Field::Draw(CDC*& pDC, CRect& clientRect)
{
	pDC->SetBkColor(RGB(255, 255, 255));
	pDC->SetBkMode(OPAQUE); 

	for (int i = 0; i < 10; ++i) {
		CString digit;
		digit.Format(_T("%d"), i + 1);
		pDC->TextOut(left + padding + i * cellSize + cellSize / 2, top - cellSize / 2, digit);
	}

	TCHAR letter = _T('A');
	for (int i = 0; i < 10; ++i) {
		CString alpha;
		alpha.Format(_T("%c"), letter++);
		pDC->TextOut(left - cellSize / 2, top + padding + i * cellSize + cellSize / 2, alpha);
	}

	letter = _T('A');
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			Coord coord(j, i);
			if (field[i][j].number <= -1) {
				CRect cellRect(left + clientRect.left + j * cellSize + padding, top + clientRect.top + i * cellSize + padding,
					right + clientRect.left + (j + 1) * cellSize + padding, bottom + clientRect.top + (i + 1) * cellSize + padding);
				pDC->Rectangle(&cellRect);
				field[i][j] = Cell(letter, j, coord, cellRect);
			}
			else {
				pDC->Rectangle(&field[i][j].rect);
			}

			if (IsShip(field[i][j].rect) || IsShip(coord)) {
				pDC->FillSolidRect(field[i][j].rect, GetShip(coord)->IsShootedDeck(coord) 
					? SHOOTED_DECK_COLOR 
					: !useSmoke 
						? USER_SHIP_COLOR
						: BASE_FIELD_COLOR
				);
				continue;
			}

			if (field[i][j].shooted) {
				pDC->FillSolidRect(field[i][j].rect, SHOOTED_FIELD_COLOR);
				continue;
			}

			pDC->FillSolidRect(field[i][j].rect, BASE_FIELD_COLOR);
		}
		letter++;
	}
}

