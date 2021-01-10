#!/bin/bash

#directory where files are located
WORKDIR="../rootfiles/merged_HiForestAOD_skimmed3"
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
mkdir "${WORKDIR}/${OUTDIR}"

#fitconf files to read
CONFIGFILES=()
ALLFITFILES=$(find ${WORKDIR} -name "*.fitconf")
for i in $ALLFITFILES
do
    echo "reading fitconf file '${i}'"
    CONFIGFILES=( ${CONFIGFILES[@]} $(basename $i) )
done

echo "fitting..."
#do the fits
if [ 0 = 1 ]
then
    #do the fitting jobs
    MAXJOBS=6
    for CONFIG in ${CONFIGFILES[@]}
    do
        FITFILE="${WORKDIR}/${CONFIG}"
        echo "fitting file: $CONFIG"
        ./fit.sh "$SKIMFILE" "$FITFILE" "${WORKDIR}/${OUTDIR}/${CONFIG%.*}" &
        JOBS=( $(jobs -p) )
        JOBNUM="${#JOBS[@]}"
        if [ $MAXJOBS = $JOBNUM ]
        then
            wait -n
        fi
    done
    wait
fi

echo "all fits done"

#do comparation drawing
if [ 1 = 1 ]
then
    echo "starting comparative drawings"
    COMPCONFFILES=$(find ${WORKDIR} -name "*.drawcomp")
    for COMP in $COMPCONFFILES
    do
        COMPFILES=$( cat ${COMP} )
        COMPNAME=$( basename ${COMP} )
        echo "reading file: $COMPNAME"
        ./multidraw.sh "${WORKDIR}/${OUTDIR}" "${WORKDIR}/${OUTDIR}/${COMPNAME%.*}" "${COMPFILES[@]}"
    done
fi

echo "all done"