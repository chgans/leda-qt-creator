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
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://www.qt.io/licensing.  For further information
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
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"
#include "gmock/gmock.h"

#include "gtest-qt-printing.h"
#include "translationunitdoesnotexistcommand.h"

#include <QString>
#include <QBuffer>
#include <QVariant>
#include <vector>

#include <cmbendcommand.h>
#include <cmbalivecommand.h>
#include <cmbcommands.h>
#include <cmbechocommand.h>
#include <cmbregistertranslationunitsforcodecompletioncommand.h>
#include <cmbunregistertranslationunitsforcodecompletioncommand.h>
#include <cmbcodecompletedcommand.h>
#include <cmbregisterprojectsforcodecompletioncommand.h>
#include <cmbunregisterprojectsforcodecompletioncommand.h>
#include <cmbcompletecodecommand.h>
#include <writecommandblock.h>
#include <readcommandblock.h>

#include <ipcserverproxy.h>
#include <ipcclientproxy.h>
#include <projectpartsdonotexistcommand.h>

#include "mockipclient.h"
#include "mockipcserver.h"

using namespace CodeModelBackEnd;

namespace {

using ::testing::Args;
using ::testing::Property;
using ::testing::Eq;

class ClientServerInProcess : public ::testing::Test
{
protected:
    ClientServerInProcess();

    void SetUp();
    void TearDown();


    void scheduleServerCommands();
    void scheduleClientCommands();

    QBuffer buffer;
    MockIpcClient mockIpcClient;
    MockIpcServer mockIpcServer;

    CodeModelBackEnd::IpcServerProxy serverProxy;
    CodeModelBackEnd::IpcClientProxy clientProxy;
};

TEST_F(ClientServerInProcess, SendEndCommand)
{
    EXPECT_CALL(mockIpcServer, end())
        .Times(1);

    serverProxy.end();
    scheduleServerCommands();
}

TEST_F(ClientServerInProcess, SendAliveCommand)
{
    EXPECT_CALL(mockIpcClient, alive())
        .Times(1);

    clientProxy.alive();
    scheduleClientCommands();
}

TEST_F(ClientServerInProcess, SendRegisterTranslationUnitForCodeCompletionCommand)
{
    CodeModelBackEnd::FileContainer fileContainer(Utf8StringLiteral(TESTDATA_DIR"/complete_extractor_function.cpp"),
                                                  Utf8StringLiteral("pathToProjectPart.pro"));
    CodeModelBackEnd::RegisterTranslationUnitForCodeCompletionCommand command({fileContainer});

    EXPECT_CALL(mockIpcServer, registerTranslationUnitsForCodeCompletion(command))
        .Times(1);

    serverProxy.registerTranslationUnitsForCodeCompletion(command);
    scheduleServerCommands();
}

TEST_F(ClientServerInProcess, SendUnregisterTranslationUnitsForCodeCompletionCommand)
{
    CodeModelBackEnd::FileContainer fileContainer(Utf8StringLiteral("foo.cpp"), Utf8StringLiteral("pathToProjectPart.pro"));
    CodeModelBackEnd::UnregisterTranslationUnitsForCodeCompletionCommand command({fileContainer});

    EXPECT_CALL(mockIpcServer, unregisterTranslationUnitsForCodeCompletion(command))
        .Times(1);

    serverProxy.unregisterTranslationUnitsForCodeCompletion(command);
    scheduleServerCommands();
}

TEST_F(ClientServerInProcess, SendCompleteCodeCommand)
{
    CodeModelBackEnd::CompleteCodeCommand command(Utf8StringLiteral("foo.cpp"), 24, 33, Utf8StringLiteral("do what I want"));

    EXPECT_CALL(mockIpcServer, completeCode(command))
        .Times(1);

    serverProxy.completeCode(command);
    scheduleServerCommands();
}

TEST_F(ClientServerInProcess, SendCodeCompletedCommand)
{
    QVector<CodeModelBackEnd::CodeCompletion> codeCompletions({Utf8StringLiteral("newFunction()")});
    CodeModelBackEnd::CodeCompletedCommand command(codeCompletions, 1);

    EXPECT_CALL(mockIpcClient, codeCompleted(command))
        .Times(1);

    clientProxy.codeCompleted(command);
    scheduleClientCommands();
}

TEST_F(ClientServerInProcess, SendRegisterProjectPartsForCodeCompletionCommand)
{
    CodeModelBackEnd::ProjectPartContainer projectContainer(Utf8StringLiteral(TESTDATA_DIR"/complete.pro"));
    CodeModelBackEnd::RegisterProjectPartsForCodeCompletionCommand command({projectContainer});

    EXPECT_CALL(mockIpcServer, registerProjectPartsForCodeCompletion(command))
        .Times(1);

    serverProxy.registerProjectPartsForCodeCompletion(command);
    scheduleServerCommands();
}

TEST_F(ClientServerInProcess, SendUnregisterProjectPartsForCodeCompletionCommand)
{
    CodeModelBackEnd::UnregisterProjectPartsForCodeCompletionCommand command({Utf8StringLiteral(TESTDATA_DIR"/complete.pro")});

    EXPECT_CALL(mockIpcServer, unregisterProjectPartsForCodeCompletion(command))
        .Times(1);

    serverProxy.unregisterProjectPartsForCodeCompletion(command);
    scheduleServerCommands();
}

TEST_F(ClientServerInProcess, SendTranslationUnitDoesNotExistCommand)
{
    CodeModelBackEnd::FileContainer fileContainer(Utf8StringLiteral(TESTDATA_DIR"/complete_extractor_function.cpp"),
                                                  Utf8StringLiteral("pathToProjectPart.pro"));
    CodeModelBackEnd::TranslationUnitDoesNotExistCommand command(fileContainer);

    EXPECT_CALL(mockIpcClient, translationUnitDoesNotExist(command))
        .Times(1);

    clientProxy.translationUnitDoesNotExist(command);
    scheduleClientCommands();
}


TEST_F(ClientServerInProcess, SendProjectPartDoesNotExistCommand)
{
    CodeModelBackEnd::ProjectPartsDoNotExistCommand command({Utf8StringLiteral("pathToProjectPart.pro")});

    EXPECT_CALL(mockIpcClient, projectPartsDoNotExist(command))
        .Times(1);

    clientProxy.projectPartsDoNotExist(command);
    scheduleClientCommands();
}
ClientServerInProcess::ClientServerInProcess()
    : serverProxy(&mockIpcClient, &buffer),
      clientProxy(&mockIpcServer, &buffer)
{
}

void ClientServerInProcess::SetUp()
{
    buffer.open(QIODevice::ReadWrite);
}

void ClientServerInProcess::TearDown()
{
    buffer.close();
}

void ClientServerInProcess::scheduleServerCommands()
{
    buffer.seek(0);
    clientProxy.readCommands();
    buffer.buffer().clear();
}

void ClientServerInProcess::scheduleClientCommands()
{
    buffer.seek(0);
    serverProxy.readCommands();
    buffer.buffer().clear();
}

}
