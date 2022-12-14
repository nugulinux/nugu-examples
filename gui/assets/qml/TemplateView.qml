import QtQuick 2.0
import QtQuick.Controls 2.0
import QtWebEngine 1.8
import QtWebChannel 1.0
import Template 1.0

Item {
    property var showProgress: false

    Template {
        id: backendObject
        WebChannel.id: "backend"
    }

    WebChannel {
        id: webchannel
        registeredObjects: [backendObject]
    }

    WebEngineView {
        id: webview
        width: parent.width
        height: parent.height
        url: defaultPage
        webChannel: webchannel

        property var defaultPage: "qrc:/images/nugu_logo.png"
        property string template_server
        property bool isLoaded: false
        property string renderId
        property string pendingCommand
        property variant pendingCommands: []

        function clearDisplay() {
            webview.url = defaultPage
        }

        onLoadingChanged: {
            if (loadRequest.status === WebEngineView.LoadSucceededStatus) {
                if (backendObject.isTemplateView((url))) {
                    loading_progress_bar.hide()

                    runJavaScript(JsWebChannel);
                    runJavaScript(backendObject.getInjection())

                    isLoaded = true

                    if (webview.pendingCommands.length > 0) {
                        webview.pendingCommands.forEach(command => {
                            runJavaScript(command)
                        })
                        webview.pendingCommands = []
                    }

                    backendObject.displayRendered(renderId)
                } else {
                    backendObject.displayCleared(renderId)
                    renderId = ""
                }
            }
        }

        Component.onCompleted: {
            template_server = backendObject.getTemplateServerUrl()
        }

        Connections {
            target: backendObject

            onNotifyRenderDisplay: {
                loading_progress_bar.show()

                webview.isLoaded = false
                webview.renderId = id

                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function () {
                    if (xhttp.readyState === 4 && xhttp.status === 200) {
                        webview.loadHtml(xhttp.responseText, webview.template_server)
                    }
                }

                xhttp.open("POST", webview.template_server, true);
                xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded; charset=UTF-8");
                xhttp.send(params)
            }
            onNotifyClearDisplay: {
                if (webview.renderId === id) {
                    webview.clearDisplay()
                }
            }
            onSendCommand: {
                if (webview.isLoaded) {
                    if (webview.renderId) {
                        webview.runJavaScript(command)
                    }
                } else {
                    webview.pendingCommands.push(command)
                }
            }
        }
    }

    // loading progress bar
    Item {
        id: loading_progress_bar
        anchors.fill: parent
        visible: false

        function show() {
            if (parent.showProgress) {
                visible = true
            }
        }

        function hide() {
            if (parent.showProgress) {
                visible = false
            }
        }

        Rectangle {
            anchors.fill: parent
            color: "black"
            opacity: 0.7

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    // consume click event
                }
            }
        }

        BusyIndicator {
            width: 200
            height: 200
            anchors.centerIn: parent
            palette.dark: "white"
        }
    }
}
