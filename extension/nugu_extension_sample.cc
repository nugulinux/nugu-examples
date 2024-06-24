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

#include <glib.h>
#include <iostream>
#include <memory>
#include <signal.h>
#include <string.h>
#include <sstream>

#ifndef _WIN32
#include <unistd.h>
#endif

#include "command_manager.hh"
#include "sdk_manager_wrapper.hh"

static std::shared_ptr<SDKManagerWrapper> sdk_manager_wrapper = nullptr;
static std::shared_ptr<CommandManager> command_manager = nullptr;
static GMainLoop* loop = nullptr;

std::vector<std::string> parseArguments(std::string args)
{
    std::vector<std::string> arguments;
    std::istringstream iss(args);
    std::string command;
    std::string param;

    iss >> command;
    arguments.emplace_back(command);
    std::getline(iss, param);
    if (param.size())
        arguments.emplace_back(param);

    return arguments;
}

static gboolean onKeyInput(GIOChannel* src, GIOCondition con, gpointer userdata)
{
    const unsigned int BUF_SIZE = 4096;
    char keybuf[BUF_SIZE];
    std::vector<std::string> arguments;
    CommandManager* cmd_manager = static_cast<CommandManager*>(userdata);

    if (fgets(keybuf, BUF_SIZE, stdin) == NULL)
        return TRUE;

    if (strlen(keybuf) > 0 && keybuf[strlen(keybuf) - 1] == '\n')
        keybuf[strlen(keybuf) - 1] = '\0';

    cmd_manager->showCommand();

    if (strlen(keybuf) < 1)
        return TRUE;

    arguments = parseArguments(keybuf);
    cmd_manager->handleUserCommand(arguments);

    return TRUE;
}

#ifdef _WIN32
static char *optarg;
static int optind = 1, opterr = 1, optopt;

static char *next = NULL;

static int getopt(int argc, char * const argv[], const char *optstring) {
    if (optind == 0) {
        optind = 1;
        next = NULL;
    }
    if (next == NULL || *next == '\0') {
        if (optind == argc || argv[optind][0] != '-' || argv[optind][1] == '\0') {
            return -1;
        }
        if (strcmp(argv[optind], "--") == 0) {
            optind++;
            return -1;
        }
        next = argv[optind] + 1;
        optind++;
    }

    optopt = *next++;
    const char *opt = strchr(optstring, optopt);
    if (opt == NULL || optopt == ':') {
        if (opterr) {
            fprintf(stderr, "Unknown option -%c\n", optopt);
        }
        return '?';
    }
    if (opt[1] == ':') {
        if (*next != '\0') {
            optarg = next;
            next = NULL;
        } else if (optind < argc) {
            optarg = argv[optind];
            optind++;
        } else {
            if (opterr) {
                fprintf(stderr, "Option -%c requires an argument\n", optopt);
            }
            return '?';
        }
    }
    return optopt;
}
#endif

bool handleArguments(const int& argc, char** argv)
{
    int c;

    while ((c = getopt(argc, argv, "d")) != -1) {
        switch (c) {
        case 'd':
            nugu_log_set_system(NUGU_LOG_SYSTEM_STDERR);
            break;
        default:
            std::cout << "Usage: \n"
                      << "\t nugu_extension_sample [Option...]\n\n"
                      << "Options: \n"
                      << "\t -d\t enable nugu log\n\n";

            return false;
        }
    }

    return true;
}

int main(int argc, char** argv)
{
    std::cout << "### nugu_extension sample application ###\n";

    nugu_log_set_system(NUGU_LOG_SYSTEM_NONE);

    if (!handleArguments(argc, argv))
        return EXIT_FAILURE;

    sdk_manager_wrapper = std::make_shared<SDKManagerWrapper>();
    command_manager = std::make_shared<CommandManager>(sdk_manager_wrapper.get());

    command_manager->setQuitCommand([&] {
        if (loop)
            g_main_loop_quit(loop);
    });

    command_manager->showCommand();

#ifdef _WIN32
    GIOChannel* channel = g_io_channel_unix_new(0);
#else
    GIOChannel* channel = g_io_channel_unix_new(STDIN_FILENO);
#endif
    g_io_add_watch(channel, (GIOCondition)(G_IO_IN | G_IO_ERR | G_IO_HUP | G_IO_NVAL), onKeyInput, command_manager.get());
    g_io_channel_unref(channel);

    loop = g_main_loop_new(g_main_context_default(), FALSE);
    g_main_loop_run(loop);

    std::cout << "exit application\n";

    g_main_loop_unref(loop);

    return EXIT_SUCCESS;
}
