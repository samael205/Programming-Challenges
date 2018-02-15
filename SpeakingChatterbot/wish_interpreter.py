import webbrowser
import wikipedia
import os
from os.path import expanduser
import speak
import random
import glob
from PyDictionary import PyDictionary
import vlc

def wikipedia_find(sentence):
    data = wikipedia.page(sentence)
    return data.content[:data.content.index("\n")]

def show_on_googlemaps(place):
    webbrowser.open("https://www.google.com/maps/place/" + place + "/&amp;")

def search_in_web(find):
    webbrowser.open("www.duckduckgo.com/"+find)

def play_music(mp3):
    play_audio = vlc.MediaPlayer(mp3)
    play_audio.play()

def play_some_music():
    random_mp3 = random.choice(os.listdir(expanduser("~") + "/Music/"))
    mp3_file = expanduser("~/Music/") + random_mp3
    play_music(mp3_file)

def play_prefer_music(preferred):
    music = glob.glob(expanduser("~/Music/") + "*.mp3")
    categorize_music = dict()
    for better_search in music:
        categorize_music[better_search] = better_search.lower()   
    for key, value in categorize_music.items():
        if preferred.lower() in value.lower():
            play_music(key)
            break     

def word_meaning(word):
    dictionary = PyDictionary()
	word_mean = return dictionary.meaning(word)
	return word_mean
