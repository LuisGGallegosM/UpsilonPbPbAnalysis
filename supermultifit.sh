#!/bin/bash

TAGM="DATA_skimjet_jdown_w"
TAG="jt1020"
INPUTROOTFILE=${1:-"../rootfiles/analysis/merged_HiForestAOD_${TAGM}/merged_HiForestAOD_${TAGM}.root"}
INPUTDIR=${2:-"../rootfiles/analysis/merged_HiForestAOD_${TAGM}/supermultifit_${TAG}"}
OUTPUTDIR=${3:-"../rootfiles/analysis/merged_HiForestAOD_${TAGM}/supermultifit_${TAG}"}
PREFIX=${4:-"multifit_baseline_${TAG}"}

INPUTFILES=( \
cheb0.multifit \
cheb1.multifit \
cheb2.multifit \
              )

OUTPUTS=( )

mkdir ${OUTPUTDIR}

for FILE in ${INPUTFILES[@]}
do
    MULTIFITOUT="${OUTPUTDIR}/${PREFIX}_${FILE%.*}"
    echo ">>multifitting ${INPUTDIR}/${FILE} to "
    ./multifit.sh "${INPUTDIR}/${FILE}" "$INPUTROOTFILE" "${MULTIFITOUT}"
    OUTPUTS=( ${OUTPUTS[@]}  ${MULTIFITOUT})
    cp "${INPUTDIR}/${FILE}" "${OUTPUTDIR}/${FILE}"
done

./Fitting/fit "-smult" "${OUTPUTDIR}" ${OUTPUTS[@]} > ${OUTPUTDIR}/smult.log