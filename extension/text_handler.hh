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

#ifndef __TEXT_LISTENER_WITH_HANDLER_H__
#define __TEXT_LISTENER_WITH_HANDLER_H__

#include <capability/text_interface.hh>
#include <clientkit/capability_interface.hh>

#include "base_listener_with_handler.hh"

using namespace NuguClientKit;
using namespace NuguCapability;

class TextHandler : public BaseListenerWithHandler,
                    public ITextListener {
public:
    explicit TextHandler() = default;
    virtual ~TextHandler() = default;

    // implements ITextListener
    void onState(TextState state, const std::string& dialog_id) override;
    void onComplete(const std::string& dialog_id) override;
    void onError(TextError error, const std::string& dialog_id) override;
    bool handleTextCommand(const std::string& text, const std::string& token) override;

    // implements handler
    std::string requestTextInput(const std::string& text, const std::string& token = "", const std::string& source = "", bool include_dialog_attribute = true);
    void setAttribute(TextAttribute&& attribute);
};

#endif /* __TEXT_LISTENER_WITH_HANDLER_H__ */
