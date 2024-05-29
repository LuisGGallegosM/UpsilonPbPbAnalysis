#!/bin/bash

TAG="DATA_skimjet_jup"

DATA_INPUT=${1:-"../rootfiles/analysis/merged_HiForestAOD_${TAG}/merged_HiForestAOD_${TAG}.root"}
OUTPUTDIR="../rootfiles/analysis/merged_HiForestAOD_${TAG}_w"
ACC=${3:-"../rootfiles/analysis/acceff/Acc/OutAcc2D.root"}
EFF=${4:-"../rootfiles/analysis/acceff/systematic_errors/OutEff2DSyst.root"}
OUTPUT=${2:-"${OUTPUTDIR}/merged_HiForestAOD_${TAG}_w.root"}
mkdir -p $( dirname ${OUTPUT})
./AccEff/acceff -addw ${DATA_INPUT} ${ACC} ${EFF} ${OUTPUT}