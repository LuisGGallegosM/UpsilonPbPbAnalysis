#!/bin/bash

#input file for acceptancy located in datasets directory
MAINDIR="../rootfiles/analysis"

CORRECTED=${1:-"-nocorr"}
DATA_MULTIFITINPUTFILENAME=${2:-"../rootfiles/analysis/merged_HiForestAOD_DATA_skimjet"}
MC_MULTIFITINPUTFILENAME=${3:-"../rootfiles/analysis/merged_HiForestAOD_MC_skimjet"}

#folder to output all files
DATA_FITINPUTFILENAME="${DATA_MULTIFITINPUTFILENAME}/fit.root"
MC_FITINPUTFILENAME="${MC_MULTIFITINPUTFILENAME}/fit.root"

if [ $CORRECTED = "-corr" ]
then
OUTPUTFOLDER="${DATA_MULTIFITINPUTFILENAME}/acceff/CrossSectCorr"
ACCEFF_FILE="${DATA_MULTIFITINPUTFILENAME}/acceff/AccEffCorr/outputAccXEffCorr.root"
else
OUTPUTFOLDER="${DATA_MULTIFITINPUTFILENAME}/acceff/CrossSect"
ACCEFF_FILE="${DATA_MULTIFITINPUTFILENAME}/acceff/AccEff/outputAccXEff.root"
fi

OUTPUT="${OUTPUTFOLDER}/outputCrossSection.root"
mkdir -p "$OUTPUTFOLDER"

./AccEff/acceff -crosssection "${ACCEFF_FILE}" "${DATA_FITINPUTFILENAME}" "${MC_FITINPUTFILENAME}" "${OUTPUT}" |& tee "${OUTPUT%.*}.log"