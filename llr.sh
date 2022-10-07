#!/bin/bash

TAGM="DATA_skimjet"
TAG="data_z_tcb_jt1020_ffix"
MAINDIR="../rootfiles/analysis/merged_HiForestAOD_${TAGM}"
OUTPUTFILE="../rootfiles/analysis/merged_HiForestAOD_${TAGM}"
FITFILENAME="config.multifit"
PREFIX="multifit_baseline_${TAG}"

INPUTFILES=( \
${MAINDIR}/${PREFIX}_cheb0/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb1/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb2/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb3/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb4/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb5/${FITFILENAME} \
              )

#echo ${INPUTFILES[@]}
./Fitting/fit -llr "${OUTPUTFILE}" ${INPUTFILES[@]} > ${MAINDIR}/LLR_${TAG}.log