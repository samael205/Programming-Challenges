from __future__ import unicode_literals

import youtube_dl


def download(link):
    ydl_opts = {
    'format' : 'bestaudio/best',
    'postprocessors' : [{
        'key' : 'FFmpegExtractAudio',
        'preferredquality' : '320',
            'preferredcodec' : 'mp3'
    }],
    }
    with youtube_dl.YoutubeDL(ydl_opts) as ydl:
        info_dict = ydl.extract_info(link, download=True)
        video_title = info_dict.get("title", None)
        with open("history.txt", 'a') as file:
            file.write(video_title + "   " + link + "\n")







