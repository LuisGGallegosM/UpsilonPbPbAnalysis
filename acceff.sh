#!/bin/bash

#input file for acceptancy located in datasets directory
ACCINPUTFILENAME="OniaTree_Ups1SMM_5p02TeV_TuneCUETP8M1_nofilter_pp502Fall15-MCRUN2_71_V1-v1_GENONLY.root"
EFFINPUTFILENAME="merged_HiForestAOD_MC.root"
CONFIG="../rootfiles/analysis/merged_HiForestAOD_MC_skim/merged_HiForestAOD_MC_skim.cutconf"

CORRECTED=${1:-"-nocorr"}
DATA_MULTIFITINPUTFILENAME=${2:-"merged_HiForestAOD_DATA_skim/multifit_baseline_data"}


#input file with relative path for Acceptancy
ACCINPUTFILE="../rootfiles/datasets/${ACCINPUTFILENAME}"
#input file with relative path for Effiency
EFFINPUTFILE="../rootfiles/datasets/${EFFINPUTFILENAME}"
#folder to output all files
OUTPUTFOLDER="../rootfiles/analysis/${DATA_MULTIFITINPUTFILENAME}/acceff"

#output file, put in OUTPUTFOLDER and same as output folder but with .root extension
ACC_OUTPUTFILE="${OUTPUTFOLDER}/Acc/outputAcc.root"
EFF_OUTPUTFILE="${OUTPUTFOLDER}/Eff/outputEff.root"
ACCEFF_OUTPUTFILE="${OUTPUTFOLDER}/AccEff/outputAccXEff.root"
ACC_CORR_OUTPUTFILE="${OUTPUTFOLDER}/AccCorr/outputAccCorr.root"
EFF_CORR_OUTPUTFILE="${OUTPUTFOLDER}/EffCorr/outputEffCorr.root"
ACCEFF_CORR_OUTPUTFILE="${OUTPUTFOLDER}/AccEffCorr/outputAccXEffCorr.root"
YIELDFIT_OUTPUTFILE="${OUTPUTFOLDER}/YieldFit/yieldFit.root"

mkdir -p "$OUTPUTFOLDER"

TIMECMD=/usr/bin/time
TIMEARGS=-v

if [ $CORRECTED = "-corr" ]
then
mkdir -p "${OUTPUTFOLDER}/AccCorr"
$TIMECMD $TIMEARGS ./AccEff/acceff -acc "${ACCINPUTFILE}" "${ACC_CORR_OUTPUTFILE}" "${YIELDFIT_OUTPUTFILE}" |& tee  "${ACC_CORR_OUTPUTFILE%.*}.log"

mkdir -p "${OUTPUTFOLDER}/EffCorr"
$TIMECMD $TIMEARGS ./AccEff/acceff -eff "${EFFINPUTFILE}" "${EFF_CORR_OUTPUTFILE}" "${CONFIG}" "${YIELDFIT_OUTPUTFILE}" |& tee  "${EFF_CORR_OUTPUTFILE%.*}.log"

mkdir -p "${OUTPUTFOLDER}/AccEffCorr"
$TIMECMD $TIMEARGS ./AccEff/acceff -acceff "${ACC_CORR_OUTPUTFILE}" "${EFF_CORR_OUTPUTFILE}" "${ACCEFF_CORR_OUTPUTFILE}" |& tee  "${ACCEFF_CORR_OUTPUTFILE%.*}.log"

else
mkdir -p "${OUTPUTFOLDER}/Acc"
$TIMECMD $TIMEARGS ./AccEff/acceff -acc "${ACCINPUTFILE}" "${ACC_OUTPUTFILE}" |& tee "${ACC_OUTPUTFILE%.*}.log"

mkdir -p "${OUTPUTFOLDER}/Eff"
$TIMECMD $TIMEARGS ./AccEff/acceff -eff "${EFFINPUTFILE}" "${EFF_OUTPUTFILE}" "${CONFIG}" |& tee "${EFF_OUTPUTFILE%.*}.log"

mkdir -p "${OUTPUTFOLDER}/AccEff"
$TIMECMD $TIMEARGS ./AccEff/acceff -acceff "${ACC_OUTPUTFILE}" "${EFF_OUTPUTFILE}" "${ACCEFF_OUTPUTFILE}" |& tee "${ACCEFF_OUTPUTFILE%.*}.log"
fi