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

#ifndef __SDK_MANAGER_WRAPPER_H__
#define __SDK_MANAGER_WRAPPER_H__

#include "sdk_manager.hh"

using namespace NuguExtension;

class SDKManagerWrapper : public ISDKManagerListener,
                          public INetworkManagerListener {
public:
    SDKManagerWrapper();
    virtual ~SDKManagerWrapper();

    std::shared_ptr<SDKManager> getSDKManager();
    void start();
    void stop();
    bool isStarted();
    bool isConnected();
    void startRecognition();
    void sendTextCommand(const std::string& text);

    // ISDKManagerListener
    void sdkStatusChanged(SDKManagerStatus status) override;

    // INetworkManagerListener
    void onStatusChanged(NetworkStatus status) override;
    void onError(NetworkError error) override;

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

#endif /* __SDK_MANAGER_WRAPPER_H__ */
