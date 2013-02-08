#!/usr/bin/python
# 
# mini connect script, useful for e.g. load testing

import socket
import sys
import time


def get_nr_connect_per_second(host, port, duration_in_seconds,
                              after_connect_fn=None):
    now = time.time()
    i=0
    while True:
        s = socket.SocketType()
        s.connect((host, port))
        if after_connect_fn:
            after_connect_fn(s)
        s.close()
        i+=1
        if time.time() - now > duration_in_seconds:
            break
    return i/(duration_in_seconds)


if __name__ == "__main__":

    # when run this too long, we run into TIME_WAIT problems with
    # the tcp connections :)
    TIMEOUT=0.2

    if not len(sys.argv) > 1:
        print "Please specify a hostname to connect to"
        print "WARNING: THIS WILL DoS the host for %s seconds" % TIMEOUT
        sys.exit(1)

    hostname = sys.argv[1]
    ip = socket.gethostbyname(hostname)
    for host, port, fn, descr in ( 
            (ip, 80, None, ""),
            (hostname, 80, None, ""),
            (hostname, 80, 
             lambda s: s.send("HEAD / HTTP/1.0\n\n") and s.recvfrom(1024),
             "HEAD /"),
            (hostname, 80, 
             lambda s: s.send("GET / HTTP/1.0\n\n") and s.recvfrom(1024),
             "GET /"),
        ):
        print host, port, fn, descr
        count = get_nr_connect_per_second(host, port, TIMEOUT, fn)
        print "connect per sec: %s" % count
        print

