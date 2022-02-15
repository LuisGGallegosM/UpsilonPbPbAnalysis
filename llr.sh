#!/bin/bash

MAINDIR="../rootfiles/analysis/merged_HiForestAOD_MC_skimjet"
OUTPUTFILE="../rootfiles/analysis/merged_HiForestAOD_MC_skimjet"
FITFILENAME="config.multifit"
PREFIX="multifit_baseline_z_jt3040"

INPUTFILES=( \
${MAINDIR}/${PREFIX}_cheb0/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb1/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb2/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb3/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb4/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb5/${FITFILENAME} \
              )

#echo ${INPUTFILES[@]}
./Fitting/fit -llr "${OUTPUTFILE}" ${INPUTFILES[@]} > ${MAINDIR}/LLR_z_jt3040.log