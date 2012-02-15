/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#ifndef SSHCONNECTIONMANAGER_H
#define SSHCONNECTIONMANAGER_H

#include <utils/utils_global.h>

#include <QScopedPointer>
#include <QSharedPointer>

namespace Utils {
class SshConnection;
class SshConnectionParameters;
namespace Internal { class SshConnectionManagerPrivate; }

class QTCREATOR_UTILS_EXPORT SshConnectionManager
{
    friend class Internal::SshConnectionManagerPrivate;
public:
    static SshConnectionManager &instance();

    QSharedPointer<SshConnection> acquireConnection(const SshConnectionParameters &sshParams);
    void releaseConnection(const QSharedPointer<SshConnection> &connection);
    // Make sure the next acquireConnection with the given parameters will return a new connection.
    void forceNewConnection(const SshConnectionParameters &sshParams);

private:
    explicit SshConnectionManager();
    virtual ~SshConnectionManager();
    SshConnectionManager(const SshConnectionManager &);
    SshConnectionManager &operator=(const SshConnectionManager &);

    const QScopedPointer<Internal::SshConnectionManagerPrivate> d;
};

} // namespace Utils

#endif // SSHCONNECTIONMANAGER_H
