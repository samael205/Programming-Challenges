def get_file_content():
    data = [i for i in open("workers.txt").readlines()]
    return data


def create_data_list(data):
    new_data = list()
    for worker in data:
        new_data.append(worker.split(";"))
    return new_data


def occupy_all_earning_histogram(data):
    histogram = dict()
    for worker in data:
        if worker[1] not in histogram:
            histogram[worker[1]] = int(worker[2])
        else:
            histogram[worker[1]] += int(worker[2])
    return histogram


def occupy_histogram(data):
    histogram = dict()
    for worker in data:
        if worker[1] not in histogram:
            histogram[worker[1]] = 1
        else:
            histogram[worker[1]] += 1
    return histogram


def occupy_avg_earning_histogram(data):
    avg_earning = dict()
    sum = occupy_all_earning_histogram(data)
    count = occupy_histogram(data)
    for worker in data:
        if worker[1] not in avg_earning:
            avg_earning[worker[1]] = sum[worker[1]]/count[worker[1]]
    return avg_earning
