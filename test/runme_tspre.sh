#!/bin/bash

# Input data
RUNM=0
INDIRcsv=/home/luis/Documents/cpp/HYDATsfana/dayUnregLongFLOWSandCatch/
INDIR=/home/luis/hub/canNatFlowRegi/data/raw/
OUTDIR=/home/luis/hub/canNatFlowRegi/data/pretre/
PLOTDIR=/home/luis/hub/canNatFlowRegi/plots/
EXC=/home/luis/Documents/projects/hydrotsa/bin/tspre

if [ $RUNM -eq 0 ]
then
	# Set variables
	#FILENAME=06AC006_DLY_FLOWS.csv
	#FILENAME=01EO003_DLY_FLOWS.bin
	INFILE=02HL003_DLY_FLOWS.bin
	INFILEcsv=$(cut -d. -f1 <<< ${INFILE}).csv
	PLOTNAME=$(cut -d. -f1 <<< ${INFILE})_pre.jpg
	STNAME=$(cut -d_ -f1 <<< ${INFILE})
	OUTFILE=$(cut -d. -f1 <<< ${INFILE})_pre.csv

	# Execute the code
	$EXC $INDIR $INFILE $OUTDIR $OUTFILE

	# Plot, display and save into jpg
	if [ -f  "$OUTDIR$OUTFILE" ]; then
		gnuplot -e "filename_csv='${INDIRcsv}${INFILEcsv}'; filename_pre='${OUTDIR}${OUTFILE}'; outfile='${PLOTDIR}${PLOTNAME}'; stname='${STNAME}'" tools/gpts_tspre
		display -density 200 ${PLOTDIR}${PLOTNAME} &
		pid=$!
		sleep 2
		kill $pid
	fi
else
	#for file in ${DIRNAME}*FLOWS.csv
	for file in ${INDIR}*FLOWS.bin
	do
		# Set variables
		INFILE=$(echo "$file" | awk -F'/' '{print $NF}')
		INFILEcsv=$(cut -d. -f1 <<< ${INFILE}).csv
		PLOTNAME=$(cut -d. -f1 <<< ${INFILE})_pre.jpg
		STNAME=$(cut -d_ -f1 <<< ${INFILE})
		OUTFILE=$(cut -d. -f1 <<< ${INFILE})_pre.csv

		# Execute the code
		$EXC $INDIR $INFILE $OUTDIR $OUTFILE

		# Plot, display and save into jpg
		if [ -f  "$OUTDIR$OUTFILE" ]; then
			gnuplot -e "filename_csv='${INDIRcsv}${INFILEcsv}'; filename_pre='${OUTDIR}${OUTFILE}'; outfile='${PLOTDIR}${PLOTNAME}'; stname='${STNAME}'" tools/gpts_tspre
			display -density 200 ${PLOTDIR}${PLOTNAME} &
			pid=$!
			sleep 2
			kill $pid
		fi

	done #> out.txt
fi

