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

#ifndef __VOICE_CHROME_ADAPTOR_H__
#define __VOICE_CHROME_ADAPTOR_H__

#include <QObject>

#include <extension/voice_chrome_interface.hh>

using namespace NuguExtension;

class VoiceChromeAdaptor : public QObject,
                           public IVoiceChromeListener {
    Q_OBJECT
public:
    VoiceChromeAdaptor() = default;
    virtual ~VoiceChromeAdaptor() = default;

    // implements IVoiceChromeListener
    void render(VoiceChromeState state, const VoiceChromeAttribute& attribute) override;
    void onASRResult(const std::string& asr_result) override;

signals:
    void render(const QString& state, const QString& attribute);
    void asrResult(const QString& asr_result);

private:
    const std::map<VoiceChromeState, std::string> VOICE_CHROME_STATE_TEXTS {
        { VoiceChromeState::Idle, "IDLE" },
        { VoiceChromeState::PassiveListening, "PASSIVE_LISTENING" },
        { VoiceChromeState::ActiveListening, "ACTIVE_LISTENING" },
        { VoiceChromeState::Processing, "PROCESSING" },
        { VoiceChromeState::DialogSpeaking, "DIALOG_SPEAKING" }
    };

    const std::map<ChipsType, std::string> CHIPS_TYPE_TEXTS {
        { ChipsType::NUDGE, "NUDGE" },
        { ChipsType::ACTION, "ACTION" },
        { ChipsType::GENERAL, "GENERAL" }
    };

    const std::map<VoiceChromeIndicator, std::string> VOICE_CHROME_INDICATOR_TEXTS {
        { VoiceChromeIndicator::Intro, "INTRO" },
        { VoiceChromeIndicator::Passive, "PASSIVE" },
        { VoiceChromeIndicator::ActiveTrans, "ACTIVE_TRANS" },
        { VoiceChromeIndicator::Active, "ACTIVE" },
        { VoiceChromeIndicator::SpeakingTrans, "SPEAKING_TRANS" },
        { VoiceChromeIndicator::Speaking, "SPEAKING" },
        { VoiceChromeIndicator::Processing, "PROCESSING" },
        { VoiceChromeIndicator::None, "NONE" }
    };
};

#endif // __VOICE_CHROME_ADAPTOR_H__
