import QtQuick 2.0
import QtQuick.Controls 2.0

ApplicationWindow {
    id: root
    width: 1280
    height: 850
    visible: true
    title: qsTr("NUGU SDK GUI Sample")
    color: "black"

    property var controlbarHeight: 50

    Connections {
        target: _nugu
        onASRState: {
            text_command.hide()
            nugu_status.text = state
        }
        onNetworkStatus: {
            if (status === "DISCONNECTED") {
                nugu_status.text = status
            }
        }
        onVolumeChanged: {
            volume_popup.show(volume)
        }
        onMuteChanged: {
            mute_status.visible = mute
        }
    }

    Item {
        width: parent.width
        height: controlbarHeight

        Text {
            id: nugu_status
            height: parent.height
            anchors.left: parent.left
            anchors.leftMargin: 15
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 12
            color: "white"
            text: "IDLE"
        }

        ListModel {
            id: control_bar_model

            ListElement { iconSource: "qrc:/images/text.svg"; command: function() {
                text_command.show()
            }}
            ListElement { iconSource: "qrc:/images/mic.svg"; command: function() {
                _nugu.startListening()
            }}
            ListElement { iconSource: "qrc:/images/volume_up.svg"; command: function() {
                if (mute_status.visible) {
                    _nugu.toggleMute()
                }
                _nugu.volumeUp()
            }}
            ListElement { iconSource: "qrc:/images/volume_down.svg"; command: function() {
                if (mute_status.visible) {
                    _nugu.toggleMute()
                }
                _nugu.volumeDown()
            }}
            ListElement { iconSource: "qrc:/images/mute.svg"; command: function() {
                volume_popup.hide()
                _nugu.toggleMute()

            }}
            ListElement { iconSource: "qrc:/images/stop.svg"; command: function() {
                text_command.hide()
                _nugu.stopListening()
                _nugu.sendTextCommand("그만")
            }}
            ListElement { iconSource: "qrc:/images/power.svg"; command: function() {
                _nugu.powerOff()
            }}
        }

        Row {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 5
            Repeater {
                model: control_bar_model
                Button {
                    icon.source: iconSource
                    icon.color: pressed ? "gray" : "white"
                    background: Rectangle {
                        color: "black"
                    }
                    onClicked: {
                        command()
                    }
                }
            }
        }
    }

    TemplateView {
        id: template_view
        y: controlbarHeight
        width: parent.width
        height: parent.height - controlbarHeight
    }

    VoiceChrome {
        stopper: function() {
            _nugu.stopListening()
        }
    }

    TextCommand {
        id: text_command
        y: controlbarHeight
        textSender: function(text) {
            _nugu.sendTextCommand(text)
        }
    }

    VolumePopup {
        id: volume_popup
    }

    Button {
        id: mute_status
        width: 75
        height: 75
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 75
        icon.source: "qrc:/images/mute_status.svg"
        icon.color: "white"
        opacity: 0.4
        visible: false
        background: Rectangle {
            color: "#7e7e7e"
            radius: 50
        }
        onClicked: {
            _nugu.toggleMute()
        }
    }
}
