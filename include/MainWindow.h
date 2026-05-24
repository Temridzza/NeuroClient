#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ChatGPTClient.h"

#include <QMainWindow>
#include <QApplication>
#include <QWidget>
#include <QCloseEvent>

#include "SettingsController.h"
#include "SidePanel.h"
#include <QVector>

#include "MessageWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget {
Q_OBJECT

public:
    static MainWindow* instance(QWidget *parent = nullptr);
    explicit MainWindow(QWidget *parent = nullptr);
    void addMessage(const QString &text, bool isUser);
    void clearMessages();

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    SidePanel *sidePanel = nullptr;

    ChatGPTClient *client;
    QString textEdit;

    void setStyleSheet() const;

    SettingsController* settingsController = nullptr;
    QMenu* settingsMenu = nullptr;

    void closeEvent(QCloseEvent *event)
    {
        this->hide();
        event->ignore();
    }

    QVector<MessageWidget> *messages = new QVector<MessageWidget>;


private slots:
    void on_sendBtn_clicked();
    void on_settingsBtn_clicked();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;

};

#endif //MAINWINDOW_H
