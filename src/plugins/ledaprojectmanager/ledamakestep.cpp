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

#include "ledamakestep.h"
#include "ledaprojectconstants.h"
#include "ledaproject.h"
#include "ui_ledamakestep.h"
#include "ledabuildconfiguration.h"

#include <extensionsystem/pluginmanager.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/gnumakeparser.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/toolchain.h>
//#include <qtsupport/qtkitinformation.h>
//#include <qtsupport/qtparser.h>
#include <utils/stringutils.h>
#include <utils/qtcassert.h>
#include <utils/qtcprocess.h>

#include <QDir>

using namespace Core;
using namespace ProjectExplorer;

namespace LedaProjectManager {
namespace Internal {

const char LEDA_MS_ID[] = "LedaProjectManager.LedaMakeStep";
const char LEDA_MS_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("LedaProjectManager::Internal::LedaMakeStep",
                                                     "Make");

const char BUILD_TARGETS_KEY[] = "LedaProjectManager.LedaMakeStep.BuildTargets";
const char MAKE_ARGUMENTS_KEY[] = "LedaProjectManager.LedaMakeStep.MakeArguments";
const char MAKE_COMMAND_KEY[] = "LedaProjectManager.LedaMakeStep.MakeCommand";
const char CLEAN_KEY[] = "LedaProjectManager.LedaMakeStep.Clean";

LedaMakeStep::LedaMakeStep(BuildStepList *parent) :
    AbstractProcessStep(parent, Id(LEDA_MS_ID)),
    m_clean(false)
{
    ctor();
}

LedaMakeStep::LedaMakeStep(BuildStepList *parent, const Id id) :
    AbstractProcessStep(parent, id),
    m_clean(false)
{
    ctor();
}

LedaMakeStep::LedaMakeStep(BuildStepList *parent, LedaMakeStep *bs) :
    AbstractProcessStep(parent, bs),
    m_buildTargets(bs->m_buildTargets),
    m_makeArguments(bs->m_makeArguments),
    m_makeCommand(bs->m_makeCommand),
    m_clean(bs->m_clean)
{
    ctor();
}

void LedaMakeStep::ctor()
{
    setDefaultDisplayName(QCoreApplication::translate("LedaProjectManager::Internal::LedaMakeStep",
                                                      LEDA_MS_DISPLAY_NAME));
}

LedaMakeStep::~LedaMakeStep()
{
}

bool LedaMakeStep::init()
{
    BuildConfiguration *bc = buildConfiguration();
    if (!bc)
        bc = target()->activeBuildConfiguration();
    if (!bc)
        emit addTask(Task::buildConfigurationMissingTask());

    ToolChain *tc = ToolChainKitInformation::toolChain(target()->kit());
    if (!tc)
        emit addTask(Task::compilerMissingTask());

    if (!bc || !tc) {
        emitFaultyConfigurationMessage();
        return false;
    }

    ProcessParameters *pp = processParameters();
    pp->setMacroExpander(bc->macroExpander());
    pp->setWorkingDirectory(bc->buildDirectory().toString());
    Utils::Environment env = bc->environment();
    // Force output to english for the parsers. Do this here and not in the toolchain's
    // addToEnvironment() to not screw up the users run environment.
    env.set(QLatin1String("LC_ALL"), QLatin1String("C"));
    pp->setEnvironment(env);
    pp->setCommand(makeCommand(bc->environment()));
    pp->setArguments(allArguments());
    pp->resolveAll();

    // If we are cleaning, then make can fail with an error code, but that doesn't mean
    // we should stop the clean queue
    // That is mostly so that rebuild works on an already clean project
    setIgnoreReturnValue(m_clean);

    setOutputParser(new GnuMakeParser());
    IOutputParser *parser = target()->kit()->createOutputParser();
    if (parser)
        appendOutputParser(parser);
    outputParser()->setWorkingDirectory(pp->effectiveWorkingDirectory());

    return AbstractProcessStep::init();
}

void LedaMakeStep::setClean(bool clean)
{
    m_clean = clean;
}

bool LedaMakeStep::isClean() const
{
    return m_clean;
}

QVariantMap LedaMakeStep::toMap() const
{
    QVariantMap map(AbstractProcessStep::toMap());

    map.insert(QLatin1String(BUILD_TARGETS_KEY), m_buildTargets);
    map.insert(QLatin1String(MAKE_ARGUMENTS_KEY), m_makeArguments);
    map.insert(QLatin1String(MAKE_COMMAND_KEY), m_makeCommand);
    map.insert(QLatin1String(CLEAN_KEY), m_clean);
    return map;
}

bool LedaMakeStep::fromMap(const QVariantMap &map)
{
    m_buildTargets = map.value(QLatin1String(BUILD_TARGETS_KEY)).toStringList();
    m_makeArguments = map.value(QLatin1String(MAKE_ARGUMENTS_KEY)).toString();
    m_makeCommand = map.value(QLatin1String(MAKE_COMMAND_KEY)).toString();
    m_clean = map.value(QLatin1String(CLEAN_KEY)).toBool();

    return BuildStep::fromMap(map);
}

QString LedaMakeStep::allArguments() const
{
    QString args = m_makeArguments;
    Utils::QtcProcess::addArgs(&args, m_buildTargets);
    return args;
}

QString LedaMakeStep::makeCommand(const Utils::Environment &environment) const
{
    QString command = m_makeCommand;
    if (command.isEmpty()) {
        ToolChain *tc = ToolChainKitInformation::toolChain(target()->kit());
        if (tc)
            command = tc->makeCommand(environment);
        else
            command = QLatin1String("make");
    }
    return command;
}

void LedaMakeStep::run(QFutureInterface<bool> &fi)
{
    AbstractProcessStep::run(fi);
}

BuildStepConfigWidget *LedaMakeStep::createConfigWidget()
{
    return new LedaMakeStepConfigWidget(this);
}

bool LedaMakeStep::immutable() const
{
    return false;
}

bool LedaMakeStep::buildsTarget(const QString &target) const
{
    return m_buildTargets.contains(target);
}

void LedaMakeStep::setBuildTarget(const QString &target, bool on)
{
    QStringList old = m_buildTargets;
    if (on && !old.contains(target))
         old << target;
    else if (!on && old.contains(target))
        old.removeOne(target);

    m_buildTargets = old;
}

//
// LedaMakeStepConfigWidget
//

LedaMakeStepConfigWidget::LedaMakeStepConfigWidget(LedaMakeStep *makeStep)
    : m_makeStep(makeStep)
{
    m_ui = new Ui::LedaMakeStep;
    m_ui->setupUi(this);

    LedaProject *pro = static_cast<LedaProject *>(m_makeStep->target()->project());
    foreach (const QString &target, pro->buildTargets()) {
        QListWidgetItem *item = new QListWidgetItem(target, m_ui->targetsList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(m_makeStep->buildsTarget(item->text()) ? Qt::Checked : Qt::Unchecked);
    }

    m_ui->makeLineEdit->setText(m_makeStep->m_makeCommand);
    m_ui->makeArgumentsLineEdit->setText(m_makeStep->m_makeArguments);
    updateMakeOverrrideLabel();
    updateDetails();

    connect(m_ui->targetsList, &QListWidget::itemChanged,
            this, &LedaMakeStepConfigWidget::itemChanged);
    connect(m_ui->makeLineEdit, &QLineEdit::textEdited,
            this, &LedaMakeStepConfigWidget::makeLineEditTextEdited);
    connect(m_ui->makeArgumentsLineEdit, &QLineEdit::textEdited,
            this, &LedaMakeStepConfigWidget::makeArgumentsLineEditTextEdited);

    connect(ProjectExplorerPlugin::instance(), SIGNAL(settingsChanged()),
            this, SLOT(updateMakeOverrrideLabel()));
    connect(ProjectExplorerPlugin::instance(), SIGNAL(settingsChanged()),
            this, SLOT(updateDetails()));

    connect(m_makeStep->target(), SIGNAL(kitChanged()),
            this, SLOT(updateMakeOverrrideLabel()));

    connect(pro, &LedaProject::environmentChanged,
            this, &LedaMakeStepConfigWidget::updateMakeOverrrideLabel);
    connect(pro, &LedaProject::environmentChanged,
            this, &LedaMakeStepConfigWidget::updateDetails);
}

LedaMakeStepConfigWidget::~LedaMakeStepConfigWidget()
{
    delete m_ui;
}

QString LedaMakeStepConfigWidget::displayName() const
{
    return tr("Make", "LedaMakestep display name.");
}

void LedaMakeStepConfigWidget::updateMakeOverrrideLabel()
{
    BuildConfiguration *bc = m_makeStep->buildConfiguration();
    if (!bc)
        bc = m_makeStep->target()->activeBuildConfiguration();

    m_ui->makeLabel->setText(tr("Override %1:").arg(QDir::toNativeSeparators(m_makeStep->makeCommand(bc->environment()))));
}

void LedaMakeStepConfigWidget::updateDetails()
{
    BuildConfiguration *bc = m_makeStep->buildConfiguration();
    if (!bc)
        bc = m_makeStep->target()->activeBuildConfiguration();

    ProcessParameters param;
    param.setMacroExpander(bc->macroExpander());
    param.setWorkingDirectory(bc->buildDirectory().toString());
    param.setEnvironment(bc->environment());
    param.setCommand(m_makeStep->makeCommand(bc->environment()));
    param.setArguments(m_makeStep->allArguments());
    m_summaryText = param.summary(displayName());
    emit updateSummary();
}

QString LedaMakeStepConfigWidget::summaryText() const
{
    return m_summaryText;
}

void LedaMakeStepConfigWidget::itemChanged(QListWidgetItem *item)
{
    m_makeStep->setBuildTarget(item->text(), item->checkState() & Qt::Checked);
    updateDetails();
}

void LedaMakeStepConfigWidget::makeLineEditTextEdited()
{
    m_makeStep->m_makeCommand = m_ui->makeLineEdit->text();
    updateDetails();
}

void LedaMakeStepConfigWidget::makeArgumentsLineEditTextEdited()
{
    m_makeStep->m_makeArguments = m_ui->makeArgumentsLineEdit->text();
    updateDetails();
}

//
// LedaMakeStepFactory
//

LedaMakeStepFactory::LedaMakeStepFactory(QObject *parent) :
    IBuildStepFactory(parent)
{
}

bool LedaMakeStepFactory::canCreate(BuildStepList *parent, const Id id) const
{
    if (parent->target()->project()->id() == Constants::LEDAPROJECT_ID)
        return id == LEDA_MS_ID;
    return false;
}

BuildStep *LedaMakeStepFactory::create(BuildStepList *parent, const Id id)
{
    if (!canCreate(parent, id))
        return 0;
    LedaMakeStep *step = new LedaMakeStep(parent);
    if (parent->id() == ProjectExplorer::Constants::BUILDSTEPS_CLEAN) {
        step->setClean(true);
        step->setBuildTarget(QLatin1String("clean"), /* on = */ true);
    } else if (parent->id() == ProjectExplorer::Constants::BUILDSTEPS_BUILD) {
        step->setBuildTarget(QLatin1String("all"), /* on = */ true);
    }
    return step;
}

bool LedaMakeStepFactory::canClone(BuildStepList *parent, BuildStep *source) const
{
    return canCreate(parent, source->id());
}

BuildStep *LedaMakeStepFactory::clone(BuildStepList *parent, BuildStep *source)
{
    if (!canClone(parent, source))
        return 0;
    LedaMakeStep *old(qobject_cast<LedaMakeStep *>(source));
    Q_ASSERT(old);
    return new LedaMakeStep(parent, old);
}

bool LedaMakeStepFactory::canRestore(BuildStepList *parent, const QVariantMap &map) const
{
    return canCreate(parent, idFromMap(map));
}

BuildStep *LedaMakeStepFactory::restore(BuildStepList *parent, const QVariantMap &map)
{
    if (!canRestore(parent, map))
        return 0;
    LedaMakeStep *bs(new LedaMakeStep(parent));
    if (bs->fromMap(map))
        return bs;
    delete bs;
    return 0;
}

QList<Id> LedaMakeStepFactory::availableCreationIds(BuildStepList *parent) const
{
    if (parent->target()->project()->id() == Constants::LEDAPROJECT_ID)
        return QList<Id>() << Id(LEDA_MS_ID);
    return QList<Id>();
}

QString LedaMakeStepFactory::displayNameForId(const Id id) const
{
    if (id == LEDA_MS_ID)
        return QCoreApplication::translate("LedaProjectManager::Internal::LedaMakeStep",
                                           LEDA_MS_DISPLAY_NAME);
    return QString();
}

} // namespace Internal
} // namespace LedaProjectManager
