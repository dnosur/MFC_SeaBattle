
// SeaBattleView.cpp: реализация класса CSeaBattleView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "SeaBattle.h"
#endif

#include "SeaBattleDoc.h"
#include "SeaBattleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSeaBattleView

IMPLEMENT_DYNCREATE(CSeaBattleView, CView)

BEGIN_MESSAGE_MAP(CSeaBattleView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_START_GAME, &CSeaBattleView::OnStartGame)
	ON_COMMAND(ID_END_GAME, &CSeaBattleView::OnEndGame)
	ON_COMMAND(ID_RESET_SHIPS, &CSeaBattleView::OnResetShips)
	ON_COMMAND(ID_GENERATE_SHIPS, &CSeaBattleView::OnGenerateShips)
	ON_UPDATE_COMMAND_UI(ID_END_GAME, &CSeaBattleView::OnUpdateEndGame)
	ON_UPDATE_COMMAND_UI(ID_START_GAME, &CSeaBattleView::OnUpdateStartGame)
	ON_UPDATE_COMMAND_UI(ID_RESET_SHIPS, &CSeaBattleView::OnUpdateResetShips)
	ON_UPDATE_COMMAND_UI(ID_GENERATE_SHIPS, &CSeaBattleView::OnUpdateGenerateShips)
END_MESSAGE_MAP()

// Создание или уничтожение CSeaBattleView

CSeaBattleView::CSeaBattleView() noexcept
{
	if (!AllocConsole()) {
		AfxMessageBox(_T("Failed to create the console!"), MB_ICONEXCLAMATION);
	}
	StartLocationStage();
}

void CSeaBattleView::MoveShip(CDC*& pDC, Cell cell)
{
	isCanSet = true;
	selectedShip.ClearDeck();
	for (int i = 0; i < selectedShip.GetSize(); i++) {
		Deck deck = !i ? Deck(cell) : selectedShip[i - 1];

		if (i) {
			deck.cell.coord = isHorizontal
				? Coord(deck.cell.coord.X + 1, deck.cell.coord.Y)
				: Coord(deck.cell.coord.X, deck.cell.coord.Y + 1);

			if (deck.cell.coord.X >= 10 ||
				deck.cell.coord.Y >= 10) {
				isCanSet = false;
				break;
			}

			deck.cell.letter++;
			deck.cell.rect = userField[deck.cell.coord].rect;
			deck.cell.number++;
		}

		if (!userField.IsCanSetShip(deck.cell.coord)) {
			isCanSet = false;
			if (!i) {
				selectedShip[i] = deck;
			}
			break;
		}

		selectedShip[i] = deck;
	}

	userField.ShipHover(pDC, selectedShip, isCanSet);
	ReleaseDC(pDC);
}

void CSeaBattleView::ResetShips()
{
	ships[0] = 4;
	ships[1] = 3;
	ships[2] = 2;
	ships[3] = 1;
	selectedShipSize = 4;
}

void CSeaBattleView::StartLocationStage()
{
	isShipSelected = true;
	isHorizontal = true;
	isCanSet = false;

	isGameStart = false;
	isGameEnd = false;
	isWin = false;

	selectedShipSize = 4;
	selectedShip = Ship(selectedShipSize);

	userField = Field(50, 130);
	enemiField = Field(900, 130, true);

	enemi = Enemi(&userField);

	ResetShips();
}

void CSeaBattleView::StartGame()
{
	isGameStart = true;
	isGameEnd = false;
	isWin = false;

	isShipSelected = false;
	isCanSet = false;

	CDC* pDC = GetDC();
	enemiField.AutoSet(pDC);
	userField.UpdateShipsCount(pDC);
	enemiField.UpdateShipsCount(pDC);
	ReleaseDC(pDC);
}

void CSeaBattleView::GameEnd()
{
	isGameStart = true;
	isGameEnd = true;
	isWin = !enemiField.GetShipsCount();
	Invalidate();
}

void CSeaBattleView::PrintText(CDC*& pDC, const CString text, int fontSize, COLORREF color, Coord coord)
{
	// Определение прямоугольника, где будет нарисован текст
	CRect textRect(coord.X, coord.Y, coord.X + 300, coord.Y + 100);

	// Установка цвета фона
	COLORREF bgColor = RGB(255, 255, 255);
	pDC->FillSolidRect(&textRect, bgColor);

	// Установка цвета текста
	pDC->SetTextColor(color);

	// Установка фона текста на прозрачный
	pDC->SetBkMode(TRANSPARENT);

	// Создание шрифта
	CFont font;
	font.CreatePointFont(fontSize * 10, _T("Arial")); // Умножаем на 10, чтобы соответствовать логическим единицам точки

	// Сохранение старого шрифта и установка нового шрифта в контекст устройства
	CFont* pOldFont = pDC->SelectObject(&font);

	// Рисование текста
	pDC->TextOut(coord.X, coord.Y, text);

	// Восстановление старого шрифта
	pDC->SelectObject(pOldFont);
}


CSeaBattleView::~CSeaBattleView()
{
}

BOOL CSeaBattleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CSeaBattleView

void CSeaBattleView::OnDraw(CDC* pDC)
{
	CSeaBattleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (isGameStart && isGameEnd) {
		PrintText(pDC,
			isWin
				? L"Перемога!"
				: L"Поражение :(",
			100,
			isWin
				? WIN_COLOR
				: LOSE_COLOR,
			Coord(300, 120)
		);

		PrintText(pDC, 
			L"Нажмите любую кнопку чтобы начать заново", 
			40, 
			isWin
				? WIN_COLOR
				: LOSE_COLOR, 
			Coord(200, 300)
		);
		isGameEnd = true;
		isGameStart = false;
		return;
	}

	GetClientRect(&clientRect);
	userField.Draw(pDC, clientRect);
	enemiField.Draw(pDC, clientRect);
}


// Печать CSeaBattleView

BOOL CSeaBattleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CSeaBattleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CSeaBattleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}


// Диагностика CSeaBattleView

#ifdef _DEBUG
void CSeaBattleView::AssertValid() const
{
	CView::AssertValid();
}

void CSeaBattleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSeaBattleDoc* CSeaBattleView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSeaBattleDoc)));
	return (CSeaBattleDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CSeaBattleView


void CSeaBattleView::OnMouseMove(UINT nFlags, CPoint point)
{
	if ((!isGameStart &&
		userField.GetShipsCount() == SHIPS_COUNT) ||
		isGameStart && isGameEnd) {
		return;
	}

	CDC* pDC = GetDC();
	Cell* cell = isGameStart 
		? enemiField[point] 
		: userField[point];

	if (cell == prevCell) {
		return;
	}
	prevCell = cell;

	if (isGameStart) {
		enemiField.MouseHover(pDC, point);
		return;
	}

	if (!isShipSelected || cell == nullptr) {
		userField.MouseHover(pDC, point);
		return;
	}

	MoveShip(pDC, *cell);
	ReleaseDC(pDC);

	CView::OnMouseMove(nFlags, point);
}


void CSeaBattleView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	if (isGameStart || isGameEnd) {
		StartLocationStage();
		Invalidate();
		return;
	}

	if (nChar == 'R' || nChar == 'r') {
		CDC* pDC = GetDC();
		isHorizontal = !isHorizontal;

		MoveShip(pDC, selectedShip[0].cell);
		ReleaseDC(pDC);
		return;
	}

	if (nChar == 'F' || nChar == 'f') {
		CDC* pDC = GetDC();
		userField.AutoSet(pDC);
		ReleaseDC(pDC);
		return;
	}


	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CSeaBattleView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if ((isShipSelected && 
		!isCanSet && 
		!isGameStart) ||
		isGameStart && isGameEnd) {
		return;
	}

	CDC* pDC = GetDC();

	int userShipsCount = userField.GetShipsCount();
	int enemiShipsCount = enemiField.GetShipsCount();

	if (isGameStart && !isGameEnd) {
		if (enemiField[point]->shooted) {
			return;
		}

		bool isHit = false;
		enemiField.ShootToCell(pDC, *enemiField[point], isHit);


		PrintText(
			pDC, 
			!isHit 
				? MISS_TEXT
				: SHOOTED_TEXT, 
			40, 
			!isHit 
				? MISS_COLOR
				: SHOOTED_COLOR
		);

		if (!isHit) {
			Sleep(1000);
			enemi.Shoot(pDC);
		}

		if (userField.GetShipsCount() <= 0 ||
			enemiField.GetShipsCount() <= 0) {
			GameEnd();
			return;
		}

		if (userShipsCount > userField.GetShipsCount()) {
			userField.UpdateShipsCount(pDC);
		}

		if (enemiShipsCount > enemiField.GetShipsCount()) {
			PrintText(
				pDC,
				KILL_TEXT,
				40,
				KILL_COLOR
			);
			enemiField.UpdateShipsCount(pDC);
		}
		
		ReleaseDC(pDC);
		return;
	}

	userField.SetShip(pDC, selectedShip);
	ReleaseDC(pDC);

	ships[selectedShipSize-1]--;
	if (ships[selectedShipSize-1] <= 0) {
		selectedShipSize--;
	}

	if (selectedShipSize <= 0) {
		return;
	}

	selectedShip.ClearDeck();
	selectedShip = Ship(selectedShipSize);

	CView::OnLButtonDblClk(nFlags, point);
}


void CSeaBattleView::OnStartGame()
{
	if (isGameStart) {
		return;
	}

	StartGame();
}


void CSeaBattleView::OnEndGame()
{
	GameEnd();
}


void CSeaBattleView::OnResetShips()
{
	ResetShips();
	CDC* pDC = GetDC();
	userField.ClearShips(pDC);
	ReleaseDC(pDC);
	selectedShip = Ship(selectedShipSize);
}


void CSeaBattleView::OnGenerateShips()
{
	CDC* pDC = GetDC();
	userField.AutoSet(pDC);
	ReleaseDC(pDC);
}


void CSeaBattleView::OnUpdateEndGame(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(isGameStart);
}


void CSeaBattleView::OnUpdateStartGame(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!isGameStart && userField.GetShipsCount() == SHIPS_COUNT);
}


void CSeaBattleView::OnUpdateResetShips(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(userField.GetShipsCount() && !isGameStart);
}


void CSeaBattleView::OnUpdateGenerateShips(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!isGameStart);
}


void CSeaBattleView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}
