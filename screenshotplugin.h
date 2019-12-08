#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include "screenshot_global.h"

#include <extensionsystem/iplugin.h>

namespace Screenshot {
namespace Internal {

class ScreenshotPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Screenshot.json")

public:
    ScreenshotPlugin();
    ~ScreenshotPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private:
    void triggerAction();
};

} // namespace Internal
} // namespace Screenshot

#endif // SCREENSHOT_H
