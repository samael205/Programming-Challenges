import wish_interpreter
import sys
import csv
from time import sleep

class speaker_bot:
    def __init__(self):
        self.answer = None
        self.question = None
        self.memory = None

        with open("new_memory.csv", "r") as csv_memo:
            reader = csv.reader(csv_memo)
            self.memory = dict(reader)
            self.memory = dict(map(str.strip, x) for x in self.memory.items())

    def interaction(self, user):
        self.question = user.lower()
        return self.answer if self.is_that_wish() or self.find_answer() else None

    def find_answer(self):
        if self.question in self.memory.keys():
            self.answer = self.memory[self.question]
            return True
        else:
            return False            

    def refresh_data(self):
        self.memory.clear()
        with open("new_memory.csv", "r") as csv_memo:
            reader = csv.reader(csv_memo)
            self.memory = dict(reader)
            self.memory = dict(map(str.strip, x) for x in self.memory.items())

    def save_user_answer(self, answer):
        with open("new_memory.csv", "a") as append_csv:
            writer = csv.writer(append_csv)
            writer.writerow([self.question.lower(), answer.lower()])

    def is_that_wish(self):
        if "what is" in self.question:
            tell_me_whatis = self.question[self.question.index("what is")+8:]
            self.answer = wish_interpreter.wikipedia_find(tell_me_whatis)
            return True
        elif "what does" in self.question and "mean" in self.question:
            tell_me_mean = self.question[self.question.index("what does")+10:self.question.index("mean")]
            word_mean = wish_interpreter.word_meaning(tell_me_mean)
            try:
                self.answer = word_mean["Noun"][0]
            except Exception:
                self.answer = "Can't found meaning of this word"    
            return True
        elif "search in" in self.question:
            search_key = self.question[self.question.index("search in")+10:]
            find_specific_for_me = search_key[search_key.index(" ")+1:]
            specific_web = search_key[:search_key.index(" ")]
            wish_interpreter.search_in_web("!" + specific_web + " " + find_specific_for_me)
            return True
        elif "search" in self.question:
            find_for_me = self.question[self.question.index("search")+7:]
            self.answer = "I search " + find_for_me + " for you!"
            wish_interpreter.search_in_web(find_for_me)
            return True
        elif "play" in self.question and "music" in self.question:
            self.answer = "Happy listen!"
            wish_interpreter.play_some_music()
            return True
        elif "play" in self.question:
            play_preferred_music = self.question[self.question.index("play")+5:]
            self.answer = "Play " + play_preferred_music.split("/")[-1]
            wish_interpreter.play_prefer_music(play_preferred_music)
            return True
        elif "where is" in self.question:
            show_for_me = self.question[self.question.index("where is")+9:]
            self.answer = "I show you " + show_for_me + " where is"
            wish_interpreter.show_on_googlemaps(show_for_me)
            return True
        elif "exit" in self.question:
            sleep(1)
            sys.exit()    
        else:
            return False