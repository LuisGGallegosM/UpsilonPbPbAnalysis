#!/bin/bash

TYPE="MC_skim"
TAG="mc"
MULTIFITDIR="../rootfiles/analysis/merged_HiForestAOD_${TYPE}/multifit_baseline_${TAG}"
OUTPUTFILE="../rootfiles/analysis/merged_HiForestAOD_${TYPE}/multifit_baseline_${TAG}/an.log"

./Fitting/fit -extraan ${MULTIFITDIR} > "${OUTPUTFILE}"