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

#ifndef __NUGU_ADAPTOR_H__
#define __NUGU_ADAPTOR_H__

#include <QObject>

#include <capability/text_interface.hh>
#include <extension/sdk_manager.hh>

#include "speaker_controller.hh"
#include "voice_chrome_adaptor.hpp"

using namespace NuguCapability;
using namespace NuguExtension;

class NuguAdaptor : public QObject,
                    public INetworkManagerListener,
                    public ISpeechRecognizerAggregatorListener,
                    public ISpeakerControllerListener {
    Q_OBJECT
public:
    static NuguAdaptor* getInstance();
    static void destroyInstance();

    NuguAdaptor(const NuguAdaptor&) = delete;
    NuguAdaptor(NuguAdaptor&&) = delete;

    NuguAdaptor& operator=(const NuguAdaptor&) = delete;
    NuguAdaptor& operator=(NuguAdaptor&&) = delete;

    // implements ISpeechRecognizerAggregatorListener
    void onWakeupState(WakeupDetectState state, float power_noise, float power_speech) override;
    void onASRState(ASRState state, const std::string& dialog_id, ASRInitiator initiator) override;
    void onResult(const RecognitionResult& result, const std::string& dialog_id) override;

    // implements INetworkManagerListener
    void onStatusChanged(NetworkStatus status) override;
    void onError(NetworkError error) override;

    // implements ISpeakerControllerListener
    void setVolume(int volume) override;
    void setMute(bool mute) override;

    void setup();
    IDisplayManagerHandler* getDisplayManagerHandler();
    VoiceChromeAdaptor* getVoiceChromeAdaptor();

signals:
    void aSRState(const QString& state);
    void networkStatus(const QString& status);
    void volumeChanged(int volume);
    void muteChanged(bool mute);
    void finish();

public slots:
    void startListening();
    void stopListening();
    void sendTextCommand(const QString& text, bool include_dialog_attribute = true, const QString& token = "");
    void volumeUp();
    void volumeDown();
    void toggleMute();
    void powerOff();

private:
    explicit NuguAdaptor(QObject* parent = nullptr);
    virtual ~NuguAdaptor();

    template <typename HT>
    HT* getCapabilityHandler(std::string&& capability_name);

    static NuguAdaptor* instance;

    std::shared_ptr<SDKManager> sdk_manager = nullptr;
    std::shared_ptr<VoiceChromeAdaptor> voice_chrome_adaptor = nullptr;
    std::shared_ptr<SpeakerController> speaker_controller = nullptr;
    IDisplayManagerHandler* display_manager = nullptr;
    ISpeechOperator* speech_operator = nullptr;
    ITextHandler* text_handler = nullptr;
};

#endif // __NUGU_ADAPTOR_H__
