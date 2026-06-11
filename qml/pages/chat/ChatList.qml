import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import ChatGPT 1.0
import NeuroClient
import qs.modules.theme
import qs.modules.components
import "../../components"
import "../../theme"

ListView {
    id: chatView
    property bool sidebarAnimating: false

    spacing: 12
    clip: true
    model: App.messages
    maximumFlickVelocity: 12000
    flickDeceleration: 8000
    boundsBehavior: Flickable.StopAtBounds
    boundsMovement: Flickable.StopAtBounds

    // --------------------------------------------------------------
    //  Прокрутка мышью / тачпадом (скорость можно менять)
    // --------------------------------------------------------------
    WheelHandler {
        acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
        onWheel: {
            const factor = 1.5
            let newY = chatView.contentY - event.angleDelta.y * factor

            // Ограничиваем сверху и снизу
            newY = Math.max(0, Math.min(newY, chatView.contentHeight - chatView.height))

            chatView.contentY = newY
            event.accepted = true
        }
    }

    // --------------------------------------------------------------
    //  Таймеры и вспомогательные функции
    // --------------------------------------------------------------
    onContentYChanged: scrollTimer.restart()
    onHeightChanged: tickVisibleRange()
    onCountChanged: tickVisibleRange()

    Timer {
        id: scrollTimer
        interval: 50
        repeat: false
        onTriggered: chatView.tickVisibleRange()
    }

    function tickVisibleRange() {
        // console.log("tickVisibleRange", count)
        var first = indexAt(10, contentY)
        var last  = indexAt(10, contentY + height)

        if (first < 0) first = 0
        if (last  < 0) last  = count - 1

        for (var i = first; i <= last; ++i)
            App.messages.ensureRendered(i)
    }

    Component.onCompleted: tickVisibleRange()

    // --------------------------------------------------------------
    //  Делегат сообщения
    // --------------------------------------------------------------
    delegate: Item {
        id: row
        width: chatView.width
        implicitHeight: bubble.implicitHeight + 8
        readonly property bool isUser: role === "user"
        property bool editing: false
        property string draftText: message

        Rectangle {
            id: bubble
            property int pad: 14
            property int maxBubbleWidth: Math.round(chatView.width * 0.82)
            property bool hovered: false

            // Позиционирование «речевого пузыря» слева/справа
            anchors {
                top: parent.top
                right: isUser ? parent.right : undefined
                left:  !isUser ? parent.left  : undefined
                rightMargin: isUser ? 10 : 0
                leftMargin:  !isUser ? 10 : 0
            }

            // Размеры
            width: Math.min(
                       maxBubbleWidth,
                       Math.max(120, metrics.boundingRect.width + pad * 2)
                   )

            implicitHeight: contentColumn.implicitHeight + pad * 2
            radius: 18

            color: isUser ? Colors.primary : Colors.inverseOnSurface

            border.color: isUser ? Colors.background : Colors.primary

            HoverHandler {
                onHoveredChanged: bubble.hovered = hovered
            }

            // Кнопка‑контекст‑меню (правый клик)
            TapHandler {
                acceptedButtons: Qt.RightButton
                onTapped: contextMenu.popup()
            }

            // ------------------------------------------------------
            //  Текст сообщения
            // ------------------------------------------------------
            Column {
                id: contentColumn
                anchors.fill: parent
                anchors.margins: bubble.pad
                spacing: 6

                TextEdit {
                    id: textItem

                    width: parent.width
                    height: implicitHeight

                    text: row.editing ? row.draftText : (htmlReady ? html : message)

                    textFormat: htmlReady ? TextEdit.RichText : TextEdit.PlainText

                    readOnly: !row.editing
                    selectByMouse: true
                    cursorVisible: row.editing

                    wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
                    renderType: Text.NativeRendering

                    onTextChanged: {
                        if (row.editing) {
                            row.draftText = text
                        }
                    }
                    color: isUser ? Colors.background : Colors.overSurface
                }

                Row {
                    spacing: 6
                    visible: bubble.hovered
                    opacity: visible ? 1 : 0

                    StyledToolButton {
                        visible: !isUser
                        buttonText: "⧉"

                        onClicked: App.messages.copyAt(index, App.PlainText)
                    }

                    StyledToolButton {
                        visible: !isUser
                        buttonText: "⟳"

                        onClicked: {
                            // App.chat.regenerate(index)
                        }
                    }

                    StyledToolButton {
                        visible: isUser
                        buttonText: "🖉"

                        onClicked: {
                            row.editing = true
                            row.draftText = message
                            textItem.forceActiveFocus()
                            textItem.cursorPosition = textItem.length
                        }
                    }

                    StyledToolButton {
                        visible: isUser && row.editing
                        buttonText: "✔"

                        onClicked: {
                            App.messages.updateAt(index, row.draftText)
                            row.editing = false
                        }
                    }

                    StyledToolButton {
                        visible: isUser && row.editing
                        buttonText: "✖"

                        onClicked: {
                            row.draftText = message
                            row.editing = false
                        }
                    }
                }
            }


            // ------------------------------------------------------
            //  Контекстное меню
            // ------------------------------------------------------
            StyledMenu {
                id: contextMenu

                StyledMenu {
                    title: qsTr("Копировать")

                    StyledMenuItem {
                        text: qsTr("Markdown")
                        onTriggered: App.messages.copyAt(index, App.RawMarkdown)
                    }

                    StyledMenuItem {
                        text: qsTr("Текст")
                        onTriggered: App.messages.copyAt(index, App.PlainText)
                    }

                    StyledMenuItem {
                        text: qsTr("Видимый текст")
                        onTriggered: App.messages.copyAt(index, App.VisibleText)
                    }

                    StyledMenuItem {
                        text: qsTr("Выделенное")
                        enabled: textItem.selectedText.length > 0
                        onTriggered: App.messages.copyAt(index, App.PlainText)
                    }
                }

                StyledMenuItem {
                    text: qsTr("Выделить всё")
                    onTriggered: textItem.selectAll()
                }
            }

            // ------------------------------------------------------
            //  Помощник измерения ширины текста
            // ------------------------------------------------------
            TextMetrics {
                id: metrics
                font: textItem.font
                text: sidebarAnimating ? "…" : message
            }
        }
    }

    // --------------------------------------------------------------
    //  Скролл‑бар
    // --------------------------------------------------------------
    ScrollBar.vertical: ScrollBar {
        policy: ScrollBar.AsNeeded
    }

    // --------------------------------------------------------------
    //  Синхронизация
    // --------------------------------------------------------------
    Connections {
        target: App.messages
        function onRowsInserted() {
            chatView.positionViewAtEnd()
            chatView.tickVisibleRange()
        }
    }
    Connections {
        target: App.chat
        function onMessagesChanged() {
            chatView.positionViewAtEnd()
            chatView.tickVisibleRange()
        }
    }
    Connections {
        target: App.chatOrchestrator
        function onChatLoadingFinished(chatId) {
            chatView.positionViewAtEnd()
            chatView.tickVisibleRange()
        }
    }
}