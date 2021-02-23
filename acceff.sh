#!/bin/bash

CLING="NO"
#input file to skim
INPUTFILE="../rootfiles/merged_HiForestAOD_MCFix.root"
WORKDIR="../rootfiles/merged_HiForestAOD_MCFix_AccEff"
#how to folder name skim result file
OUTPUTFOLDER="${WORKDIR}/output"
#cut configuration file
CONFIG="${WORKDIR}/merged_HiForestAOD_MC.cutconf"
#output skimmed file, put in OUTPUTFOLDER and same as output folder but with .root extensiom
OUTPUTFILE="${OUTPUTFOLDER}/output.root"

mkdir -p $OUTPUTFOLDER

#execute skim
if [ $CLING = "YES" ]
then
cd AccEff
root -q 'AccEff.cpp("'../${INPUTFILE}'","'../${OUTPUTFILE}'","'../${CONFIG}'")'
cd ..
else
./AccEff/acceff "${INPUTFILE}" "${OUTPUTFILE}" "${CONFIG}"
fi