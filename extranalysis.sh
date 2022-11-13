#!/bin/bash

TYPE="MC_skimjet"
TAG="z_mc_tcb_jt3040"
MULTIFITDIR=${1:-"../rootfiles/analysis/merged_HiForestAOD_${TYPE}/multifit_baseline_${TAG}"}
OUTPUTFILE="${MULTIFITDIR}/an.log"

./Fitting/fit -extraan ${MULTIFITDIR} > "${OUTPUTFILE}"