import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2
import QtWebEngine 1.8

import NuguAuth 1.0

ApplicationWindow {
    id: main_window
    width: 720
    height: 1600
    visible: true

    property var actionButtonWidth: main_window.width / 2 - actionButtonMargin * 2
    property var actionButtonHeight: 40
    property var actionButtonMargin: 10

    function reset() {
        device_serial_value.text = ""
        load_config.textInput = ""
        config_info.contents = ""
        auth_info.contents = ""
        auth_token.textInput = ""

        authorization_auth.enabled = false
        credential_auth.enabled = false
        refresh_token.enabled = false
        revoke.enabled = false
        save_config.enabled = false
    }

    function checkDeviceSerial() {
        if (device_serial_value.text == "") {
            message_dialog.text = "Please Input Device Serial"
            message_dialog.open()
            device_serial_value.forceActiveFocus()

            return false
        }

        return true
    }

    Component.onCompleted : {
        main_window.reset()
    }

    NuguAuth {
        id: nugu_auth
    }

    Connections {
        target: nugu_auth

        onConfigLoaded: {
            device_serial_value.text = device_serial
            config_info.contents = config_data

            if (config_info.contents) {
                authorization_auth.enabled = true
                credential_auth.enabled = true
            }
        }

        onLoadUrl: {
            webview.loadUrl(url)
        }

        onClose: {
            webview.close()
        }

        onAuthReceived: {
            var authInfo = auth_data ? JSON.parse(auth_data) : {}
            auth_info.contents = auth_data

            auth_token.textInput = authInfo.access_token
            refresh_token.enabled = authInfo.refresh_token ? true : false
            revoke.enabled = authInfo.access_token ? true : false
            save_config.enabled = authInfo.access_token ? true : false
        }
    }

    // input device serial
    Label {
        id: device_serial_title
        width: 100
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 15
        anchors.leftMargin: 15
        font.bold: true
        text: "Device Serial: "
    }

    Rectangle {
        id: device_serial
        width: 250
        height: 50
        anchors.top: device_serial_title.top
        anchors.bottom: device_serial_title.bottom
        anchors.left: device_serial_title.right
        anchors.topMargin: -5
        anchors.bottomMargin: -5
        anchors.rightMargin: 5
        border.color: "#949494"
        border.width: 1

        TextEdit {
            id: device_serial_value
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            verticalAlignment: TextEdit.AlignVCenter
            layer.enabled: true
        }
    }

    // reset
    Button {
        id: reset
        width: 80
        anchors.top: device_serial.top
        anchors.bottom: device_serial.bottom
        anchors.right: parent.right
        anchors.rightMargin: 15
        text: "RESET"
        onClicked: {
            nugu_auth.reset()
            main_window.reset()
        }
    }

    // input nugu-config.json
    TextInputForm {
        id: load_config
        anchors.top: device_serial_title.bottom
        title: "Config File :   "
        actionName: "OPEN"
        onAction: function() {
            config_file_dialog.opens(function(fileUrl) {
                load_config.textInput = fileUrl
                nugu_auth.openConfig(fileUrl)
            }, false)
        }
    }

    // border
    Rectangle {
        id: input_area_border
        height: 1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: load_config.bottom
        anchors.topMargin: 10
        color: "#949494"
    }

    // config info
    TextBoxForm {
        id: config_info
        anchors.top: input_area_border.bottom
        anchors.topMargin: 15
        title: "Nugu Config"
    }

    // auth by tid
    Button {
        id: authorization_auth
        width: actionButtonWidth
        height: actionButtonHeight
        anchors.top: config_info.bottom
        anchors.left: parent.left
        anchors.margins: actionButtonMargin
        enabled: config_info.contents ? true : false
        text: "AUTHORIZATION AUTH"
        onClicked: {
            if (checkDeviceSerial()) {
                nugu_auth.authByAuthorization(device_serial_value.text)
            }
        }
    }

    // auth by credential
    Button {
        id: credential_auth
        width: actionButtonWidth
        height: actionButtonHeight
        anchors.top: config_info.bottom
        anchors.right: parent.right
        anchors.margins: actionButtonMargin
        text: "CREDENTIAL AUTH"
        onClicked: {
            if (checkDeviceSerial()) {
                nugu_auth.authByCredential(device_serial_value.text)
            }
        }
    }

    // auth info
    TextBoxForm {
        id: auth_info
        anchors.top: authorization_auth.bottom
        anchors.topMargin: 15
        title: "Nugu Auth"
    }

    // access token
    TextInputForm {
        id: auth_token
        anchors.top: auth_info.bottom
        title: "Access token : "
        actionName: "COPY"
        actionToolTip: "Access token is copied to clipboard."
        onAction: function() {
            copy()
        }
    }

    // refresh token
    Button {
        id: refresh_token
        width: actionButtonWidth
        height: actionButtonHeight
        anchors.top: auth_token.bottom
        anchors.left: parent.left
        anchors.margins: actionButtonMargin
        text: "REFRESH TOKEN"
        onClicked: {
            nugu_auth.refreshToken()
        }
    }

    // revoke
    Button {
        id: revoke
        width: actionButtonWidth
        height: actionButtonHeight
        anchors.top: auth_token.bottom
        anchors.right: parent.right
        anchors.margins: actionButtonMargin
        text: "REVOKE"
        onClicked: {
            nugu_auth.revoke()
            auth_token.textInput = ""
        }
    }

    // save config file
    Button {
        id: save_config
        width: parent.width
        height: actionButtonHeight
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: actionButtonMargin
        text: "SAVE CONFIG"
        onClicked: {
            config_file_dialog.opens(function(fileUrl) {
                nugu_auth.saveConfig(fileUrl)
            }, true)
        }
    }

    WebView {
        id: webview
        onRedirect: function(redirect_url) {
            nugu_auth.handleRedirect(redirect_url)
        }
    }

    FileDialog {
        id: config_file_dialog
        property var isSave: false
        property var onAction

        function opens(action, save) {
            onAction = action
            isSave = save
            open()
        }

        modality: Qt.NonModal
        title: isSave ? "Please save a file" : "Please choose a file"
        folder: shortcuts.home
        nameFilters: [ "config file (*.json)", "All files (*)" ]
        selectedNameFilter: "config file (*.json)"
        selectExisting: !isSave
        onAccepted: {
            if (onAction) {
                onAction(fileUrl)
            }
        }
    }

    MessageDialog {
        id: message_dialog
        title: "warning"
    }
}
