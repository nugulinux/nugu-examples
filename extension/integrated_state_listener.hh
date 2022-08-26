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

#ifndef __INTEGRATED_STATE_LISTENER_H__
#define __INTEGRATED_STATE_LISTENER_H__

#include <capability/audio_player_interface.hh>
#include <capability/tts_interface.hh>
#include <clientkit/speech_recognizer_aggregator_interface.hh>

using namespace NuguClientKit;
using namespace NuguCapability;

class IntegratedStateListener : public ISpeechRecognizerAggregatorListener,
                                public ITTSListener,
                                public IAudioPlayerListener,
                                public IAudioPlayerDisplayListener {

public:
    virtual ~IntegratedStateListener() = default;

    // implements ISpeechRecognizerAggregatorListener
    void onWakeupState(WakeupDetectState state, float power_noise, float power_speech) override;
    void onASRState(ASRState state, const std::string& dialog_id, ASRInitiator initiator) override;
    void onResult(const RecognitionResult& result, const std::string& dialog_id) override;

    // implements ITTSListener
    void onTTSState(TTSState state, const std::string& dialog_id) override;
    void onTTSText(const std::string& text, const std::string& dialog_id) override;
    void onTTSCancel(const std::string& dialog_id) override;

    // implements IAudioPlayerListener
    void mediaStateChanged(AudioPlayerState state, const std::string& dialog_id) override;
    void mediaEventReport(AudioPlayerEvent event, const std::string& dialog_id) override;
    void durationChanged(int duration) override;
    void positionChanged(int position) override;
    void favoriteChanged(bool favorite, const std::string& dialog_id) override;
    void shuffleChanged(bool shuffle, const std::string& dialog_id) override;
    void repeatChanged(RepeatType repeat, const std::string& dialog_id) override;
    void requestContentCache(const std::string& key, const std::string& playurl) override;
    bool requestToGetCachedContent(const std::string& key, std::string& filepath) override;

    // implements IAudioPlayerDisplayListener
    void renderDisplay(const std::string& id, const std::string& type, const std::string& json, const std::string& dialog_id) override;
    bool clearDisplay(const std::string& id, bool unconditionally, bool has_next) override;
    void controlDisplay(const std::string& id, ControlType type, ControlDirection direction) override;
    void updateDisplay(const std::string& id, const std::string& json) override;
    bool requestLyricsPageAvailable(const std::string& id, bool& visible) override;
    bool showLyrics(const std::string& id) override;
    bool hideLyrics(const std::string& id) override;
    void updateMetaData(const std::string& id, const std::string& json) override;
};

#endif /* __INTEGRATED_STATE_LISTENER_H__ */
