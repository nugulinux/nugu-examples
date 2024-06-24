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

#include <iomanip>
#include <iostream>
#include <map>

#include "command_manager.hh"

const char* C_RED = "\033[1;91m";
const char* C_GREEN = "\033[1;92m";
const char* C_YELLOW = "\033[1;93m";
const char* C_BLUE = "\033[1;94m";
const char* C_CYAN = "\033[1;96m";
const char* C_WHITE = "\033[1;97m";
const char* C_RESET = "\033[0m";

void error(std::string&& message)
{
    std::cout << C_RED
              << message
              << std::endl
              << C_RESET;
    std::cout.flush();
}

struct CommandManager::Impl {
    SDKManagerWrapper* sdk_manager_wrapper = nullptr;
    QuitCommand quit_command = nullptr;
    std::map<std::string, std::function<void(const std::string&)>> key_handle_factory;

    explicit Impl(SDKManagerWrapper* sdk_manager_wrapper)
        : sdk_manager_wrapper(sdk_manager_wrapper)
    {
    }
};

CommandManager::CommandManager(SDKManagerWrapper* sdk_manager_wrapper)
    : pimpl(std::unique_ptr<Impl>(new Impl(sdk_manager_wrapper)))
{
    pimpl->key_handle_factory = {
        { "start", [&](const std::string& param) {
             if (!pimpl->sdk_manager_wrapper->isStarted())
                 pimpl->sdk_manager_wrapper->start();
         } },
        { "stop", [&](const std::string& param) {
             if (pimpl->sdk_manager_wrapper->isStarted())
                 pimpl->sdk_manager_wrapper->stop();
         } },
        { "asr", [&](const std::string& param) {
             if (pimpl->sdk_manager_wrapper->isStarted() && pimpl->sdk_manager_wrapper->isConnected())
                 pimpl->sdk_manager_wrapper->startRecognition();
             else
                 error("start command first!");
         } },
        { "text", [&](const std::string& param) {
             if (pimpl->sdk_manager_wrapper->isStarted() && pimpl->sdk_manager_wrapper->isConnected())
                 pimpl->sdk_manager_wrapper->sendTextCommand(param);
             else
                 error("start command first!");
         } },
        { "quit", [&](const std::string& param) {
             if (pimpl->sdk_manager_wrapper->isStarted())
                 pimpl->sdk_manager_wrapper->stop();

             if (pimpl->quit_command)
                 pimpl->quit_command();
         } },
    };
}

CommandManager::~CommandManager()
{
}

void CommandManager::showCommand()
{
    std::cout << C_YELLOW
              << std::endl
              << "=======================================================\n"
              << C_RED
              << "NUGU Extension Command (" << (pimpl->sdk_manager_wrapper->isConnected() ? "Connected" : "Disconnected") << ")\n"
              << C_YELLOW
              << "=======================================================\n"
              << C_BLUE
              << std::setw(10) << std::left << "start"
              << ": start nugu with wakeup\n"
              << std::setw(10) << std::left << "stop"
              << ": stop nugu\n"
              << std::setw(10) << std::left << "asr"
              << ": start listening\n"
              << std::setw(10) << std::left << "text [cmd]"
              << ": text command\n"
              << std::setw(10) << std::left << "quit"
              << ": quit application\n"
              << C_YELLOW
              << "-------------------------------------------------------\n"
              << C_WHITE
              << "Command > "
              << C_RESET;

    fflush(stdout);
}

void CommandManager::handleUserCommand(const std::vector<std::string>& arguments)
{
    try {
        std::string command = arguments.at(0);
        std::string param = arguments.size() > 1 ? arguments.at(1) : "";

        param.erase(std::remove(param.begin(), param.end(), '"'), param.end());
        pimpl->key_handle_factory.at(command)(param);
    } catch (const std::out_of_range& e) {
        error("No Command exist!");
    }

    fflush(stdout);
}

void CommandManager::setQuitCommand(QuitCommand&& command)
{
    pimpl->quit_command = command;
}
