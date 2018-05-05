# Description

###### Speaking chatter bot with voice recognition
He can talk
He learn by saving all unknown answers from user to csv file 

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

You can ask him for `wishes`

Some like

``"What is Shiba Inu"``

```python
 if "what is" in self.question:
    tell_me_whatis = self.question[self.question.index("what is")+8:]
    self.answer = wish_interpreter.wikipedia_find(tell_me_whatis)
    self.speaker.say(self.answer)
```
``"play music"``

```python
elif "play" in self.question and "music" in self.question:
    self.answer = "Happy listen!"
    wish_interpreter.play_some_music()
```
