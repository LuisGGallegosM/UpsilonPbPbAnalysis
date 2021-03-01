#!/bin/bash

CLING="NO"
#input file for acceptancy located in datasets directory
ACCINPUTFILENAME="OniaTree_Ups1SMM_5p02TeV_TuneCUETP8M1_nofilter_pp502Fall15-MCRUN2_71_V1-v1_GENONLY.root"
EFFINPUTFILENAME="merged_HiForestAOD_MCFix2.root"
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
OUTPUTFILE="${OUTPUTFOLDER}/output.root"

mkdir -p $OUTPUTFOLDER

#execute skim
if [ $CLING = "YES" ]
then
cd AccEff
root -q 'AccEff.cpp("'../${ACCINPUTFILE}'","'../${EFFINPUTFILE}'","'../${OUTPUTFILE}'","'../${CONFIG}'")'
cd ..
else
./AccEff/acceff "${ACCINPUTFILE}" "${EFFINPUTFILE}" "${OUTPUTFILE}" "${CONFIG}"
fi