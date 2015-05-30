#ifndef LEDADOCUMENT_H
#define LEDADOCUMENT_H

#include "coreplugin/idocument.h"

namespace LedaEditor {
namespace Internal {

class LedaEditor;
class LedaEditorWidget;

class LedaDocument : public Core::IDocument
{
public:
    LedaDocument(LedaEditor *editor, LedaEditorWidget *widget);

    bool open(QString *errorString, const QString &fileName);

private slots:
    void setModified();

private:
    LedaEditor *m_editor;
    LedaEditorWidget *m_widget;
    QString m_fileName;
    bool m_modified;
    void setModified(bool modified);

    // IDocument interface
public:
    virtual bool save(QString *errorString, const QString &fileName, bool autoSave);
    virtual QString defaultPath() const;
    virtual QString suggestedFileName() const;
    virtual bool isModified() const;
    virtual bool isSaveAsAllowed() const;
    virtual bool reload(QString *errorString, ReloadFlag flag, ChangeType type);
    virtual bool isFileReadOnly() const;
};

}} // LedaEditor::Internal

#endif // LEDADOCUMENT_H
