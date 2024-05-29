#!/bin/bash

TAGM="MC_skimjet"
TAG="jt2030"
SAMPLETYPE="MC"

INPUTROOTFILE="../rootfiles/analysis/merged_HiForestAOD_${TAGM}/merged_HiForestAOD_${TAGM}.root"
MULTIFITFILE="../rootfiles/confFiles/${SAMPLETYPE}/merged_HiForestAOD_${SAMPLETYPE}_baseline_${TAG}.multifit"
OUTPUTDIR="../rootfiles/analysis/acceff/toys/fits"

TOYSBASE="../rootfiles/analysis/acceff/toys/acceffmaps"

OUTPUTS=( )

mkdir ${OUTPUTDIR}

PREFIX="multifit_baseline_jt2030"

# echo "multifiting nominal"
# ./multifit.sh "${MULTIFITFILE}" "${INPUTROOTFILE}" "../rootfiles/analysis/acceff/toys/merged_HiForestAOD_MC_skimjet_w" "../rootfiles/analysis/acceff/toys/merged_HiForestAOD_MC_skimjet_w.root"

echo ">>multifitting ${INPUTROOTFILE} ${MULTIFITFILE}"
for i in {0..199}
do
    MULTIFITOUT_ACC="${OUTPUTDIR}/${PREFIX}_acc_$i"
    CORRFILE_ACC="${TOYSBASE}/merged_HiForestAOD_MC_skimjet_w_acc_$i.root"
    echo "fitting for ${CORRFILE_ACC} to ${MULTIFITOUT_ACC}"
    ./multifit.sh "${MULTIFITFILE}" "${INPUTROOTFILE}" "${MULTIFITOUT_ACC}" "${CORRFILE_ACC}"

    MULTIFITOUT_EFF="${OUTPUTDIR}/${PREFIX}_eff_$i"
    CORRFILE_EFF="${TOYSBASE}/merged_HiForestAOD_MC_skimjet_w_eff_$i.root"
    echo "fitting for ${CORRFILE_EFF} to ${MULTIFITOUT_EFF}"
    ./multifit.sh "${MULTIFITFILE}" "${INPUTROOTFILE}" "${MULTIFITOUT_EFF}" "${CORRFILE_EFF}"

    OUTPUTS=( ${OUTPUTS[@]}  ${MULTIFITOUT_EFF} ${MULTIFITOUT_ACC})
done