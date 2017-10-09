//RemoveTemplateKey.cpp

#include "RemoveTemplateKey.h"
#include "ClassDiagramForm.h"
#include "HistoryGraphic.h"
#include "Selection.h"
#include "Class.h"
#include "TextEdit.h"
#include "SelfRelation.h"

RemoveTemplateKey::RemoveTemplateKey() {
}

RemoveTemplateKey::RemoveTemplateKey(const RemoveTemplateKey& source) {
}

RemoveTemplateKey::~RemoveTemplateKey() {
}


void RemoveTemplateKey::KeyPress(ClassDiagramForm *classDiagramForm,CDC *cdc) {
	if (classDiagramForm->selection->GetLength() > 0) {
		Class *object = static_cast<Class*>(classDiagramForm->selection->GetAt(0));
		if (object->GetTempletePosition() != -1) {
			classDiagramForm->historyGraphic->PushUndo(classDiagramForm->diagram);
			object->RemoveTemplate();
			Long i = 0;
			while (i < object->GetLength()) {
				if (object->GetTempletePosition() == -1) {
					if (dynamic_cast<SelfRelation*>(object->GetAt(i))) {
						SelfRelation* selfRelation = static_cast<SelfRelation*>(object->GetAt(i));
						selfRelation->Move(0, +17);
						Long k = 0;
						while (k < 5) {
							CPoint cPoint(selfRelation->rollNamePoints->GetAt(k).x, selfRelation->rollNamePoints->GetAt(k).y + 17);
							selfRelation->rollNamePoints->Modify(k, cPoint);
							k++;
						}
					}
				}
				i++;
			}
		}
	}
}

void RemoveTemplateKey::KeyPress(TextEdit *textEdit) {

}