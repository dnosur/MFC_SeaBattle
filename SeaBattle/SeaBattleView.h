
// SeaBattleView.h: интерфейс класса CSeaBattleView
//

#pragma once
#include <vector>

#include "Field.h"
#include "Enemi.h"

#define SHOOTED_COLOR RGB(224, 229, 130)
#define KILL_COLOR RGB(188, 229, 130)
#define MISS_COLOR RGB(172, 172, 162)

#define WIN_COLOR RGB(163, 213, 152)
#define LOSE_COLOR RGB(213, 152, 158)

#define SHOOTED_TEXT L"Попадание!"
#define MISS_TEXT L"Промах"
#define KILL_TEXT L"Уничтожен!"

class CSeaBattleView : public CView
{
protected: // создать только из сериализации
	CSeaBattleView() noexcept;
	DECLARE_DYNCREATE(CSeaBattleView)

// Атрибуты
private:
	void MoveShip(CDC*& pDC, Cell cell);
	void ResetShips();

	void StartLocationStage();
	void StartGame();
	void GameEnd();

	void PrintText(CDC*& pDC, const CString text, int fontSize, COLORREF color = RGB(255, 255, 255), Coord coord = Coord(630, 10));
public:
	CSeaBattleDoc* GetDocument() const;

	Field userField, enemiField;

	CRect clientRect;
	Cell* prevCell;

	Enemi enemi;

	bool isShipSelected;
	bool isHorizontal;
	bool isCanSet;

	bool isGameStart;
	bool isGameEnd;
	bool isWin;

	int selectedShipSize;
	int ships[4];
	Ship selectedShip;
// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Реализация
public:
	virtual ~CSeaBattleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnStartGame();
	afx_msg void OnEndGame();
	afx_msg void OnResetShips();
	afx_msg void OnGenerateShips();
	afx_msg void OnUpdateEndGame(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStartGame(CCmdUI* pCmdUI);
	afx_msg void OnUpdateResetShips(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGenerateShips(CCmdUI* pCmdUI);
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // версия отладки в SeaBattleView.cpp
inline CSeaBattleDoc* CSeaBattleView::GetDocument() const
   { return reinterpret_cast<CSeaBattleDoc*>(m_pDocument); }
#endif

