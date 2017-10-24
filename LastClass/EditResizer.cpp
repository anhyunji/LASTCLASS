//EditResizer.cpp
#include "EditResizer.h"
#include "EditResizerProcess.h"
#include "ClassDiagramForm.h"
#include "TextEdit.h"
#include "Text.h"
#include "Row.h"
#include "Figure.h"
#include "Diagram.h"
#include "Class.h"
#include "Finder.h"
#include "Selection.h"
#include "MemoBox.h"
#include "Relation.h"
#include "SelfRelation.h"
#include "ClassName.h"
#include "Template.h"
#include "ClassDiagramForm.h"

EditResizer::EditResizer() {

}
EditResizer::EditResizer(const EditResizer& source) {

}
EditResizer::~EditResizer() {

}

void EditResizer::ResizeEdit(TextEdit *textEdit, CDC *cdc) {
	EditResizerProcess resizer;
	RECT rt;
	textEdit->GetClientRect(&rt);
	Long gabY_ = GabY + GabY;
	if (dynamic_cast<MemoBox*>(textEdit->figure) || dynamic_cast<ClassName*>(textEdit->figure)) {
		gabY_ += MemoGab;
	}

	if (textEdit->text->MaxWidth(cdc) + GabX * 2  > textEdit->GetCriteriaWidth()) {         //�۳ʺ� Ŭ������ �Ѿ�µ�
		if (textEdit->GetRowHeight()*textEdit->text->GetLength() + gabY_ > textEdit->GetCriteriaHeight()) {
			resizer.ResizeEditAll(textEdit, cdc);                                          //�۳��̰� Ŭ������ �Ѿ�� �Ѵ�O
		}
		else if (!dynamic_cast<Template*>(textEdit->figure)) {                                 //�۳��̰� Ŭ������ �ȳѾ�� �ʺ�
			resizer.ResizeEditWidth(textEdit, cdc);
		}
		else {                                                                     //���ø��̸� ���ø��ʺ�
			resizer.ResizeEditWidthToLeft(textEdit, cdc);

		}
	}                                                                           //�۳ʺ� Ŭ������ �ȳѾ�µ�
	else if (textEdit->GetRowHeight() * textEdit->text->GetLength() + gabY_ > textEdit->GetCriteriaHeight()) {
		resizer.ResizeEditHeight(textEdit, cdc);
	}
	else {
		//�۳��̰� Ŭ������ �Ѿ�� �Ѵ�X
		resizer.RewindEdit(textEdit, cdc);
	}
}

void EditResizer::ResizeClass(TextEdit *textEdit, CDC *cdc) {
	EditResizerProcess resizer;
	RECT rt;
	textEdit->GetClientRect(&rt);
	if (dynamic_cast<Template*>(textEdit->figure)) {
		if (rt.right + GabX * 2  > textEdit->GetCriteriaWidth()) {
			resizer.ResizeTemplateWidth(textEdit);
			resizer.AffectedRelation(textEdit);
		}
	}
	else {
		ClassDiagramForm *classDiagramForm = (ClassDiagramForm*)textEdit->GetParent();
		Long gabY_ = GabY * 2;

		if (dynamic_cast<Class*>(classDiagramForm->selection->GetAt(0))) {
			if (textEdit->text->MaxWidth(cdc) + GabX * 2 + CaretWidth < 120 * classDiagramForm->zoomRate / 100) {
				textEdit->figure->SetMinimumWidth(120 * classDiagramForm->zoomRate / 100);
				static_cast<Class*>(classDiagramForm->selection->GetAt(0))->SetMinimumWidthR(classDiagramForm->zoomRate);
			}
			else {
				textEdit->figure->SetMinimumWidth(textEdit->text->MaxWidth(cdc) + GabX * 2);
				static_cast<Class*>(classDiagramForm->selection->GetAt(0))->SetMinimumWidthR(classDiagramForm->zoomRate);
			}
		}
		else {
			gabY_ += MemoGab;
			if (textEdit->text->MaxWidth(cdc) + GabX * 2 < 120 * classDiagramForm->zoomRate / 100) {
				textEdit->figure->SetMinimumWidth(120 * classDiagramForm->zoomRate / 100);
			}
			else {
				textEdit->figure->SetMinimumWidth(textEdit->text->MaxWidth(cdc) + GabX * 2);
			}
		}
		if (dynamic_cast<ClassName*>(textEdit->figure)) {
			gabY_ += MemoGab;
		}
		textEdit->figure->SetMinimumHeight(textEdit->GetRowHeight()*textEdit->text->GetLength() + gabY_);
		if (!dynamic_cast<Relation*>(textEdit->GetFigure()) && !dynamic_cast<SelfRelation*>(textEdit->GetFigure())) {
		resizer.ResizeClassWidth(textEdit);
		resizer.ResizeClassHeight(textEdit);
		}
	}
}