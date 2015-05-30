#include "ledaeditorfactory.h"
#include "ledaeditorconstants.h"
#include "ledaeditor.h"
#include "ledaeditorwidget.h"
#include "ledaeditorplugin.h"

#include <QApplication>

namespace LedaEditor {
namespace Internal {

LedaEditorFactory::LedaEditorFactory(LedaEditorPlugin *owner):
    Core::IEditorFactory(owner)
{
    setId(Constants::C_LEDAEDITOR);
    setDisplayName(qApp->translate("OpenWith::Editors", Constants::C_LEDAEDITOR_DISPLAY_NAME));
    addMimeType(Constants::C_LEDAEDITOR_MIMETYPE);
}

Core::IEditor *LedaEditorFactory::createEditor()
{
    LedaEditorWidget *widget = new LedaEditorWidget();
    LedaEditor *editor = new LedaEditor(widget);
    // plugin->initialiseEditor(editor);
    return editor;
}


}} // LedaEditor::Internal
