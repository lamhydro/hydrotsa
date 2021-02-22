#!/bin/bash

# Input data
RUNM=0
#INDIR=/home/luis/Documents/cpp/HYDATsfana/dayUnregLongFLOWSandCatch/
INDIR=/home/luis/hub/canNatFlowRegi/data/raw/
OUTDIR=/home/luis/hub/canNatFlowRegi/data/pretre/
EXC=/home/luis/Documents/projects/hydrotsa/bin/tspre

if [ $RUNM -eq 0 ]
then
	#FILENAME=06AC006_DLY_FLOWS.csv
	#FILENAME=01EO003_DLY_FLOWS.bin
	INFILE=05JM005_DLY_FLOWS.bin
	OUTFILE=$(cut -d. -f1 <<< ${INFILE})_pre.csv

	# Execute the code
	$EXC $INDIR $INFILE $OUTDIR $OUTFILE

	# Plot, display and save into jpg
	#gnuplot -e "filename='$OUTDIR/data/dre_$STATNAME.csv'; outfile='$OUTDIR/plots/dre_$STATNAME.jpg'; statname='$STATNAME'" gpts
	#display -density 200  $OUTDIR/plots/dre_$STATNAME.jpg &
	#pid=$!
	#sleep 2
	#kill $pid
else
	#for file in ${DIRNAME}*FLOWS.csv
	for file in ${INDIR}*FLOWS.bin
	do
		# Execute the code
		FILENAME=$(echo "$file" | awk -F'/' '{print $NF}')
		echo '------------->' $FILENAME
		$EXC $INDIR $FILENAME

		# Copy output into a file
		IFS='_'
		read -a STATNAME <<< "$FILENAME"
		mv ${INDIR}dre.csv $OUTDIR/data/dre_${STATNAME}.csv
		#echo $STATNAME
		IFS=''

		# Plot, display and save into jpg
		#gnuplot -e "filename='$OUTDIR/data/dre_$STATNAME.csv'; outfile='$OUTDIR/plots/dre_$STATNAME.jpg'; statname='$STATNAME'" gpts
		#display -density 200  $OUTDIR/plots/dre_$STATNAME.jpg &
		#pid=$!
		#sleep 2
		#kill $pid

	done #> out.txt
fi

