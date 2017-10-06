#include "DrawingAggregations.h"
#include "DefaultState.h"
#include "Figure.h"
#include "Selection.h"
#include "Class.h"
#include "Finder.h"
#include "Aggregations.h"
#include "SelfAggregations.h"
#include "ClassDiagramForm.h"
#include "HistoryGraphic.h"

DrawingAggregations* DrawingAggregations::instance = 0;

MouseLButtonAction* DrawingAggregations::Instance() {
	if (instance == 0) {
		instance = new DrawingAggregations;
	}
	return instance;
}

void DrawingAggregations::MouseLButtonUp(MouseLButton *mouseLButton, ClassDiagramForm *classDiagramForm, Diagram *diagram, Selection *selection, Long  startX, Long startY, Long currentX, Long currentY) {
	Long index;
	Figure *figure = 0;

	//if (selection->GetLength() == 1 && dynamic_cast<Class*>(selection->GetAt(0))) {

	classDiagramForm->historyGraphic->PushUndo(diagram);
	selection->SelectByPoint(diagram, currentX, currentY);

	if (selection->GetLength() == 2 && selection->GetAt(0) != selection->GetAt(1) && dynamic_cast<Class*>(selection->GetAt(1))) {

		CPoint lineStart(startX, startY);
		CPoint lineEnd(currentX, currentY);

		CRect rect(selection->GetAt(0)->GetX(), selection->GetAt(0)->GetY(),
			selection->GetAt(0)->GetX() + selection->GetAt(0)->GetWidth(),
			selection->GetAt(0)->GetY() + selection->GetAt(0)->GetHeight());

		Finder finder;
		CPoint cross1 = finder.GetCrossPoint(lineStart, lineEnd, rect);

		rect.left = selection->GetAt(1)->GetX();
		rect.top = selection->GetAt(1)->GetY();
		rect.right = selection->GetAt(1)->GetX() + selection->GetAt(1)->GetWidth();
		rect.bottom = selection->GetAt(1)->GetY() + selection->GetAt(1)->GetHeight();
		CPoint cross2 = finder.GetCrossPoint(lineStart, lineEnd, rect);

		Aggregations object(cross1.x, cross1.y, cross2.x - cross1.x, cross2.y - cross1.y);
		index = static_cast<FigureComposite*>(selection->GetAt(0))->Add(object.Clone());
		figure = static_cast<FigureComposite*>(selection->GetAt(0))->GetAt(index);

	}

	if (selection->GetLength() == 2 && selection->GetAt(0) == selection->GetAt(1)) {
		Class *object = static_cast<Class*>(selection->GetAt(0));
		SelfAggregations selfAggregations(object->GetX() + object->GetWidth() - 30, object->GetY(), 30, 30);
		if(object->GetTempletePosition() != -1) {
			selfAggregations.Move(0, -17);
			Long k = 0;
			while (k < 5) {
				CPoint cPoint(selfAggregations.rollNamePoints->GetAt(k).x, selfAggregations.rollNamePoints->GetAt(k).y - 17);
				selfAggregations.rollNamePoints->Modify(k, cPoint);
				k++;
			}
		}
		index = object->Add(selfAggregations.Clone());
		figure = object->GetAt(index);
	}
	selection->DeleteAllItems();
	this->ChangeDefault(mouseLButton);
}
void DrawingAggregations::MouseLButtonDown(MouseLButton *mouseLButton, Diagram *diagram, Selection *selection, Long  startX, Long startY, Long currentX, Long currentY) {
	selection->DeleteAllItems();
	selection->SelectByPoint(diagram, currentX, currentY);
}

void DrawingAggregations::MouseLButtonDrag(MouseLButton *mouseLButton, Diagram *diagram, Selection *selection, Long  startX, Long startY, Long currentX, Long currentY, CDC *pDC) {
	if (startX == currentX&&startY == currentY) {
		selection->DeleteAllItems();
		selection->SelectByPoint(diagram, currentX, currentY);
	}
	pDC->MoveTo(startX, startY);
	pDC->LineTo(currentX, currentY);

	CBrush white(RGB(255, 255, 255));
	CBrush myBrush;
	myBrush.CreateSolidBrush(RGB(255, 255, 255));
	CBrush *oldBrush = pDC->SelectObject(&myBrush);

	double degree = atan2(currentX - startX, startY - currentY); // 기울기

	double distance = sqrt(pow(currentX - startX, 2) + pow(startY - currentY, 2));
	// 루트안에 = 루트(제곱(
	double dX = (currentX)-(10 * (currentX - startX) / distance); //뒤로 온 기준점 x
	double dY = (currentY)+(10 * (startY - currentY) / distance); //뒤로 온 기준점 y

																  // 수직 기울기

	CPoint pts[3];

	pts[0].x = (currentX); //마우스 현재위치 점
	pts[0].y = (currentY);

	pts[1].x = static_cast<LONG>(dX - 10 * cos(degree)); // 윗점
	pts[1].y = static_cast<LONG>(dY - 10 * sin(degree));

	pts[2].x = static_cast<LONG>(dX + 10 * cos(degree)); // 아랫점
	pts[2].y = static_cast<LONG>(dY + 10 * sin(degree));

	pDC->MoveTo(pts[0].x, pts[0].y);
	pDC->LineTo(pts[1].x, pts[1].y);

	pDC->MoveTo(pts[0].x, pts[0].y);
	pDC->LineTo(pts[2].x, pts[2].y);


	//여기까지 화살표 다음부터 마름모

	dX = (startX)+(9 * (currentX - startX) / distance); //뒤로 온 기준점 x
	dY = (startY)-(9 * (startY - currentY) / distance); //뒤로 온 기준점 y

	double dX2 = (startX)-((currentX - startX) / distance);
	double dY2 = (startY)+((startY - currentY) / distance);

	CPoint pts2[4];

	pts2[0].x = static_cast<LONG>(dX - 10 * cos(degree)); // 윗점
	pts2[0].y = static_cast<LONG>(dY - 10 * sin(degree));

	pts2[1].x = static_cast<LONG>(dX2); //마우스 처음 점
	pts2[1].y = static_cast<LONG>(dY2);

	pts2[2].x = static_cast<LONG>(dX + 10 * cos(degree)); // 아랫점
	pts2[2].y = static_cast<LONG>(dY + 10 * sin(degree));

	pts2[3].x = static_cast<LONG>(dX) + static_cast<LONG>(10 * (currentX - startX) / distance); // 윗점
	pts2[3].y = static_cast<LONG>(dY) - static_cast<LONG>(10 * (startY - currentY) / distance);

	pDC->SelectObject(&white);
	pDC->Polygon(pts2, 4);
	pDC->SelectObject(oldBrush);
	myBrush.DeleteObject();

}