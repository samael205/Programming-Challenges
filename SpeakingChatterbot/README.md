# Description

Speaking chatter bot with voice recognition

He can talk

He learn saving all unknown answers from user to csv file 

```python
def ask_question(self):
        self.speaker.say(self.question)
        user = self.recognizer.record() # Get user answer 
        with open("new_memory.csv", "a") as append_csv:
            writer = csv.writer(append_csv)
writer.writerow([self.question.lower(), user.lower()])
```
reload memory with new questions and answers 

```python
def refresh_data(self):
        self.memory.clear()
        with open("new_memory.csv", "r") as csv_memo:
            reader = csv.reader(csv_memo)
            self.memory = dict(reader)
self.memory = dict(map(str.strip, x) for x in self.memory.items())
```

You can ask him for "wishes"

Some like

"Search in youtube Shiba Inu"

```python
elif "search in" in self.question:
	search_key = self.question[self.question.index("search in")+10:]
        find_specific_for_me = search_key[search_key.index(" ")+1:]
        specific_web = search_key[:search_key.index(" ")]
        print(find_specific_for_me, specific_web)
	wish_interpreter.search_in_web("!" + specific_web + " " + find_specific_for_me)
```
or play music

```python
def play_some_music():
    random_mp3 = random.choice(os.listdir(expanduser("~") + "/Music/"))
    mp3_file = expanduser("~/Music/") + random_mp3
    play_music(mp3_file)
```
