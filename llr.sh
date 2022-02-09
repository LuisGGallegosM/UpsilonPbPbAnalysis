#!/bin/bash

MAINDIR="../rootfiles/analysis/merged_HiForestAOD_DATA_skim"
OUTPUTFILE="../rootfiles/analysis/merged_HiForestAOD_DATA_skim"
FITFILENAME="merged_HiForestAOD_DATA_baseline.multifit"

INPUTFILES=( ${MAINDIR}/multifit_baseline_cheb1_fx/${FITFILENAME} \
${MAINDIR}/multifit_baseline_cheb2_fx/${FITFILENAME} \
${MAINDIR}/multifit_baseline_cheb3_fx/${FITFILENAME} \
${MAINDIR}/multifit_baseline_cheb4_fx/${FITFILENAME} \
${MAINDIR}/multifit_baseline_cheb5_fx/${FITFILENAME} \
              )

#echo ${INPUTFILES[@]}
./Fitting/fit -llr "${OUTPUTFILE}" ${INPUTFILES[@]} > ${MAINDIR}/LLR_fx.log