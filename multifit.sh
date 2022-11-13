#!/bin/bash
MAXJOBS=12
DOFIT="true"
DODRAW="true"
DOCDRAW="true"

#multifit input file
TAG="z_data_tcb_jt1020_ffxfix"
SAMPLETYPE="DATA"
TYPE="${SAMPLETYPE}_skimjet_w"
MULTIFITFILE=${1:-"../rootfiles/confFiles/${SAMPLETYPE}/merged_HiForestAOD_${SAMPLETYPE}_baseline_${TAG}.multifit"}
INPUTFILENAME=${2:-"../rootfiles/analysis/merged_HiForestAOD_${TYPE}/merged_HiForestAOD_${TYPE}.root"}
OUTDIR=${3:-"${INPUTFILENAME%/*}/multifit_baseline_${TAG}"}
CUTFILE=${4:-"${INPUTFILENAME%.*}.cutconf"}
DRAWCONFIG=${5:-"../rootfiles/confFiles/merged_HiForestAOD.drawconf"}
SKIMFILE="${INPUTFILENAME}"

#directory where to save multifit results

echo "multiple fitting"
echo "saving files in '${OUTDIR}'"
mkdir -p "${OUTDIR}"

#fitconf files to read
./HelperScripts/fitconfGen.sh "${MULTIFITFILE}" "${OUTDIR}"
CONFIGFILES=$(find ${OUTDIR} -maxdepth 1 -name "*.fitconf")

cp "$MULTIFITFILE" "${OUTDIR}/config.multifit"

#do the fits
if [ ${DOFIT} = "true" ]
then
    echo "fitting..."
    
    echo "reading skim file '${SKIMFILE}'"
    #do the fitting jobs
    for CONFIG in ${CONFIGFILES[@]}
    do
        FITFILE="${CONFIG}"
        FITNAME=$(basename $CONFIG)
        FITOUTPUTDIR="${OUTDIR}/${FITNAME%.*}"
        echo "fitting file: $CONFIG"
        mkdir "${FITOUTPUTDIR}"
        ./fit.sh "$SKIMFILE" "$CUTFILE" "$FITFILE" "${FITOUTPUTDIR}" &
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

rm ${CONFIGFILES}

#draw graphs
if [ ${DODRAW} = "true" ]
then
    #execute drawing
    echo "generating drawings"
    echo "reading drawing configuration file: ${DRAWCONFIG}"
    for CONFIG in ${CONFIGFILES[@]}
    do
        NAME=$(basename $CONFIG)
        ROOTFILEDRAW="${OUTDIR}/${NAME%.*}"
        echo "drawing ${ROOTFILEDRAW}"
        ./draw.sh "${ROOTFILEDRAW}" "${DRAWCONFIG}" > "${OUTDIR}/draw.log"
    done
fi

#do comparation drawing
if [ ${DOCDRAW} = "true" ]
then
    echo "starting comparative drawings..."
    FITS=$(find ${OUTDIR} -maxdepth 1 -type d -name "fit*")
    ./Fitting/fit -multidraw "${MULTIFITFILE}" "${OUTDIR}" $FITS
    ./HelperScripts/reportGen.sh "${MULTIFITFILE}" "${OUTDIR}" $FITS
fi

echo "all done"