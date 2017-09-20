
//TemplateButton.h


#ifndef _TEMPLATEBUTTON_H
#define _TEMPLATEBUTTON_H


#include"ButtonState.h"
class Selection;
typedef signed long int Long;
class Figure;


class TemplateButton : public ButtonState {
public:

	TemplateButton();
	TemplateButton(const TemplateButton& source);
	virtual ~TemplateButton();

	virtual void ChangeState(DrawingController *drawingController, UINT nChar);
	virtual Figure* AddToArray(Diagram *diagram, Selection *selection, Long startX, Long startY, Long currentX, Long currentY);
	virtual void Draw(Selection *selection, Long startX, Long startY, Long currentX, Long currentY, CDC *cPaintDc);

	TemplateButton& operator=(const TemplateButton& source);
};


#endif //!_TEMPLATEBUTTON_H