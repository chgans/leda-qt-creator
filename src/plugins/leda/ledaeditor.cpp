#include "ledaeditor.h"
#include "ledaeditorwidget.h"
#include "ledadocument.h"

#include "utils/fileutils.h"

namespace LedaEditor {
namespace Internal {

LedaEditor::LedaEditor(LedaEditorWidget *widget):
    m_widget(widget),
    m_document(new LedaDocument(this, widget))
{
    setWidget(m_widget);
    //setContext(); ??
}

}} // LedaEditor::Internal


bool LedaEditor::Internal::LedaEditor::open(QString *errorString,
                                            const QString &fileName,
                                            const QString &realFileName)
{
    if (!m_document->open(errorString, realFileName))
        return false;
    m_document->setFilePath(Utils::FileName::fromString(fileName));
    return true;
}

Core::IDocument *LedaEditor::Internal::LedaEditor::document()
{
    return m_document;
}

QWidget *LedaEditor::Internal::LedaEditor::toolBar()
{
    return nullptr;
}
