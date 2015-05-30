#ifndef LEDAEDITOR_H
#define LEDAEDITOR_H

#include "coreplugin/editormanager/ieditor.h"

namespace LedaEditor {
namespace Internal {

class LedaEditorWidget;
class LedaDocument;

class LedaEditor : public Core::IEditor
{
    Q_OBJECT

public:
    LedaEditor(LedaEditorWidget *widget);

private:
    LedaEditorWidget *m_widget;
    LedaDocument *m_document;

    // IEditor interface
public:
    virtual bool open(QString *errorString, const QString &fileName, const QString &realFileName);
    virtual Core::IDocument *document();
    virtual QWidget *toolBar();
};

}} // LedaEditor::Internal

#endif // LEDAEDITOR_H
