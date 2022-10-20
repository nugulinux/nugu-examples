import QtQuick 2.0
import QtQuick.Controls 2.0
import QtWebEngine 1.8

Item {
    anchors.fill: parent

    property var stopper

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        visible: voice_chrome_window.visible

        MouseArea {
            anchors.fill: parent
            onClicked: {
                stopper()
            }
        }
    }

    Item {
        id: voice_chrome_window
        y: parent.height - height
        width: parent.width
        height: 100
        visible: false

        Connections {
            target: _voicechrome
            onRender: {
                var jsonAttribute = JSON.parse(attribute)

                voice_chrome_window.visible = jsonAttribute.window_visible
                voice_chrome_stt_guide.visible = jsonAttribute.stt_guide_visible
                voice_chrome_stt.visible = jsonAttribute.stt_visible
                voice_chrome_chips_container.visible = jsonAttribute.utterance_visible
                voice_chrome_chips_container.expectSpeech = jsonAttribute.expect_speech

                voice_chrome_indicator.renderIndicator(jsonAttribute.indicator)

                if (jsonAttribute.stt_clear) {
                    voice_chrome_stt.text = ""
                }

                voice_chrome_chips_list.model.clear()
                jsonAttribute.chips.forEach(chip => {
                    voice_chrome_chips_list.model.append({
                        chip_text: chip.text,
                        chip_token: chip.token,
                        chip_type: chip.type})
                })
            }
            onAsrResult: {
                voice_chrome_stt.text = asr_result
            }
        }

        Rectangle {
            width: parent.width
            height: parent.height
            color: "#2d3339"
        }

        // status
        Item {
            id: voice_chrome_status
            z: 1
            width: getWidth()
            height: parent.height

            function getWidth() {
                var totalWidth = 0

                for (var i = 1; i < voice_chrome_status.children.length; i++) {
                    var child = voice_chrome_status.children[i]

                    if (child.visible) {
                        totalWidth += (child.width + child.anchors.leftMargin)
                    }
                }

                return totalWidth
            }

            Rectangle {
                width: parent.width
                height: parent.height
                color: "#2d3339"
            }

            // indicator
            Item {
                id: voice_chrome_indicator
                width: 70
                height: 70
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 30

                WebEngineView {
                    id: lottie
                    anchors.fill: parent
                    url: "qrc:/scripts/voice_chrome_indicator.html"
                    backgroundColor: "transparent"
                }

                function renderIndicator(indicator) {
                    if (voice_chrome_window.visible) {
                        lottie.runJavaScript("renderIndicator('%1','%2')".arg(indicator.preload).arg(indicator.main))
                    } else {
                        lottie.runJavaScript("clearIndicator()")
                    }
                }
            }

            // stt guide
            Text {
                id: voice_chrome_stt_guide
                height: parent.height
                anchors.left: voice_chrome_indicator.right
                anchors.leftMargin: 30
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 18
                color: "white"
                text: "말씀해주세요"
                visible: false
            }

            // stt
            Text {
                id: voice_chrome_stt
                height: parent.height
                anchors.left: voice_chrome_indicator.right
                anchors.leftMargin: 30
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 18
                color: "white"
                visible: false
            }
        }

        // Chips
        Item {
            id: voice_chrome_chips_container
            z: voice_chrome_status.z - 1
            width: parent.width
            height: parent.height
            anchors.left: voice_chrome_status.right
            anchors.leftMargin: 30

            property bool expectSpeech: false

            Component {
                id: voice_chrome_chips_item

                Button {
                    height: 45
                    anchors.verticalCenter: parent ? parent.verticalCenter : undefined
                    leftPadding: 20
                    rightPadding: 20
                    background: Rectangle {
                        color: pressed? "#555e63" : "#37444e"
                        radius: 30
                    }
                    contentItem: Text {
                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: 13
                        color: (chip_type === "ACTION") ? "#55beff" : "white"
                        text: chip_text
                    }
                    onClicked: {
                        _nugu.sendTextCommand(chip_text, voice_chrome_chips_container.expectSpeech, chip_token)
                    }
                }
            }

            ListView {
                id: voice_chrome_chips_list
                anchors.fill: parent
                spacing: 10
                orientation: ListView.Horizontal
                model: ListModel {}
                delegate: voice_chrome_chips_item
            }
        }
    }
}
