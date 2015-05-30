#ifndef LEDAEDITORPLUGIN_H
#define LEDAEDITORPLUGIN_H

#include <extensionsystem/iplugin.h>

namespace LedaEditor {
namespace Internal {

class LedaEditorPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "LedaEditor.json")

public:
    LedaEditorPlugin();
    ~LedaEditorPlugin();

    bool initialize(const QStringList &arguments, QString *errorMessage = 0);
    void extensionsInitialized();

protected slots:
    void aboutLibreEDA();
};


} // namespace Internal
} // namespace BinEditor

#endif // LEDAEDITORPLUGIN_H
