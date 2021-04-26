#!/bin/bash

CLING="NO"
#input file for acceptancy located in datasets directory
FLAGS=${1:-"-all"}
ACCINPUTFILENAME="OniaTree_Ups1SMM_5p02TeV_TuneCUETP8M1_nofilter_pp502Fall15-MCRUN2_71_V1-v1_GENONLY.root"
EFFINPUTFILENAME="merged_HiForestAOD_MCFix2.root"
CONFIG="../rootfiles/confFiles/merged_HiForestAOD_MC.cutconf"
DATA_MULTIFITINPUTFILENAME="merged_HiForestAOD_DATA_skim/multifit1"
MC_MULTIFITINPUTFILENAME="merged_HiForestAOD_MCFix2_skim/multifit1"

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

mkdir -p "$OUTPUTFOLDER"
mkdir "${OUTPUTFOLDER}/Acc"
mkdir "${OUTPUTFOLDER}/Eff"
mkdir "${OUTPUTFOLDER}/AccEff"
mkdir "${OUTPUTFOLDER}/YieldFit"
mkdir "${OUTPUTFOLDER}/ClosureTest"

#execute skim
if [ $CLING = "YES" ]
then
cd AccEff
root -q 'AccEff.cpp("'../${ACCINPUTFILE}'","'../${EFFINPUTFILE}'","'../${ACCOUTPUTFILE}'","'../${CONFIG}'")'
cd ..
else
case $FLAGS in
    "-acc")
    ./AccEff/acceff -acc "${ACCINPUTFILE}" "${ACCOUTPUTFILE}"
    ;;
    "-eff")
    ./AccEff/acceff -eff "${EFFINPUTFILE}" "${EFFOUTPUTFILE}" "${CONFIG}"
    ;;
    "-final")
    ./AccEff/acceff -final "${ACCOUTPUTFILE}" "${EFFOUTPUTFILE}" "${DATA_FITINPUTFILENAME}" "${MC_FITINPUTFILENAME}" "${FINALOUTPUTFILE}"
    ;;
    "-closure")
    ./AccEff/acceff -closure "${FINALOUTPUTFILE}" "${ACCOUTPUTFILE}" "${MC_FITINPUTFILENAME}" "${CLOSUREOUTPUTFILE}"
    ;;
    "-fit")
    ./AccEff/acceff -fit "${FINALOUTPUTFILE}" "${YIELDFITOUTPUTFILE}"
    ;;
    "-all")
    ./AccEff/acceff -acc "${ACCINPUTFILE}" "${ACCOUTPUTFILE}"
    ./AccEff/acceff -eff "${EFFINPUTFILE}" "${EFFOUTPUTFILE}" "${CONFIG}"
    ./AccEff/acceff -final "${ACCOUTPUTFILE}" "${EFFOUTPUTFILE}" "${DATA_FITINPUTFILENAME}" "${MC_FITINPUTFILENAME}" "${FINALOUTPUTFILE}"
    ./AccEff/acceff -closure "${FINALOUTPUTFILE}" "${ACCOUTPUTFILE}" "${MC_FITINPUTFILENAME}" "${CLOSUREOUTPUTFILE}"
    ./AccEff/acceff -fit "${FINALOUTPUTFILE}" "${YIELDFITOUTPUTFILE}"
    ;;
esac
fi