import QtQuick 2.0

Item {
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: 300
    height: 200
    visible: false

    function show(volume) {
        volume_popup_timer.stop()
        volume_popup_level.text = volume
        volume_popup.visible = true
        volume_popup_timer.start()
    }

    function hide() {
        volume_popup_timer.stop()
        volume_popup.visible = false
    }

    Timer {
        id: volume_popup_timer
        interval: 2000
        onTriggered: volume_popup.visible = false
    }

    Rectangle {
        anchors.fill: parent
        opacity: 0.8
        color: "#7e7e7e"
        radius: 15
    }

    Text {
        id: volume_popup_level
        text: "40"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 40
        font.pointSize: 48
        font.bold: true
        color: "white"
    }

    Text {
        text: "%"
        anchors.bottom: volume_popup_level.bottom
        anchors.left: volume_popup_level.right
        anchors.bottomMargin: 8
        font.pointSize: 25
        font.bold: true
        color: "white"
    }

    Image {
        source: "qrc:/images/volume_level.svg"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: volume_popup_level.top
        anchors.bottomMargin: 5
    }
}
