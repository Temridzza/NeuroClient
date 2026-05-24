//
// Created by temridzza on 11/29/25.
//

#ifndef CHATGPT_SIDEPANEL_H
#define CHATGPT_SIDEPANEL_H

#pragma once
#include <QWidget>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QTimer>

class SidePanel : public QWidget {
    Q_OBJECT
public:
    explicit SidePanel(QWidget *parent = nullptr);

    void showPanel();
    void hidePanel();
    void loadChats(const QList<QVariantMap> &chats);

    signals:
        void chatSelected(const QString &name);

protected:
    // void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QPropertyAnimation *anim;
    QTimer hideTimer;
    QListWidget *chatList;
    QList<QVariantMap> chats;

private:
    QPushButton *newChatBtn;

private slots:
    void createNewChat();
};




#endif //CHATGPT_SIDEPANEL_H