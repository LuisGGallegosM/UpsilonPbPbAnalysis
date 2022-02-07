#!/bin/bash

MAINDIR="../rootfiles/analysis/merged_HiForestAOD_DATA_skim"
OUTPUTFILE="../rootfiles/analysis/merged_HiForestAOD_DATA_skim"
FITFILENAME="merged_HiForestAOD_DATA_baseline.multifit"

INPUTFILES=( ${MAINDIR}/multifit_baseline_cheb1/${FITFILENAME} \
${MAINDIR}/multifit_baseline_cheb2/${FITFILENAME} \
${MAINDIR}/multifit_baseline_cheb3/${FITFILENAME} \
${MAINDIR}/multifit_baseline_cheb4/${FITFILENAME} \
${MAINDIR}/multifit_baseline_cheb5/${FITFILENAME} \
              )

#echo ${INPUTFILES[@]}
./Fitting/fit -llr "${OUTPUTFILE}" ${INPUTFILES[@]} > ${MAINDIR}/LLR.log