/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "remotelinuxqmltoolingsupport.h"

#include <qmldebug/qmldebugcommandlinearguments.h>

using namespace ProjectExplorer;
using namespace Utils;

namespace RemoteLinux {
namespace Internal {

RemoteLinuxQmlToolingSupport::RemoteLinuxQmlToolingSupport(RunControl *runControl)
    : SimpleTargetRunner(runControl)
{
    setId("RemoteLinuxQmlToolingSupport");

    m_portsGatherer = new PortsGatherer(runControl);
    addStartDependency(m_portsGatherer);

    // The ports gatherer can safely be stopped once the process is running, even though it has to
    // be started before.
    addStopDependency(m_portsGatherer);

    m_runworker = runControl->createWorker(QmlDebug::runnerIdForRunMode(runControl->runMode()));
    m_runworker->addStartDependency(this);
    addStopDependency(m_runworker);
}

void RemoteLinuxQmlToolingSupport::start()
{
    const QUrl serverUrl = m_portsGatherer->findEndPoint();

    m_runworker->recordData("QmlServerUrl", serverUrl);

    QmlDebug::QmlDebugServicesPreset services = QmlDebug::servicesForRunMode(runControl()->runMode());

    Runnable r = runnable();
    QtcProcess::addArg(&r.commandLineArguments,
                       QmlDebug::qmlDebugTcpArguments(services, serverUrl),
                       device()->osType());

    setRunnable(r);

    SimpleTargetRunner::start();
}

} // namespace Internal
} // namespace RemoteLinux
