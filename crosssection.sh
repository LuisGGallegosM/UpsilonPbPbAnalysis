#!/bin/bash

#input file for acceptancy located in datasets directory
CORRECTED=${1:-"-nocorr"}
DATA_MULTIFITINPUTFILENAME=${2:-"merged_HiForestAOD_DATA_skim/multifit_baseline_data"}
MC_MULTIFITINPUTFILENAME="merged_HiForestAOD_MC_skim/baseline/multifit_baseline_mc"

#folder to output all files
DATA_FITINPUTFILENAME="../rootfiles/analysis/${DATA_MULTIFITINPUTFILENAME}/fit.root"
MC_FITINPUTFILENAME="../rootfiles/analysis/${MC_MULTIFITINPUTFILENAME}/fit.root"

if [ $CORRECTED = "-corr" ]
then
OUTPUTFOLDER="../rootfiles/analysis/${DATA_MULTIFITINPUTFILENAME}/acceff/CrossSectCorr"
ACCEFF_FILE="../rootfiles/analysis/${DATA_MULTIFITINPUTFILENAME}/acceff/AccEffCorr/outputAccXEffCorr.root"
else
OUTPUTFOLDER="../rootfiles/analysis/${DATA_MULTIFITINPUTFILENAME}/acceff/CrossSect"
ACCEFF_FILE="../rootfiles/analysis/${DATA_MULTIFITINPUTFILENAME}/acceff/AccEff/outputAccXEff.root"
fi

OUTPUT="${OUTPUTFOLDER}/outputCrossSection.root"
mkdir -p "$OUTPUTFOLDER"
TIMECMD=/usr/bin/time
TIMEARGS=-v

$TIMECMD $TIMEARGS ./AccEff/acceff -crosssection "${ACCEFF_FILE}" "${DATA_FITINPUTFILENAME}" "${MC_FITINPUTFILENAME}" "${OUTPUT}" |& tee "${OUTPUT%.*}.log"