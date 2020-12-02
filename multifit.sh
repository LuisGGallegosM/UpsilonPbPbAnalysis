#!/bin/bash

#directory where files are located
WORKDIR="../rootfiles/merged_HiForestAOD_skimmed2"
#directory where to save multifit results, inside WORKDIR
OUTDIR="output"

#skimmed file is inside WORKDIR
SKIMFILE="${WORKDIR}/$( basename $WORKDIR ).root"
#drawing configuration file
DRAWCONFIG="${SKIMFILE%.*}.drawconf"
#cut configuration file
CUTCONFIG="${SKIMFILE%.*}.cutconf"

echo "multiple fitting"
echo "reading skim file '${SKIMFILE}'"
echo "reading draw configuration file '${DRAWCONFIG}'"
echo "reading cut file '${CUTCONFIG}'"

for F in "$SKIMFILE" "$DRAWCONFIG" "$CUTCONFIG"
do
    if [ ! -f "$F" ]
    then
        echo "Error: ${F} file does not exist!"
        exit 1
    fi
done

echo "saving files in '${OUTDIR}' directory"

#fitconf files to read
CONFIGFILES=( \
 "merged_HiForestAOD_fit0.fitconf"\
 "merged_HiForestAOD_fit1.fitconf"\
 "merged_HiForestAOD_fit2.fitconf"\
 "merged_HiForestAOD_fit3.fitconf"\
 "merged_HiForestAOD_fit4.fitconf"\
 "merged_HiForestAOD_fit5.fitconf")

INTEGRATEDCFILE="merged_HiForestAOD_fit_integrated.fitconf"

echo "fitting..."
#do the fits
if [ 1 = 1 ]
then
    #check if all fitconfig files exists
    for CONFIG in ${CONFIGFILES[@]}
    do
        FITFILE="${WORKDIR}/${CONFIG}"
        echo "reading fit file '${FITFILE}'"
        if [ ! -f "$FITFILE" ]
        then
            echo "Error: ${FITFILE} file does not exists!"
            exit 1
        fi
    done

    #do the fitting jobs
    JOBS=()

    for CONFIG in ${CONFIGFILES[@]}
    do
        FITFILE="${WORKDIR}/${CONFIG}"
        ./fit.sh "$SKIMFILE" "$FITFILE" "${WORKDIR}/${OUTDIR}/${CONFIG%.*}" &
        JOBS=( "${JOBS[@]}" "$!" )
    done

    #wait for all jobs to complete
    for JOB in ${JOBS[@]}
    do
        wait $JOB
    done
fi

if [ 1 = 1 ]
then
    ./fit.sh "$SKIMFILE" "${WORKDIR}/${INTEGRATEDCFILE}" "${WORKDIR}/${OUTDIR}/${INTEGRATEDCFILE%.*}"
fi

echo "all fits done"

if [ 1 = 1 ]
then
    echo "multiple drawing"
    #do fit comparison drawings
    NAME="${WORKDIR}/${OUTDIR}/merged_HiForestAOD_fit.pdf"

    FITFILENAMES=()
    for CONFIG in ${CONFIGFILES[@]}
    do
        OUTPUTFILE="${WORKDIR}/${OUTDIR}/${CONFIG%.*}/${CONFIG%.*}.fit"
        echo "reading fit result file '${OUTPUTFILE}'"

        if [ ! -f ${OUTPUTFILE} ]
        then
            echo "Error: ${OUTPUTFILE} does not exists!"
            exit 1
        fi

        FITFILENAMES=( ${FITFILENAMES[@]} $OUTPUTFILE)
    done

    ./Drawing/draw -m $NAME "${FITFILENAMES[@]}" > "${NAME%.*}_Drawing.log" 2>&1 
fi

echo "all done"