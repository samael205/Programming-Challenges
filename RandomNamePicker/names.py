import pickle
from os.path import isfile

names = {
    "prefixes" : [
        "Ae",
        "Nu",
        "Bo",
        "Di",
        "Wan",
        "Gor",
        "Fam",
        "Ju",
        "Zan",
        "Po",
        "Wen",
        "Han",
    ],
    "suffixes" : [
        "gar",
        "tres",
        "romir",
        "kil",
        "dar",
        "badar",
        "por",
        "polin",
        "hon",
        "raen",
        "gomon",
    ],
}


def save_name_paths():
    file = open("names_paths", "wb")
    pickle.dump(names, file)
    file.close()

if not isfile("names_path"):
    save_name_paths()
