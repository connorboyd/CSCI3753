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
				(sudo /usr/bin/time -f "Real: %E,\tUser: %U,\tSystem: %S,\tContext Switches: %w" ./main $PARAM1 $PARAM2 $PARAM3) 2>> output.txt
			done
			echo "\n" >> output.txt
		done
	done
done


