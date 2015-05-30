#include "ledadocument.h"
#include "ledaeditorconstants.h"
#include "ledaeditor.h"
#include "ledaeditorwidget.h"

#include <QFile>

namespace LedaEditor {
namespace Internal {

LedaDocument::LedaDocument(LedaEditor *editor, LedaEditorWidget *widget):
    Core::IDocument(editor),
    m_editor(editor), m_widget(widget)
{
    setMimeType(QLatin1String(Constants::C_LEDAEDITOR_MIMETYPE));
    setId(Core::Id(Constants::C_LEDAEDITOR));
}

bool LedaDocument::open(QString *errorString, const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        *errorString = file.errorString();
        return false;
    }

    // TODO

    setModified(false);
    return true;
}

void LedaDocument::setModified()
{
    setModified(true);
}

void LedaDocument::setModified(bool modified)
{
    if (m_modified == modified)
        return;
    m_modified = modified;
    emit changed();
}

bool LedaDocument::save(QString *errorString, const QString &fileName, bool /*autoSave*/)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        *errorString = file.errorString();
        return false;
    }

    // TODO

    setModified(false);
    return true;
}

QString LedaDocument::defaultPath() const
{
    return QString();
}

QString LedaDocument::suggestedFileName() const
{
    return QString();
}

bool LedaDocument::isModified() const
{
    return m_modified;
}

bool LedaDocument::isFileReadOnly() const
{
    return false;
}

bool LedaDocument::isSaveAsAllowed() const
{
    return true;
}

bool LedaDocument::reload(QString *errorString, Core::IDocument::ReloadFlag flag,
                          Core::IDocument::ChangeType type)
{
    Q_UNUSED(errorString);
    Q_UNUSED(flag);
    Q_UNUSED(type);
    return true;
}


}} // LedaEditor::Internal
