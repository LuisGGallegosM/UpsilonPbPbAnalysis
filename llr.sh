#!/bin/bash

TAGM="MC_skimjet"
TAG="jt1020"
MAINDIR=${1:-"../rootfiles/analysis/merged_HiForestAOD_${TAGM}"}
OUTPUTFILE=${3}
FITFILENAME="config.multifit"
PREFIX=${2:-"multifit_baseline_${TAG}"}

INPUTFILES=( \
${MAINDIR}/${PREFIX}_cheb0/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb1/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb2/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb3/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb4/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb5/${FITFILENAME} \
              )

#echo ${INPUTFILES[@]}
./Fitting/fit -llr "bkg.type" "${OUTPUTFILE}" ${INPUTFILES[@]} > "${OUTPUTFILE}.log"