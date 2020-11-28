#!/bin/bash

echo "multiple fitting"

WORKDIR="../rootfiles/merged_HiForestAOD_MC_skimmed2"
OUTDIR="output"

CONFIGFILES=( \
 "merged_HiForestAOD_fit0.fitconf"\
 "merged_HiForestAOD_fit1.fitconf"\
 "merged_HiForestAOD_fit2.fitconf"\
 "merged_HiForestAOD_fit3.fitconf"\
 "merged_HiForestAOD_fit4.fitconf"\
 "merged_HiForestAOD_fit5.fitconf")

INTEGRATEDCFILE="merged_HiForestAOD_fit_integrated.fitconf"

SKIMFILE="../rootfiles/merged_HiForestAOD_MC_skimmed2/merged_HiForestAOD_MC_skimmed2.root"

if [ 1 = 1 ]
then

    JOBS=()

    for CONFIG in ${CONFIGFILES[@]}
    do
    FITFILE="${WORKDIR}/${CONFIG}"
    echo "reading fit file '${FITFILE}'"
    ./fit.sh "$SKIMFILE" "$FITFILE" "${WORKDIR}/${OUTDIR}/${CONFIG%.*}" &
    JOBS=( "${JOBS[@]}" "$!" )
    done

    for JOB in ${JOBS[@]}
    do
    wait $JOB
    done
    #./fit.sh "$SKIMFILE" "${WORKDIR}${INTEGRATEDCFILE}"
    echo "all fits done"
fi

echo "multiple drawing"
#do fit comparison drawings
NAME="${WORKDIR}/${OUTDIR}/merged_HiForestAOD_fit.pdf"

FITFILENAMES=()
for CONFIG in ${CONFIGFILES[@]}
do
OUTPUTFILE="${WORKDIR}/${OUTDIR}/${CONFIG%.*}/${CONFIG%.*}.fit"
echo "reading fit result file '${OUTPUTFILE}'"
FITFILENAMES=( ${FITFILENAMES[@]} $OUTPUTFILE)
done

./Drawing/draw -m $NAME "${FITFILENAMES[@]}" > "${NAME%.*}_Drawing.log" 2>&1 

echo "all done"