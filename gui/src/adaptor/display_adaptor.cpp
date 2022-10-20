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

#include "display_adaptor.hpp"
#include "nugu_adaptor.hpp"

DisplayAdaptor::DisplayAdaptor(QObject* parent)
{
    display_manager_handler = NuguAdaptor::getInstance()->getDisplayManagerHandler();
    display_manager_handler->setListener(this);
    template_server_url = display_manager_handler->getTemplateServerUrl().c_str();
}

/*******************************************************************************
 * define IDisplayManagerListener
 ******************************************************************************/

void DisplayAdaptor::renderDisplay(const std::string& id, const std::string& params)
{
    emit notifyRenderDisplay(QString::fromStdString(id), QString::fromStdString(params));
}

void DisplayAdaptor::clearDisplay(const std::string& id)
{
    emit notifyClearDisplay(QString::fromStdString(id));
}

void DisplayAdaptor::sendTemplateCommand(const std::string& command)
{
    if (command.empty())
        return;

    emit sendCommand(QString::fromStdString(command));
}

/*******************************************************************************
 * delegates to IDisplayManagerHandler
 ******************************************************************************/

void DisplayAdaptor::displayRendered(const QString& id)
{
    invokeMethod(__func__, [=]() {
        display_manager_handler->displayRendered(id.toStdString());
    });
}

void DisplayAdaptor::displayCleared(const QString& id)
{
    if (!id.isEmpty())
        invokeMethod(__func__, [=]() {
            display_manager_handler->displayCleared(id.toStdString());
        });
}

void DisplayAdaptor::onButtonEvent(const QString& event_type, const QString& json_data)
{
    invokeMethod(__func__, [=]() {
        display_manager_handler->onButtonEvent(event_type.toStdString(), json_data.toStdString());
    });
}

void DisplayAdaptor::onContextChanged(const QString& json_context)
{
    invokeMethod(__func__, [=]() {
        display_manager_handler->onContextChanged(json_context.toStdString());
    });
}

void DisplayAdaptor::onControlResult(const QString& action, const QString& result)
{
    invokeMethod(__func__, [=]() {
        display_manager_handler->onControlResult(action.toStdString(), result.toStdString());
    });
}

void DisplayAdaptor::close()
{
    invokeMethod(__func__, [&]() {
        display_manager_handler->close();
    });
}

void DisplayAdaptor::closeAll()
{
    invokeMethod(__func__, [&]() {
        display_manager_handler->closeAll();
    });
}

void DisplayAdaptor::onNuguButtonSelected()
{
    invokeMethod(__func__, [&]() {
        display_manager_handler->onNuguButtonSelected();
    });
}

void DisplayAdaptor::onChipSelected(const QString& text)
{
    invokeMethod(__func__, [=]() {
        display_manager_handler->onChipSelected(text.toStdString());
    });
}

void DisplayAdaptor::showToast(const QString& toast)
{
    invokeMethod(__func__, [=]() {
        display_manager_handler->showToast(toast.toStdString());
    });
}

void DisplayAdaptor::invokeActivity(const QString& class_name)
{
    invokeMethod(__func__, [=]() {
        display_manager_handler->invokeActivity(class_name.toStdString());
    });
}

void DisplayAdaptor::requestTTS(const QString& text)
{
    invokeMethod(__func__, [=]() {
        display_manager_handler->requestTTS(text.toStdString());
    });
}

void DisplayAdaptor::playerCommand(const QString& command, const QString& param)
{
    invokeMethod(__func__, [=]() {
        display_manager_handler->playerCommand(command.toStdString(), param.toStdString());
    });
}

/*******************************************************************************
 * define pure TemplateAdaptor
 ******************************************************************************/

QString DisplayAdaptor::getTemplateServerUrl()
{
    return template_server_url;
}

QString DisplayAdaptor::getInjection()
{
    return "new QWebChannel(qt.webChannelTransport, function (channel) { Android = channel.objects.backend; });";
}

bool DisplayAdaptor::isTemplateView(const QString& url)
{
    return url == template_server_url;
}
