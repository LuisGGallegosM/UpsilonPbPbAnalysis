#!/bin/bash

WORKDIR="../rootfiles/merged_HiForestAOD_skimmed/"

CONFIGFILES=( \
 "merged_HiForestAOD_fit0.fitconf"\
 "merged_HiForestAOD_fit1.fitconf"\
 "merged_HiForestAOD_fit2.fitconf"\
 "merged_HiForestAOD_fit3.fitconf"\
 "merged_HiForestAOD_fit4.fitconf"\
 "merged_HiForestAOD_fit5.fitconf"\
 "merged_HiForestAOD_fit6.fitconf" )

FILENAMES="${CONFIGFILES[@]/#/${WORKDIR}}"

echo ${FILENAMES[@]}

JOBS=()

for CONFIG in ${FILENAMES[@]}
do
./fit.sh "$CONFIG" &
JOBS=( "${JOBS[@]}" "$!" )
done

for JOB in ${JOBS[@]}
do
wait $JOB
done

NAME="${WORKDIR}/merged_HiForestAOD_fit.pdf"

FITFILENAMES=()
for FILENAME in ${FILENAMES[@]}
do
OUTPUTDIR="${FILENAME%.*}"
OUTPUTFILE="${OUTPUTDIR}/$( basename $OUTPUTDIR ).fit"
FITFILENAMES=( ${FITFILENAMES[@]} $OUTPUTFILE)
done

./Drawing/draw -m $NAME "${FITFILENAMES[@]}" #> "${NAME%.*}_Drawing.log" 2>&1 

echo "all done"