//
// Created by temridzza on 11/28/25.
//

#ifndef CHATGPT_MESSAGEWIDGET_H
#define CHATGPT_MESSAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>


class MessageWidget : public QWidget
{
    Q_OBJECT
public:
    MessageWidget(const QString &text, bool isUser, QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QLabel *label;
};


#endif //CHATGPT_MESSAGEWIDGET_H