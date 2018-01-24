from get_data import occupy_avg_earning_histogram, create_data_list, get_file_content
import numpy as np
import random
import matplotlib.pyplot as plt
import os

os.system("./run")

r = lambda: random.randint(0, 255)

workers_data = occupy_avg_earning_histogram(create_data_list(get_file_content()))

pos = np.arange(len(workers_data.keys()))
width = 1.0

n = 25
ind = np.random.randint(0, len(workers_data.values()), 25)

heights = np.random.randint(3, 12, len(workers_data.values()))

plt.title("Average earn occupations")
plt.xlabel("Occupy")
plt.ylabel("Earn")

bar = plt.bar(workers_data.keys(), workers_data.values(), width=1, color='b')

for i in range(0, 25):
    bar[i].set_color('#%02X%02X%02X' % (r(), r(), r()))

plt.subplots_adjust(bottom=0.3)
plt.tick_params(axis='both', which='major', labelsize=6)
plt.xticks(rotation=90)

plt.show()
