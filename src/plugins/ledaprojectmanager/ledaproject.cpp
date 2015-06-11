/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.  For licensing terms and
** conditions see http://www.qt.io/terms-conditions.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "ledaproject.h"

#include "ledabuildconfiguration.h"
#include "ledamakestep.h"
#include "ledaprojectconstants.h"

#include <coreplugin/documentmanager.h>
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <extensionsystem/pluginmanager.h>
#include <projectexplorer/abi.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/headerpath.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <utils/fileutils.h>
#include <utils/qtcassert.h>

#include <QDir>
#include <QProcessEnvironment>
#include <QString>

#include <QDebug>
#define DBG qDebug() << QLatin1String(__PRETTY_FUNCTION__)

using namespace Core;
using namespace ProjectExplorer;

namespace LedaProjectManager {
namespace Internal {

////////////////////////////////////////////////////////////////////////////////////
//
// LedaProject
//
////////////////////////////////////////////////////////////////////////////////////

LedaProject::LedaProject(Manager *manager, const QString &fileName)
    : m_manager(manager),
      m_fileName(fileName)
{
    setId(Constants::LEDAPROJECT_ID);
    setProjectContext(Context(LedaProjectManager::Constants::PROJECTCONTEXT));
    setProjectLanguages(Context(ProjectExplorer::Constants::LANG_CXX));

    QFileInfo fileInfo(m_fileName);

    m_projectName = fileInfo.completeBaseName();
    m_document = new LedaProjectFile(this, m_fileName, LedaProject::Everything);
    DocumentManager::addDocument(m_document);
    m_rootNode = new LedaProjectNode(this, m_document);

    m_manager->registerProject(this);
}

LedaProject::~LedaProject()
{
    m_manager->unregisterProject(this);

    delete m_rootNode;
}

bool LedaProject::addFiles(const QStringList &filePaths)
{
    DBG << filePaths;
    m_files.append(filePaths);
    writeProject();
    refresh(LedaProject::Everything);
    return true;
}

bool LedaProject::removeFiles(const QStringList &filePaths)
{
    DBG << filePaths;
    foreach (const QString &path, filePaths)
        m_files.removeAll(path);
    writeProject();
    refresh(LedaProject::Everything);
    return true;
}

bool LedaProject::setFiles(const QStringList &filePaths)
{
    DBG << filePaths;
    m_files = filePaths;
    writeProject();
    refresh(LedaProject::Everything);
    return true;
}

bool LedaProject::renameFile(const QString &filePath, const QString &newFilePath)
{
    DBG << filePath << newFilePath;
    m_files.removeOne(filePath);
    m_files.append(newFilePath);
    writeProject();
    return true;
}

// TODO: check file exists?
void LedaProject::readProject(RefreshOptions options)
{
    DBG;
    if (options & Files) {
        QSettings settings(m_fileName, QSettings::IniFormat);
        m_files.clear();
        settings.beginGroup(QStringLiteral("files"));
        foreach (const QString &key, settings.allKeys()) {
            m_files.append(settings.value(key).toString());
        }
        settings.endGroup();
        emit fileListChanged();
    }
}

bool LedaProject::writeProject()
{
    DocumentManager::expectFileChange(m_fileName);
    QSettings settings(m_fileName, QSettings::IniFormat);
    settings.beginGroup(QStringLiteral("files"));
    for(int i = 0; i < m_files.count(); i++) {
        settings.setValue(QStringLiteral("%1").arg(i), m_files.at(i));
    }
    settings.endGroup();
    DocumentManager::unexpectFileChange(m_fileName);
    return true;
}

void LedaProject::refresh(RefreshOptions options)
{
    DBG;
    QSet<QString> oldFileList;
    if (options & Files)
        oldFileList = m_files.toSet();

    readProject(options);

    if (options & Files)
        m_rootNode->refresh();
}

QStringList LedaProject::files() const
{
    QStringList result = m_files;
    result << m_fileName; // Project file
    return result;
}

QString LedaProject::displayName() const
{
    return m_projectName;
}

IDocument *LedaProject::document() const
{
    return m_document;
}

IProjectManager *LedaProject::projectManager() const
{
    return m_manager;
}

LedaProjectNode *LedaProject::rootProjectNode() const
{
    return m_rootNode;
}

QStringList LedaProject::files(FilesMode fileMode) const
{
    Q_UNUSED(fileMode)
    return m_files;
}

bool LedaProject::fromMap(const QVariantMap &map)
{
    if (!Project::fromMap(map))
        return false;

    refresh(Everything);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
//
// LedaProjectFile
//
////////////////////////////////////////////////////////////////////////////////////

LedaProjectFile::LedaProjectFile(LedaProject *parent, QString fileName, LedaProject::RefreshOptions options)
    : IDocument(parent),
      m_project(parent),
      m_options(options)
{
    setId("Leda.ProjectFile");
    setMimeType(QLatin1String(Constants::LEDAMIMETYPE));
    setFilePath(Utils::FileName::fromString(fileName));
}

bool LedaProjectFile::save(QString *, const QString &, bool)
{
    return false;
}

QString LedaProjectFile::defaultPath() const
{
    return QString();
}

QString LedaProjectFile::suggestedFileName() const
{
    return QString();
}

bool LedaProjectFile::isModified() const
{
    return false;
}

bool LedaProjectFile::isSaveAsAllowed() const
{
    return false;
}

IDocument::ReloadBehavior LedaProjectFile::reloadBehavior(ChangeTrigger state, ChangeType type) const
{
    Q_UNUSED(state)
    Q_UNUSED(type)
    return BehaviorSilent;
}

bool LedaProjectFile::reload(QString *errorString, ReloadFlag flag, ChangeType type)
{
    Q_UNUSED(errorString)
    Q_UNUSED(flag)
    if (type == TypePermissions)
        return true;
    m_project->refresh(m_options);
    return true;
}

} // namespace Internal
} // namespace LedaProjectManager
