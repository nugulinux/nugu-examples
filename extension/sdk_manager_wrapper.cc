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

#include <iostream>

#include "integrated_state_listener.hh"
#include "sdk_manager_wrapper.hh"
#include "speech_operator_handler.hh"
#include "text_handler.hh"

struct SDKManagerWrapper::Impl {
    std::shared_ptr<SDKManager> sdk_manager = nullptr;
    std::shared_ptr<IntegratedStateListener> integrated_state_listener = nullptr;
    std::shared_ptr<TextHandler> text_listener_with_handler = nullptr;
    std::shared_ptr<SpeechOperatorHandler> speech_operator = nullptr;
    bool is_connected = false;

    Impl()
        : sdk_manager(std::make_shared<SDKManager>())
        , integrated_state_listener(std::make_shared<IntegratedStateListener>())
        , text_listener_with_handler(std::make_shared<TextHandler>())
        , speech_operator(std::make_shared<SpeechOperatorHandler>(sdk_manager))
    {
    }
};

SDKManagerWrapper::SDKManagerWrapper()
    : pimpl(std::unique_ptr<Impl>(new Impl()))
{
    pimpl->sdk_manager->addListener(this);
    pimpl->sdk_manager->addNetworkListener(this);
    pimpl->sdk_manager->getSpeechOperator()->addListener(pimpl->integrated_state_listener.get());
}

SDKManagerWrapper::~SDKManagerWrapper()
{
}

std::shared_ptr<SDKManager> SDKManagerWrapper::getSDKManager()
{
    return pimpl->sdk_manager;
}

void SDKManagerWrapper::start()
{
    std::string nugu_config_file = pimpl->sdk_manager->getStoragePath();
    if (nugu_config_file.size() > 0) {
        nugu_config_file += NUGU_CONFIG_FILENAME;
        pimpl->sdk_manager->readAuthConfigurationFile(nugu_config_file);
    } else {
        nugu_error("invalid storage path for configuration");
    }

    pimpl->sdk_manager->activateCapabilities({
        "ASR",
        "AudioPlayer",
        "TTS",
        "Text",
        "System",
        "Display",
        "Session",
        "Mic",
        "Speaker",
        "Chips",
        "Sound",
        "Utility",
        "Nudge",
        "Routine",
        "Extension",
        "Alerts",
    });
    pimpl->sdk_manager->addCapabilityListener("TTS", pimpl->integrated_state_listener.get());
    pimpl->sdk_manager->addCapabilityListener("AudioPlayer", pimpl->integrated_state_listener.get());
    pimpl->sdk_manager->addCapabilityListener("Text", pimpl->text_listener_with_handler.get());

    pimpl->sdk_manager->setDefaultConfiguration();
    pimpl->sdk_manager->constructCapabilities();
    pimpl->sdk_manager->start();
    pimpl->sdk_manager->connect();
}

void SDKManagerWrapper::stop()
{
    pimpl->sdk_manager->disconnect();
    pimpl->sdk_manager->stop();
}

bool SDKManagerWrapper::isStarted()
{
    return pimpl->sdk_manager->isStarted();
}

bool SDKManagerWrapper::isConnected()
{
    return pimpl->is_connected;
}

void SDKManagerWrapper::startRecognition()
{
    pimpl->speech_operator->startRecognition();
}

void SDKManagerWrapper::sendTextCommand(const std::string& text)
{
    pimpl->text_listener_with_handler->requestTextInput(text);
}

void SDKManagerWrapper::sdkStatusChanged(SDKManagerStatus status)
{
    std::cout << "[SDK Status] ";

    switch (status) {
    case SDKManagerStatus::IDLE:
        std::cout << "IDLE\n";
        break;
    case SDKManagerStatus::READY:
        std::cout << "READY\n";
        break;
    case SDKManagerStatus::STARTED:
        std::cout << "STARTED\n";
        break;
    case SDKManagerStatus::STOPPED:
        std::cout << "STOPPED\n";
        break;
    default:
        break;
    }
}

void SDKManagerWrapper::onStatusChanged(NetworkStatus status)
{
    std::cout << "[Network Status] ";

    switch (status) {
    case NetworkStatus::DISCONNECTED:
        pimpl->is_connected = false;
        std::cout << "DISCONNECTED\n";
        break;
    case NetworkStatus::CONNECTED:
        pimpl->is_connected = true;
        std::cout << "CONNECTED\n";
        fflush(stdout);
        break;
    case NetworkStatus::READY:
        pimpl->is_connected = true;
        std::cout << "READY\n";
        break;
    case NetworkStatus::CONNECTING:
        std::cout << "CONNECTING\n";
        break;
    default:
        break;
    }
}

void SDKManagerWrapper::onError(NetworkError error)
{
    std::cout << "[Network Error] ";

    switch (error) {
    case NetworkError::FAILED:
        std::cout << "FAILED\n";
        break;
    case NetworkError::TOKEN_ERROR:
        std::cout << "TOKEN_ERROR\n";
        break;
    case NetworkError::UNKNOWN:
        std::cout << "UNKNOWN\n";
        break;
    }
}
