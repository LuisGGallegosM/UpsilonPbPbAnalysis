#!/bin/bash

TAG="unfolding_baseline_1d_jtpt"
INPUTFILE="${1:-../rootfiles/datasets/merged_HiForestAOD_MC.root}"
OUTPUTFILE_TRAINING="${2:-../rootfiles/analysis/merged_HiForestAOD_MC_skimjet/${TAG}/unfold_training.root}"
OUTPUTFILE_TESTING="${3:-../rootfiles/analysis/merged_HiForestAOD_MC_skimjet/${TAG}/unfold_testing.root}"

mkdir "../rootfiles/analysis/merged_HiForestAOD_MC_skimjet/${TAG}"
#./AccEff/acceff -unfoldtrain ${INPUTFILE} ${OUTPUTFILE_TRAINING}
#./AccEff/acceff -unfold ${OUTPUTFILE_TRAINING} ${OUTPUTFILE_TESTING} > "${OUTPUTFILE_TESTING%.*}.log"


./AccEff/acceff -unfoldtrain1d ${INPUTFILE} ${OUTPUTFILE_TRAINING}
./AccEff/acceff -unfold1d ${OUTPUTFILE_TRAINING} ${OUTPUTFILE_TESTING} > "${OUTPUTFILE_TESTING%.*}.log"