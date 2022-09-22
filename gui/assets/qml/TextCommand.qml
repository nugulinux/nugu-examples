import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: text_input
    width: parent.width / 3
    height: 50
    anchors.right: parent.right
    anchors.margins: 5
    opacity: 0.7
    visible: false

    property var textSender

    function show() {
        visible = true
    }

    function hide() {
        visible = false
    }

    Rectangle {
        anchors.fill: parent
        color: "white"
        border.width: 3
        border.color: "gray"
        radius: 5
    }

    TextInput {
        id: text_input_field
        width: parent.width - text_input_send.width
        anchors.left: parent.left
        anchors.right: text_input_send.left
        anchors.margins: 10
        anchors.verticalCenter: parent.verticalCenter
        clip: true
        font.pixelSize: 20
        font.bold: true
        focus: true
    }

    Button {
        id: text_input_send
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 5
        icon.source: "qrc:/images/send_text.svg"
        icon.color: pressed ? "gray" : "black"
        background: Rectangle {
            color: "transparent"
        }
        onClicked: {
            let sendingText = text_input_field.text.trim()

            if (sendingText.length > 0) {
                text_input.textSender(sendingText)
                text_input_field.text = ""
                text_input.visible = false
            }
        }
    }
}
