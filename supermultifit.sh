#!/bin/bash

TAGM="MC_skimjet"
TAG="jt1020"
INPUTROOTFILE=${1:-"../rootfiles/analysis/merged_HiForestAOD_${TAGM}/merged_HiForestAOD_${TAGM}.root"}
INPUTDIR=${2:-"../rootfiles/analysis/merged_HiForestAOD_${TAGM}/supermultifit_${TAG}"}
OUTPUTDIR=${3:-"../rootfiles/analysis/merged_HiForestAOD_${TAGM}"}
OUTNAME=${4:-"supermultifit_${TAG}"}
PREFIX=${5:-"multifit_baseline_${TAG}"}

INPUTFILES=( \
cheb0.multifit \
cheb1.multifit \
cheb2.multifit \
cheb3.multifit \
cheb4.multifit \
cheb5.multifit \
              )

OUTPUTS=( )

mkdir ${OUTPUTDIR}/${OUTNAME}

for FILE in ${INPUTFILES[@]}
do
    MULTIFITOUT="${OUTPUTDIR}/${PREFIX}_${FILE%.*}"
    echo ">>multifitting ${INPUTDIR}/${FILE} to "
    ./multifit.sh "${INPUTDIR}/${FILE}" "$INPUTROOTFILE" "${MULTIFITOUT}"
    OUTPUTS=( ${OUTPUTS[@]}  ${MULTIFITOUT})
    cp "${INPUTDIR}/${FILE}" "${OUTPUTDIR}/${OUTNAME}/${FILE}"
done

./Fitting/fit "-smult" "${OUTPUTDIR}/${OUTNAME}" ${OUTPUTS[@]} > ${OUTPUTDIR}/${OUTNAME}/${OUTNAME}.log