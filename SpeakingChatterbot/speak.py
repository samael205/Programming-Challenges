from gtts import gTTS
import speech_recognition as sr
import os
import vlc


class Speak:
    def __init__(self):
        self.text_to_speech = None

    def say(self, tts, language='en'):
        self.text_to_speech = gTTS(text=tts, lang=language)
        self.text_to_speech.save("speak.mp3")
        play_sound = vlc.MediaPlayer("speak.mp3")
        play_sound.play()


class Speech():
    def __init__(self):
        self.speech = sr.Recognizer()

    def record(self):
        r = sr.Recognizer()
        with sr.Microphone() as source:
            print("I'm listening")
            audio = r.listen(source)
        data = ""
        try:
            data = r.recognize_google(audio)
        except sr.UnknownValueError:
            Speak.say(Speak, "I can't understand you!")
        except sr.RequestError as e:
            print(e)
        return data
