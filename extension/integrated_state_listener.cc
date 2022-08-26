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

void IntegratedStateListener::onWakeupState(WakeupDetectState state, float power_noise, float power_speech)
{
    std::cout << "[Wakeup] ";

    switch (state) {
    case WakeupDetectState::WAKEUP_IDLE:
        std::cout << "wakeup idle\n";
        break;
    case WakeupDetectState::WAKEUP_DETECTING:
        std::cout << "wakeup detecting...\n";
        break;
    case WakeupDetectState::WAKEUP_DETECTED:
        std::cout << "wakeup detected\n";
        break;
    case WakeupDetectState::WAKEUP_FAIL:
        std::cout << "wakeup failed\n";
        break;
    }
}

void IntegratedStateListener::onASRState(ASRState state, const std::string& dialog_id, ASRInitiator initiator)
{
    std::cout << "[ASR] ";

    switch (state) {
    case ASRState::IDLE: {
        std::cout << "IDLE\n";
        break;
    }
    case ASRState::EXPECTING_SPEECH: {
        std::cout << "EXPECTING_SPEECH\n";
        break;
    }
    case ASRState::LISTENING: {
        std::cout << "LISTENING\n";
        break;
    }
    case ASRState::RECOGNIZING: {
        std::cout << "RECOGNIZING\n";
        break;
    }
    case ASRState::BUSY: {
        std::cout << "BUSY\n";
        break;
    }
    }
}

void IntegratedStateListener::onResult(const RecognitionResult& result, const std::string& dialog_id)
{
    switch (result.status) {
    case RecognitionResult::Status::None:
    case RecognitionResult::Status::Partial:
        break;
    case RecognitionResult::Status::Complete:
        std::cout << "[ASR] " << result.recognized_text << std::endl;
        break;
    case RecognitionResult::Status::Cancel:
        std::cout << "[ASR] canceled\n";
        break;
    case RecognitionResult::Status::Error:
        std::cout << "[ASR] ";

        switch (result.error) {
        case ASRError::RESPONSE_TIMEOUT: {
            std::cout << "RESPONSE_TIMEOUT\n";
            break;
        }
        case ASRError::LISTEN_TIMEOUT: {
            std::cout << "LISTEN_TIMEOUT\n";
            break;
        }
        case ASRError::LISTEN_FAILED: {
            std::cout << "LISTEN_FAILED\n";
            break;
        }
        case ASRError::RECOGNIZE_ERROR: {
            std::cout << "RECOGNIZE_ERROR\n";
            break;
        }
        case ASRError::UNKNOWN: {
            std::cout << "UNKNOWN\n";
            break;
        }
        }
        break;
    }
}

void IntegratedStateListener::onTTSState(TTSState state, const std::string& dialog_id)
{
    std::cout << "[TTS] ";

    switch (state) {
    case TTSState::TTS_SPEECH_START:
        std::cout << "START\n";
        break;
    case TTSState::TTS_SPEECH_STOP:
        std::cout << "STOP\n";
        break;
    case TTSState::TTS_SPEECH_FINISH:
        std::cout << "FINISH\n";
        break;
    }
}

void IntegratedStateListener::onTTSText(const std::string& text, const std::string& dialog_id)
{
    const std::string WAKEUP_WORD_TAG = "{W}";
    const std::string WAKEUP_WORD = "아리아";

    // remove '< ~ >' tag
    std::string extracted_text;
    bool is_skip = false;

    for (const auto& character : text) {
        if (character == '<')
            is_skip = true;
        else if (character == '>')
            is_skip = false;

        if (!is_skip && character != '>')
            extracted_text.push_back(character);
    }

    // replace '{W}' tag to wakeup word
    std::size_t wakeup_tag_found = extracted_text.find(WAKEUP_WORD_TAG);

    if (wakeup_tag_found != std::string::npos)
        extracted_text.replace(wakeup_tag_found, WAKEUP_WORD_TAG.length(), WAKEUP_WORD);

    std::cout << "[TTS] " << extracted_text << std::endl;
}

void IntegratedStateListener::onTTSCancel(const std::string& dialog_id)
{
}

void IntegratedStateListener::mediaStateChanged(AudioPlayerState state, const std::string& dialog_id)
{
    std::cout << "[AudioPlayer] ";

    switch (state) {
    case AudioPlayerState::IDLE:
        std::cout << "IDLE\n";
        break;
    case AudioPlayerState::PLAYING:
        std::cout << "PLAYING\n";
        break;
    case AudioPlayerState::STOPPED:
        std::cout << "STOPPED\n";
        break;
    case AudioPlayerState::PAUSED:
        std::cout << "PAUSED\n";
        break;
    case AudioPlayerState::FINISHED:
        std::cout << "FINISHED\n";
        break;
    }
}

void IntegratedStateListener::mediaEventReport(AudioPlayerEvent event, const std::string& dialog_id)
{
}

void IntegratedStateListener::durationChanged(int duration)
{
}

void IntegratedStateListener::positionChanged(int position)
{
}

void IntegratedStateListener::favoriteChanged(bool favorite, const std::string& dialog_id)
{
}

void IntegratedStateListener::shuffleChanged(bool shuffle, const std::string& dialog_id)
{
}

void IntegratedStateListener::repeatChanged(RepeatType repeat, const std::string& dialog_id)
{
}

void IntegratedStateListener::requestContentCache(const std::string& key, const std::string& playurl)
{
}

bool IntegratedStateListener::requestToGetCachedContent(const std::string& key, std::string& filepath)
{
    return false;
}

void IntegratedStateListener::renderDisplay(const std::string& id, const std::string& type, const std::string& json, const std::string& dialog_id)
{
}

bool IntegratedStateListener::clearDisplay(const std::string& id, bool unconditionally, bool has_next)
{
    return false;
}

void IntegratedStateListener::controlDisplay(const std::string& id, ControlType type, ControlDirection direction)
{
}

void IntegratedStateListener::updateDisplay(const std::string& id, const std::string& json)
{
}

bool IntegratedStateListener::requestLyricsPageAvailable(const std::string& id, bool& visible)
{
    return true;
}

bool IntegratedStateListener::showLyrics(const std::string& id)
{
    return true;
}

bool IntegratedStateListener::hideLyrics(const std::string& id)
{
    return true;
}

void IntegratedStateListener::updateMetaData(const std::string& id, const std::string& json)
{
}
