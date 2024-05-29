#!/bin/bash

TAG="MC_skimjet"

DATA_INPUT=${1:-"../rootfiles/analysis/merged_HiForestAOD_${TAG}/merged_HiForestAOD_${TAG}.root"}
OUTPUTDIR=${2:-"../rootfiles/analysis/acceff/toys/acceffmaps"}
ACC=${3:-"../rootfiles/analysis/acceff/toys/OutAcc2D.root"}
EFF=${4:-"../rootfiles/analysis/acceff/toys/OutEff2D.root"}
#mkdir -p $( dirname ${OUTPUT})

./AccEff/acceff -addw2 ${DATA_INPUT} ${ACC} ${EFF} "../rootfiles/analysis/acceff/toys/merged_HiForestAOD_${TAG}_w.root"

for i in {0..199}; do
    ACC_SYS="../rootfiles/analysis/acceff/toys/acceffmaps/OutAcc2D_${i}.root"
    OUTPUT="${OUTPUTDIR}/merged_HiForestAOD_${TAG}_w_acc_${i}.root"
    ./AccEff/acceff -addw2 ${DATA_INPUT} ${ACC_SYS} ${EFF} ${OUTPUT}
done

for i in {0..199}; do
    EFF_SYS="../rootfiles/analysis/acceff/toys/acceffmaps/OutEff2D_${i}.root"
    OUTPUT="${OUTPUTDIR}/merged_HiForestAOD_${TAG}_w_eff_${i}.root"
    ./AccEff/acceff -addw2 ${DATA_INPUT} ${ACC} ${EFF_SYS} ${OUTPUT}
done