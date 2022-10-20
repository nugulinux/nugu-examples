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

#include "speaker_controller.hh"

SpeakerController::SpeakerController(ISpeakerHandler* speaker_handler,
    ITTSHandler* tts_handler,
    IAudioPlayerHandler* audio_player_handler,
    BeepPlayer* beep_player,
    ISpeakerControllerListener* listener)
    : speaker_handler(speaker_handler)
    , tts_handler(tts_handler)
    , audio_player_handler(audio_player_handler)
    , beep_player(beep_player)
    , listener(listener)
{
    setSpeakerConfiguration();
}

/*******************************************************************************
 * define ISpeakerListener
 ******************************************************************************/

void SpeakerController::requestSetVolume(const std::string& ps_id, SpeakerType type, int volume, bool linear)
{
    speaker_handler->sendEventVolumeChanged(ps_id, setVolume(type, volume));
}

void SpeakerController::requestSetMute(const std::string& ps_id, SpeakerType type, bool mute)
{
    speaker_handler->sendEventMuteChanged(ps_id, setMute(type, mute));
}

/*******************************************************************************
 * define pure SpeakerController
 ******************************************************************************/

void SpeakerController::volumeUp()
{
    int target_volume = volume + NUGU_SPEAKER_DEFAULT_STEP;

    if (target_volume <= NUGU_SPEAKER_MAX_VOLUME) {
        if (setVolume(SpeakerType::NUGU, target_volume))
            playVolumeControlBeep();
    }
}

void SpeakerController::volumeDown()
{
    int target_volume = volume - NUGU_SPEAKER_DEFAULT_STEP;

    if (target_volume >= NUGU_SPEAKER_MIN_VOLUME) {
        if (setVolume(SpeakerType::NUGU, target_volume))
            playVolumeControlBeep();
    }
}

void SpeakerController::toggleMute()
{
    setMute(SpeakerType::NUGU, !mute);
}

void SpeakerController::setSpeakerConfiguration()
{
    SpeakerInfo nugu_speaker;
    nugu_speaker.type = SpeakerType::NUGU;
    nugu_speaker.mute = false;
    nugu_speaker.can_control = true;

    speaker_handler->setSpeakerInfo({
        { SpeakerType::NUGU, nugu_speaker },
    });
}

bool SpeakerController::setVolume(SpeakerType type, int volume)
{
    bool result = tts_handler->setVolume(volume) && audio_player_handler->setVolume(volume);

    if (result) {
        this->volume = volume;
        beep_player->setVolume(volume);
        speaker_handler->informVolumeChanged(type, volume);

        listener->setVolume(volume);
    }

    return result;
}

bool SpeakerController::setMute(SpeakerType type, bool mute)
{
    bool result = tts_handler->setMute(mute) && audio_player_handler->setMute(mute);

    if (result) {
        this->mute = mute;
        beep_player->setMute(mute);
        speaker_handler->informMuteChanged(type, mute);

        listener->setMute(mute);
    }

    return result;
}

void SpeakerController::playVolumeControlBeep()
{
    beep_player->play(BeepSoundType::VOLUME_CONTROL_SYSTEM);
}
