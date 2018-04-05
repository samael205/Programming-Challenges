#!/usr/bin/env python
# -*- coding: utf-8 -*-

import imp
from sys import exit

try:
   imp.find_module("gtts")
except ImportError as e:
    print(e)
    exit(0)

from gtts import gTTS

def convert_to_audio(text, save_path):
    text_to_speech = gTTS(text=text, lang="en")
    text_to_speech.save(save_path)
