#!/usr/bin/python

import re
import time


def get_nr_forks_since_dawn_of_time():
    with open("/proc/stat") as f:
        matches = re.search(r'processes\s+(\d+)', f.read(), flags=re.MULTILINE)
        nr_processes = int(matches.group(1))
    return nr_processes


if __name__ == "__main__":
    INTERVAL=5.0
    last_nr_forks = get_nr_forks_since_dawn_of_time()
    while True:
        time.sleep(INTERVAL)
        nr_forks = get_nr_forks_since_dawn_of_time()
        print "Forks / sec: %s " % ((nr_forks - last_nr_forks)/INTERVAL)
        last_nr_forks = nr_forks
