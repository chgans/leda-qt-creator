
#include "ledaeditorplugin.h"
#include "ledaeditorconstants.h"
#include "ledaeditorfactory.h"

#include <coreplugin/icore.h>

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegExp>
#include <QVariant>

#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QToolBar>

#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/id.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/idocument.h>
#include <extensionsystem/pluginmanager.h>
#include <utils/qtcassert.h>
#include <utils/mimetypes/mimedatabase.h>

using namespace Utils;
using namespace Core;

namespace LedaEditor {
namespace Internal {

LedaEditorPlugin::LedaEditorPlugin():
    ExtensionSystem::IPlugin()
{
}

LedaEditorPlugin::~LedaEditorPlugin()
{
}

bool LedaEditorPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorMessage)

    Core::ICore *core = Core::ICore::instance();
    Core::EditorManager *editorManager = Core::EditorManager::instance();
    Core::ActionManager *actionManager = Core::ActionManager::instance();

    connect(editorManager, SIGNAL(currentEditorChanged(Core::IEditor*)),
        this, SLOT(updateCurrentEditor(Core::IEditor*)));

    addAutoReleasedObject(new LedaEditorFactory(this));
    //addAutoReleasedObject(new LedaEditorWidgetFactory);

    // Create command in the help menu
    Core::Command *command = actionManager->registerAction(new QAction(this),
                                                "leda.about");
    command->action()->setText(tr("About LibreEDA"));
    actionManager->actionContainer(Core::Constants::M_HELP)->addAction(command);
    connect(command->action(), SIGNAL(triggered(bool)),
            this, SLOT(aboutLibreEDA()));

    // Create a custom menu
    Core::ActionContainer *ac = actionManager->createMenu("leda.menu.extra");
    ac->menu()->setTitle(tr("Leda"));
    actionManager->actionContainer(Core::Constants::MENU_BAR)->addMenu(ac);
    ac->addAction(command);

    // Register a mime-type
    Utils::MimeDatabase::addMimeTypes(QStringLiteral(":/leda/LedaEditor.mimetypes.xml"));

    return true;
}

void LedaEditorPlugin::extensionsInitialized()
{
}

void LedaEditorPlugin::aboutLibreEDA()
{
    QMessageBox::information(
    Core::ICore::instance()->mainWindow(),
    tr("About LibreEDA"),
    tr("LibreEDA is ...")
    );
}

} // namespace Internal
} // namespace BinEditor

