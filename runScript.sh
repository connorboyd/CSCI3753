#!/bin/bash

echo "Output:\n" > output.txt

for PROCESS_TYPE in CPU IO MIXED
do
	echo "$PROCESS_TYPE"
	for LOAD in LOW MEDIUM HIGH
	do
		echo " $LOAD"
		for SCHEDULER in SCHED_RR SCHED_FIFO SCHED_OTHER
		do
			echo "  $SCHEDULER"
			echo "$SCHEDULER, $LOAD, $PROCESS_TYPE" >> output.txt

			for trial in a b c
			do
				echo "   $trial"
				(sudo /usr/bin/time -f "Real: %E,\tUser: %U,\tSystem: %S,\tContext Switches: %w" ./main $SCHEDULER $PROCESS_TYPE $LOAD) 2>> output.txt
			done
			echo "" >> output.txt
		done
	done
done


