#!/bin/bash

TAG="unfolding_baseline"
INPUTFILE="${1:-../rootfiles/datasets/merged_HiForestAOD_MC.root}"
OUTPUTFILE_TRAINING="${2:-../rootfiles/analysis/merged_HiForestAOD_MC_skimjet/${TAG}/unfold_training.root}"
OUTPUTFILE_TESTING="${3:-../rootfiles/analysis/merged_HiForestAOD_MC_skimjet/${TAG}/unfold_testing.root}"

mkdir -p "$( dirname ${OUTPUTFILE_TRAINING} )"
mkdir -p "$( dirname ${OUTPUTFILE_TESTING} )/dagostini/refolded"
mkdir -p "$( dirname ${OUTPUTFILE_TESTING} )/dagostini/unfolded"
mkdir -p "$( dirname ${OUTPUTFILE_TESTING} )/inversion"
./AccEff/acceff -unfoldtrain ${INPUTFILE} ${OUTPUTFILE_TRAINING}
./AccEff/acceff -unfoldtest ${OUTPUTFILE_TRAINING} ${OUTPUTFILE_TESTING} > "${OUTPUTFILE_TESTING%.*}.log"
