#!/bin/bash

TAGM="MC_skimjet"
TAG="mc_z_tcb_jt3040"
INPUTROOTFILE="../rootfiles/analysis/merged_HiForestAOD_${TAGM}/merged_HiForestAOD_${TAGM}.root"
INPUTDIR="../rootfiles/confFiles/MC/${TAG}"
OUTPUTDIR="../rootfiles/analysis/merged_HiForestAOD_${TAGM}"
OUTNAME="supermultifit_${TAG}"
PREFIX="multifit_baseline_${TAG}"

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

./Fitting/fit "-smult" "${OUTPUTDIR}/${OUTNAME}" ${OUTPUTS[@]} > ${OUTPUTDIR}/${OUTNAME}.log