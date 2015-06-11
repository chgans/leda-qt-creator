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

#include "ledaprojectnodes.h"
#include "ledaproject.h"

#include <utils/fileutils.h>
#include <coreplugin/idocument.h>
#include <projectexplorer/projectexplorer.h>

#include <QFileInfo>

#include <QDebug>
#define DBG qDebug() << QLatin1String(__PRETTY_FUNCTION__)

using namespace ProjectExplorer;

namespace LedaProjectManager {
namespace Internal {

LedaProjectNode::LedaProjectNode(LedaProject *project, Core::IDocument *projectFile)
    : ProjectNode(projectFile->filePath())
    , m_project(project)
    , m_projectFile(projectFile)
{
    setDisplayName(projectFile->filePath().toFileInfo().completeBaseName());
}

Core::IDocument *LedaProjectNode::projectFile() const
{
    return m_projectFile;
}

QString LedaProjectNode::projectFilePath() const
{
    return m_projectFile->filePath().toString();
}

void LedaProjectNode::refresh()
{
    DBG;
    QSet<QString> newFileList = m_project->files().toSet();

    QList<FileNode*> nodes = fileNodes();
    QList<QString> files;

    QList<FileNode*> nodesToAdd;
    QList<FileNode*> nodesToRemove;

    foreach (FileNode *node, nodes) {
        QString file = node->path().fileName();
        files.append(file);
        if (!newFileList.contains(file))
            nodesToRemove.append(node);
    }
    removeFileNodes(nodesToRemove);

    foreach (const QString &file, newFileList) {
        if (!files.contains(file)) {
            FileNode *node = new FileNode(Utils::FileName::fromString(file),
                                          SourceType, /*generated = */ false);
            nodesToAdd.append(node);
        }
    }
    addFileNodes(nodesToAdd);
}

bool LedaProjectNode::showInSimpleTree() const
{
    return true;
}

QList<ProjectAction> LedaProjectNode::supportedActions(Node *node) const
{
    Q_UNUSED(node);
    return QList<ProjectAction>()
            << AddNewFile
            << AddExistingFile
            << AddExistingDirectory
            << RemoveFile
            << Rename;
}

bool LedaProjectNode::canAddSubProject(const QString &proFilePath) const
{
    Q_UNUSED(proFilePath)
    return false;
}

bool LedaProjectNode::addSubProjects(const QStringList &proFilePaths)
{
    Q_UNUSED(proFilePaths)
    return false;
}

bool LedaProjectNode::removeSubProjects(const QStringList &proFilePaths)
{
    Q_UNUSED(proFilePaths)
    return false;
}

bool LedaProjectNode::addFiles(const QStringList &filePaths, QStringList *notAdded)
{
    Q_UNUSED(notAdded)
    DBG << filePaths;
    return m_project->addFiles(filePaths);
}

bool LedaProjectNode::removeFiles(const QStringList &filePaths, QStringList *notRemoved)
{
    Q_UNUSED(notRemoved)
    DBG << filePaths;
    return m_project->removeFiles(filePaths);
}

bool LedaProjectNode::deleteFiles(const QStringList &filePaths)
{
    Q_UNUSED(filePaths)
    DBG << filePaths;
    return false;
}

bool LedaProjectNode::renameFile(const QString &filePath, const QString &newFilePath)
{
    DBG << filePath << newFilePath;
    return m_project->renameFile(filePath, newFilePath);
}

} // namespace Internal
} // namespace LedaProjectManager
