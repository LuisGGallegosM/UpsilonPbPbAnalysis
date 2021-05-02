#!/bin/bash

CLING="NO"
#input file for acceptancy located in datasets directory
FLAGS=${1:-"-all"}
ACCINPUTFILENAME="OniaTree_Ups1SMM_5p02TeV_TuneCUETP8M1_nofilter_pp502Fall15-MCRUN2_71_V1-v1_GENONLY.root"
EFFINPUTFILENAME="merged_HiForestAOD_MCFix2.root"
CONFIG="../rootfiles/confFiles/merged_HiForestAOD_MC_baseline.cutconf"
DATA_MULTIFITINPUTFILENAME="merged_HiForestAOD_DATA_skim/multifit_baseline"
MC_MULTIFITINPUTFILENAME="merged_HiForestAOD_MCFix2_skim/multifit_baseline"

#input file with relative path for Acceptancy
ACCINPUTFILE="../rootfiles/datasets/${ACCINPUTFILENAME}"
#input file with relative path for Effiency
EFFINPUTFILE="../rootfiles/datasets/${EFFINPUTFILENAME}"
#files with multifit results
DATA_FITINPUTFILENAME="../rootfiles/analysis/${DATA_MULTIFITINPUTFILENAME}/fit.root"
MC_FITINPUTFILENAME="../rootfiles/analysis/${MC_MULTIFITINPUTFILENAME}/fit.root"
#folder to output all files
OUTPUTFOLDER="../rootfiles/analysis/${DATA_MULTIFITINPUTFILENAME}/acceff"

#output file, put in OUTPUTFOLDER and same as output folder but with .root extension
ACCOUTPUTFILE="${OUTPUTFOLDER}/Acc/outputAcc.root"
EFFOUTPUTFILE="${OUTPUTFOLDER}/Eff/outputEff.root"
FINALOUTPUTFILE="${OUTPUTFOLDER}/AccEff/outputAccXEff.root"
YIELDFITOUTPUTFILE="${OUTPUTFOLDER}/YieldFit/yieldFit.root"
CLOSUREOUTPUTFILE="${OUTPUTFOLDER}/ClosureTest/closureTest.root"
ACCCORROUTPUTFILE="${OUTPUTFOLDER}/AccCorr/outputAccCorr.root"
EFFCORROUTPUTFILE="${OUTPUTFOLDER}/EffCorr/outputEffCorr.root"
FINALCORROUTPUTFILE="${OUTPUTFOLDER}/AccEffCorr/outputAccXEffCorr.root"

mkdir -p "$OUTPUTFOLDER"
mkdir -p "${OUTPUTFOLDER}/Acc"
mkdir -p "${OUTPUTFOLDER}/Eff"
mkdir -p "${OUTPUTFOLDER}/AccEff"
mkdir -p "${OUTPUTFOLDER}/YieldFit"
mkdir -p "${OUTPUTFOLDER}/ClosureTest"
mkdir -p "${OUTPUTFOLDER}/AccCorr"
mkdir -p "${OUTPUTFOLDER}/EffCorr"
mkdir -p "${OUTPUTFOLDER}/AccEffCorr"

TIMECMD=/usr/bin/time
TIMEARGS=-v

acc () { $TIMECMD $TIMEARGS ./AccEff/acceff -acc "${ACCINPUTFILE}" "${ACCOUTPUTFILE}" |& tee "${ACCOUTPUTFILE%.*}.log"; }
eff () { $TIMECMD $TIMEARGS ./AccEff/acceff -eff "${EFFINPUTFILE}" "${EFFOUTPUTFILE}" "${CONFIG}" |& tee "${EFFOUTPUTFILE%.*}.log";}
final () { $TIMECMD $TIMEARGS ./AccEff/acceff -final "${ACCOUTPUTFILE}" "${EFFOUTPUTFILE}" "${DATA_FITINPUTFILENAME}" "${MC_FITINPUTFILENAME}" "${FINALOUTPUTFILE}" |& tee "${FINALOUTPUTFILE%.*}.log";}
closure () { $TIMECMD $TIMEARGS ./AccEff/acceff -closure "${FINALOUTPUTFILE}" "${ACCOUTPUTFILE}" "${MC_FITINPUTFILENAME}" "${CLOSUREOUTPUTFILE}" |& tee "${CLOSUREOUTPUTFILE%.*}.log"; }
fit () { $TIMECMD $TIMEARGS ./AccEff/acceff -fit "${FINALOUTPUTFILE}" "${YIELDFITOUTPUTFILE}" |& tee "${YIELDFITOUTPUTFILE%.*}.log";}
acccorr () { $TIMECMD $TIMEARGS ./AccEff/acceff -acc "${ACCINPUTFILE}" "${ACCCORROUTPUTFILE}" "${YIELDFITOUTPUTFILE}" |& tee  "${ACCCORROUTPUTFILE%.*}.log"; }
effcorr () { $TIMECMD $TIMEARGS ./AccEff/acceff -effcorr "${EFFINPUTFILE}" "${EFFCORROUTPUTFILE}" "${CONFIG}" |& tee  "${EFFCORROUTPUTFILE%.*}.log"; }
finalcorr () { $TIMECMD $TIMEARGS ./AccEff/acceff -final "${ACCCORROUTPUTFILE}" "${EFFCORROUTPUTFILE}" "${DATA_FITINPUTFILENAME}" "${MC_FITINPUTFILENAME}" "${FINALCORROUTPUTFILE}" |& tee  "${FINALCORROUTPUTFILE%.*}.log"; }

#execute skim
if [ $CLING = "YES" ]
then
cd AccEff
root -q 'AccEff.cpp("'../${ACCINPUTFILE}'","'../${EFFINPUTFILE}'","'../${ACCOUTPUTFILE}'","'../${CONFIG}'")'
cd ..
else
case $FLAGS in
    "-acc")
    acc
    ;;
    "-eff")
    eff
    ;;
    "-final")
    final
    ;;
    "-closure")
    closure
    ;;
    "-fit")
    fit
    ;;
    "-acccorr")
    acccorr
    ;;
    "-effcorr")
    effcorr
    ;;
    "-finalcorr")
    finalcorr
    ;;
    "-all")
    acc
    eff
    final
    closure
    fit
    acccorr
    effcorr
    finalcorr
    ;;
esac
fi