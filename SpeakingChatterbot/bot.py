import speak
import wish_interpreter
import sys
import csv
from time import sleep
from threading import Thread

class speaker_bot:

    def __init__(self):
        self.speaker = speak.Speak()
        self.answer = None
        self.question = None
        self.end_of_file_position = 0
        self.recognizer = speak.Speech()
        self.memory = None

        with open("new_memory.csv", "r") as csv_memo:
            reader = csv.reader(csv_memo)
            self.memory = dict(reader)
            self.memory = dict(map(str.strip, x) for x in self.memory.items())

    def interaction(self):
        user = self.recognizer.record()
        self.question = user.lower()
        if self.is_that_wish():
            print(self.answer)
        elif self.find_answer():
            self.speaker.say(self.answer)
        else:
            self.ask_question()
            self.refresh_data()

    def find_answer(self):
        if self.question in self.memory.keys():
            self.answer = self.memory[self.question]
            print(self.answer)
            return True
        else:
            return False            

    def refresh_data(self):
        self.memory.clear()
        with open("new_memory.csv", "r") as csv_memo:
            reader = csv.reader(csv_memo)
            self.memory = dict(reader)
            self.memory = dict(map(str.strip, x) for x in self.memory.items())

    def ask_question(self):
        self.speaker.say(self.question)
        user = self.recognizer.record()
        with open("new_memory.csv", "a") as append_csv:
            writer = csv.writer(append_csv)
            writer.writerow([self.question.lower(), user.lower()])

    def random_line(self, file):
        line = next(file)
        for num, aline in enumerate(file):
            if random.randrange(num + 2):
                continue
            line = aline
        return line

    def question_from_bot(self):
        with open("questions.txt") as questions:
            question = self.random_line(questions)
            return question

    def continue_dialogue(self):
        with open("sentences.txt") as sentences:
            sentence = self.random_line(sentences)
            return sentence

    def is_that_wish(self):
        if "what is" in self.question:
            tell_me_whatis = self.question[self.question.index("what is")+8:]
            self.answer = wish_interpreter.wikipedia_find(tell_me_whatis)
            self.speaker.say(self.answer)
            return True
        elif "what does" in self.question and "mean" in self.question:
            tell_me_mean = self.question[self.question.index("what does")+10:self.question.index("mean")]
            word_mean = wish_interpreter.word_meaning(tell_me_mean)
            if "Noun" in word_mean.keys():
                self.answer = word_mean["Noun"]
            else:
                self.answer = "Can't found meaning of this word"    
            self.speaker.say(self.answer)
            return True
        elif "search in" in self.question:
            search_key = self.question[self.question.index("search in")+10:]
            find_specific_for_me = search_key[search_key.index(" ")+1:]
            specific_web = search_key[:search_key.index(" ")]
            print(find_specific_for_me, specific_web)
            wish_interpreter.search_in_web("!" + specific_web + " " + find_specific_for_me)
            return True
        elif "search" in self.question:
            find_for_me = self.question[self.question.index("search")+7:]
            self.answer = "I search " + find_for_me + " for you!"
            wish_interpreter.search_in_web(find_for_me)
            return True
        elif "play" in self.question and "music" in self.question:
            self.answer = "Happy listen!"
            music_thread = Thread(wish_interpreter.play_some_music())
            music_thread.start()
            music_thread.join()
            return True
        elif "play" in self.question:
            play_preferred_music = self.question[self.question.index("play")+5:]
            self.answer = "Play " + play_preferred_music.split("/")[-1];
            music_thread = Thread(wish_interpreter.play_prefer_music(play_preferred_music))
            music_thread.start()
            music_thread.join()
            return True;
        elif "where is" in self.question:
            show_for_me = self.question[self.question.index("where is")+9:]
            self.answer = "i show you " + show_for_me + " where is"
            wish_interpreter.show_on_googlemaps(show_for_me)
            return True
        elif "exit" in self.question:
            sleep(1)
            sys.exit()    
        else:
            return False