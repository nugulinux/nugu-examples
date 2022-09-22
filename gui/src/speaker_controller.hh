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

#ifndef __SPEAKER_CONTROLLER_H__
#define __SPEAKER_CONTROLLER_H__

#include <capability/audio_player_interface.hh>
#include <capability/speaker_interface.hh>
#include <capability/tts_interface.hh>
#include <clientkit/nugu_runner.hh>
#include <extension/beep_player.hh>

using namespace NuguCapability;
using namespace NuguExtension;

class ISpeakerControllerListener {
public:
    virtual ~ISpeakerControllerListener() = default;

    virtual void setVolume(int volume) = 0;
    virtual void setMute(bool mute) = 0;
};

class SpeakerController : public ISpeakerListener,
                          public NuguRunner {
public:
    explicit SpeakerController(ISpeakerHandler* speaker_handler,
        ITTSHandler* tts_handler,
        IAudioPlayerHandler* audio_player_handler,
        BeepPlayer* beep_player,
        ISpeakerControllerListener* listener);
    virtual ~SpeakerController() = default;

    // implements ISpeakerListener
    void requestSetVolume(const std::string& ps_id, SpeakerType type, int volume, bool linear) override;
    void requestSetMute(const std::string& ps_id, SpeakerType type, bool mute) override;

    void volumeUp();
    void volumeDown();
    void toggleMute();

private:
    void setSpeakerConfiguration();
    bool setVolume(SpeakerType type, int volume);
    bool setMute(SpeakerType type, bool mute);
    void playVolumeControlBeep();

    const int DEFAULT_VOLUME = 50;

    ISpeakerHandler* speaker_handler = nullptr;
    ITTSHandler* tts_handler = nullptr;
    IAudioPlayerHandler* audio_player_handler = nullptr;
    BeepPlayer* beep_player = nullptr;
    ISpeakerControllerListener* listener = nullptr;

    int volume = DEFAULT_VOLUME;
    bool mute = false;
};

#endif // __SPEAKER_CONTROLLER_H__
