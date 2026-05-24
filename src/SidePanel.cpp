//
// Created by temridzza on 11/29/25.
//

#include "../include/SidePanel.h"

#include "SidePanel.h"
#include <QDebug>
#include <QLabel>

#include "SidePanel.h"
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>

SidePanel::SidePanel(QWidget *parent)
    : QWidget(parent)
{
    setFixedWidth(220);

    // ---- СТИЛЬ ПАНЕЛИ ----
    setStyleSheet(R"(
        QWidget {
            background-color: rgba(25, 25, 25, 180);
            border: 1px solid rgba(255,255,255,40);
            border-radius: 10px;
        }
        QListWidget {
            background: transparent;
            border: none;
            color: white;
            font-size: 14px;
        }
        QListWidget::item {
            padding: 8px;
            border-radius: 6px;
        }
        QListWidget::item:hover {
            background-color: rgba(255,255,255,35);
        }
        QListWidget::item:selected {
            background-color: rgba(255,255,255,60);
        }
    )");

    auto shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(25);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(0, 0, 0, 150));
    setGraphicsEffect(shadow);

    // ---- ВЕРСТКА ----
    auto *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel("Ваши чаты");
    title->setStyleSheet("color: white; font-size: 16px; padding: 8px;");
    layout->addWidget(title);

    // Кнопка "Новый чат"
    newChatBtn = new QPushButton("+ Новый чат", this);
        newChatBtn->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(255,255,255,40);
            border: none;
            color: white;
            padding: 8px;
            border-radius: 6px;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: rgba(255,255,255,70);
        }
    )");
        layout->addWidget(newChatBtn);

    connect(newChatBtn, &QPushButton::clicked, this, &SidePanel::createNewChat);

    chatList = new QListWidget(this);
    layout->addWidget(chatList);

    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    // Анимация
    anim = new QPropertyAnimation(this, "geometry");
    anim->setDuration(250);
    anim->setEasingCurve(QEasingCurve::OutCubic);

    hideTimer.setSingleShot(true);
    hideTimer.setInterval(500);

    connect(&hideTimer, &QTimer::timeout, this, &SidePanel::hidePanel);

    // Сигнал выбора чата
    connect(chatList, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
        QString id = item->data(Qt::UserRole).toString();
        emit chatSelected(id);
    });
}

void SidePanel::loadChats(const QList<QVariantMap> &chats)
{
    chatList->clear();


    for (const auto &chat : chats) {
        auto *item = new QListWidgetItem(chat["title"].toString());
        item->setData(Qt::UserRole, chat["id"]);
        chatList->addItem(item);
    }
}

void SidePanel::showPanel() {
    hideTimer.stop();

    int parentW = parentWidget()->width();
    QRect r = geometry();

    anim->stop();
    anim->setStartValue(r);
    anim->setEndValue(QRect(parentW - width(), r.y(), width(), r.height()));
    anim->start();
}

void SidePanel::hidePanel() {
    int parentW = parentWidget()->width();
    QRect r = geometry();

    anim->stop();
    anim->setStartValue(r);

    // панель спрятана, но не вышла за пределы окна полностью!
    int hiddenX = parentW - 2;

    anim->setEndValue(QRect(hiddenX, r.y(), width(), r.height()));
    anim->start();
}


// void SidePanel::enterEvent(QEnterEvent *) {
//
//     showPanel();
// }

void SidePanel::leaveEvent(QEvent *) {
    hideTimer.start();
}

#include "database/ChatRepository.h"

void SidePanel::createNewChat()
{
    ChatRepository repo;

    // создаём новый чат в базе
    QString newId = repo.createChat("Новый чат");

    // перегружаем список
    auto list = repo.listChats();
    loadChats(list);

    // находим новый элемент в списке
    for (int i = 0; i < chatList->count(); ++i) {
        auto *item = chatList->item(i);
        if (item->data(Qt::UserRole).toString() == newId) {

            chatList->setCurrentItem(item);

            emit chatSelected(newId);
            break;
        }
    }
}
