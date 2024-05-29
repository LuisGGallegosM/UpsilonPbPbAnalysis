#!/bin/bash

TAG="unfolding_baseline_jtpt"
INPUTFILE="${1:-../rootfiles/analysis/merged_HiForestAOD_DATA_skimjet/merged_HiForestAOD_DATA_skimjet.root}"
RESPONSEFILE="${2:-../rootfiles/analysis/merged_HiForestAOD_MC_skimjet/unfolding_baseline_flatprior/unfold_training.root}"
OUTPUTFILE="${3:-../rootfiles/analysis/merged_HiForestAOD_DATA_skimjet_w/unfolding_baseline_flatprior/unfolding.root}"

mkdir -p "$( dirname ${OUTPUTFILE} )"
mkdir -p "$( dirname ${OUTPUTFILE} )/dagostini/refolded"
mkdir -p "$( dirname ${OUTPUTFILE} )/dagostini/unfolded"
mkdir -p "$( dirname ${OUTPUTFILE} )/inversion"
mkdir -p "$( dirname ${OUTPUTFILE} )/inversion_yagg"
./AccEff/acceff -unfold ${INPUTFILE} ${RESPONSEFILE} ${OUTPUTFILE} > "${OUTPUTFILE%.*}.log"
