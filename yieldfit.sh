#!/bin/bash

#folder to output all files

OUTPUTFOLDER=${1:-"../rootfiles/analysis/merged_HiForestAOD_DATA_skim/multifit_baseline_data/acceff"}

CROSSSECTION_FILE="${OUTPUTFOLDER}/CrossSect/outputCrossSection.root"
YIELDFIT_OUTPUTFILE="${OUTPUTFOLDER}/YieldFit/yieldFit.root"

mkdir -p "$OUTPUTFOLDER"

TIMECMD=/usr/bin/time
TIMEARGS=-v


mkdir -p "${OUTPUTFOLDER}/YieldFit"
$TIMECMD $TIMEARGS ./AccEff/acceff -fit "${CROSSSECTION_FILE}" "${YIELDFIT_OUTPUTFILE}" |& tee "${YIELDFIT_OUTPUTFILE%.*}.log"

