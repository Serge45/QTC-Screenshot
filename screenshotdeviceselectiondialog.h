#ifndef SCREENSHOTDEVICESELECTIONDIALOG_H
#define SCREENSHOTDEVICESELECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class ScreenshotDeviceSelectionDialog;
}

class ScreenshotDeviceSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    struct ScreenshotSetting {
        int imageWidth{0};
        int imageHeight{0};
        QImage::Format imageFormat{QImage::Format_RGB16};
        QString deviceIp;
        QString outputFolder;
    };

    explicit ScreenshotDeviceSelectionDialog(QWidget *parent = nullptr);
    ~ScreenshotDeviceSelectionDialog();
    ScreenshotSetting screenshotSetting() const { return m_screenshotSetting; }

private:
    Ui::ScreenshotDeviceSelectionDialog *ui;
    ScreenshotSetting m_screenshotSetting;
};

#endif // SCREENSHOTDEVICESELECTIONDIALOG_H
