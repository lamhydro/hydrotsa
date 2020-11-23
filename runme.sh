#/bin/sh

RUNM=0
DIRNAME=/home/luis/Documents/cpp/HYDATsfana/dayUnregLongFLOWSandCatch/
EXE=./freqa/bin/freqa

if [ $RUNM -eq 0 ]
then
	FILENAME=04LJ001_DLY_FLOWS.csv
	$EXE $DIRNAME $FILENAME
else
	#echo here
	for file in ${DIRNAME}*FLOWS.csv
	do
		FILENAME=$(echo "$file" | awk -F'/' '{print $NF}')
		./bin/$EXE $DIRNAME $FILENAME
	
	done 
fi

