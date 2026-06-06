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

    // --------------------------------------------------------------
    //  Прокрутка мышью / тачпадом (скорость можно менять)
    // --------------------------------------------------------------
    WheelHandler {
        acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
        onWheel: {
            const factor = 2               // <-- скорость прокрутки
            chatView.contentY -= event.angleDelta.y * factor
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
            implicitHeight: textItem.implicitHeight + pad * 2
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

            // StyledToolButton {
            //     buttonText: Theme.iconCopy
            //     onClicked: App.messages.copyAt(index, MessageModel.RawMarkdown)
            // }

            // ------------------------------------------------------
            //  Текст сообщения
            // ------------------------------------------------------
            TextEdit {
                id: textItem
                anchors.fill: parent
                anchors.margins: bubble.pad

                text: sidebarAnimating ? "…" : (htmlReady ? html : message)
                textFormat: htmlReady ? TextEdit.RichText : TextEdit.PlainText

                readOnly: true
                selectByMouse: true
                persistentSelection: true
                cursorVisible: false
                wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
                renderType: Text.NativeRendering

                color: isUser
                           ? Colors.background
                           : Colors.overSurface
                font.pixelSize: 15


                selectionColor: Qt.rgba(Colors.surfaceTint.r,
                                        Colors.surfaceTint.g,
                                        Colors.surfaceTint.b, 0.2)
                selectedTextColor: Colors.overSurface

                onLinkActivated: Qt.openUrlExternally(link)
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
            //  Помощник измерения ширины текста (не меняем)
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
    //  Синхронизация с изменениями в модели
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