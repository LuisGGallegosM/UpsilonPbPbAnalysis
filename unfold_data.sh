#!/bin/bash

TAG="unfolding_baseline_1d_jtpt"
INPUTFILE="${1:-../rootfiles/analysis/merged_HiForestAOD_DATA_skimjet/merged_HiForestAOD_DATA_skimjet.root}"
RESPONSEFILE="${2:-../rootfiles/analysis/merged_HiForestAOD_MC_skimjet/unfolding_baseline/unfold_training.root}"
OUTPUTFILE="${3:-../rootfiles/analysis/merged_HiForestAOD_DATA_skimjet_w/unfolding_baseline/unfolding.root}"

mkdir -p "$( dirname ${OUTPUTFILE} )"
./AccEff/acceff -unfold ${INPUTFILE} ${RESPONSEFILE} ${OUTPUTFILE} > "${OUTPUTFILE%.*}.log"
