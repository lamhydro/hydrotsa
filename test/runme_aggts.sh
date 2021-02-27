#!/bin/bash

# Input data
RUNM=1
AGG=mregi # Type of ts aggregation: month year dregi mregi
INDIR=/home/luis/hub/canNatFlowRegi/data/pretre/
OUTDIR=/home/luis/hub/canNatFlowRegi/data/analy/
PLOTDIR=/home/luis/hub/canNatFlowRegi/plots/
EXC=/home/luis/Documents/projects/hydrotsa/bin/aggts
IFPLOT=false
IFVALG=true # Run valgrind

# Set the extension of the outfile
case $AGG in 
	month)
		EXTE=_$AGG.csv
		;;
	year)
		EXTE=_$AGG.csv
		;;
	dregi)
		EXTE=_$AGG.csv
		;;
	mregi)
		EXTE=_$AGG.csv
		;;
	*) 
		echo "$AGG aggregation is not processed"
		;;
esac

if [ $RUNM -eq 0 ]
then
	# Set variables
	#FILENAME=06AC006_DLY_FLOWS.csv
	#FILENAME=01EO003_DLY_FLOWS.bin
	INFILE=08NG044_DLY_FLOWS_pre.csv
	OUTFILE=$(cut -d. -f1 <<< ${INFILE})$EXTE

	# Execute the code
	if [ "$IFVALG" = true ]; then
		valgrind --tool=memcheck --leak-check=yes --show-reachable=yes $EXC $AGG $INDIR $INFILE $OUTDIR $OUTFILE
	else
		$EXC $AGG $INDIR $INFILE $OUTDIR $OUTFILE
	fi

	# Plot, display and save into jpg
	if [ "$IFPLOT" = true ]; then
		PLOTNAME=$(cut -d. -f1 <<< ${INFILE})_$AGG.jpg
		STNAME=$(cut -d_ -f1 <<< ${INFILE})
		if [ -f  "$OUTDIR$OUTFILE" ]; then
			gnuplot -e "filename='${OUTDIR}${OUTFILE}'; outfile='${PLOTDIR}${PLOTNAME}'; stname='${STNAME}'" tools/gpts_$AGG
			display -density 200 ${PLOTDIR}${PLOTNAME} &
			pid=$!
			sleep 2
			kill $pid
		fi
	fi
else
	#for file in ${DIRNAME}*FLOWS.csv
	for file in ${INDIR}*FLOWS_pre.csv
	do
		# Set variables
		INFILE=$(echo "$file" | awk -F'/' '{print $NF}')
		OUTFILE=$(cut -d. -f1 <<< ${INFILE})$EXTE

		# Execute the code
		$EXC $AGG $INDIR $INFILE $OUTDIR $OUTFILE

		# Plot, display and save into jpg
		if [ "$IFPLOT" = true ]; then
			PLOTNAME=$(cut -d. -f1 <<< ${INFILE})_$AGG.jpg
			STNAME=$(cut -d_ -f1 <<< ${INFILE})
			if [ -f  "$OUTDIR$OUTFILE" ]; then
				gnuplot -e "filename='${OUTDIR}${OUTFILE}'; outfile='${PLOTDIR}${PLOTNAME}'; stname='${STNAME}'" tools/gpts_$AGG
				display -density 200 ${PLOTDIR}${PLOTNAME} &
				pid=$!
				sleep 2
				kill $pid
			fi
		fi

	done #> out.txt
fi

