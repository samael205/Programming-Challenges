import bot

if __name__ == '__main__':

    chatter_bot = bot.speaker_bot()
    chatter_bot.speaker.say("Hello, World!")

    while 1:
        chatter_bot.interaction()