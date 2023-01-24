/*
 * Copyright (c) 2019 SK Telecom Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __NUGU_AUTH_HELPER_H__
#define __NUGU_AUTH_HELPER_H__

#include <QApplication>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>
#include <memory>

class NuguAuthHelper : public QObject {
    Q_OBJECT
public:
    explicit NuguAuthHelper(QObject* parent = nullptr);
    virtual ~NuguAuthHelper() = default;

    Q_INVOKABLE void openConfig(const QUrl& file_name);
    Q_INVOKABLE void saveConfig(const QUrl& file_name);
    Q_INVOKABLE void discovery();
    Q_INVOKABLE void authByAuthorization(const QString& device_serial);
    Q_INVOKABLE void authByCredential(const QString& device_serial);
    Q_INVOKABLE void handleRedirect(const QUrl& url);
    Q_INVOKABLE void refreshToken();
    Q_INVOKABLE void revoke();
    Q_INVOKABLE void reset();

signals:
    void configLoaded(const QString& config_data, const QString& device_serial = "");
    void loadUrl(const QUrl& url);
    void close();
    void authReceived(const QString& auth_data);

private slots:
    void finished(QNetworkReply* reply);

private:
    using NuguConfig = struct _NuguConfig {
        QString server_url;
        QString client_id;
        QString client_secret;
        QString redirect_uri;
        QString poc_id;
        QString device_type_code;
    };

    using DiscoveryInfo = struct _DiscoveryInfo {
        QString discovery_url;
        QString authorization_endpoint;
        QString token_endpoint;
        QString revocation_endpoint;
    };

    QString getRequestDeviceSerial();
    int getTokenExpiredTime();
    void handleDiscoveryResult();
    void requestToken(const QString& auth_code);
    void clearAuthData();
    void post(const QUrl& url, const QUrlQuery& data);

    const QString AUTH_ENDPOINT_DISCOVERY = "/.well-known/oauth-authorization-server/";
    const QString AUTH_ENDPOINT_AUTHORIZE = "/v1/auth/oauth/authorize";
    const QString AUTH_ENDPOINT_TOKEN = "/v1/auth/oauth/token";
    const QString AUTH_ENDPOINT_REVOKE = "/v1/auth/oauth/revoke";

    std::shared_ptr<QNetworkAccessManager> network_manager = nullptr;
    NuguConfig nugu_config {};
    DiscoveryInfo discovery_info {};
    QJsonDocument auth_data;
    QString device_serial;
};

#endif // __NUGU_AUTH_HELPER_H__
