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

#include "text_handler.hh"

void TextHandler::onState(TextState state, const std::string& dialog_id)
{
    std::cout << "[Text] ";

    switch (state) {
    case TextState::IDLE:
        std::cout << "IDLE\n";
        break;

    case TextState::BUSY:
        std::cout << "BUSY\n";
        break;
    }
}

void TextHandler::onComplete(const std::string& dialog_id)
{
}

void TextHandler::onError(TextError error, const std::string& dialog_id)
{
}

bool TextHandler::handleTextCommand(const std::string& text, const std::string& token)
{
    return false;
}

std::string TextHandler::requestTextInput(const std::string& text, const std::string& token, const std::string& source, bool include_dialog_attribute)
{
    std::string ret_str;

    invokeMethod(
        __func__, [&, text, token, source, include_dialog_attribute]() {
            ret_str = dynamic_cast<ITextHandler*>(handler)->requestTextInput(text, token, source, include_dialog_attribute);
        },
        ExecuteType::Blocking);

    return ret_str;
}
