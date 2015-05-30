#ifndef LEDAEDITORFACTORY_H
#define LEDAEDITORFACTORY_H

#include "coreplugin/editormanager/ieditorfactory.h"


namespace LedaEditor {
namespace Internal {

class LedaEditorPlugin;

class LedaEditorFactory : public Core::IEditorFactory
{
public:
    LedaEditorFactory(LedaEditorPlugin *owner);

    // IEditorFactory interface
public:
    virtual Core::IEditor *createEditor();
};

}} // LedaEditor::Internal
#endif // LEDAEDITORFACTORY_H
