#!/usr/bin/make -f

measure_forks_per_sec: *.c
	gcc -o $@ $< -lrt

clean:
	rm measure_forks_per_sec

