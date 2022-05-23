#!/bin/bash

TAGM="MC_skimjet"
TAG="mc_z_tcb_jt3040"
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