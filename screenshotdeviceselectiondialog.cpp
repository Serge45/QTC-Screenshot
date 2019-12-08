#include "screenshotdeviceselectiondialog.h"
#include "ui_screenshotdeviceselectiondialog.h"
#include <QFileDialog>
#include <QStandardPaths>

ScreenshotDeviceSelectionDialog::ScreenshotDeviceSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScreenshotDeviceSelectionDialog)
{
    ui->setupUi(this);
    ui->toolButtonLineEdit->setText(QStandardPaths::locate(QStandardPaths::HomeLocation,
                                                           "",
                                                           QStandardPaths::LocateDirectory));
    connect(ui->toolButton, &QToolButton::clicked, [this] () {
        const auto res = QFileDialog::getExistingDirectory(this,
                                                           tr("Select Screenshot Output Folder"),
                                                           ui->toolButtonLineEdit->text());
        if (res.size()) {
            ui->toolButtonLineEdit->setText(res);
        }
    });

    connect(this, &QDialog::finished, [this] (int) {
        const QImage::Format availableFormats[] = {
            QImage::Format_RGB16,
            QImage::Format_RGB30,
            QImage::Format_RGB32,
            QImage::Format_RGB444,
            QImage::Format_RGB555,
            QImage::Format_RGB666,
            QImage::Format_RGB888,
        };
        m_screenshotSetting.deviceIp = ui->ipLineEdit->text();
        m_screenshotSetting.imageWidth = ui->widthSpinBox->value();
        m_screenshotSetting.imageHeight = ui->heightSpinBox->value();
        m_screenshotSetting.imageFormat = availableFormats[ui->rgbFormatComboBox->currentIndex()];
        m_screenshotSetting.outputFolder = ui->toolButtonLineEdit->text();
    });
}

ScreenshotDeviceSelectionDialog::~ScreenshotDeviceSelectionDialog()
{
    delete ui;
}
