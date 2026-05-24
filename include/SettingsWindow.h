//
// Created by temridzza on 11/27/25.
//

#ifndef CHATGPT_SETTINGSWINDOW_H
#define CHATGPT_SETTINGSWINDOW_H

#pragma once

#include <QDialog>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
    class SettingsWindow;
}

class SettingsWindow : public QDialog {
    Q_OBJECT

public:
    ~SettingsWindow();
    static SettingsWindow* instance(QWidget *parent = nullptr);

    QJsonObject getUpdatedConfig() const;

private slots:
    void on_saveBtn_clicked();
    void on_cancelBtn_clicked();

    void onModelsListContextMenu(const QPoint &pos);

private:
    Ui::SettingsWindow *ui;
    explicit SettingsWindow(QWidget *parent = nullptr);

    static SettingsWindow *self;

    void on_addModelBtn_clicked();
    void on_editModelBtn_clicked();
    void on_removeModelBtn_clicked();
    void reloadModelsList();
};

#endif //CHATGPT_SETTINGSWINDOW_H
