#!/bin/bash

CLING="NO"
#input file for acceptancy located in datasets directory
ACCINPUTFILENAME="OniaTree_Ups1SMM_5p02TeV_TuneCUETP8M1_nofilter_pp502Fall15-MCRUN2_71_V1-v1_GENONLY.root"
EFFINPUTFILENAME="merged_HiForestAOD_MCFix2.root"
FITINPUTFILENAME="../rootfiles/merged_HiForestAOD_skimmed7/output/merged_HiForestAOD_comp0/fit.root"
FLAGS=${1:-"-final"}
#input file with relative path for Acceptancy
ACCINPUTFILE="../rootfiles/datasets/${ACCINPUTFILENAME}"
#input file with relative path for Effiency
EFFINPUTFILE="../rootfiles/datasets/${EFFINPUTFILENAME}"
#working directory
WORKDIR="../rootfiles/merged_HiForestAOD_MCFix2_AccEff"
#folder to output all files
OUTPUTFOLDER="${WORKDIR}/output"
#cut configuration file, located in workdir
CONFIG="${WORKDIR}/merged_HiForestAOD_MC.cutconf"
#output file, put in OUTPUTFOLDER and same as output folder but with .root extension
ACCOUTPUTFILE="${OUTPUTFOLDER}/outputAcc.root"
EFFOUTPUTFILE="${OUTPUTFOLDER}/outputEff.root"
FINALOUTPUTFILE="${OUTPUTFOLDER}/outputAccXEff.root"

mkdir -p $OUTPUTFOLDER

#execute skim
if [ $CLING = "YES" ]
then
cd AccEff
root -q 'AccEff.cpp("'../${ACCINPUTFILE}'","'../${EFFINPUTFILE}'","'../${ACCOUTPUTFILE}'","'../${CONFIG}'")'
cd ..
else
case $FLAGS in
    "-acc")
    ./AccEff/acceff -acc "${ACCINPUTFILE}" "${ACCOUTPUTFILE}" "${CONFIG}"
    ;;
    "-eff")
    ./AccEff/acceff -eff "${EFFINPUTFILE}" "${EFFOUTPUTFILE}" "${CONFIG}"
    ;;
    "-final")
    ./AccEff/acceff -final "${ACCOUTPUTFILE}" "${EFFOUTPUTFILE}" "${FITINPUTFILENAME}" "${FINALOUTPUTFILE}"
    ;;
    "-all")
    ./AccEff/acceff -acc "${ACCINPUTFILE}" "${ACCOUTPUTFILE}" "${CONFIG}"
    ./AccEff/acceff -eff "${EFFINPUTFILE}" "${EFFOUTPUTFILE}" "${CONFIG}"
    ./AccEff/acceff -final "${ACCOUTPUTFILE}" "${EFFOUTPUTFILE}" "${FITINPUTFILENAME}" "${FINALOUTPUTFILE}"
    ;;
esac
fi