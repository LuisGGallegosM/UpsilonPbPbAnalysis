#!/bin/bash

#directory where files are located
WORKDIR="../rootfiles/merged_HiForestAOD_skimmed4"
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

echo "generating fitconf files..."
./fitconfGen.sh "$WORKDIR"

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
ALLFITFILES=$(find ${WORKDIR} -maxdepth 1 -name "*.fitconf")
for i in $ALLFITFILES
do
    echo "reading fitconf file '${i}'"
    CONFIGFILES=( ${CONFIGFILES[@]} $(basename $i) )
done

echo "write a comment:"
read
printf "$REPLY" > "${WORKDIR}/${OUTDIR}/readme.txt"

#do the fits
if [ 1 = 1 ]
then
    echo "fitting..."
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
    echo "all fits done"
fi

#draw graphs
if [ 1 = 1 ]
then
    #execute drawing
    echo "generating drawings"
    echo "reading drawing configuration file: ${DRAWCONFIG}"
    for CONFIG in ${CONFIGFILES[@]}
    do
    NAME=${CONFIG%.*}
    ROOTFILEDRAW="${WORKDIR}/${OUTDIR}/${NAME}/${NAME}.root"
    echo "drawing ${ROOTFILEDRAW}"
    ./draw.sh "${ROOTFILEDRAW}" "${DRAWCONFIG}"
    done
fi

#do comparation drawing
if [ 1 = 1 ]
then
    echo "starting comparative drawings..."
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