#!/bin/bash

echo "Output:\n" > output.txt

for PROCESS_TYPE in CPU IO MIXED
do
	for LOAD in LOW MEDIUM HIGH
	do
		for SCHEDULER in SCHED_RR SCHED FIFO SCHED_OTHER
		do
			echo "$SCHEDULER, $LOAD, $PROCESS_TYPE" >> output.txt

			for trial in a b c
			do
				(sudo /usr/bin/time -f "User: %U,\tSystem: %S,\tReal: %E,\tContext Switches: %w" ./os_test $PARAM1 $PARAM2 $PARAM3) 2>> results.txt


