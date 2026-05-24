#include "MessageWidget.h"
#include <QResizeEvent>
#include <QDebug>

MessageWidget::MessageWidget(const QString &text, bool isUser, QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background: transparent;");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 5, 10, 5);
    layout->setSpacing(6);

    label = new QLabel(text, this);
    label->setWordWrap(true);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    // --- ПЕРВОНАЧАЛЬНАЯ ШИРИНА ПО РОДИТЕЛЮ ---
    int parentWidth = parent ? parent->width() : 400;
    int maxWidth = parentWidth * 0.7;
    if (maxWidth < 200) maxWidth = 200;
    label->setMaximumWidth(maxWidth);

    // ----- GitHub стили -----
    if (isUser) {
        label->setStyleSheet(
            "background:#ddf4ff;"
            "border:1px solid #b6e3ff;"
            "padding:8px;"
            "border-radius:12px;"
            "color:#24292e;"
        );
        layout->addStretch();
        layout->addWidget(label);
    } else {
        label->setStyleSheet(
            "background:#f6f8fa;"
            "border:1px solid #d0d7de;"
            "padding:8px;"
            "border-radius:12px;"
            "color:#24292e;"
        );
        layout->addWidget(label);
        layout->addStretch();
    }

    label->adjustSize();
    updateGeometry();
}

void MessageWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // ширина контейнера, а не самого bubble
    QWidget *par = parentWidget();
    int containerWidth = par ? par->width() : width();

    int newMaxWidth = containerWidth * 0.7;
    if (newMaxWidth < 200) newMaxWidth = 200;

    label->setMaximumWidth(newMaxWidth);
    label->adjustSize();

    updateGeometry();
}