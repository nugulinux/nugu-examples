#!/usr/bin/python
# -*- coding: utf-8 -*-

import dbus
import dbus.mainloop.glib
import json
#import argparse

try:
    from gi.repository import GObject  # python3
except ImportError:
    import gobject as GObject  # python2

asr = None
tts = None
text = None
aplayer = None

def asr_wakeup_state_changed(state):
    global asr
    print("ASR Wakeup State: '%s'" % state)

def asr_state_changed(state, dialog_id, initiator):
    global asr
    print("ASR State: '%s'" % state)

def aplayer_position_changed(position):
    global aplayer
    print("position: '%d'" % position)

def main(cmd):
    global asr, tts, text, aplayer
    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

    bus = dbus.SystemBus()

    manager = dbus.Interface(bus.get_object("com.sktnugu.nugud", '/SDK'), "com.sktnugu.nugud.SDK")

    asr = dbus.Interface(bus.get_object("com.sktnugu.nugud", '/Capability/ASR'), "com.sktnugu.nugud.ASR")
    asr.connect_to_signal("onWakeupState", asr_wakeup_state_changed)
    asr.connect_to_signal("onState", asr_state_changed)

    tts = dbus.Interface(bus.get_object("com.sktnugu.nugud", '/Capability/TTS'), "com.sktnugu.nugud.TTS")

    text = dbus.Interface(bus.get_object("com.sktnugu.nugud", '/Capability/Text'), "com.sktnugu.nugud.Text")

    aplayer = dbus.Interface(bus.get_object("com.sktnugu.nugud", '/Capability/AudioPlayer'), "com.sktnugu.nugud.AudioPlayer")
    aplayer.connect_to_signal("positionChanged", aplayer_position_changed)

    cap_lists= [
        "ASR",
        "AudioPlayer",
        "TTS",
        "Text",
        "System",
        "Display",
        "Session",
        "Mic",
        "Speaker",
        "Chips",
        "Sound",
        "Utility",
        "Nudge",
        "Routine",
        "Extension",
        "Alerts"
    ]
    manager.activateCapabilities(cap_lists)
    manager.constructCapabilities()
    manager.setToken("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOjE2NjEzMjg3ODAsInVzZXJfbmFtZSI6IkFMREY1VDFTRjY5Q0VBREFBNUNCIiwianRpIjoiYWFiMzFhNzktNzc4NS00MzI0LTg4MzktZjQ5ZDA3NjI5N2Q1IiwiY2xpZW50X2lkIjoic3BlYWtlci5udWd1Lm51MTEwIiwic2NvcGUiOlsiZGV2aWNlOlMuSS5ELiJdLCJleHQiOnsidXNyIjoiQUxERjVUMVNGNjlDRUFEQUE1Q0IiLCJkdmMiOiJBTERGUzRCRUhMR0hBMkIzN0RBQSIsInRrbiI6Ijk3N0YxMEU1M0I0RjRFMjdCMzg2NDkyNTkwNURDQjQxIiwicG9jIjoic3BlYWtlci5udWd1Lm51MTEwIiwic3JsIjoibGludXhfTlUxMTBfQzAyWTcxMkRKRzVKIiwic3ZjIjoiYWxhZGRpbiJ9fQ.ca6qRNZO6JjdS7DoT7yG9QBabKY4j-vH0DJmJvaDXRw")
    manager.start()
    manager.connect()

    asr.startRecognition("TAP")

    mainloop = GObject.MainLoop()
    try:
        mainloop.run()
    except KeyboardInterrupt:
        manager.stop()

if __name__ == '__main__':
    main("")
