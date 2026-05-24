// SettingsController.cpp
#include "SettingsController.h"
#include "SettingsWindow.h"
#include <QLineEdit>

SettingsController::SettingsController(QObject *parent)
    : QObject(parent)
      , m_mainWindow(nullptr)
{
}

SettingsController::~SettingsController()
{
}

void SettingsController::setupSettingsMenu(QMenu *settingsMenu)
{
    settings = new QAction("Настройки", this);
    editApiKeyPath = new QAction("О приложении", this);
    exitAction = new QAction("Выход", this);

    connect(settings, &QAction::triggered, this, &SettingsController::onSettingsClicked);
    connect(exitAction, &QAction::triggered, this, &SettingsController::onExitClicked);

    settingsMenu->addAction(settings);
    settingsMenu->addAction(editApiKeyPath);
    settingsMenu->addSeparator();
    settingsMenu->addAction(exitAction);

}

void SettingsController::setMainWindow(QWidget *mainWindow)
{
    m_mainWindow = mainWindow;
}

void SettingsController::onSettingsClicked() const {
    SettingsWindow::instance(m_mainWindow)->show();
}

void SettingsController::onExitClicked() const {
    m_mainWindow->deleteLater();
}
