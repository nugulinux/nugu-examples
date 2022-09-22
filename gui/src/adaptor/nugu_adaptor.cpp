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

#include <capability/speaker_interface.hh>

#include "nugu_adaptor.hpp"

NuguAdaptor* NuguAdaptor::instance = nullptr;

NuguAdaptor::NuguAdaptor(QObject* parent)
    : sdk_manager(std::make_shared<SDKManager>(true))
    , voice_chrome_adaptor(std::make_shared<VoiceChromeAdaptor>())
{
    nugu_log_set_system(NUGU_LOG_SYSTEM_NONE);

    sdk_manager->addNetworkListener(this);
    sdk_manager->activateCapabilities({
        "System",
        "TTS",
        "ASR",
        "Chips",
        "AudioPlayer",
        "Speaker",
        "Display",
        "Text",
        "Session",
        "Nudge",
        "Routine",
    });

    speaker_controller = std::make_shared<SpeakerController>(
        getCapabilityHandler<ISpeakerHandler>("Speaker"),
        getCapabilityHandler<ITTSHandler>("TTS"),
        getCapabilityHandler<IAudioPlayerHandler>("AudioPlayer"),
        sdk_manager->getBeepPlayer(),
        this);

    text_handler = getCapabilityHandler<ITextHandler>("Text");
    display_manager = sdk_manager->getDisplayManager();
    speech_operator = sdk_manager->getSpeechOperator();
    speech_operator->addListener(this);
    sdk_manager->getVoiceChrome()->setListener(voice_chrome_adaptor.get());

    sdk_manager->readAuthConfigurationFile(std::string(NUGU_STORAGE_PATH) + NUGU_GUI_CONFIG_FILENAME);
    display_manager->loadTemplateConfig(std::string(NUGU_STORAGE_PATH) + NUGU_TEMPLATE_CONFIG_FILENAME);
}

NuguAdaptor::~NuguAdaptor()
{
    sdk_manager->disconnect();
    sdk_manager->stop();
}

NuguAdaptor* NuguAdaptor::getInstance()
{
    if (!instance)
        instance = new NuguAdaptor();

    return instance;
}

void NuguAdaptor::destroyInstance()
{
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

/*******************************************************************************
 * define ISpeechRecognizerAggregatorListener
 ******************************************************************************/

void NuguAdaptor::onWakeupState(WakeupDetectState state, float power_noise, float power_speech)
{
}

void NuguAdaptor::onASRState(ASRState state, const std::string& dialog_id, ASRInitiator initiator)
{
    std::string state_str;

    switch (state) {
    case ASRState::IDLE:
        state_str = "IDLE";
        break;
    case ASRState::EXPECTING_SPEECH:
        state_str = "EXPECTING_SPEECH";
        break;
    case ASRState::LISTENING:
        state_str = "LISTENING";
        break;
    case ASRState::RECOGNIZING:
        state_str = "RECOGNIZING";
        break;
    case ASRState::BUSY:
        state_str = "BUSY";
        break;
    }

    emit aSRState(QString::fromStdString(state_str));
}

void NuguAdaptor::onResult(const RecognitionResult& result, const std::string& dialog_id)
{
}

/*******************************************************************************
 * define INetworkManagerListener
 ******************************************************************************/

void NuguAdaptor::onStatusChanged(NetworkStatus status)
{
    std::string status_str = "UNKNOWN";

    switch (status) {
    case NetworkStatus::DISCONNECTED:
        status_str = "DISCONNECTED";
        break;
    case NetworkStatus::CONNECTING:
        status_str = "CONNECTING";
        break;
    case NetworkStatus::READY:
        status_str = "READY";
        break;
    case NetworkStatus::CONNECTED:
        status_str = "CONNECTED";
        break;
    default:
        break;
    }

    emit networkStatus(QString::fromStdString(status_str));
}

void NuguAdaptor::onError(NetworkError error)
{
    switch (error) {
    case NetworkError::FAILED:
        break;
    case NetworkError::TOKEN_ERROR:
        break;
    case NetworkError::UNKNOWN:
        break;
    }
}

/*******************************************************************************
 * define ISpeakerControllerListener
 ******************************************************************************/

void NuguAdaptor::setVolume(int volume)
{
    emit volumeChanged(volume);
}

void NuguAdaptor::setMute(bool mute)
{
    emit muteChanged(mute);
}

/*******************************************************************************
 * define pure NuguAdaptor
 ******************************************************************************/

void NuguAdaptor::setup()
{
    sdk_manager->addCapabilityListener("Speaker", speaker_controller.get());

    sdk_manager->setDefaultConfiguration();
    sdk_manager->constructCapabilities();

    sdk_manager->start();
    sdk_manager->connect();
}

IDisplayManagerHandler* NuguAdaptor::getDisplayManagerHandler()
{
    return display_manager;
}

VoiceChromeAdaptor* NuguAdaptor::getVoiceChromeAdaptor()
{
    return voice_chrome_adaptor.get();
}

void NuguAdaptor::startListening()
{
    speech_operator->startListening();
}
void NuguAdaptor::stopListening()
{
    speech_operator->stopListening();
}

void NuguAdaptor::sendTextCommand(const QString& text, bool include_dialog_attribute, const QString& token)
{
    text_handler->requestTextInput(text.toStdString(), token.toStdString(), "USERINPUT", include_dialog_attribute);
}

void NuguAdaptor::volumeUp()
{
    speaker_controller->volumeUp();
}

void NuguAdaptor::volumeDown()
{
    speaker_controller->volumeDown();
}

void NuguAdaptor::toggleMute()
{
    speaker_controller->toggleMute();
}

void NuguAdaptor::powerOff()
{
    emit finish();
}

template <typename HT>
HT* NuguAdaptor::getCapabilityHandler(std::string&& capability_name)
{
    return dynamic_cast<HT*>(sdk_manager->getCapabilityHandler(capability_name));
}
