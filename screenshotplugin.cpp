#include "screenshotplugin.h"
#include "screenshotconstants.h"
#include "screenshotdeviceselectiondialog.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>
#include <QInputDialog>
#include <QProcess>
#include <QStandardPaths>
#include <QImage>
#include <QLabel>
#include <QDateTime>

namespace Screenshot {
namespace Internal {

ScreenshotPlugin::ScreenshotPlugin()
{
    // Create your members
}

ScreenshotPlugin::~ScreenshotPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool ScreenshotPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    auto action = new QAction(tr("Take a screenshot from remote device"), this);
    Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+Meta+S")));
    connect(action, &QAction::triggered, this, &ScreenshotPlugin::triggerAction);

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("Screenshot"));
    menu->addAction(cmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    return true;
}

void ScreenshotPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag ScreenshotPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void ScreenshotPlugin::triggerAction()
{
    ScreenshotDeviceSelectionDialog d;
    const auto userResponse = d.exec();

    if (userResponse == QDialog::Rejected) {
        return;
    }

    const auto screenshotSetting = d.screenshotSetting();

    if (screenshotSetting.deviceIp.size()) {
        const auto sshRes = QProcess::execute("ssh", {QString("root@%1").arg(screenshotSetting.deviceIp), "cat /dev/fb0 > ~/screen.data"});

        if (sshRes) {
            QMessageBox::warning(Core::ICore::mainWindow(), tr("Error"), tr("Cannot connect to device"));
            return;
        }

        const QString dstPath = QStandardPaths::locate(QStandardPaths::HomeLocation, "screen.data");

        const auto scpRes = QProcess::execute("scp", {QString("root@%1:/home/root/screen.data").arg(screenshotSetting.deviceIp), dstPath});

        if (scpRes != 0) {
            QMessageBox::warning(Core::ICore::mainWindow(), tr("Error"), tr("Cannot copy screenshot from device"));
            return;
        }

        QFile screenData(dstPath);

        screenData.open(QFile::ReadOnly);
        const auto screenBytes = screenData.readAll();
        screenData.close();

        QImage screenImage((const uchar *)screenBytes.data(),
                           screenshotSetting.imageWidth,
                           screenshotSetting.imageHeight,
                           screenshotSetting.imageFormat);

        screenImage.save(QString("%1/screenshot_%2.png").arg(screenshotSetting.outputFolder)
                                                        .arg(QDateTime::currentDateTime().toString()));
        QLabel *imageLabel = new QLabel(nullptr);
        imageLabel->setAttribute(Qt::WA_DeleteOnClose);
        imageLabel->setPixmap(QPixmap::fromImage(screenImage));
        imageLabel->show();
    }
}

} // namespace Internal
} // namespace Screenshot
