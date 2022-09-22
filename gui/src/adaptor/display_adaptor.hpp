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

#ifndef __DISPLAY_ADAPTOR_H__
#define __DISPLAY_ADAPTOR_H__

#include <QObject>

#include <extension/display_manager_interface.hh>

using namespace NuguExtension;

class DisplayAdaptor : public QObject,
                       public IDisplayManagerListener {
    Q_OBJECT
public:
    explicit DisplayAdaptor(QObject* parent = nullptr);
    virtual ~DisplayAdaptor();

    // implements IDisplayManagerListener-ITemplateListener
    void renderDisplay(const std::string& id, const std::string& params) override;
    void clearDisplay(const std::string& id) override;
    void sendTemplateCommand(const std::string& command) override;

    // delegates to IDisplayManagerHandler-ITemplateHandler
    Q_INVOKABLE void displayRendered(const QString& id);
    Q_INVOKABLE void displayCleared(const QString& id);
    Q_INVOKABLE void onButtonEvent(const QString& event_type, const QString& json_data);
    Q_INVOKABLE void onContextChanged(const QString& json_context);
    Q_INVOKABLE void onControlResult(const QString& action, const QString& result);
    Q_INVOKABLE void close();
    Q_INVOKABLE void closeAll();
    Q_INVOKABLE void onNuguButtonSelected();
    Q_INVOKABLE void onChipSelected(const QString& text);
    Q_INVOKABLE void showToast(const QString& toast);
    Q_INVOKABLE void invokeActivity(const QString& class_name);
    Q_INVOKABLE void requestTTS(const QString& text);
    Q_INVOKABLE void playerCommand(const QString& command, const QString& param);

    Q_INVOKABLE QString getTemplateServerUrl();
    Q_INVOKABLE QString getInjection();
    Q_INVOKABLE bool isTemplateView(const QString& url);

signals:
    void notifyRenderDisplay(const QString& id, const QString& params);
    void notifyClearDisplay(const QString& id);
    void sendCommand(const QString& command);

private:
    IDisplayManagerHandler* display_manager_handler = nullptr;
    QString template_server_url;
};

#endif // __DISPLAY_ADAPTOR_H__
