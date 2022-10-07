#!/bin/bash

TYPE="MC_skimjet"
TAG="z_mc_tcb_jt3040"
MULTIFITDIR="../rootfiles/analysis/merged_HiForestAOD_${TYPE}/multifit_baseline_${TAG}"
OUTPUTFILE="../rootfiles/analysis/merged_HiForestAOD_${TYPE}/multifit_baseline_${TAG}/an.log"

./Fitting/fit -extraan ${MULTIFITDIR} > "${OUTPUTFILE}"