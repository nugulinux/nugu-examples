import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    height: 30
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: 15

    property var title
    property var textInput: ""
    property var actionName
    property var actionToolTip: ""
    property var onAction

    function copy() {
        text_input.selectAll()
        text_input.copy()
        text_input.deselect()
    }

    Label {
        id: label
        anchors.left: parent.left
        font.bold: true
        text: title
    }

    Rectangle {
        id: text_input_background
        width: parent.width
        anchors.top: label.top
        anchors.bottom: label.bottom
        anchors.left: label.right
        anchors.right: action_button.left
        anchors.topMargin: -5
        anchors.bottomMargin: -5
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        border.color: "#949494"
        border.width: 1

        TextEdit {
            id: text_input
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            verticalAlignment: TextEdit.AlignVCenter
            layer.enabled: true
            readOnly: true
            text: textInput ? textInput : ""
        }
    }

    Button {
        id: action_button
        width: 80
        anchors.top: text_input_background.top
        anchors.bottom: text_input_background.bottom
        anchors.right: parent.right
        ToolTip.visible: actionToolTip != "" ? hovered : false
        ToolTip.text: actionToolTip
        text: actionName
        onClicked: {
            if (onAction) {
                onAction()
            }
        }
    }
}
