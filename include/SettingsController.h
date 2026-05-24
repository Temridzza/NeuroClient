#ifndef CHATGPT_SETTINGSCONTROLLER_H
#define CHATGPT_SETTINGSCONTROLLER_H

#include <QMenu>
#include <QAction>
#include <QSettings>

#include "SettingsWindow.h"

class SettingsController : public QObject
{
    Q_OBJECT

public:
    explicit SettingsController(QObject *parent = nullptr);
    ~SettingsController() override;

    void setupSettingsMenu(QMenu *settingsMenu);
    void setMainWindow(QWidget *mainWindow);

private slots:
    void onSettingsClicked() const;
    void onExitClicked() const;

private:
    QWidget *m_mainWindow;

    QAction *settings = nullptr;
    QAction *editApiKeyPath = nullptr;
    QAction *exitAction = nullptr;
};

#endif //CHATGPT_SETTINGSCONTROLLER_H