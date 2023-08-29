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

#include "voice_chrome_adaptor.hpp"

/*******************************************************************************
 * define IVoiceChromeListener
 ******************************************************************************/

void VoiceChromeAdaptor::render(VoiceChromeState state, const VoiceChromeAttribute& attribute)
{
    NJson::Value attribute_json;
    NJson::FastWriter writer;

    attribute_json["window_visible"] = attribute.window_visible;
    attribute_json["stt_guide_visible"] = attribute.stt_guide_visible;
    attribute_json["stt_visible"] = attribute.stt_visible;
    attribute_json["utterance_visible"] = attribute.utterance_visible;
    attribute_json["expect_speech"] = attribute.expect_speech;
    attribute_json["stt_clear"] = attribute.stt_clear;
    attribute_json["indicator"]["preload"] = VOICE_CHROME_INDICATOR_TEXTS.at(attribute.indicator.first);
    attribute_json["indicator"]["main"] = VOICE_CHROME_INDICATOR_TEXTS.at(attribute.indicator.second);
    attribute_json["chips"] = NJson::arrayValue;

    for (const auto& item : attribute.chips) {
        NJson::Value chip;
        chip["type"] = CHIPS_TYPE_TEXTS.at(item.type);
        chip["text"] = item.text;
        chip["token"] = item.token;

        attribute_json["chips"].append(chip);
    }

    emit render(VOICE_CHROME_STATE_TEXTS.at(state).c_str(), writer.write(attribute_json).c_str());
}

void VoiceChromeAdaptor::onASRResult(const std::string& asr_result)
{
    emit asrResult(asr_result.c_str());
}
