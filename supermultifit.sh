#!/bin/bash

INPUTROOTFILE="../rootfiles/analysis/merged_HiForestAOD_MC_skimjet/merged_HiForestAOD_MC_skimjet.root"
INPUTDIR="../rootfiles/confFiles/jetmc_jt3040"
OUTPUTDIR="../rootfiles/analysis/merged_HiForestAOD_MC_skimjet"
TAG="z_jt3040"
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
done

./Fitting/fit "-smult" "${OUTPUTDIR}/${OUTNAME}" ${OUTPUTS[@]} > ${OUTPUTDIR}/${OUTNAME}.log