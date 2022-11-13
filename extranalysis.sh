#!/bin/bash

TYPE="MC_skimjet"
TAG="jt3040"
MULTIFITDIR=${1:-"../rootfiles/analysis/merged_HiForestAOD_${TYPE}/multifit_baseline_${TAG}"}
MULTIIN=${2:-"../rootfiles/confFiles/DATA/merged_HiForestAOD_DATA_baseline_${TAG}_proto.multifit"}
MULTIOUT=${3:-"../rootfiles/confFiles/DATA/merged_HiForestAOD_DATA_baseline_${TAG}.multifit"}
OUTPUTFILE="${MULTIFITDIR}/an.log"

./Fitting/fit -extraan ${MULTIFITDIR} ${MULTIIN} ${MULTIOUT} > "${OUTPUTFILE}"