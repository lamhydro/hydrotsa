#!/bin/bash

# Input data
RUNM=0
INDIR=/home/luis/Documents/cpp/HYDATsfana/dayUnregLongFLOWSandCatch/
OUTDIR=/home/luis/hub/canNatFlowRegi
EXC=../bin/tspre

if [ $RUNM -eq 0 ]
then
	# Execute the code
	#FILENAME=06AC006_DLY_FLOWS.csv
	FILENAME=08NG044_DLY_FLOWS.csv
	$EXC $INDIR $FILENAME

	# Copy output into a file
	IFS='_'
	read -a STATNAME <<< "$FILENAME"
	mv ./dre.csv $OUTDIR/data/dre_${STATNAME}.csv

	# Plot, display and save into jpg
	gnuplot -e "filename='$OUTDIR/data/dre_$STATNAME.csv'; outfile='$OUTDIR/plots/dre_$STATNAME.jpg'; statname='$STATNAME'" gpts
	display -density 200  $OUTDIR/plots/dre_$STATNAME.jpg &
	pid=$!
	sleep 2
	kill $pid
else
	for file in ${DIRNAME}*FLOWS.csv
	do
		# Execute the code
		FILENAME=$(echo "$file" | awk -F'/' '{print $NF}')
		$EXC $INDIR $FILENAME

		# Copy output into a file
		IFS='_'
		read -a STATNAME <<< "$FILENAME"
		mv ./dre.csv $OUTDIR/data/dre_${STATNAME}.csv
		IFS=''

		# Plot, display and save into jpg
		gnuplot -e "filename='$OUTDIR/data/dre_$STATNAME.csv'; outfile='$OUTDIR/plots/dre_$STATNAME.jpg'; statname='$STATNAME'" gpts
		display -density 200  $OUTDIR/plots/dre_$STATNAME.jpg &
		pid=$!
		sleep 2
		kill $pid

	done #> out.txt
fi

