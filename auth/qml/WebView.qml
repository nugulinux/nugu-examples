import QtQuick 2.0
import QtQuick.Controls 2.0
import QtWebEngine 1.8

Item {
    anchors.fill: parent

    property var onRedirect

    function loadUrl(requestUrl) {
        webview.url = requestUrl
        webview.visible = true
    }

    function close() {
        webview.visible = false
        webview.url = "about:blank"
    }

    WebEngineView {
        id: webview
        anchors.fill: parent
        visible: false
        onLoadingChanged: {
            if (onRedirect) {
                onRedirect(loadRequest.url)
            }
        }
    }

    Button {
        width: 30
        height: 30
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 15
        background: Rectangle {
            color: "#7e7e7e"
            radius: 50
        }
        opacity: 0.4
        visible: webview.visible
        text: "X"
        onClicked: {
            close()
        }
    }
}
