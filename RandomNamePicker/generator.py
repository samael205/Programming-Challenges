import random
import pickle


def get_random_surname():
    line = random.choice(open("surnames.txt").readlines())
    return ''.join(line[0].upper() + line[1:])


def random_name_pick():
    file = open("names_paths", "rb")
    paths = pickle.load(file)
    file.close()

    first_path = paths["prefixes"][random.randint(0, len(paths["prefixes"]) - 1)]
    second_path = paths["suffixes"][random.randint(0, len(paths["suffixes"]) - 1)]

    surname = get_random_surname()

    full_name = first_path + second_path + " " + surname
    return full_name
