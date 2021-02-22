#/bin/sh

RUNM=0
DIRNAME=/home/luis/Documents/cpp/HYDATsfana/dayUnregLongFLOWSandCatch/

if [ $RUNM -eq 0 ]
then
	#FILENAME=06AC006_DLY_FLOWS.csv
	FILENAME=04LJ001_DLY_FLOWS.csv
	./bin/tsana $DIRNAME $FILENAME
	gnuplot < gpts
	display -density 200 output.jpg &
	pid=$!
	sleep 2
	kill $pid
else
	#echo here
	for file in ${DIRNAME}*FLOWS.csv
	do
		FILENAME=$(echo "$file" | awk -F'/' '{print $NF}')
		./bin/tsana $DIRNAME $FILENAME
	
		#gnuplot < gpts
		#display -density 200 output.jpg &
		#pid=$!
		#sleep 2
		#kill $pid
	done > out.txt
fi

