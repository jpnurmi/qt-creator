#pragma once

#include <extensionsystem/iplugin.h>

namespace Gcode {

class GcodePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Gcode.json")

public:
    ~GcodePlugin() final;

private:
    bool initialize(const QStringList &arguments, QString *errorString) final;
    void extensionsInitialized() final;
};

} // namespace Gcode
