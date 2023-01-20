import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    width: parent.width
    height: label.height + contentsHeight + internal.contentsMargin * 2

    property var title
    property var contents: ""
    property var contentsHeight: internal.contentsHeight

    QtObject {
        id: internal
        property var contentsHeight: 144
        property var contentsMargin: 15
    }

    Label {
        id: label
        anchors.left: parent.left
        anchors.leftMargin: 15
        font.bold: true
        text: title
    }

    Rectangle {
        id: text_info
        height: contentsHeight
        anchors.top: label.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: internal.contentsMargin
        border.color: "#949494"
        border.width: 2

        TextArea {
            anchors.fill: parent
            anchors.margins: 2
            background: Rectangle {
                color: "#ebecf0"
            }
            readOnly: true
            text: contents
        }
    }
}
