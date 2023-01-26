#!/bin/bash

CORRECTED=${1:-"-nocorr"}

#input file with relative path for Acceptancy
ACCINPUTFILE=${2:-"../rootfiles/datasets/OniaTree_Ups1SMM_5p02TeV_TuneCUETP8M1_nofilter_pp502Fall15-MCRUN2_71_V1-v1_GENONLY.root"}
#input file with relative path for Efficiency
EFFINPUTFILE=${3:-"../rootfiles/datasets/merged_HiForestAOD_MC.root"}
#folder to output all files
OUTPUTFOLDER=${4:-"../rootfiles/analysis/merged_HiForestAOD_MC_skimjet/multifit_baseline/acceff"}
CONFIG=${5:-"../rootfiles/analysis/merged_HiForestAOD_MC_skimjet/merged_HiForestAOD_MC_skimjet.cutconf"}

#output file, put in OUTPUTFOLDER and same as output folder but with .root extension
ACC_OUTPUTFILE="${OUTPUTFOLDER}/Acc/outputAcc.root"
EFF_OUTPUTFILE="${OUTPUTFOLDER}/Eff/outputEff.root"
ACCEFF_OUTPUTFILE="${OUTPUTFOLDER}/AccEff/outputAccXEff.root"
ACC_CORR_OUTPUTFILE="${OUTPUTFOLDER}/AccCorr/outputAccCorr.root"
EFF_CORR_OUTPUTFILE="${OUTPUTFOLDER}/EffCorr/outputEffCorr.root"
ACCEFF_CORR_OUTPUTFILE="${OUTPUTFOLDER}/AccEffCorr/outputAccXEffCorr.root"
YIELDFIT_OUTPUTFILE="${OUTPUTFOLDER}/YieldFit/yieldFit.root"

mkdir -p "$OUTPUTFOLDER"

if [ $CORRECTED = "-corr" ]
then
mkdir -p "${OUTPUTFOLDER}/AccCorr"
./AccEff/acceff -acc "${ACCINPUTFILE}" "${ACC_CORR_OUTPUTFILE}" "${YIELDFIT_OUTPUTFILE}" > "${ACC_CORR_OUTPUTFILE%.*}.log"

mkdir -p "${OUTPUTFOLDER}/EffCorr"
./AccEff/acceff -eff "${EFFINPUTFILE}" "${EFF_CORR_OUTPUTFILE}" "${CONFIG}" "${YIELDFIT_OUTPUTFILE}" > "${EFF_CORR_OUTPUTFILE%.*}.log"

mkdir -p "${OUTPUTFOLDER}/AccEffCorr"
./AccEff/acceff -acceff "${ACC_CORR_OUTPUTFILE}" "${EFF_CORR_OUTPUTFILE}" "${ACCEFF_CORR_OUTPUTFILE}" > "${ACCEFF_CORR_OUTPUTFILE%.*}.log"

else
mkdir -p "${OUTPUTFOLDER}/Acc"
./AccEff/acceff -acc "${ACCINPUTFILE}" "${ACC_OUTPUTFILE}" > "${ACC_OUTPUTFILE%.*}.log"

mkdir -p "${OUTPUTFOLDER}/Eff"
./AccEff/acceff -eff "${EFFINPUTFILE}" "${EFF_OUTPUTFILE}" "${CONFIG}" > "${EFF_OUTPUTFILE%.*}.log"

mkdir -p "${OUTPUTFOLDER}/AccEff"
./AccEff/acceff -acceff "${ACC_OUTPUTFILE}" "${EFF_OUTPUTFILE}" "${ACCEFF_OUTPUTFILE}" > "${ACCEFF_OUTPUTFILE%.*}.log"
fi