#!/bin/bash

TAGM="MC_skimjet"
TAG="jt1020"
MAINDIR=${1:-"../rootfiles/analysis/merged_HiForestAOD_${TAGM}/supermultifit_${TAG}"}
PREFIX=${2:-"multifit_baseline_${TAG}"}
OUTPUTFILE=${3:-"../rootfiles/analysis/merged_HiForestAOD_${TAGM}/supermultifit_${TAG}/llr.multifit"}
FITFILENAME="config.multifit"

INPUTFILES=( \
${MAINDIR}/${PREFIX}_cheb0/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb1/${FITFILENAME} \
${MAINDIR}/${PREFIX}_cheb2/${FITFILENAME} \
              )

#echo ${INPUTFILES[@]}
./Fitting/fit -llr "bkg.type" "${OUTPUTFILE}" ${INPUTFILES[@]} > "${OUTPUTFILE}.log"