//EditResizer.cpp

#include "EditResizer.h"
#include "EditResizerProcess.h"
#include "ClassDiagramForm.h"
#include "TextEdit.h"
#include "Text.h"
#include "Row.h"
#include "Figure.h"
#include "FigureComposite.h"
#include "Diagram.h"
#include "Class.h"
#include "Finder.h"
#include "Selection.h"
#include "MemoBox.h"
#include "Relation.h"
#include "ClassName.h"
#include "Template.h"

EditResizer::EditResizer() {

}
EditResizer::EditResizer(const EditResizer& source) {

}
EditResizer::~EditResizer() {

}

void EditResizer::ResizeEdit(TextEdit *textEdit, CDC *cdc) {
	if (!dynamic_cast<Relation*>(textEdit->figure)) {
		EditResizerProcess resizer;
		RECT rt;
		textEdit->GetClientRect(&rt);
		Long gabY_ = GabY * 2;
		if (dynamic_cast<MemoBox*>(textEdit->figure) || dynamic_cast<ClassName*>(textEdit->figure)) {
			gabY_ += MemoGab;
		}

		if (textEdit->text->MaxWidth(cdc) + GabX * 2 > textEdit->GetCriteriaWidth()) {			//글너비가 클래스를 넘어가는데
			if (textEdit->GetRowHeight()*textEdit->text->GetLength() + gabY_ > textEdit->GetCriteriaHeight()) {
				resizer.ResizeEditAll(textEdit, cdc);											//글높이가 클래스를 넘어가면 둘다O
			}
			else if (!dynamic_cast<Template*>(textEdit->figure)){								//글높이가 클래스를 안넘어가면 너비만
				resizer.ResizeEditWidth(textEdit, cdc);
			}
			else {																				//템플릿이면 템플릿너비만
				resizer.ResizeEditWidthToLeft(textEdit, cdc);
			}
		}																						//글너비가 클래스를 안넘어가는데
		else if (textEdit->GetRowHeight()*textEdit->text->GetLength() + gabY_ > textEdit->GetCriteriaHeight()) {
			resizer.ResizeEditHeight(textEdit, cdc);
		}
		else {																					//글높이가 클래스를 넘어가면 둘다X
			resizer.RewindEdit(textEdit, cdc);
		}
	}
}

void EditResizer::ResizeClass(TextEdit *textEdit, CDC *cdc) {
	EditResizerProcess resizer;
	RECT rt;
	textEdit->GetClientRect(&rt);
	if (!dynamic_cast<Relation*>(textEdit->figure) && !dynamic_cast<Template*>(textEdit->figure)) {
		ClassDiagramForm *classDiagramForm = (ClassDiagramForm*)textEdit->GetParentFrame();
		Long gabY_ = GabY * 2;

		if (dynamic_cast<Class*>(classDiagramForm->selection->GetAt(0))) {
			textEdit->figure->SetMinimumWidth(textEdit->text->MaxWidth(cdc) + GabX * 2);
			static_cast<Class*>(classDiagramForm->selection->GetAt(0))->SetMinimumWidth();
		}
		else {
			gabY_ += MemoGab;
			if (textEdit->text->MaxWidth(cdc) + GabX * 2 > 120) {
				textEdit->figure->SetMinimumWidth(textEdit->text->MaxWidth(cdc) + GabX * 2);
			}
		}

		textEdit->figure->SetMinimumHeight(textEdit->GetRowHeight()*textEdit->text->GetLength() + gabY_);

		if (rt.right + GabX * 2 > textEdit->GetCriteriaWidth()) {
			resizer.ResizeClassWidth(textEdit);
		}
		if (rt.bottom + gabY_ > textEdit->GetCriteriaHeight()) {
			resizer.ResizeClassHeight(textEdit);
		}
	}
	else if (dynamic_cast<Template*>(textEdit->figure)) {
		if ( rt.right + GabX*2 + CaretWidth > textEdit->GetCriteriaWidth()) {
			resizer.ResizeTemplateWidth(textEdit);
		}
	}
}
