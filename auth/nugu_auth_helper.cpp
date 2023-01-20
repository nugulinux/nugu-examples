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

#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QUrlQuery>
#include <chrono>

#include "nugu_auth_helper.hh"

NuguAuthHelper::NuguAuthHelper(QObject* parent)
    : QObject(parent)
    , network_manager(std::make_shared<QNetworkAccessManager>(this))
{
    connect(network_manager.get(), &QNetworkAccessManager::finished, this, &NuguAuthHelper::finished);
}

void NuguAuthHelper::openConfig(const QUrl& file_name)
{
    if (file_name.isEmpty() || !file_name.isLocalFile()) {
        qDebug() << "The file name is empty or not local file.";
        return;
    }

    QFile file(file_name.toLocalFile());

    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QJsonDocument config_data = QJsonDocument::fromJson(file.readAll());
        nugu_config.server_url = config_data["OAuthServerUrl"].toString();
        nugu_config.client_id = config_data["OAuthClientId"].toString();
        nugu_config.client_secret = config_data["OAuthClientSecret"].toString();
        nugu_config.redirect_uri = config_data["OAuthRedirectUri"].toString();
        nugu_config.poc_id = config_data["PoCId"].toString();
        nugu_config.device_type_code = config_data["DeviceTypeCode"].toString();

        discovery();

        emit configLoaded(config_data.toJson(QJsonDocument::Indented));
    } else {
        qDebug() << "It's failed to open the config file.";
    }
}

void NuguAuthHelper::saveConfig(const QUrl& file_name)
{
    if (file_name.isEmpty() || !file_name.isLocalFile()) {
        qDebug() << "The file name is empty or not local file.";
        return;
    }

    QJsonObject config_auth(auth_data.object());

    if (auto expired_time = getTokenExpiredTime())
        config_auth.insert("expired_time", expired_time);

    QJsonObject config_device;
    config_device.insert("client_id", nugu_config.client_id);
    config_device.insert("client_secret", nugu_config.client_secret);
    config_device.insert("serial_number", device_serial);
    config_device.insert("device_type_code", nugu_config.device_type_code);
    config_device.insert("poc_id", nugu_config.poc_id);

    QJsonObject config;
    config.insert("auth_server", nugu_config.server_url);
    config.insert("auth", config_auth);
    config.insert("device", config_device);

    QJsonDocument document;
    document.setObject(config);

    // write to file
    QFile config_file(file_name.toLocalFile());

    if (config_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream input_stream(&config_file);
        input_stream.setCodec("utf-8");
        input_stream << document.toJson(QJsonDocument::Indented);
    } else {
        qDebug() << "It's failed to save the config file.";
    }
}

void NuguAuthHelper::discovery()
{
    discovery_info.discovery_url
        .append(nugu_config.server_url)
        .append(AUTH_ENDPOINT_DISCOVERY)
        .append(nugu_config.client_id);

    QNetworkRequest request(QUrl(discovery_info.discovery_url));
    network_manager->get(request);
}

void NuguAuthHelper::authByAuthorization(const QString& device_serial)
{
    if (device_serial.isEmpty()) {
        qDebug() << "The device serial is empty.";
        return;
    }

    this->device_serial = device_serial;

    QUrlQuery request_query;
    request_query.addQueryItem("response_type", "code");
    request_query.addQueryItem("client_id", nugu_config.client_id);
    request_query.addQueryItem("redirect_uri", nugu_config.redirect_uri);
    request_query.addQueryItem("data", getRequestDeviceSerial());

    QUrl server_url = QUrl(discovery_info.authorization_endpoint);
    server_url.setQuery(request_query);

    emit loadUrl(server_url);
}

void NuguAuthHelper::authByCredential(const QString& device_serial)
{
    if (device_serial.isEmpty()) {
        qDebug() << "The device serial is empty.";
        return;
    }

    this->device_serial = device_serial;

    QUrl server_url = QUrl(discovery_info.token_endpoint);
    QUrlQuery request_query;
    request_query.addQueryItem("grant_type", "client_credentials");
    request_query.addQueryItem("client_id", nugu_config.client_id);
    request_query.addQueryItem("client_secret", nugu_config.client_secret);
    request_query.addQueryItem("data", getRequestDeviceSerial());

    post(server_url, request_query);
}

void NuguAuthHelper::handleRedirect(const QUrl& url)
{
    if (url.matches(QUrl(nugu_config.redirect_uri), QUrl::RemoveQuery)) {
        QUrlQuery query(url);
        requestToken(query.queryItemValue("code"));

        emit close();
    }
}

void NuguAuthHelper::refreshToken()
{
    QUrl server_url = QUrl(discovery_info.token_endpoint);
    QUrlQuery request_query;
    request_query.addQueryItem("grant_type", "refresh_token");
    request_query.addQueryItem("refresh_token", auth_data["refresh_token"].toString());
    request_query.addQueryItem("client_id", nugu_config.client_id);
    request_query.addQueryItem("client_secret", nugu_config.client_secret);
    request_query.addQueryItem("data", getRequestDeviceSerial());

    post(server_url, request_query);
}

void NuguAuthHelper::revoke()
{
    QUrl server_url = QUrl(discovery_info.revocation_endpoint);
    QUrlQuery request_query;
    request_query.addQueryItem("token", auth_data["access_token"].toString());
    request_query.addQueryItem("client_id", nugu_config.client_id);
    request_query.addQueryItem("client_secret", nugu_config.client_secret);

    post(server_url, request_query);
}

void NuguAuthHelper::reset()
{
    nugu_config = {};
    discovery_info = {};
    device_serial.clear();
    clearAuthData();
}

void NuguAuthHelper::finished(QNetworkReply* reply)
{
    auto request_url = reply->request().url();
    auth_data = QJsonDocument::fromJson(reply->readAll());

    if (request_url == discovery_info.discovery_url)
        handleDiscoveryResult();
    else if (request_url == discovery_info.revocation_endpoint)
        clearAuthData();

    emit authReceived(auth_data.toJson(QJsonDocument::Indented));
}

QString NuguAuthHelper::getRequestDeviceSerial()
{
    QJsonObject request_query_data;
    request_query_data.insert("deviceSerialNumber", device_serial);

    QJsonDocument document(request_query_data);

    return document.toJson(QJsonDocument::Compact);
}

int NuguAuthHelper::getTokenExpiredTime()
{
    QStringList token_items = auth_data["access_token"].toString().split(".");
    QJsonDocument token_payload;

    if (token_items.size() > 1)
        token_payload = QJsonDocument::fromJson(QByteArray::fromBase64(token_items.at(1).toLocal8Bit()));

    auto expired_time = token_payload.object().contains("exp") ? token_payload["exp"].toInt() : 0;

    if (expired_time == 0) {
        using namespace std::chrono;
        auto current_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() / 1000;
        expired_time = current_time + auth_data["expires_in"].toInt();
    }

    return expired_time;
}

void NuguAuthHelper::handleDiscoveryResult()
{
    discovery_info.authorization_endpoint = auth_data["authorization_endpoint"].toString();
    discovery_info.token_endpoint = auth_data["token_endpoint"].toString();
    discovery_info.revocation_endpoint = auth_data["revocation_endpoint"].toString();

    if (discovery_info.authorization_endpoint.isEmpty())
        discovery_info.authorization_endpoint = nugu_config.server_url + AUTH_ENDPOINT_AUTHORIZE;
    if (discovery_info.token_endpoint.isEmpty())
        discovery_info.token_endpoint = nugu_config.server_url + AUTH_ENDPOINT_TOKEN;
    if (discovery_info.revocation_endpoint.isEmpty())
        discovery_info.revocation_endpoint = nugu_config.server_url + AUTH_ENDPOINT_REVOKE;

    clearAuthData();
}

void NuguAuthHelper::requestToken(const QString& auth_code)
{
    QUrl server_url = QUrl(nugu_config.server_url + AUTH_ENDPOINT_TOKEN);
    QUrlQuery request_query;
    request_query.addQueryItem("grant_type", "authorization_code");
    request_query.addQueryItem("code", auth_code);
    request_query.addQueryItem("redirect_uri", nugu_config.redirect_uri);
    request_query.addQueryItem("client_id", nugu_config.client_id);
    request_query.addQueryItem("client_secret", nugu_config.client_secret);
    request_query.addQueryItem("data", getRequestDeviceSerial());

    post(server_url, request_query);
}

void NuguAuthHelper::clearAuthData()
{
    QJsonDocument empty_data;
    auth_data.swap(empty_data);
}

void NuguAuthHelper::post(const QUrl& url, const QUrlQuery& data)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    network_manager->post(request, data.toString(QUrl::FullyEncoded).toUtf8());
}
